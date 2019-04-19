import json
import os

import pqclean
import helpers


def test_metadata_sizes():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_metadata_sizes, implementation


@helpers.filtered_test
def check_metadata_sizes(implementation):
    metadata = implementation.scheme.metadata()
    impl_meta = next((impl for impl in metadata['implementations']
                      if impl['name'] == implementation.name), None)
    helpers.make('printparams',
                 TYPE=implementation.scheme.type,
                 SCHEME=implementation.scheme.name,
                 IMPLEMENTATION=implementation.name,
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

    if implementation.scheme.type == 'kem':
        assert (
            parsed['CRYPTO_CIPHERTEXTBYTES'] == metadata['length-ciphertext'])
        assert parsed['CRYPTO_BYTES'] == metadata['length-shared-secret']
    else:
        assert parsed['CRYPTO_BYTES'] == metadata['length-signature']


if __name__ == '__main__':
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
