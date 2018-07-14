import numpy as np
import subprocess as sp


def evaluate(x):
    sp.run('clang-format -i -style="' + str(x) + '" sample_src/input.h', stdout=sp.PIPE)
    ret_val = sp.run('git diff --numstat sample_src/input.h', stdout=sp.PIPE)
    sp.run('git checkout sample_src/input.h', stdout=sp.PIPE)
    ret_val = ret_val.stdout.decode('utf-8').split('\t')
    return int(ret_val[0]) + int(ret_val[1])


def sample():
    styles = {
        "AlignConsecutiveAssignments": False,
        "AlignConsecutiveDeclarations": False,
        "AlignOperands":   True,
        "AlignTrailingComments": True,
        "AllowAllParametersOfDeclarationOnNextLine": True,
        "AllowShortBlocksOnASingleLine": False,
        "AllowShortCaseLabelsOnASingleLine": False,
        "AllowShortIfStatementsOnASingleLine": False,
        "AllowShortLoopsOnASingleLine": False,
        "AlwaysBreakBeforeMultilineStrings": False,
        "AlwaysBreakTemplateDeclarations": False,
        "BinPackArguments": True,
        "BinPackParameters": True,
        "BreakBeforeInheritanceComma": False,
        "BreakBeforeTernaryOperators": True,
        "BreakConstructorInitializersBeforeComma": False,
        "BreakAfterJavaFieldAnnotations": False,
        "BreakStringLiterals": True,
        "ColumnLimit":     80,
        "CompactNamespaces": False,
        "ConstructorInitializerAllOnOneLineOrOnePerLine": False,
        "ConstructorInitializerIndentWidth": 4,
        "ContinuationIndentWidth": 4,
        "Cpp11BracedListStyle": True,
        "DerivePointerAlignment": False,
        "DisableFormat":   False,
        "ExperimentalAutoDetectBinPacking": False,
        "FixNamespaceComments": True,
        "IndentCaseLabels": False,
        "IndentWidth":     2,
        "IndentWrappedFunctionNames": False,
        "JavaScriptWrapImports": True,
        "KeepEmptyLinesAtTheStartOfBlocks": True,
        "MaxEmptyLinesToKeep": 1,
        "ObjCBlockIndentWidth": 2,
        "ObjCSpaceAfterProperty": False,
        "ObjCSpaceBeforeProtocolList": True,
        "PenaltyBreakAssignment": 2,
        "PenaltyBreakBeforeFirstCallParameter": 19,
        "PenaltyBreakComment": 300,
        "PenaltyBreakFirstLessLess": 120,
        "PenaltyBreakString": 1000,
        "PenaltyExcessCharacter": 1000000,
        "PenaltyReturnTypeOnItsOwnLine": 60,
        "ReflowComments":  True,
        "SortIncludes":    True,
        "SortUsingDeclarations": True,
        "SpaceAfterCStyleCast": False,
        "SpaceAfterTemplateKeyword": True,
        "SpaceBeforeAssignmentOperators": True,
        "SpaceInEmptyParentheses": False,
        "SpacesBeforeTrailingComments": 1,
        "SpacesInAngles":  False,
        "SpacesInContainerLiterals": True,
        "SpacesInCStyleCastParentheses": False,
        "SpacesInParentheses": False,
        "SpacesInSquareBrackets": False
    }
    for key, val in styles.items():
        if type(val) == bool:
            if val:
                styles[key] = 'true'
            else:
                styles[key] = 'false'
    return str(styles)


def main():
    x = sample()
    eval = evaluate(x)
    print(eval)


if __name__ == '__main__':
    main()
