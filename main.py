import subprocess as sp


def evaluate():
    sp.run('clang-format -i sample_src/input.h', stdout=sp.PIPE)
    ret_val = sp.run('git diff --numstat sample_src/input.h', stdout=sp.PIPE)
    sp.run('git checkout sample_src/input.h', stdout=sp.PIPE)
    ret_val = ret_val.stdout.decode('utf-8').split('\t')
    return int(ret_val[0]) + int(ret_val[1])


def main():
    eval = evaluate()
    print(eval)


if __name__ == '__main__':
    main()
