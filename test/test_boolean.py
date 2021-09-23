
"""
Checks that the implementation does not make use of boolean operations (==, <=, !, etc)
in assignments or function calls.
"""

import os

import pytest
import unittest

import helpers
import platform
import pqclean
import pycparser


def setup_module():
    if not(os.path.exists(os.path.join('pycparser', '.git'))):
        print("Please run `git submodule update --init`")


class ForbiddenLineVisitor(pycparser.c_ast.NodeVisitor): 
  def __init__(self):
    self.errors = []

  def visit_Assignment(self, node): 
    v = ForbiddenOpVisitor();
    v.visit(node.rvalue)
    self.errors.extend(v.errors)

  def visit_Decl(self, node):
    if node.init:
        v = ForbiddenOpVisitor();
        v.visit(node.init)
        self.errors.extend(v.errors)

  def visit_FuncCall(self, node): 
    if node.args:
        v = ForbiddenOpVisitor();
        v.visit(node.args)
        self.errors.extend(v.errors)

class ForbiddenOpVisitor(pycparser.c_ast.NodeVisitor): 
  def __init__(self):
    self.errors = []

  def visit_BinaryOp(self, node): 
    v = ForbiddenOpVisitor();
    v.visit(node.left)
    self.errors.extend(v.errors)
    if node.op in ['<', '<=', '>', '>=', '==', '!=', '&&', '||']:
      err = "\n {} at {c.file}:{c.line}:{c.column}".format(node.op, c=node.coord)
      self.errors.append(err)
    v = ForbiddenOpVisitor();
    v.visit(node.right)
    self.errors.extend(v.errors)

  def visit_UnaryOp(self, node): 
    if node.op == '!':
      err = "\n {} at {c.file}:{c.line}:{c.column}".format(node.op, c=node.coord)
      self.errors.append(err)
    v = ForbiddenOpVisitor();
    v.visit(node.expr)
    self.errors.extend(v.errors)

  def visit_TernaryOp(self, node): 
    err = "\n ternary operator at {c.file}:{c.line}:{c.column}".format(c=node.coord)
    self.errors.append(err)


@pytest.mark.parametrize(
    'implementation',
    pqclean.Scheme.all_implementations(),
    ids=str,
)
@helpers.skip_windows()
@helpers.filtered_test
def test_boolean(implementation):
    errors = []
    # pyparser's fake_libc does not support the Arm headers
    if 'supported_platforms' in implementation.metadata():
        for platform in implementation.metadata()['supported_platforms']:
            if platform['architecture'] == "arm_8":
                raise unittest.SkipTest()

    for fname in os.listdir(implementation.path()):
        if not fname.endswith(".c"):
            continue
        tdir, _ = os.path.split(os.path.realpath(__file__))
        ast = pycparser.parse_file(
            os.path.join(implementation.path(), fname),
            use_cpp=True,
            cpp_path='cc',  # not all platforms link cpp correctly; cc -E works
            cpp_args=[
                '-E',
                '-std=c99',
                '-nostdinc',  # pycparser cannot deal with e.g. __attribute__
                '-I{}'.format(os.path.join(tdir, "common")),  # include test common files to overrule compat.h
                '-I{}'.format(os.path.join(tdir, "../common")),
                # necessary to mock e.g. <stdint.h>
                '-I{}'.format(
                    os.path.join(tdir, 'pycparser/utils/fake_libc_include')),
            ]
        )
        v = ForbiddenLineVisitor()
        v.visit(ast)
        errors.extend(v.errors)
    if errors:
        raise AssertionError(
            "Prohibited use of boolean operations in assignment or function call" +
            "".join(errors)
        )


if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)
