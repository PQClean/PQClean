"""
Checks that every .c and .h file in an implementation is present as a
dependency of that scheme's Makefile.
"""

import os
import pqclean
import helpers
import glob
import datetime


def test_makefile_dependencies():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            # initial build - want to have *all* files in place at beginning
            helpers.run_subprocess(['make', 'clean'], implementation.path())
            helpers.run_subprocess(['make'], implementation.path())
            # test case for each candidate file
            cfiles = glob.glob(os.path.join(implementation.path(), '*.c'))
            hfiles = glob.glob(os.path.join(implementation.path(), '*.h'))
            for file in cfiles + hfiles:
                yield (check_makefile_dependencies, scheme.name,
                       implementation.name, file)


def check_makefile_dependencies(scheme_name, implementation_name, file):
    implementation = pqclean.Implementation.by_name(scheme_name,
                                                    implementation_name)

    cfiles = glob.glob(os.path.join(implementation.path(), '*.c'))
    hfiles = glob.glob(os.path.join(implementation.path(), '*.h'))
    ofiles = glob.glob(os.path.join(implementation.path(), '*.o'))

    libfile = os.path.join(implementation.path(), implementation.libname())

    # modification time-based calculations is tricky on a sub-second basis
    # so we reset all the modification times to a known and "sensible" order
    now = datetime.datetime.now()
    ago15 = now - datetime.timedelta(seconds=15)
    ago10 = now - datetime.timedelta(seconds=10)
    ago5 = now - datetime.timedelta(seconds=5)
    formatstring = "%Y%m%d%H%M.%S"
    helpers.run_subprocess(
        ['touch', '-t', ago15.strftime(formatstring)] + cfiles + hfiles)
    helpers.run_subprocess(
        ['touch', '-t', ago10.strftime(formatstring)] + ofiles)
    helpers.run_subprocess(
        ['touch', '-t', ago5.strftime(formatstring), libfile])

    # touch the candidate .c / .h file
    helpers.run_subprocess(['touch', '-t', now.strftime(formatstring), file])

    # check if it needs to be rebuilt using make -q
    helpers.run_subprocess(['make', '-q'], implementation.path(),
                           expected_returncode=1)


if __name__ == '__main__':
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
