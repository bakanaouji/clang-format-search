import subprocess as sp


def main():
    sp.run('clang-format -i sample_src/input.h', stdout=sp.PIPE)
    ret_val = sp.run('git diff --numstat sample_src/input.h', stdout=sp.PIPE)
    sp.run('git checkout sample_src/input.h', stdout=sp.PIPE)
    print(ret_val.stdout)


if __name__ == '__main__':
    main()
