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
    init()
    dest_path = os.path.join(test_dir, 'bin')
    helpers.make('nistkat',
                 TYPE=implementation.scheme.type,
                 SCHEME=implementation.scheme.name,
                 IMPLEMENTATION=implementation.name,
                 SCHEME_DIR=impl_path,
                 DEST_DIR=dest_path,
                 working_dir=os.path.join(test_dir, 'test'))
    out = helpers.run_subprocess(
        [os.path.join(dest_path, 'nistkat_{}_{}{}'.format(
            implementation.scheme.name,
            implementation.name,
            '.exe' if os.name == 'nt' else ''
        ))],
    ).replace('\r', '')
    assert(implementation.scheme.metadata()['nistkat-sha256'].lower()
           == hashlib.sha256(out.encode('utf-8')).hexdigest().lower())
    destr()


if __name__ == '__main__':
    import sys
    pytest.main(sys.argv)
