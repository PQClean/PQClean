import os
import re

import pqclean


def test_preprocessor():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_preprocessor, implementation


def check_preprocessor(implementation: pqclean.Implementation):
    apipath = os.path.join(implementation.path(), 'api.h')
    errors = []
    p = re.compile(r'^\s*#include\s*"')
    with open(apipath) as f:
        for i, line in enumerate(f):
            if p.match(line):
                errors.append("\n at {}:{}".format(apipath, i+1))
    if errors:
        raise AssertionError(
            "Prohibited external include in api.h" + "".join(errors)
        )

if __name__ == "__main__":
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
