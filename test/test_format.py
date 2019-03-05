import shutil
import unittest

import pqclean
from helpers import run_subprocess


def test_formatting():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_format, implementation


def check_format(implementation: pqclean.Implementation):
    if shutil.which('astyle') is None:
        raise unittest.SkipTest("AStyle is not installed")
    cfiles = implementation.cfiles()
    hfiles = implementation.hfiles()
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
