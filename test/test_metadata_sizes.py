import json
import os

import pytest

import helpers
import pqclean


@pytest.mark.parametrize(
    'implementation,test_dir,impl_path, init, destr',
    [(impl, *helpers.isolate_test_files(impl.path(), 'test_printparams_'))
     for impl in pqclean.Scheme.all_implementations()],
    ids=[str(impl) for impl in pqclean.Scheme.all_implementations()],
)
@helpers.filtered_test
def test_metadata_sizes(implementation, impl_path, test_dir, init, destr):
    init()
    metadata = implementation.scheme.metadata()
    helpers.make('printparams',
                 TYPE=implementation.scheme.type,
                 SCHEME=implementation.scheme.name,
                 IMPLEMENTATION=implementation.name,
                 SCHEME_DIR=impl_path,
                 working_dir=os.path.join('..', 'test'))

    out = helpers.run_subprocess(
            [os.path.join('..', 'bin', 'printparams_{}_{}{}'.format(
                implementation.scheme.name,
                implementation.name,
                '.exe' if os.name == 'nt' else ''
            ))],
            os.path.join('..', 'bin'),
        ).replace('\r', '')

    parsed = json.loads(out)

    assert parsed['CRYPTO_SECRETKEYBYTES'] == metadata['length-secret-key']
    assert parsed['CRYPTO_PUBLICKEYBYTES'] == metadata['length-public-key']
    assert parsed['CRYPTO_ALGNAME'] == metadata['name']

    if implementation.scheme.type == 'kem':
        assert (
            parsed['CRYPTO_CIPHERTEXTBYTES'] == metadata['length-ciphertext'])
        assert parsed['CRYPTO_BYTES'] == metadata['length-shared-secret']
    else:
        assert parsed['CRYPTO_BYTES'] == metadata['length-signature']
    destr()


if __name__ == '__main__':
    import sys
    pytest.main(sys.argv)
