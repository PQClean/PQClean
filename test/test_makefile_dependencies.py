"""
Checks that every .c and .h file in an implementation is present as a
dependency of that scheme's Makefile.
"""

import os
import pqclean
import helpers
import glob
import datetime
import unittest


def _skipped_test(*args, **kwargs):
    """Used to indicate skipped tests"""
    raise unittest.SkipTest("Skipped makefile dependencies test")


def test_makefile_dependencies():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            if not helpers.permit_test(
                    'makefile_dependencies', implementation):
                yield _skipped_test, implementation
                continue

            # initial build - want to have *all* files in place at beginning
            helpers.make('clean', working_dir=implementation.path())
            helpers.make(working_dir=implementation.path())
            # test case for each candidate file
            cfiles = glob.glob(os.path.join(implementation.path(), '*.c'))
            hfiles = glob.glob(os.path.join(implementation.path(), '*.h'))
            for file in (cfiles + hfiles):
                yield (check_makefile_dependencies, implementation, file)


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


def check_makefile_dependencies(implementation, file):
    cfiles = implementation.cfiles()
    hfiles = implementation.hfiles()
    ofiles = implementation.ofiles()

    libfile = os.path.join(implementation.path(), implementation.libname())

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
    make_check(implementation.path())

    # touch the candidate .c / .h file
    touch(now, file)

    # check if it needs to be rebuilt using make -q
    make_check(implementation.path(), expect_error=True)


if __name__ == '__main__':
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
