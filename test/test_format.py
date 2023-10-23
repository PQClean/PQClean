import os
import platform
import pytest

import helpers
import pqclean

__tracebackhide__ = True

if platform.machine() == "ppc":
    pytest.skip("Skipping this test on PowerPC to save cycles.", allow_module_level=True)

helpers.ensure_available('astyle')

# Check AStyle version
def version_check():
    result = helpers.run_subprocess(['astyle', '--version'])
    if ("Artistic Style Version 3.4.8" in result or
        "Artistic Style Version 3.4.9" in result):
        return False
    return "Artistic Style Version 3.4" in result

if not version_check() and "CI" not in os.environ:
    pytest.skip("Incompatible AStyle version (need 3.4.x) (not 3.4.{8,9})", allow_module_level=True)

@pytest.mark.parametrize(
    'implementation',
    pqclean.Scheme.all_implementations(),
    ids=str,
)
@helpers.filtered_test
def test_format(implementation: pqclean.Implementation):
    cfiles = implementation.cfiles()
    hfiles = implementation.hfiles()
    result = helpers.run_subprocess(
        ['astyle',
         '--dry-run',
         '--options=../.astylerc',
         *cfiles,
         *hfiles])
    assert 'Formatted' not in result


if __name__ == '__main__':
    import sys
    pytest.main(sys.argv)
