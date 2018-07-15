import json
import os
import pandas as pd

from function import Function, bool_keys
from optimizer.ga.ga import GA


def main():
    params = {
        'keys': bool_keys(),
        'obj_func': Function('../target_code'),
        'path': '../log/ga',
        'max_evals': 2000,
        'pop_size': 100,
        'tournament_size': 3
    }
    if not os.path.isdir(params['path']):
        os.makedirs(params['path'])
    optimizer = GA(**params)

    # optimize
    log = {'g': [], 'evals': [], 'fval': []}
    while optimizer.evals < params['max_evals']:
        optimizer.one_iteration()
        # stock log
        log['g'].append(optimizer.g)
        log['evals'].append(optimizer.evals)
        log['fval'].append(optimizer.fval)
        print(optimizer.g, optimizer.evals, optimizer.fval, optimizer.styles)

    # save log
    df = pd.DataFrame(log)
    df.index.name = '#index'
    df.to_csv('%s/log.csv' % params['path'], sep=',')
    best_info = {
        'best_styles': optimizer.best_styles,
        'best': optimizer.best_fval
    }
    json.dump(best_info, open('%s/best.json' % params['path'], 'w'))

    print('best:', optimizer.best_fval)
    print('best styles:', optimizer.best_styles)


if __name__ == '__main__':
    main()
