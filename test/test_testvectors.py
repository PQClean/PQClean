"""
Checks that (hash of the) test vectors produced on this platform matches
the one provided in the META file for every scheme/implementation.
"""

import hashlib
import os
import unittest

import pytest

import helpers
import pqclean


@pytest.mark.parametrize(
    'implementation,test_dir,impl_path,init,destr',
    [(impl, *helpers.isolate_test_files(impl.path(), 'test_testvectors_'))
     for sig in pqclean.Signature.all_sigs()
     for impl in sig.implementations],
    ids=[str(impl) for sig in pqclean.Signature.all_sigs()
         for impl in sig.implementations],
)
@helpers.filtered_test
def test_testvectors(implementation, impl_path, test_dir, init, destr):
    if not implementation.supported_on_current_platform():
        raise unittest.SkipTest("Not supported on current platform")
    init()
    dest_dir = os.path.join(test_dir, 'bin')
    helpers.make('testvectors',
                 TYPE=implementation.scheme.type,
                 SCHEME=implementation.scheme.name,
                 SCHEME_DIR=impl_path,
                 IMPLEMENTATION=implementation.name,
                 DEST_DIR=dest_dir,
                 working_dir=os.path.join(test_dir, 'test'))
    out = helpers.run_subprocess(
        [os.path.join(dest_dir, 'testvectors_{}_{}{}'.format(
            implementation.scheme.name,
            implementation.name,
            '.exe' if os.name == 'nt' else ''
        ))],
    ).replace('\r', '')
    assert(implementation.scheme.metadata()['testvectors-sha256'].lower()
           == hashlib.sha256(out.encode('utf-8')).hexdigest().lower())
    destr()


if __name__ == '__main__':
    import sys
    pytest.main(sys.argv)
