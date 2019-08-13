import os
import platform
import unittest
from glob import glob

import pytest

import helpers
import pqclean

additional_flags = [] #['-fix-errors']


@pytest.mark.parametrize(
    'implementation',
    pqclean.Scheme.all_supported_implementations(),
    ids=str,
)
@helpers.skip_windows()
@helpers.filtered_test
def test_clang_tidy(implementation: pqclean.Implementation):
    if platform.machine() in ['i386']:
        raise unittest.SkipTest("Clang-tidy has false-positives on i386")
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
    pytest.main(sys.argv)
