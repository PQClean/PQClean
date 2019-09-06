"""
Runs the test files under valgrind to detect memory problems
"""

import functools
import os
import platform
import re
import unittest

import pytest

import helpers
import pqclean


@functools.lru_cache()
def valgrind_supports_exit_early():
    """Checks if we support early exit from valgrind"""
    version = helpers.run_subprocess(['valgrind', '--version'])
    match = re.match(r'valgrind-(\d)\.(\d+).*', version)
    if match:
        return int(match.group(2)) >= 14
    return False


@pytest.mark.parametrize(
    'implementation,test_dir,impl_path, init, destr',
    [(impl, *helpers.isolate_test_files(impl.path(), 'test_functest_'))
     for impl in pqclean.Scheme.all_supported_implementations()],
    ids=[str(impl) for impl in pqclean.Scheme.all_supported_implementations()],
)
@helpers.slow_test
@helpers.filtered_test
def test_valgrind(implementation: pqclean.Implementation, impl_path, test_dir,
                  init, destr):
    if (platform.machine() not in ('i386', 'x86_64') or
            platform.system() != 'Linux'):
        raise unittest.SkipTest()
    init()

    dest_dir = os.path.join(test_dir, 'bin')

    helpers.make(TYPE=implementation.scheme.type,
                 SCHEME=implementation.scheme.name,
                 SCHEME_DIR=os.path.abspath(impl_path),
                 IMPLEMENTATION=implementation.name,
                 DEST_DIR=dest_dir,
                 NTESTS=1,
                 working_dir=os.path.join(test_dir, 'test'))
    functest_name = './functest_{}_{}'.format(implementation.scheme.name,
                                              implementation.name)
    helpers.run_subprocess(
        ['valgrind',
         '--error-exitcode=1',
         *(['--exit-on-first-error=yes']
           if valgrind_supports_exit_early()
           else []),
         '--max-stackframe=20933064',
         '--vex-guest-max-insns=25',
         functest_name],
        dest_dir)
    destr()


if __name__ == '__main__':
    import sys
    pytest.main(sys.argv)
