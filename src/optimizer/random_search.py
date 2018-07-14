import numpy as np


class RandomSearch(object):
    def __init__(self, keys):
        self.keys = keys

    def sample(self):
        styles = {}
        for key in self.keys:
            styles[key] = bool(np.random.randint(2))
        return styles
