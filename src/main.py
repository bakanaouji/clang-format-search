from function import Function, bool_keys
from optimizer.random_search import RandomSearch


def convert(styles):
    for key, val in styles.items():
        if type(val) == bool:
            if val:
                styles[key] = 'true'
            else:
                styles[key] = 'false'
    return str(styles)


def main():
    obj_func = Function('../sample_src')
    optimizer = RandomSearch(bool_keys())

    best_fval = 1e10
    best_styles = None
    for i in range(10):
        styles = optimizer.sample()
        styles = convert(styles)
        fval = obj_func.evaluate(styles)
        if best_fval > fval:
            best_fval = fval
            best_styles = styles
        print(fval, styles)
    print(best_fval, best_styles)


if __name__ == '__main__':
    main()
