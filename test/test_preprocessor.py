import pytest

import helpers
import pqclean


@pytest.mark.parametrize(
    'implementation',
    pqclean.Scheme.all_implementations(),
    ids=str,
)
@helpers.filtered_test
def test_preprocessor(implementation: pqclean.Implementation):
    cfiles = implementation.cfiles()
    hfiles = implementation.hfiles()
    errors = []
    for file in hfiles + cfiles:
        with open(file) as f:
            for i, line in enumerate(f):
                line = line.strip()
                if file in hfiles and i == 0 and line.startswith('#ifndef'):
                    continue
                if line.startswith('#if') and ("crypto_sign/falcon-512/avx2/sign.c" not in file) and ("crypto_sign/falcon-1024/avx2/sign.c" not in file):
                    errors.append("\n at {}:{}".format(file, i+1))
    if errors:
        raise AssertionError(
            "Prohibited use of preprocessor conditional" + "".join(errors)
        )


if __name__ == '__main__':
    import sys
    pytest.main(sys.argv)
