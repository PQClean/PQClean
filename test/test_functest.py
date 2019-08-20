"""
Checks that the functional test program (functest) can be successfully built
and executed for every scheme/implementation.
"""

import os
import platform
import unittest

import pytest

import helpers
import pqclean


@pytest.mark.parametrize(
    'implementation,test_dir,impl_path, init, destr',
    [(impl, *helpers.isolate_test_files(impl.path(), 'test_functest_'))
     for impl in pqclean.Scheme.all_supported_implementations()],
    ids=[str(impl) for impl in pqclean.Scheme.all_supported_implementations()],
)
@helpers.filtered_test
def test_functest(implementation, impl_path, test_dir,
                  init, destr):
    init()
    dest_dir = os.path.join(test_dir, 'bin')
    helpers.make('functest',
                 TYPE=implementation.scheme.type,
                 SCHEME=implementation.scheme.name,
                 IMPLEMENTATION=implementation.name,
                 SCHEME_DIR=impl_path,
                 DEST_DIR=dest_dir,
                 working_dir=os.path.join(test_dir, 'test'))
    helpers.run_subprocess(
        [os.path.join(dest_dir, 'functest_{}_{}{}'.format(
            implementation.scheme.name,
            implementation.name,
            '.exe' if os.name == 'nt' else ''
        ))],
    )
    destr()


@pytest.mark.parametrize(
    'implementation,test_dir,impl_path, init, destr',
    [(impl,
      *helpers.isolate_test_files(impl.path(), 'test_functest_sanitizers_'))
     for impl in pqclean.Scheme.all_supported_implementations()],
    ids=[str(impl) for impl in pqclean.Scheme.all_supported_implementations()],
)
@helpers.skip_windows()
@helpers.filtered_test
@helpers.slow_test
def test_functest_sanitizers(implementation, impl_path, test_dir,
                             init, destr):
    dest_dir = os.path.join(test_dir, 'bin')
    env = None
    if platform.machine() == 'ppc' and os.environ.get('CC', 'gcc') == 'clang':
        raise unittest.SkipTest("Clang does not support ASAN on ppc")
    elif platform.machine() in ['armv7l', 'aarch64']:
        env = {'ASAN_OPTIONS': 'detect_leaks=0'}
    elif platform.system() == 'Darwin':
        raise unittest.SkipTest('ASAN is not reliable on OSX')
    else:
        print("Supported platform: {}".format(platform.machine()))

    init()

    helpers.make('clean-scheme', 'functest',
                 TYPE=implementation.scheme.type,
                 SCHEME=implementation.scheme.name,
                 IMPLEMENTATION=implementation.name,
                 EXTRAFLAGS='-g -fsanitize=address,undefined',
                 SCHEME_DIR=impl_path,
                 DEST_DIR=dest_dir,
                 working_dir=os.path.join(test_dir, 'test'),
                 env=env)
    helpers.run_subprocess(
        [os.path.join(dest_dir, 'functest_{}_{}{}'.format(
            implementation.scheme.name,
            implementation.name,
            '.exe' if os.name == 'nt' else ''
        ))],
        env=env,
    )
    destr()


if __name__ == '__main__':
    import sys
    pytest.main(sys.argv)
