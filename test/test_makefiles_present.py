"""
Checks that a Makefile and a Makefile.Microsoft_nmake file is present for every
implementation of the specified scheme.
"""

import os
import pqclean
import helpers


def test_makefiles():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_makefiles, implementation


@helpers.filtered_test
def check_makefiles(implementation):
    p1 = os.path.join(implementation.path(), 'Makefile')
    p2 = os.path.join(implementation.path(), 'Makefile.Microsoft_nmake')
    assert(os.path.isfile(p1) and os.path.isfile(p2))


if __name__ == '__main__':
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
