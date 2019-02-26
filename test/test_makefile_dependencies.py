"""
Checks that every .c and .h file in an implementation is present as a
dependency of that scheme's Makefile.
"""

import hashlib
import os
import pqclean
import helpers
import subprocess
import glob


def test_makefile_dependencies():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            # initial build - want to have all files in place at beginning
            helpers.run_subprocess(['make', 'clean'], implementation.path())
            helpers.run_subprocess(['make'], implementation.path())
            # test case for each candidate file
            cfiles = glob.glob(os.path.join(implementation.path(), '*.c'))
            hfiles = glob.glob(os.path.join(implementation.path(), '*.h'))
            for file in cfiles + hfiles:
                yield check_makefile_dependencies, scheme.name, implementation.name, file

def check_makefile_dependencies(scheme_name, implementation_name, file):
    scheme = pqclean.Scheme.by_name(scheme_name)
    implementation = pqclean.Implementation.by_name(scheme_name, implementation_name)
    
    cfiles = glob.glob(os.path.join(implementation.path(), '*.c'))
    hfiles = glob.glob(os.path.join(implementation.path(), '*.h'))
    ofiles = glob.glob(os.path.join(implementation.path(), '*.o'))

    libfile = os.path.join(implementation.path(), implementation.libname())

    # modification time-based calculations is tricky on a sub-second basis
    # so we reset all the modification times to a known and "sensible" order
    helpers.run_subprocess(['touch'] + cfiles + hfiles + ofiles)
    helpers.run_subprocess(['touch', libfile])
    helpers.run_subprocess(['touch', '-A', '-15', '-m'] + cfiles + hfiles)
    helpers.run_subprocess(['touch', '-A', '-10', '-m'] + ofiles)
    helpers.run_subprocess(['touch', '-A', '-05', '-m', libfile])
    mtime_lib_orig = os.stat(libfile).st_mtime_ns

    # touch the candidate .c / .h file
    helpers.run_subprocess(['touch', '-A', '15', '-m', file])

    # rebuild
    helpers.run_subprocess(['make'], implementation.path())

    # make sure the libfile's modification time changed
    mtime_lib_upd = os.stat(libfile).st_mtime_ns
    if (mtime_lib_orig == mtime_lib_upd):
        print("ERROR: Library was not updated after touching {}".format(file))
    assert(mtime_lib_orig != mtime_lib_upd)

if __name__ == '__main__':
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
