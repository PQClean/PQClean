"""
Checks that every .c and .h file in an implementation is present as a
dependency of that scheme's Makefile.
"""

import datetime
import glob
import os

import pytest

import helpers
import pqclean


@pytest.mark.parametrize(
    'implementation,test_dir,impl_path, init, destr',
    [(impl,
      *helpers.isolate_test_files(impl.path(), 'test_makefile_deps_'))
     for impl in pqclean.Scheme.all_implementations()],
    ids=[str(impl) for impl in pqclean.Scheme.all_implementations()],
)
@helpers.filtered_test
def test_makefile_dependencies(implementation, impl_path, test_dir,
                               init, destr):
    init()
    # initial build - want to have *all* files in place at beginning
    helpers.make('clean', working_dir=impl_path)
    helpers.make(working_dir=impl_path)
    # test case for each candidate file
    cfiles = glob.glob(os.path.join(impl_path, '*.c'))
    hfiles = glob.glob(os.path.join(impl_path, '*.h'))
    for file in (cfiles + hfiles):
        check_makefile_dependencies(implementation, impl_path, file)
    destr()


def touch(time, *files):
    for path in files:
        times = (time.timestamp(), time.timestamp())
        os.utime(path, times)


def make_check(path, expect_error=False):
    makeflag = '-q' if os.name != 'nt' else '/Q'
    expected_returncode = 0
    if expect_error:
        expected_returncode = 1 if os.name != 'nt' else 255
    helpers.make(makeflag, working_dir=path,
                 expected_returncode=expected_returncode)


def check_makefile_dependencies(implementation, impl_path, file):
    cfiles = glob.glob(os.path.join(impl_path, '*.c'))
    hfiles = glob.glob(os.path.join(impl_path, '*.h'))
    ofiles = glob.glob(
        os.path.join(impl_path,
                     '*.o' if os.name != 'nt' else '*.obj'))

    libfile = os.path.join(impl_path, implementation.libname())

    # modification time-based calculations is tricky on a sub-second basis
    # so we reset all the modification times to a known and "sensible" order
    now = datetime.datetime.now() - datetime.timedelta(seconds=10)
    ago15 = now - datetime.timedelta(minutes=15)
    ago10 = now - datetime.timedelta(minutes=10)
    ago5 = now - datetime.timedelta(minutes=5)

    touch(ago15, *cfiles, *hfiles)
    touch(ago10, *ofiles)
    touch(ago5, libfile)

    # Sanity check: the scheme is up to date
    make_check(impl_path)

    # touch the candidate .c / .h file
    touch(now, file)

    # check if it needs to be rebuilt using make -q
    make_check(impl_path, expect_error=True)


if __name__ == '__main__':
    import sys
    pytest.main(sys.argv)
