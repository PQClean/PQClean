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
    if platform.machine() == 'armv7l' and 'gcc' in os.environ.get('CC', 'gcc') and 'falcon' in implementation.scheme.name:
        raise unittest.SkipTest("this test hangs for falcon on armv7l with gcc")
    init()
    dest_dir = os.path.join(test_dir, 'bin')
    # handle Falcon PADDED and COMPACT interop testing
    if implementation.scheme.name.startswith("falcon-"):
        if implementation.scheme.name.startswith("falcon-padded-"):
            # delete "-padded" to get interop scheme name
            interop_src = pqclean.Implementation.by_name(implementation.scheme.name.replace('-padded', '', 1), implementation.name).path()
        else:
            # add "-padded" to get interop scheme name
            interop_src = pqclean.Implementation.by_name(implementation.scheme.name.replace('falcon-', 'falcon-padded-', 1), implementation.name).path()
        interop_dir = helpers.add_interop_files(interop_src, os.path.join(impl_path, '..'))
        helpers.make('functest',
                TYPE=implementation.scheme.type,
                SCHEME=implementation.scheme.name,
                IMPLEMENTATION=implementation.name,
                INTEROP_DIR=interop_dir,
                SCHEME_DIR=impl_path,
                DEST_DIR=dest_dir,
                working_dir=os.path.join(test_dir, 'test'))
    else:
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
def test_functest_sanitizers(implementation, impl_path, test_dir,
                             init, destr):
    dest_dir = os.path.join(test_dir, 'bin')
    env = None
    if platform.machine() == 'ppc' and 'clang' in os.environ.get('CC', 'gcc'):
        raise unittest.SkipTest("Clang does not support ASAN on ppc")
    elif platform.machine() == 'armv7l' and 'clang' in os.environ.get('CC', 'gcc'):
        raise unittest.SkipTest("A bug with asan on armv7l, see #471")
    elif platform.machine() == 'armv7l' and 'gcc' in os.environ.get('CC', 'gcc') and ('sphincs' in implementation.scheme.name or 'falcon' in implementation.scheme.name):
        raise unittest.SkipTest("asan for sphincs and falcon on armv7l with gcc hangs, see #470")
    elif platform.machine() in ['armv7l', 'aarch64']:
        env = {'ASAN_OPTIONS': 'detect_leaks=0'}
    elif platform.system() == 'Darwin':
        raise unittest.SkipTest('ASAN is not reliable on OSX')
    else:
        print("Supported platform: {}".format(platform.machine()))

    init()

    # handle Falcon PADDED and COMPACT interop testing
    if implementation.scheme.name.startswith("falcon-"):
        if implementation.scheme.name.startswith("falcon-padded-"):
            # delete "-padded" to get interop scheme name
            interop_src = pqclean.Implementation.by_name(implementation.scheme.name.replace('-padded', '', 1), implementation.name).path()
        else:
            # add "-padded" to get interop scheme name
            interop_src = pqclean.Implementation.by_name(implementation.scheme.name.replace('falcon-', 'falcon-padded-', 1), implementation.name).path()
        interop_dir = helpers.add_interop_files(interop_src, os.path.join(impl_path, '..'))

        helpers.make('clean-scheme', 'clean-interop', 'functest',
                 TYPE=implementation.scheme.type,
                 SCHEME=implementation.scheme.name,
                 IMPLEMENTATION=implementation.name,
                 EXTRAFLAGS=(
                     '-g -fsanitize=address,undefined '
                     '-fno-sanitize-recover=undefined '
                     # TODO(JMS) Remove explicit -latomic if/when gcc fixes:
                     # https://gcc.gnu.org/bugzilla/show_bug.cgi?id=81358
                     '-Wno-unused-command-line-argument -latomic'),
                 SCHEME_DIR=impl_path,
                 INTEROP_DIR=interop_dir,
                 DEST_DIR=dest_dir,
                 working_dir=os.path.join(test_dir, 'test'),
                 env=env)
    else:
        helpers.make('clean-scheme', 'functest',
                 TYPE=implementation.scheme.type,
                 SCHEME=implementation.scheme.name,
                 IMPLEMENTATION=implementation.name,
                 EXTRAFLAGS=(
                     '-g -fsanitize=address,undefined '
                     '-fno-sanitize-recover=undefined '
                     # TODO(JMS) Remove explicit -latomic if/when gcc fixes:
                     # https://gcc.gnu.org/bugzilla/show_bug.cgi?id=81358
                     '-Wno-unused-command-line-argument -latomic'),
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
