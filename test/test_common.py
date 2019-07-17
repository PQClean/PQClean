"""
Runs functional tests for common crypto functions (e.g., fips202, sha2, aes).

Doesn't currently need isolation for parallelisation
"""

import os
import re

import helpers


def pytest_generate_tests(metafunc):
    argvalues = []
    for d in os.listdir('test_common'):
        primitive = re.sub(r"\.c$", "", d)
        argvalues.append(primitive)
    metafunc.parametrize('primitive', argvalues)


@helpers.skip_windows()
@helpers.filtered_test
def test_common(primitive):
    binname = os.path.join('..', 'bin', 'test_common_'+primitive)
    helpers.make(binname)
    helpers.run_subprocess([binname])


if __name__ == '__main__':
    import pytest
    import sys
    pytest.main(sys.argv)
