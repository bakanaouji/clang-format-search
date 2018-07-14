import numpy as np


class Individual(np.ndarray):
    f = float('inf')

    def __new__(cls, a):
        return np.asarray(a).view(cls)
