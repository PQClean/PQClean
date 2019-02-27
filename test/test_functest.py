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
            yield check_functest, scheme.name, implementation.name


def test_functest_sanitizers():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_functest_sanitizers, scheme.name, implementation.name


def check_functest(scheme_name, implementation_name):
    implementation = pqclean.Implementation.by_name(
        scheme_name, implementation_name)
    helpers.run_subprocess(
        ['make',
         'TYPE={}'.format(implementation.scheme.type),
         'SCHEME={}'.format(scheme_name),
         'IMPLEMENTATION={}'.format(implementation_name)],
        os.path.join('..', 'test')
    )
    helpers.run_subprocess(
        ['./functest_{}_{}'.format(scheme_name, implementation_name)],
        os.path.join('..', 'bin'),
    )


def check_functest_sanitizers(scheme_name, implementation_name):
    env = None
    if platform.machine() == 'powerpc':
        raise unittest.SkipTest()
    elif platform.machine() in ['arm32', 'aarch64']:
        env = {'ASAN_OPTIONS': 'detect_leaks=0'}
    else:
        print("Supported platform: {}".format(platform.machine))
    implementation = pqclean.Implementation.by_name(
        scheme_name, implementation_name)
    helpers.make('clean-scheme', 'functest',
                 TYPE=implementation.scheme.type,
                 SCHEME=scheme_name,
                 IMPLEMENTATION=implementation_name,
                 EXTRAFLAGS='-fsanitize=address,undefined',
                 working_dir=os.path.join('..', 'test'),
                 env=env)
    helpers.run_subprocess(
        ['./functest_{}_{}'.format(scheme_name, implementation_name)],
        os.path.join('..', 'bin'),
        env=env,
    )
    # Remove files with ASAN library compiled in
    helpers.make('clean-scheme',
                 TYPE=implementation.scheme.type,
                 SCHEME=scheme_name,
                 IMPLEMENTATION=implementation_name,
                 working_dir=os.path.join('..', 'test'))


if __name__ == '__main__':
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
