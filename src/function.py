import subprocess as sp


def bool_keys():
    return [
        'AlignConsecutiveAssignments',
        'AlignConsecutiveDeclarations',
        'AlignOperands',
        'AlignTrailingComments',
        'AllowAllParametersOfDeclarationOnNextLine',
        'AllowShortBlocksOnASingleLine',
        'AllowShortCaseLabelsOnASingleLine',
        'AllowShortIfStatementsOnASingleLine',
        'AllowShortLoopsOnASingleLine',
        'AlwaysBreakBeforeMultilineStrings',
        'AlwaysBreakTemplateDeclarations',
        'BinPackArguments',
        'BinPackParameters',
        'BreakBeforeInheritanceComma',
        'BreakBeforeTernaryOperators',
        'BreakConstructorInitializersBeforeComma',
        'BreakStringLiterals',
        'CompactNamespaces',
        'ConstructorInitializerAllOnOneLineOrOnePerLine',
        'Cpp11BracedListStyle',
        'DerivePointerAlignment',
        'FixNamespaceComments',
        'IndentCaseLabels',
        'IndentWrappedFunctionNames',
        'KeepEmptyLinesAtTheStartOfBlocks',
        'ReflowComments',
        'SortIncludes',
        'SortUsingDeclarations',
        'SpaceAfterCStyleCast',
        'SpaceAfterTemplateKeyword',
        'SpaceBeforeAssignmentOperators',
        'SpaceInEmptyParentheses',
        'SpacesInAngles',
        'SpacesInContainerLiterals',
        'SpacesInCStyleCastParentheses',
        'SpacesInParentheses',
        'SpacesInSquareBrackets'
    ]


def convert(styles):
    for key, val in styles.items():
        if type(val) == bool:
            if val:
                styles[key] = 'true'
            else:
                styles[key] = 'false'
    return str(styles)


class Function(object):
    def __init__(self, directory_path):
        self.path = directory_path

    def evaluate(self, styles):
        fval = 0
        files = [self.path + '/input.h',
                 self.path + '/input.cpp']
        for file in files:
            sp.run(
                'clang-format -i -style="' + str(convert(styles)) + '" ' + file,
                stdout=sp.PIPE)
            ret_val = sp.run('git diff --numstat ' + file, stdout=sp.PIPE)
            ret_val = ret_val.stdout.decode('utf-8').split('\t')
            if not ret_val == ['']:
                fval += int(ret_val[0]) + int(ret_val[1])
        sp.run('git checkout ' + self.path, stdout=sp.PIPE)
        return fval
