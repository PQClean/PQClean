import os
from glob import glob
import sys
import unittest

import pqclean
import helpers

additional_flags = []


def test_clang_tidy():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_tidy, implementation


@helpers.filtered_test
@helpers.skip_windows()
def check_tidy(implementation: pqclean.Implementation):
    helpers.ensure_available('clang-tidy')
    cfiles = implementation.cfiles()
    common_files = glob(os.path.join('..', 'common', '*.c'))
    (returncode, _) = helpers.run_subprocess(
        ['clang-tidy',
         '-quiet',
         '-header-filter=.*',
         *additional_flags,
         *cfiles,
         *common_files,
         '--',
         '-iquote', os.path.join('..', 'common'),
         '-iquote', implementation.path()],
        expected_returncode=None,
    )

    # Detect and gracefully avoid segfaults
    if returncode == -11:
        raise unittest.SkipTest("clang-tidy segfaulted")
    else:
        assert returncode == 0, "Clang-tidy returned %d" % returncode


if __name__ == "__main__":
    # allow a user to specify --fix-errors, to immediately fix errors
    if len(sys.argv) >= 2 and sys.argv[1] == '-fix-errors':
        additional_flags = ['-fix-errors']
        sys.argv = sys.argv[0:1] + sys.argv[2:]
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
