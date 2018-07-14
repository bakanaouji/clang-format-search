import subprocess as sp


def main():
    sp.call('clang-format sample_src/input.h', shell=True)


if __name__ == '__main__':
    main()
