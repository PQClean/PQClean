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
def test_makefile_present(implementation):
    p1 = os.path.join(implementation.path(), 'Makefile')
    assert os.path.isfile(p1)


@pytest.mark.parametrize(
    'implementation',
    pqclean.Scheme.all_implementations(),
    ids=str,
)
@helpers.filtered_test
def test_microsoft_nmakefile_present(implementation):
    p2 = os.path.join(implementation.path(), 'Makefile.Microsoft_nmake')
    if implementation.supported_on_os(os='Windows'):
        assert os.path.isfile(p2)
    else:
        assert not os.path.isfile(p2), "Should not have an NMake file"


if __name__ == '__main__':
    import sys
    pytest.main(sys.argv)
