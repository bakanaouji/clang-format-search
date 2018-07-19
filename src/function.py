import json
import os
import subprocess as sp

from copy import deepcopy
from pathlib import Path


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
    return str(styles)


class Function(object):
    def __init__(self, directory_path, default_style_path):
        self.path = directory_path
        self.files = []
        for file_name in Path(self.path).glob('**/*.h'):
            self.files.append(str(file_name))
        for file_name in Path(self.path).glob('**/*.hpp'):
            self.files.append(str(file_name))
        for file_name in Path(self.path).glob('**/*.cpp'):
            self.files.append(str(file_name))
        self.default_style = {}
        if os.path.exists(default_style_path):
            with open(default_style_path) as f:
                self.default_style = json.load(f)

    def evaluate(self, styles):
        fval = 0
        merged_styles = deepcopy(styles)
        merged_styles.update(self.default_style)
        merged_styles = str(convert(merged_styles))
        for file in self.files:
            sp.run(
                'clang-format -i -style="' + merged_styles
                + '" ' + file, stdout=sp.PIPE)
            ret_val = sp.run('git diff --numstat ' + file, stdout=sp.PIPE)
            ret_val = ret_val.stdout.decode('utf-8').split('\t')
            if not ret_val == ['']:
                fval += int(ret_val[0]) + int(ret_val[1])
        sp.run('git checkout ' + self.path, stdout=sp.PIPE)
        return fval

    def evaluate_dot_clang_format(self):
        fval = 0
        for file in self.files:
            sp.run(
                'clang-format -i -style=file ' + file, stdout=sp.PIPE)
            ret_val = sp.run('git diff --numstat ' + file, stdout=sp.PIPE)
            ret_val = ret_val.stdout.decode('utf-8').split('\t')
            if not ret_val == ['']:
                fval += int(ret_val[0]) + int(ret_val[1])
        sp.run('git checkout ' + self.path, stdout=sp.PIPE)
        return fval
