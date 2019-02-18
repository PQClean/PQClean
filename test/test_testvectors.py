"""
Checks that (hash of the) test vectors produced on this platform matches
the one provided in the META file for every scheme/implementation.
"""

import hashlib
import os
import pqclean
import helpers
import subprocess


def test_testvectors():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_vectors, scheme.name, implementation.name

def check_vectors(scheme_name, implementation_name):
    scheme = pqclean.Scheme.by_name(scheme_name)
    implementation = pqclean.Implementation.by_name(scheme_name, implementation_name)
    helpers.run_subprocess(
        ['make', 'TYPE=' + implementation.scheme.type, 'SCHEME=' + scheme_name, 'IMPLEMENTATION=' + implementation_name],
        os.path.join('..', 'test')
    )
    out = helpers.run_subprocess(
        ['./testvectors_{}_{}'.format(scheme_name, implementation_name)],
        os.path.join('..', 'bin'),
    )
    assert(scheme.metadata()['testvectors-sha256'].lower() == hashlib.sha256(out.encode('utf-8')).hexdigest().lower())


if __name__ == '__main__':
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
