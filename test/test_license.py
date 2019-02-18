"""
Checks that a LICENSE or LICENSE.txt file is present for every
implementation of the specified scheme.
"""

import os
import pqclean


def test_license():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_license, scheme.name, implementation.name


def check_license(scheme_name, implementation_name):
    implementation = pqclean.Implementation.by_name(
        scheme_name, implementation_name)
    p1 = os.path.join(implementation.path(), 'LICENSE')
    p2 = os.path.join(implementation.path(), 'LICENSE.txt')
    assert(os.path.isfile(p1) or os.path.isfile(p2))


if __name__ == '__main__':
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
