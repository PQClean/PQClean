"""
Runs functional tests for common crypto functions (e.g., fips202, sha2, aes).
"""

import os
import re

import helpers


@helpers.skip_windows()
def test_common():
    for d in os.listdir('common'):
        primitive = re.sub(r"\.c$", "", d)
        if helpers.permit_test('common', None):
            yield check_common, primitive


def check_common(primitive):
    binname = os.path.join('..', 'bin', 'test_common_'+primitive)
    helpers.make(binname)
    helpers.run_subprocess([binname])


if __name__ == '__main__':
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
