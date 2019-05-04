import os
import subprocess as sp
import yaml

from copy import deepcopy


def style_maps():
    return {
        'AlignConsecutiveAssignments': ['true', 'false'],
        'AlignConsecutiveDeclarations': ['true', 'false'],
        'AlignOperands': ['true', 'false'],
        'AlignTrailingComments': ['true', 'false'],
        'AllowAllParametersOfDeclarationOnNextLine': ['true', 'false'],
        'AllowShortBlocksOnASingleLine': ['true', 'false'],
        'AllowShortCaseLabelsOnASingleLine': ['true', 'false'],
        'AllowShortIfStatementsOnASingleLine': ['true', 'false'],
        'AllowShortLoopsOnASingleLine': ['true', 'false'],
        'AlwaysBreakBeforeMultilineStrings': ['true', 'false'],
        'AlwaysBreakTemplateDeclarations': ['true', 'false'],
        'BinPackArguments': ['true', 'false'],
        'BinPackParameters': ['true', 'false'],
        'BreakBeforeInheritanceComma': ['true', 'false'],
        'BreakBeforeTernaryOperators': ['true', 'false'],
        'BreakConstructorInitializersBeforeComma': ['true', 'false'],
        'BreakStringLiterals': ['true', 'false'],
        'CompactNamespaces': ['true', 'false'],
        'ConstructorInitializerAllOnOneLineOrOnePerLine': ['true', 'false'],
        'Cpp11BracedListStyle': ['true', 'false'],
        'DerivePointerAlignment': ['true', 'false'],
        'FixNamespaceComments': ['true', 'false'],
        'IndentCaseLabels': ['true', 'false'],
        'IndentWrappedFunctionNames': ['true', 'false'],
        'KeepEmptyLinesAtTheStartOfBlocks': ['true', 'false'],
        'ReflowComments': ['true', 'false'],
        'SortIncludes': ['true', 'false'],
        'SortUsingDeclarations': ['true', 'false'],
        'SpaceAfterCStyleCast': ['true', 'false'],
        'SpaceAfterTemplateKeyword': ['true', 'false'],
        'SpaceBeforeAssignmentOperators': ['true', 'false'],
        'SpaceInEmptyParentheses': ['true', 'false'],
        'SpacesInAngles': ['true', 'false'],
        'SpacesInContainerLiterals': ['true', 'false'],
        'SpacesInCStyleCastParentheses': ['true', 'false'],
        'SpacesInParentheses': ['true', 'false'],
        'SpacesInSquareBrackets': ['true', 'false'],
        'AlignAfterOpenBracket': ['Align', 'DontAlign', 'AlwaysBreak'],
        'AlignEscapedNewlines': ['DontAlign', 'Left', 'Right'],
        'AllowShortFunctionsOnASingleLine': ['None', 'InlineOnly', 'Empty', 'Inline', 'All'],
        'AlwaysBreakAfterReturnType': ['None', 'All', 'TopLevel', 'AllDefinitions', 'TopLevelDefinitions'],
        'BreakBeforeBinaryOperators': ['None', 'NonAssignment', 'All'],
        'BreakConstructorInitializers': ['BeforeColon', 'BeforeComma', 'AfterColon'],
        'IncludeBlocks': ['Preserve', 'Merge', 'Regroup'],
        'IndentPPDirectives': ['None', 'AfterHash'],
        'NamespaceIndentation': ['None', 'Inner', 'All'],
        'PointerAlignment': ['Left', 'Middle', 'Right'],
        'SpaceBeforeParens': ['Never', 'ControlStatements', 'Always'],
        }


def convert(styles):
    for key, val in styles.items():
        if type(val) == bool:
            if val:
                styles[key] = 'true'
            else:
                styles[key] = 'false'
        # for nest block
        if type(val) == dict:
            for val_key, val_val in val.items():
                if type(val_val) == bool:
                    if val_val:
                        val[val_key] = 'true'
                    else:
                        val[val_key] = 'false'
    return styles


class Function(object):
    def __init__(self, directory_path, default_style_path=None):
        self.path = directory_path
        self.default_style = {}
        if default_style_path is not None and \
                os.path.exists(default_style_path):
            with open(default_style_path) as f:
                self.default_style = yaml.load(f)
                if self.default_style is None:
                    self.default_style = {}

    def evaluate(self, styles):
        merged_styles = deepcopy(styles)
        merged_styles.update(self.default_style)
        merged_styles = str(convert(merged_styles))
        return self.evaluate_core(merged_styles)

    def evaluate_dot_clang_format(self):
        return self.evaluate_core('file')

    def evaluate_core(self, styles_str):
        fval = 0
        sp.run(['sh', '../evaluate_target_code.sh', self.path, styles_str],
               stdout=sp.PIPE)
        ret_val = sp.run(['git', 'diff', '--numstat', self.path],
                         stdout=sp.PIPE)
        ret_vals = ret_val.stdout.decode('utf-8').split('\n')
        for val in ret_vals:
            split_val = val.split('\t')
            if not split_val == ['']:
                fval += int(split_val[0]) + int(split_val[1])
        sp.run(['git', 'checkout', self.path], stdout=sp.PIPE)
        return fval

