import numpy as np

from copy import deepcopy


class HillClimbing(object):
    def __init__(self, **params):
        self.keys = params['keys']
        self.obj_func = params['obj_func']

        self.g = 0
        self.evals = 0
        self.styles = {}
        for key in self.keys:
            self.styles[key] = bool(np.random.randint(2))
        self.fval = self.obj_func.evaluate(self.styles)
        self.best_styles = self.styles
        self.best_fval = self.fval

    def one_iteration(self):
        self.g += 1
        self.evals += 1
        self.styles = self.sample()
        self.fval = self.obj_func.evaluate(self.styles)
        print(self.fval, self.styles)
        print(self.best_fval, self.best_styles)

        if self.best_fval > self.fval:
            self.best_fval = self.fval
            self.best_styles = self.styles

    def sample(self):
        key = self.keys[(self.g - 1) % len(self.keys)]
        styles = deepcopy(self.best_styles)
        styles[key] = not styles[key]
        return styles
