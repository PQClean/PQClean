"""
Checks that a Makefile and a Makefile.Microsoft_nmake file is present for every
implementation of the specified scheme.
"""

import os

import pytest

import helpers
import pqclean


@pytest.mark.parametrize(
    'implementation',
    pqclean.Scheme.all_implementations(),
    ids=str,
)
@helpers.filtered_test
def test_makefiles_present(implementation):
    p1 = os.path.join(implementation.path(), 'Makefile')
    p2 = os.path.join(implementation.path(), 'Makefile.Microsoft_nmake')
    assert(os.path.isfile(p1) and os.path.isfile(p2))


if __name__ == '__main__':
    import sys
    pytest.main(sys.argv)
