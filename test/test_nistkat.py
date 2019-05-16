"""
Checks that (hash of the) KATs (in NIST format) produced on this platform matches
the one provided in the META file for every scheme/implementation.

Note that this only uses the first test case from the NIST-format KAT files.
The appropriate hash can be generated from the original submission's KAT file
using the command:
    cat PQCkemKAT_whatever.rsp | head -n 8 | tail -n 6 | sha256sum
"""

import hashlib
import os
import pqclean
import helpers
import unittest


def test_nistkat():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_nistkat, implementation


@helpers.filtered_test
def check_nistkat(implementation):
    helpers.make('nistkat',
                 TYPE=implementation.scheme.type,
                 SCHEME=implementation.scheme.name,
                 IMPLEMENTATION=implementation.name,
                 working_dir=os.path.join('..', 'test'))
    out = helpers.run_subprocess(
        [os.path.join('..', 'bin', 'nistkat_{}_{}{}'.format(
            implementation.scheme.name,
            implementation.name,
            '.exe' if os.name == 'nt' else ''
        ))],
        os.path.join('..', 'bin'),
    ).replace('\r', '')
    assert(implementation.scheme.metadata()['nistkat-sha256'].lower()
           == hashlib.sha256(out.encode('utf-8')).hexdigest().lower())


if __name__ == '__main__':
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
