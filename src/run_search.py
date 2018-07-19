import json
import os
import pandas as pd
import subprocess as sp

from copy import deepcopy
from function import Function, bool_keys, convert
from optimizer.ga.ga import GA
from optimizer.hill_climbing.hill_climbing import HillClimbing
from optimizer.random.random_search import RandomSearch


def main():
    optimizer = None
    with open('../config.json') as f:
        config = json.load(f)
    optimizer_name = config['optimizer']
    if optimizer_name == 'ga':
        optimizer = GA
    elif optimizer_name == 'hill_climbing':
        optimizer = HillClimbing
    elif optimizer_name == 'random':
        optimizer = RandomSearch

    params = {
        'keys': bool_keys(),
        'obj_func': Function('../target_code', '../default-style.json'),
        'path': '../log/' + optimizer_name,
        'max_evals': config['max_evals'],
        'pop_size': config['ga_pop_size'],
        'tournament_size': config['ga_tournament_size']
    }
    if not os.path.isdir(params['path']):
        os.makedirs(params['path'])
    optimizer = optimizer(**params)

    # optimize
    log = {'g': [], 'evals': [], 'fval': []}
    while optimizer.evals < params['max_evals']:
        optimizer.one_iteration()
        # stock log
        log['g'].append(optimizer.g)
        log['evals'].append(optimizer.evals)
        log['fval'].append(optimizer.fval)
        print(optimizer.g, optimizer.evals, optimizer.fval, optimizer.styles)
        if optimizer.done:
            break

    # save log
    df = pd.DataFrame(log)
    df.index.name = '#index'
    df.to_csv('%s/log.csv' % params['path'], sep=',')
    best_info = {
        'best_styles': optimizer.best_styles,
        'best': optimizer.best_fval
    }
    json.dump(best_info, open('%s/best.json' % params['path'], 'w'))
    default_style = {}
    if os.path.exists('../default-style.json'):
        with open('../default-style.json') as f:
            default_style = json.load(f)
    best_styles = deepcopy(optimizer.best_styles)
    best_styles.update(default_style)
    best_styles = str(convert(best_styles))
    ret_val = sp.run('clang-format -style="' + best_styles + '" -dump-config',
                     stdout=sp.PIPE).stdout.decode('utf-8')
    with open('%s/.clang-format' % params['path'], 'w') as f:
        f.write(ret_val)

    print('best:', optimizer.best_fval)
    print('best styles:', optimizer.best_styles)


if __name__ == '__main__':
    main()
