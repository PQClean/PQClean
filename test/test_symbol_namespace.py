"""
Checks that the all exported symbols are properly namespaced, i.e., all
start with "PQCLEAN_SCHEMENAME_".
"""

import sys
import unittest

import pytest

import helpers
import pqclean


@pytest.mark.parametrize(
    'implementation,test_dir,impl_path,init,destr',
    [(impl,
      *helpers.isolate_test_files(impl.path(), 'test_symbol_ns_'))
     for impl in pqclean.Scheme.all_supported_implementations()],
    ids=[str(impl) for impl in pqclean.Scheme.all_supported_implementations()],
)
@helpers.filtered_test
def test_symbol_namespaces(implementation, impl_path, test_dir, init, destr):
    if sys.platform not in ['linux', 'darwin']:
        raise unittest.SkipTest("Unsupported platform")
    init()
    helpers.make(working_dir=impl_path)
    out = helpers.run_subprocess(
        ['nm', '-g', implementation.libname()],
        impl_path,
    )

    lines = out.strip().split("\n")
    symbols = []
    for line in lines:
        if ' T ' in line or ' D ' in line or ' S ' in line:
            symbols.append(line)

    namespace = implementation.namespace_prefix()
    non_namespaced = []
    for symbolstr in symbols:
        *_, symtype, symbol = symbolstr.split()
        if symtype in 'TR':
            if not (symbol.startswith(namespace) or
                    symbol.startswith('_' + namespace) or
                    # KeccakP-1600 for AVX2
                    symbol.startswith('KeccakF1600times4') or
                    symbol.startswith('KeccakP1600times4') or
                    # weird things on i386
                    symbol.startswith('__x86.get_pc_thunk.')):
                non_namespaced.append(symbol)

    if non_namespaced:
        print("Missing namespace literal {}".format(namespace))
        for symbol in non_namespaced:
            print("\ttype: {}, symbol: {}".format(symtype, symbol))

    assert not non_namespaced, "Literals with missing namespaces"
    destr()


if __name__ == '__main__':
    pytest.main(sys.argv)
