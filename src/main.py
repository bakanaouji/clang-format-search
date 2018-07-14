import json
import os

from function import Function, bool_keys
from optimizer.random_search import RandomSearch


def main():
    params = {
        'keys': bool_keys(),
        'obj_func': Function('../sample_src'),
        'path': '../log/random'
    }
    if not os.path.isdir(params['path']):
        os.makedirs(params['path'])
    optimizer = RandomSearch(**params)

    for i in range(10):
        optimizer.one_iteration()
        print(optimizer.g, optimizer.fval, optimizer.styles)

    # save best styles
    best_info = {
        'best_styles': optimizer.best_styles,
        'best': optimizer.best_fval
    }
    json.dump(best_info, open('%s/best.json' % params['path'], 'w'))

    print('best:', optimizer.best_fval)
    print('best styles:', optimizer.best_styles)


if __name__ == '__main__':
    main()
