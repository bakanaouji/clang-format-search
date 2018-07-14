import numpy as np

from operator import attrgetter
from optimizer.ga.individual import Individual


class GA(object):
    def __init__(self, **params):
        self.keys = params['keys']
        self.obj_func = params['obj_func']
        self.pop_size = params['pop_size']
        self.tournament_size = params['tournament_size']
        self.pop = [
            Individual([np.random.randint(2) for _ in range(len(self.keys))])
            for _ in range(self.pop_size)]

        self.g = 0
        self.evals = 0
        self.calc_fval()
        self.best_ind = min(self.pop, key=attrgetter('f'))
        self.best_styles = self.convert_ind_to_style(self.best_ind)
        self.best_fval = self.best_ind.f
        self.fval = self.best_fval
        self.styles = self.best_styles

    def convert_ind_to_style(self, ind):
        styles = {}
        for i in range(len(self.keys)):
            styles[self.keys[i]] = bool(ind[i])
        return styles

    def calc_fval(self):
        for ind in self.pop:
            styles = self.convert_ind_to_style(ind)
            if ind.f == float('inf'):
                ind.f = self.obj_func.evaluate(styles)
                self.evals += 1
            print(self.evals, ind.f, styles)

    def crossover(self, ind1, ind2):
        size = len(ind1)
        offs1 = ind1.copy()
        offs2 = ind2.copy()
        first = np.random.randint(size)
        last = np.random.randint(first + 1, size + 1)
        offs1[first:last], offs2[first:last] = offs2[first:last].copy(), offs1[first:last].copy()
        offs1.f = float('inf')
        offs2.f = float('inf')
        return offs1, offs2

    def mutate(self, ind):
        mutant = ind.copy()
        for i in range(len(ind)):
            if np.random.random() < 0.05:
                mutant[i] = not ind[i]
        mutant.f = float('inf')
        return mutant

    def one_iteration(self):
        self.g += 1
        # selection
        offspring = self.tournament_select()

        # crossover
        crossover = []
        for child1, child2 in zip(offspring[::2], offspring[1::2]):
            if np.random.random() < 0.5:
                child1, child2 = self.crossover(child1, child2)
            crossover.append(child1)
            crossover.append(child2)
        offspring = crossover

        # mutation
        mutants = []
        for mutant in offspring:
            if np.random.random() < 0.2:
                mutant = self.mutate(mutant)
            mutants.append(mutant)
        offspring = mutants

        # update population
        self.pop = offspring
        self.calc_fval()

        self.best_ind = min(self.pop, key=attrgetter('f'))
        self.styles = self.convert_ind_to_style(self.best_ind)
        self.fval = self.best_ind.f
        if self.best_fval > self.fval:
            self.best_fval = self.fval
            self.best_styles = self.styles

    def tournament_select(self):
        offspring = []
        for _ in range(self.pop_size):
            candidates = [self.pop[np.random.randint(self.pop_size)] for _ in
                          range(self.tournament_size)]
            offspring.append(min(candidates, key=attrgetter('f')))
        return offspring
