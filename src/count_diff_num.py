from function import Function


def main():
    obj_func = Function('../target_code', '../default-style.json')
    numstat = obj_func.evaluate_dot_clang_format()
    print('diff num: ', numstat)


if __name__ == '__main__':
    main()
