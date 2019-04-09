"""
Catch-early check that sees if OBJECTS matches across Makefile and NMakefile
"""

import os
import pqclean


def test_makefile():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_makefile, implementation


def check_makefile(implementation):
    make = os.path.join(implementation.path(), 'Makefile')
    nmake = os.path.join(implementation.path(), 'Makefile.Microsoft_nmake')
    err = []
    with open(make) as f:
        for line in f:
            if line.startswith("OBJECTS"):
                make_objects = line.split('=')[1].split()
    with open(nmake) as f:
        for line in f:
            if line.startswith("OBJECTS"):
                nmake_objects = line.split('=')[1].split()
    for o in make_objects:
        o = o.replace('.o', '.obj')
        if o not in nmake_objects:
            err.append("\n {} missing from Makefile.Microsoft_nmake".format(o))
    for o in nmake_objects:
        o = o.replace('.obj', '.o')
        if o not in make_objects:
            err.append("\n {} missing from Makefile".format(o))
    if err:
        raise AssertionError(
            "OBJECTS inconsistent across Makefile and Makefile.Microsoft_nmake"
            + "".join(err)
        )

if __name__ == '__main__':
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
