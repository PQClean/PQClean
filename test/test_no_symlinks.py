"""
Checks that no implementation makes use of symbolic links.
"""

import os
import pqclean
import helpers


def test_no_symlinks():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            if helpers.permit_test('no_symlinks', implementation):
                yield check_no_symlinks, implementation


def check_no_symlinks(implementation):
    for file in os.listdir(implementation.path()):
        fpath = os.path.join(implementation.path(), file)
        if os.path.islink(fpath):
            raise AssertionError("{} is a symbolic link!".format(fpath))


if __name__ == '__main__':
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
