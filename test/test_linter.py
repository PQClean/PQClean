import os
import unittest
from glob import glob

import pytest

import helpers
import pqclean

additional_flags = []


@pytest.mark.parametrize(
    'implementation',
    pqclean.Scheme.all_implementations(),
    ids=str,
)
@helpers.skip_windows()
@helpers.filtered_test
def test_clang_tidy(implementation: pqclean.Implementation):
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

    assert returncode == 0, "Clang-tidy returned %d" % returncode


if __name__ == "__main__":
    import sys
    # allow a user to specify --fix-errors, to immediately fix errors
    if len(sys.argv) >= 2 and sys.argv[1] == '-fix-errors':
        additional_flags = ['-fix-errors']
        del sys.argv[1]

    pytest.main(sys.argv)
