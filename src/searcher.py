import json
import os
import pandas as pd
import subprocess as sp

from copy import deepcopy
from function import convert
from optimizer.ga.ga import GA
from optimizer.hill_climbing.hill_climbing import HillClimbing
from optimizer.random.random_search import RandomSearch


class Searcher(object):
    def __init__(self, **params):
        self.params = params
        self.optimizer = self.initialize_optimizer()
        if not os.path.isdir(self.params['path']):
            os.makedirs(self.params['path'])

    def initialize_optimizer(self):
        optimizer = None
        optimizer_name = self.params['optimizer']
        if optimizer_name == 'ga':
            optimizer = GA
        elif optimizer_name == 'hill_climbing':
            optimizer = HillClimbing
        elif optimizer_name == 'random':
            optimizer = RandomSearch
        return optimizer(**self.params)

    def optimize(self):
        # optimize
        log = {'g': [], 'evals': [], 'fval': []}
        g_offset, evals_offset = 0, 0

        while True:
            self.optimizer.one_iteration()
            # stock log
            log['g'].append(self.optimizer.g + g_offset)
            log['evals'].append(self.optimizer.evals + evals_offset)
            log['fval'].append(self.optimizer.fval)
            print('generation: %d, evaluation count: %d, fval: %f, styles: %s\n'
                  % (self.optimizer.g + g_offset,
                     self.optimizer.evals + evals_offset, self.optimizer.fval,
                     self.optimizer.styles))
            if self.optimizer.evals >= self.params['max_evals'] or \
                    self.optimizer.done:
                if self.params['optimizer'] == 'ga' \
                        and self.params['pipe_to_hill_climbing']:
                    print('best: %f' % self.optimizer.best_fval)
                    print('best styles: %s' % self.optimizer.best_styles)
                    print('----------pipe to hill climbing----------\n')
                    self.params['pipe_to_hill_climbing'] = False
                    self.params['initial_styles'] = self.optimizer.styles
                    g_offset = self.optimizer.g
                    evals_offset = self.optimizer.evals
                    self.optimizer = HillClimbing(**self.params)
                else:
                    break

        # save log
        df = pd.DataFrame(log)
        df.index.name = '#index'
        df.to_csv('%s/log.csv' % self.params['path'], sep=',')
        best_info = {
            'best_styles': self.optimizer.best_styles,
            'best': self.optimizer.best_fval
        }
        json.dump(best_info, open('%s/best.json' % self.params['path'], 'w'))
        default_style = {}
        if os.path.exists('../default-style.json'):
            with open('../default-style.json') as f:
                default_style = json.load(f)
        best_styles = deepcopy(self.optimizer.best_styles)
        best_styles.update(default_style)
        best_styles = str(convert(best_styles))
        ret_val = sp.run(['sh', '../dump_config.sh', best_styles],
                         stdout=sp.PIPE).stdout.decode('utf-8')
        with open('%s/.clang-format' % self.params['path'], 'w') as f:
            f.write(ret_val)

        print('best: %f' % self.optimizer.best_fval)
        print('best styles: %s\n' % self.optimizer.best_styles)
