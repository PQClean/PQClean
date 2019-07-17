"""
Checks that no implementation makes use of symbolic links.
"""

import os
import pqclean
import pytest
import helpers


@pytest.mark.parametrize(
    'implementation',
    pqclean.Scheme.all_implementations(),
    ids=str,
)
@helpers.filtered_test
def test_no_symlinks(implementation):
    for file in os.listdir(implementation.path()):
        fpath = os.path.join(implementation.path(), file)
        if os.path.islink(fpath):
            raise AssertionError("{} is a symbolic link!".format(fpath))


if __name__ == '__main__':
    import pytest
    import sys
    pytest.main(sys.argv)
