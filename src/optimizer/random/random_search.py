import numpy as np


class RandomSearch(object):
    def __init__(self, **params):
        self.maps = params['maps']
        self.obj_func = params['obj_func']

        self.g = 0
        self.evals = 0
        self.done = False
        self.fval = float('inf')
        self.styles = None
        self.best_fval = float('inf')
        self.best_styles = None

    def one_iteration(self):
        self.g += 1
        self.evals += 1
        self.styles = self.sample()
        self.fval = self.obj_func.evaluate(self.styles)

        if self.best_fval > self.fval:
            self.best_fval = self.fval
            self.best_styles = self.styles

    def sample(self):
        styles = {}
        for key, val in self.maps.items():
            styles[key] = str(np.random.choice(val))
        return styles
