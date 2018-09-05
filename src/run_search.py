import yaml

from function import Function, style_maps
from searcher import Searcher


def initialize_parameter():
    with open('../config.yml') as f:
        config = yaml.load(f)
    params = {
        'maps': style_maps(),
        'obj_func': Function('../target_code', '../default-style.json'),
        'optimizer': config['optimizer'],
        'path': '../log/' + config['optimizer'],
        'max_evals': config['max_evals'],
        'pop_size': config['ga_pop_size'],
        'tournament_size': config['ga_tournament_size'],
        'pipe_to_hill_climbing': config['ga_pipe_to_hill_climbing']
    }
    return params


def main():
    params = initialize_parameter()

    print('----------run %s search----------\n' % params['optimizer'])
    searcher = Searcher(**params)
    searcher.optimize()


if __name__ == '__main__':
    main()
