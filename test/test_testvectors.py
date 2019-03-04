"""
Checks that (hash of the) test vectors produced on this platform matches
the one provided in the META file for every scheme/implementation.
"""

import hashlib
import os
import pqclean
import helpers


def test_testvectors():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_vectors, implementation


def check_vectors(implementation):
    helpers.make('testvectors',
                 TYPE=implementation.scheme.type,
                 SCHEME=implementation.scheme.name,
                 IMPLEMENTATION=implementation.name,
                 working_dir=os.path.join('..', 'test'))
    out = helpers.run_subprocess(
        [os.path.join('..', 'bin', 'testvectors_{}_{}{}'.format(
            implementation.scheme.name,
            implementation.name,
            '.exe' if os.name == 'nt' else ''
        ))],
        os.path.join('..', 'bin'),
    ).replace('\r', '')
    assert(implementation.scheme.metadata()['testvectors-sha256'].lower()
           == hashlib.sha256(out.encode('utf-8')).hexdigest().lower())


if __name__ == '__main__':
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
