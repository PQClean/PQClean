"""
Checks that a LICENSE or LICENSE.txt file is present for every
implementation of the specified scheme.
"""

import os
import pqclean
import pytest
import helpers


@pytest.mark.parametrize(
    'implementation',
    pqclean.Scheme.all_implementations(),
    ids=str,
)
@helpers.filtered_test
def test_license(implementation):
    p1 = os.path.join(implementation.path(), 'LICENSE')
    p2 = os.path.join(implementation.path(), 'LICENSE.txt')
    assert(os.path.isfile(p1) or os.path.isfile(p2))


if __name__ == '__main__':
    import pytest
    import sys
    pytest.main(sys.argv)
