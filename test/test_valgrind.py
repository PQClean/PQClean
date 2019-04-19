"""
Runs the test files under valgrind to detect memory problems
"""

import os
import platform
import unittest

import pqclean
import helpers


def test_functest():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_valgrind, implementation


@helpers.slow_test
@helpers.filtered_test
def check_valgrind(implementation: pqclean.Implementation):
    if (platform.machine() not in ('i386', 'x86_64') or
            platform.system() != 'Linux'):
        raise unittest.SkipTest()

    helpers.make(TYPE=implementation.scheme.type,
                 SCHEME=implementation.scheme.name,
                 IMPLEMENTATION=implementation.name,
                 working_dir=os.path.join('..', 'test'))
    functest_name = './functest_{}_{}'.format(implementation.scheme.name,
                                              implementation.name)
    helpers.run_subprocess(['valgrind', functest_name],
                           os.path.join('..', 'bin'))


if __name__ == '__main__':
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
