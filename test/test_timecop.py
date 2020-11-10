import functools
import os
import platform
import unittest

import pytest

import helpers
import pqclean

@pytest.mark.parametrize(
    'implementation,test_dir,impl_path, init, destr',
    [(impl, *helpers.isolate_test_files(impl.path(), 'test_timecop_'))
     for impl in pqclean.Scheme.all_supported_implementations()],
    ids=[str(impl) for impl in pqclean.Scheme.all_supported_implementations()],
)
@helpers.filtered_test
def test_timecop(implementation: pqclean.Implementation, impl_path, test_dir,
                  init, destr):
    if (platform.machine() not in ('i386', 'x86_64') or
            platform.system() != 'Linux'):
        raise unittest.SkipTest()
    init()

    declassify = 'PQCLEAN_TIMECOP_DECLASSIFY'
    hdr = 'extern void '+declassify+'(void *addr, unsigned long long len);\n'

    test_src_dir = os.path.join(test_dir, 'crypto_bla', 'scheme', 'impl')
    wd = os.getcwd()
    os.chdir(test_src_dir)
    for fname in os.listdir('.'):
        if not fname.endswith(".c"):
            continue
        with open(fname, 'r') as f:
            content = f.read()
        if declassify not in content:
            continue
        content = content.replace('//+'+declassify, declassify)
        with open(fname, 'w') as f:
            f.write(hdr+content)
    os.chdir(wd)

    dest_dir = os.path.join(test_dir, 'bin')
    helpers.make('timecop',
                 TYPE=implementation.scheme.type,
                 SCHEME=implementation.scheme.name,
                 SCHEME_DIR=os.path.abspath(impl_path),
                 IMPLEMENTATION=implementation.name,
                 DEST_DIR=dest_dir,
                 EXTRAFLAGS="-g3",
                 working_dir=os.path.join(test_dir, 'test'))
    test_name = './timecop_{}_{}'.format(implementation.scheme.name,
                                                implementation.name)
    helpers.run_subprocess(
        ['valgrind',
         '-q',
         '--error-exitcode=1',
         '--track-origins=yes',
         test_name],
        dest_dir)
    destr()


if __name__ == '__main__':
    import sys
    pytest.main(sys.argv)
