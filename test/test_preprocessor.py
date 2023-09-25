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
        code_has_begun = False
        in_comment = True
        with open(file) as f:
            for i, line in enumerate(f):
                line = line.strip()
                if file in hfiles and not code_has_begun and line.startswith('#ifndef'):
                    continue
                if line == "":
                    continue
                if line.startswith("/*") and not code_has_begun:
                    in_comment = True
                    continue
                if in_comment and "*/" in line:
                    in_comment = False
                    continue
                if not code_has_begun and not in_comment and not line.startswith("#ifndef"):
                    code_has_begun = True
                if (line.startswith('#if') and 
                    ("crypto_kem/kyber512/aarch64/fips202x2.c" not in file) and
                    ("crypto_kem/kyber768/aarch64/fips202x2.c" not in file) and
                    ("crypto_kem/kyber1024/aarch64/fips202x2.c" not in file) and
                    ("crypto_sign/dilithium2/aarch64/fips202x2.c" not in file) and
                    ("crypto_sign/dilithium3/aarch64/fips202x2.c" not in file) and
                    ("crypto_sign/dilithium5/aarch64/fips202x2.c" not in file) and
                    ("crypto_kem\\kyber512\\aarch64\\fips202x2.c" not in file) and
                    ("crypto_kem\\kyber768\\aarch64\\fips202x2.c" not in file) and
                    ("crypto_kem\\kyber1024\\aarch64\\fips202x2.c" not in file) and
                    ("crypto_sign\\dilithium2\\aarch64\\fips202x2.c" not in file) and
                    ("crypto_sign\\dilithium3\\aarch64\\fips202x2.c" not in file) and
                    ("crypto_sign\\dilithium5\\aarch64\\fips202x2.c" not in file) and
                    ("crypto_sign/falcon-512/avx2/sign.c" not in file) and 
                    ("crypto_sign/falcon-1024/avx2/sign.c" not in file) and
                    ("crypto_sign\\falcon-512\\avx2\\sign.c" not in file) and
                    ("crypto_sign\\falcon-1024\\avx2\\sign.c" not in file)):
                    errors.append("\n at {}:{}".format(file, i+1))
    if errors:
        raise AssertionError(
            "Prohibited use of preprocessor conditional" + "".join(errors)
        )


if __name__ == '__main__':
    import sys
    pytest.main(sys.argv)
