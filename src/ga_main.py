import argparse
import json
import os
import pandas as pd

from function import Function, bool_keys
from optimizer.ga.ga import GA


def main():
    parser = argparse.ArgumentParser(description='Clang Format GA Search')
    parser.add_argument('--max_evals', type=int, default=500, help='Maximum number of evaluations.')
    parser.add_argument('--pop_size', type=int, default=100, help='Population size.')
    parser.add_argument('--tournament_size', type=int, default=3, help='Tournament size.')
    args = parser.parse_args()

    params = {
        'keys': bool_keys(),
        'obj_func': Function('../target_code'),
        'path': '../log/ga',
        'max_evals': args.max_evals,
        'pop_size': args.pop_size,
        'tournament_size': args.tournament_size
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
