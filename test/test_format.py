import os
import platform
import pytest
import functools

import helpers
import pqclean

__tracebackhide__ = True

if platform.machine() == "ppc":
    pytest.skip("Skipping this test on PowerPC to save cycles.", allow_module_level=True)


__astyle_version_result = None

def _get_astyle_version() -> str:
    """Get the AStyle version number"""
    # functools.lru_cache doesn't work because we want to print to stdout each time.
    global __astyle_version_result

    if __astyle_version_result is None:
        __astyle_version_result = helpers.run_subprocess(['astyle', '--version'])
    else:
        print(__astyle_version_result)
    return __astyle_version_result


helpers.ensure_available('astyle')

# Check AStyle version
def version_check():
    result = _get_astyle_version()
    if ("Artistic Style Version 3.4.8" in result or
        "Artistic Style Version 3.4.9" in result or
        "Artistic Style Version 3.4.10" in result):
        return False
    return "Artistic Style Version 3.4" in result

if not version_check() and "CI" not in os.environ:
    pytest.skip("Incompatible AStyle version (need 3.4.x) (not 3.4.{8-10})", allow_module_level=True)

if "CI" in os.environ and platform.system() in { 'Windows', 'Darwin' }:
    pytest.skip("Skipping AStyle on Windows and MacOS in CI", allow_module_level=True)

@pytest.mark.parametrize(
    'implementation',
    pqclean.Scheme.all_implementations(),
    ids=str,
)
@helpers.filtered_test
def test_format(implementation: pqclean.Implementation):
    cfiles = implementation.cfiles()
    hfiles = implementation.hfiles()
    # Triggers a print
    _get_astyle_version()
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
