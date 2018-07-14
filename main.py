import numpy as np
import subprocess as sp


def evaluate(x):
    sp.run('clang-format -i -style="' + str(x) + '" sample_src/input.h', stdout=sp.PIPE)
    ret_val = sp.run('git diff --numstat sample_src/input.h', stdout=sp.PIPE)
    sp.run('git checkout sample_src/input.h', stdout=sp.PIPE)
    ret_val = ret_val.stdout.decode('utf-8').split('\t')
    return int(ret_val[0]) + int(ret_val[1])


def sample():
    styles = {}
    styles['ColumnLimit'] = np.random.randint(80)
    return str(styles)


def main():
    x = sample()
    eval = evaluate(x)
    print(eval)


if __name__ == '__main__':
    main()
