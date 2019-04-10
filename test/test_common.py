"""
Runs functional tests for common crypto functions (e.g., fips202, sha2, aes).
"""

import os
import re
import platform
import unittest

import pqclean
import helpers


@helpers.skip_windows()
def test_common():
    for d in os.listdir('common'):
        primitive = re.sub(r"\.c$", "", d)
        binname = os.path.join('..', 'bin', 'test_'+primitive)
        helpers.make(binname)
        helpers.run_subprocess([binname])


if __name__ == '__main__':
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
