import os
from glob import glob

import pqclean
from helpers import run_subprocess


def test_formatting():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_format, implementation


def check_format(implementation: pqclean.Implementation):
    cfiles = glob(os.path.join(implementation.path(), '*.c'))
    hfiles = glob(os.path.join(implementation.path(), '*.h'))
    run_subprocess(['astyle',
                    '--dry-run',
                    '--options=../.astylerc',
                    *cfiles,
                    *hfiles])


if __name__ == "__main__":
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
