import argparse
import json
import os
import pandas as pd

from function import Function, bool_keys
from optimizer.random.random_search import RandomSearch


def main():
    parser = argparse.ArgumentParser(description='Clang Format Random Search')
    parser.add_argument('--max_evals', type=int, default=500, help='Maximum number of evaluations.')
    args = parser.parse_args()

    params = {
        'keys': bool_keys(),
        'obj_func': Function('../target_code'),
        'path': '../log/random_search',
        'max_evals': args.max_evals
    }
    if not os.path.isdir(params['path']):
        os.makedirs(params['path'])
    optimizer = RandomSearch(**params)

    # optimize
    log = {'g': [], 'evals': [], 'fval': []}
    for g in range(params['max_evals']):
        optimizer.one_iteration()
        # stock log
        log['g'].append(optimizer.g)
        log['evals'].append(optimizer.evals)
        log['fval'].append(optimizer.fval)
        print(optimizer.g, optimizer.fval, optimizer.styles)

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
