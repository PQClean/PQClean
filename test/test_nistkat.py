"""
Checks that (hash of the) KATs (in NIST format) produced on this platform match
the one provided in the META file for every scheme/implementation.

Note that this only uses the first test case from the NIST-format KAT files.
The appropriate hash can be generated from the original submission's KAT file
using the command:
    cat PQCkemKAT_whatever.rsp | head -n 8 | tail -n 6 | sha256sum
"""

import hashlib
import os
import platform
import unittest

import pytest

import helpers
import pqclean


@pytest.mark.parametrize(
    'implementation,test_dir,impl_path, init, destr',
    [(impl, *helpers.isolate_test_files(impl.path(), 'test_functest_'))
     for impl in pqclean.Scheme.all_supported_implementations()],
    ids=[str(impl) for impl in pqclean.Scheme.all_supported_implementations()],
)
@helpers.filtered_test
def test_nistkat(implementation, impl_path, test_dir, init, destr):
    if platform.machine() == 'armv7l' and 'gcc' in os.environ.get('CC', 'gcc') and 'falcon' in implementation.scheme.name:
        raise unittest.SkipTest("this test hangs for falcon on armv7l with gcc")
    init()
    dest_path = os.path.join(test_dir, 'bin')
    kat_rng = 'nist'
    if 'nistkat-rng' in implementation.scheme.metadata():
        kat_rng = implementation.scheme.metadata()['nistkat-rng']
    helpers.make('nistkat',
                 TYPE=implementation.scheme.type,
                 SCHEME=implementation.scheme.name,
                 IMPLEMENTATION=implementation.name,
                 SCHEME_DIR=impl_path,
                 DEST_DIR=dest_path,
                 KAT_RNG=kat_rng,
                 working_dir=os.path.join(test_dir, 'test'))
    out = helpers.run_subprocess(
        [os.path.join(dest_path, 'nistkat_{}_{}{}'.format(
            implementation.scheme.name,
            implementation.name,
            '.exe' if os.name == 'nt' else ''
        ))],
        print_output=False,
    ).replace('\r', '')
    assert(implementation.scheme.metadata()['nistkat-sha256'].lower()
           == hashlib.sha256(out.encode('utf-8')).hexdigest().lower())
    destr()


if __name__ == '__main__':
    import sys
    pytest.main(sys.argv)
