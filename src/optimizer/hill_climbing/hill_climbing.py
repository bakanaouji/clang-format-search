from copy import deepcopy


class HillClimbing(object):
    def __init__(self, **params):
        self.maps = params['maps']
        self.obj_func = params['obj_func']

        self.g = 0
        self.improved = False
        self.done = False
        self.evals = 0
        self.styles = {}
        self.keys = []
        if 'initial_styles' in params.keys():
            for key in self.maps.keys():
                self.styles[key] = params['initial_styles'][key]
                self.keys.append(key)
        else:
            for key, vals in self.maps.items():
                self.styles[key] = vals[0]
                self.keys.append(key)
        self.keys = sorted(self.keys)
        self.fval = self.obj_func.evaluate(self.styles)
        self.best_styles = self.styles
        self.best_fval = self.fval

    def one_iteration(self):
        self.g += 1

        key = self.keys[(self.g - 1) % len(self.keys)]
        vals = self.maps[key]
        vals = [val for val in vals if val != self.best_styles[key]]

        for val in vals:
            self.evals += 1
            self.styles = deepcopy(self.best_styles)
            self.styles[key] = val
            self.fval = self.obj_func.evaluate(self.styles)

            if self.best_fval > self.fval:
                self.best_fval = self.fval
                self.best_styles = self.styles
                self.improved = True

        if self.g % len(self.maps) == 0:
            if not self.improved:
                self.done = True
            else:
                self.improved = False
