
"""
Checks that the implementation does not make use of the `char` type.
This is ambiguous; compilers can freely choose `signed` or `unsigned` char.
"""

import os

import pytest

import helpers
import pqclean
import pycparser


def setup_module():
    if not(os.path.exists(os.path.join('pycparser', '.git'))):
        print("Please run `git submodule update --init`")


def walk_tree(ast):
    if type(ast) is pycparser.c_ast.IdentifierType:
        if ast.names == ['char']:
            yield ast

    for (_, child) in ast.children():
        yield from walk_tree(child)  # recursively yield prohibited nodes


@pytest.mark.parametrize(
    'implementation',
    pqclean.Scheme.all_implementations(),
    ids=str,
)
@helpers.filtered_test
@helpers.skip_windows()
def test_char(implementation):
    errors = []
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
                '-I{}'.format(os.path.join(tdir, "../common")),
                # necessary to mock e.g. <stdint.h>
                '-I{}'.format(
                    os.path.join(tdir, 'pycparser/utils/fake_libc_include')),
            ]
        )
        for node in walk_tree(ast):
            # flatten nodes to a string to easily enforce uniqueness
            err = "\n at {c.file}:{c.line}:{c.column}".format(c=node.coord)
            if err not in errors:
                errors.append(err)
    if errors:
        raise AssertionError(
            "Prohibited use of char without explicit signed/unsigned" +
            "".join(errors)
        )


if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)
