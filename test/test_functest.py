"""
Checks that the functional test program (functest) can be successfully built
and executed for every scheme/implementation.
"""

import os
import platform
import unittest

import pqclean
import helpers


def test_functest():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_functest, implementation


def test_functest_sanitizers():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_functest_sanitizers, implementation


@helpers.filtered_test
def check_functest(implementation):
    helpers.make('functest',
                 TYPE=implementation.scheme.type,
                 SCHEME=implementation.scheme.name,
                 IMPLEMENTATION=implementation.name,
                 working_dir=os.path.join('..', 'test'))
    helpers.run_subprocess(
        [os.path.join('..', 'bin', 'functest_{}_{}{}'.format(
            implementation.scheme.name,
            implementation.name,
            '.exe' if os.name == 'nt' else ''
        ))],
        os.path.join('..', 'bin'),
    )


@helpers.filtered_test
@helpers.skip_windows()
@helpers.slow_test
def check_functest_sanitizers(implementation):
    env = None
    if platform.machine() == 'ppc' and os.environ.get('CC', 'gcc') == 'clang':
        raise unittest.SkipTest("Clang does not support ASAN on ppc")
    elif platform.machine() in ['armv7l', 'aarch64']:
        env = {'ASAN_OPTIONS': 'detect_leaks=0'}
    elif platform.system() == 'Darwin':
        raise unittest.SkipTest('valgrind is not reliable on OSX')
    else:
        print("Supported platform: {}".format(platform.machine()))

    helpers.make('clean-scheme', 'functest',
                 TYPE=implementation.scheme.type,
                 SCHEME=implementation.scheme.name,
                 IMPLEMENTATION=implementation.name,
                 EXTRAFLAGS='-g -fsanitize=address,undefined',
                 working_dir=os.path.join('..', 'test'),
                 env=env)
    try:
        helpers.run_subprocess(
            [os.path.join('..', 'bin', 'functest_{}_{}{}'.format(
                implementation.scheme.name,
                implementation.name,
                '.exe' if os.name == 'nt' else ''
            ))],
            os.path.join('..', 'bin'),
            env=env,
        )
    except AssertionError as e:
        raise e
    finally:
        # Remove files with ASAN library compiled in
        helpers.make('clean-scheme',
                     TYPE=implementation.scheme.type,
                     SCHEME=implementation.scheme.name,
                     IMPLEMENTATION=implementation.name,
                     working_dir=os.path.join('..', 'test'))


if __name__ == '__main__':
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
