import os
import yaml

from copy import deepcopy
from function import Function, convert, style_maps


def main():
    obj_func = Function('../target_code', '../default-style.json')

    best_styles = {}
    if os.path.exists('../.clang-format'):
        with open('../.clang-format') as f:
            best_styles = yaml.load(f)

    maps = style_maps()
    keys = []
    for key, vals in maps.items():
        keys.append(key)
    keys = sorted(keys)

    exclusion_keys = [key for key in best_styles.keys() if not (key in keys)]
    for key in exclusion_keys:
        best_styles.pop(key)

    best_styles = convert(best_styles)
    best_fval = obj_func.evaluate(best_styles)
    print('best styles: %s, best fval: %f\n' % (best_styles, best_fval))

    influential_keys = []
    uninfluential_keys = []

    for key in keys:
        is_influential = False
        vals = maps[key]
        vals = [val for val in vals if val != best_styles[key]]

        for val in vals:
            styles = deepcopy(best_styles)
            styles[key] = val
            fval = obj_func.evaluate(styles)
            print('key: %s, val: %s, fval: %f' % (key, val, fval))
            if fval != best_fval:
                is_influential = True

        print('%s is %s\n' %
              (key, 'influential' if is_influential else 'uninfluential'))

        if is_influential:
            influential_keys.append(key)
        else:
            uninfluential_keys.append(key)

    print('influential keys: %s\n' % influential_keys)
    print('uninfluential keys: %s\n' % uninfluential_keys)


if __name__ == '__main__':
    main()
