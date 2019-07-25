import os
import re

import pytest

import helpers
import pqclean

pattern = re.compile(r'^\s*#include\s*"')


@pytest.mark.parametrize(
    'implementation',
    pqclean.Scheme.all_implementations(),
    ids=str,
)
@helpers.filtered_test
def test_api_h(implementation: pqclean.Implementation):
    apipath = os.path.join(implementation.path(), 'api.h')
    errors = []
    with open(apipath) as f:
        for i, line in enumerate(f):
            if pattern.match(line):
                errors.append("\n at {}:{}".format(apipath, i+1))
    if errors:
        raise AssertionError(
            "Prohibited external include in api.h" + "".join(errors)
        )


if __name__ == '__main__':
    import sys
    pytest.main(sys.argv)
