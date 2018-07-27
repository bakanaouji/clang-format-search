import json
import os
import pandas as pd
import subprocess as sp

from copy import deepcopy
from function import Function, style_maps, convert
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
        'maps': style_maps(),
        'obj_func': Function('../target_code', '../default-style.json'),
        'path': '../log/' + optimizer_name,
        'max_evals': config['max_evals'],
        'pop_size': config['ga_pop_size'],
        'tournament_size': config['ga_tournament_size'],
        'pipe_to_hill_climbing': config['ga_pipe_to_hill_climbing']
    }
    if not os.path.isdir(params['path']):
        os.makedirs(params['path'])
    optimizer = optimizer(**params)

    # optimize
    log = {'g': [], 'evals': [], 'fval': []}
    g_offset, evals_offset = 0, 0
    while True:
        optimizer.one_iteration()
        # stock log
        log['g'].append(optimizer.g + g_offset)
        log['evals'].append(optimizer.evals + evals_offset)
        log['fval'].append(optimizer.fval)
        print(optimizer.g + g_offset, optimizer.evals + evals_offset,
              optimizer.fval, optimizer.styles)
        if optimizer.evals >= params['max_evals'] or optimizer.done:
            if optimizer_name == 'ga' and params['pipe_to_hill_climbing']:
                print('best:', optimizer.best_fval)
                print('best styles:', optimizer.best_styles)
                print('pipe to hill climbing')
                params['pipe_to_hill_climbing'] = False
                params['initial_styles'] = optimizer.styles
                g_offset = optimizer.g
                evals_offset = optimizer.evals
                optimizer = HillClimbing(**params)
            else:
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
