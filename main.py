import numpy as np
import subprocess as sp


def evaluate(styles):
    eval = 0
    files = ['sample_src/input.h',
             'sample_src/input.cpp']
    for file in files:
        sp.run('clang-format -i -style="' + str(styles) + '" ' + file,
               stdout=sp.PIPE)
        ret_val = sp.run('git diff --numstat ' + file, stdout=sp.PIPE)
        ret_val = ret_val.stdout.decode('utf-8').split('\t')
        if not ret_val == ['']:
            eval += int(ret_val[0]) + int(ret_val[1])
    sp.run('git checkout sample_src', stdout=sp.PIPE)
    return eval


def sample():
    styles = {
        "AlignConsecutiveAssignments": bool(np.random.randint(2)),
        "AlignConsecutiveDeclarations": bool(np.random.randint(2)),
        "AlignOperands": bool(np.random.randint(2)),
        "AlignTrailingComments": bool(np.random.randint(2)),
        "AllowAllParametersOfDeclarationOnNextLine": bool(np.random.randint(2)),
        "AllowShortBlocksOnASingleLine": bool(np.random.randint(2)),
        "AllowShortCaseLabelsOnASingleLine": bool(np.random.randint(2)),
        "AllowShortIfStatementsOnASingleLine": bool(np.random.randint(2)),
        "AllowShortLoopsOnASingleLine": bool(np.random.randint(2)),
        "AlwaysBreakBeforeMultilineStrings": bool(np.random.randint(2)),
        "AlwaysBreakTemplateDeclarations": bool(np.random.randint(2)),
        "BinPackArguments": bool(np.random.randint(2)),
        "BinPackParameters": bool(np.random.randint(2)),
        "BreakBeforeInheritanceComma": bool(np.random.randint(2)),
        "BreakBeforeTernaryOperators": bool(np.random.randint(2)),
        "BreakConstructorInitializersBeforeComma": bool(np.random.randint(2)),
        "BreakStringLiterals": bool(np.random.randint(2)),
        "CompactNamespaces": bool(np.random.randint(2)),
        "ConstructorInitializerAllOnOneLineOrOnePerLine": bool(
            np.random.randint(2)),
        "Cpp11BracedListStyle": bool(np.random.randint(2)),
        "DerivePointerAlignment": bool(np.random.randint(2)),
        "FixNamespaceComments": bool(np.random.randint(2)),
        "IndentCaseLabels": bool(np.random.randint(2)),
        "IndentWrappedFunctionNames": bool(np.random.randint(2)),
        "KeepEmptyLinesAtTheStartOfBlocks": bool(np.random.randint(2)),
        "ReflowComments": bool(np.random.randint(2)),
        "SortIncludes": bool(np.random.randint(2)),
        "SortUsingDeclarations": bool(np.random.randint(2)),
        "SpaceAfterCStyleCast": bool(np.random.randint(2)),
        "SpaceAfterTemplateKeyword": bool(np.random.randint(2)),
        "SpaceBeforeAssignmentOperators": bool(np.random.randint(2)),
        "SpaceInEmptyParentheses": bool(np.random.randint(2)),
        "SpacesInAngles": bool(np.random.randint(2)),
        "SpacesInContainerLiterals": bool(np.random.randint(2)),
        "SpacesInCStyleCastParentheses": bool(np.random.randint(2)),
        "SpacesInParentheses": bool(np.random.randint(2)),
        "SpacesInSquareBrackets": bool(np.random.randint(2))
    }
    return styles


def convert(styles):
    for key, val in styles.items():
        if type(val) == bool:
            if val:
                styles[key] = 'true'
            else:
                styles[key] = 'false'
    return str(styles)


def main():
    for i in range(10):
        styles = sample()
        styles = convert(styles)
        eval = evaluate(styles)
        print(styles, eval)


if __name__ == '__main__':
    main()
