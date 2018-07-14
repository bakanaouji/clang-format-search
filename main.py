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
        "AlignConsecutiveAssignments": bool(np.random.randint(2)),
        "AlignConsecutiveDeclarations": bool(np.random.randint(2)),
        "AlignOperands":   bool(np.random.randint(2)),
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
        "BreakAfterJavaFieldAnnotations": bool(np.random.randint(2)),
        "BreakStringLiterals": bool(np.random.randint(2)),
        "ColumnLimit":     80,
        "CompactNamespaces": bool(np.random.randint(2)),
        "ConstructorInitializerAllOnOneLineOrOnePerLine": bool(np.random.randint(2)),
        "ConstructorInitializerIndentWidth": 4,
        "ContinuationIndentWidth": 4,
        "Cpp11BracedListStyle": bool(np.random.randint(2)),
        "DerivePointerAlignment": bool(np.random.randint(2)),
        "DisableFormat":   bool(np.random.randint(2)),
        "ExperimentalAutoDetectBinPacking": bool(np.random.randint(2)),
        "FixNamespaceComments": bool(np.random.randint(2)),
        "IndentCaseLabels": bool(np.random.randint(2)),
        "IndentWidth":     2,
        "IndentWrappedFunctionNames": bool(np.random.randint(2)),
        "JavaScriptWrapImports": bool(np.random.randint(2)),
        "KeepEmptyLinesAtTheStartOfBlocks": bool(np.random.randint(2)),
        "MaxEmptyLinesToKeep": 1,
        "ObjCBlockIndentWidth": 2,
        "ObjCSpaceAfterProperty": bool(np.random.randint(2)),
        "ObjCSpaceBeforeProtocolList": bool(np.random.randint(2)),
        "PenaltyBreakAssignment": 2,
        "PenaltyBreakBeforeFirstCallParameter": 19,
        "PenaltyBreakComment": 300,
        "PenaltyBreakFirstLessLess": 120,
        "PenaltyBreakString": 1000,
        "PenaltyExcessCharacter": 1000000,
        "PenaltyReturnTypeOnItsOwnLine": 60,
        "ReflowComments":  bool(np.random.randint(2)),
        "SortIncludes":    bool(np.random.randint(2)),
        "SortUsingDeclarations": bool(np.random.randint(2)),
        "SpaceAfterCStyleCast": bool(np.random.randint(2)),
        "SpaceAfterTemplateKeyword": bool(np.random.randint(2)),
        "SpaceBeforeAssignmentOperators": bool(np.random.randint(2)),
        "SpaceInEmptyParentheses": bool(np.random.randint(2)),
        "SpacesBeforeTrailingComments": 1,
        "SpacesInAngles":  bool(np.random.randint(2)),
        "SpacesInContainerLiterals": bool(np.random.randint(2)),
        "SpacesInCStyleCastParentheses": bool(np.random.randint(2)),
        "SpacesInParentheses": bool(np.random.randint(2)),
        "SpacesInSquareBrackets": bool(np.random.randint(2))
    }
    for key, val in styles.items():
        if type(val) == bool:
            if val:
                styles[key] = 'true'
            else:
                styles[key] = 'false'
    return str(styles)


def main():
    for i in range(10):
        x = sample()
        eval = evaluate(x)
        print(x, eval)


if __name__ == '__main__':
    main()
