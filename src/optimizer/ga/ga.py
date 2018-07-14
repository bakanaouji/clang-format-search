import numpy as np

from optimizer.ga.individual import Individual


class GA(object):
    def __init__(self):
        pass

    def crossover(self, ind1, ind2):
        size = len(ind1)
        offs1 = ind1.copy()
        offs2 = ind2.copy()
        first = np.random.randint(size)
        last = np.random.randint(first + 1, size + 1)
        offs1[first:last], offs2[first:last] = offs2[first:last], offs1[first:last]
        return offs1, offs2
