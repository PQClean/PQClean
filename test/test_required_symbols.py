"""
Checks that each implementation provides its API in the form of exported
symbols, as opposed to compile-time macros.
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
def test_required_symbols(implementation, impl_path, test_dir, init, destr):
    if sys.platform not in ['linux', 'darwin']:
        raise unittest.SkipTest("Unsupported platform")
    init()
    helpers.make(working_dir=impl_path)
    out = helpers.run_subprocess(
        ['nm', '-g', implementation.libname()],
        impl_path,
        print_output = False,
    )

    lines = out.strip().split("\n")
    exported_symbols = []
    for line in lines:
        if line.startswith("00"):
            *_, symtype, symbol = line.split()
            if symtype in 'TDRS':
                exported_symbols.append(symbol)

    namespace = implementation.namespace_prefix()
    match implementation.scheme.type:
        case 'kem':
            required_fns = ['kem_keypair', 'kem_enc', 'kem_dec']
        case 'sign':
            required_fns = ['sign_keypair', 'sign_signature', 'sign_verify', 'sign', 'sign_open']
    required_symbols = [f'{namespace}crypto_{fn}' for fn in required_fns]

    missing_symbols = [
        s
        for s in required_symbols
        if s not in exported_symbols and '_' + s not in exported_symbols
    ]
    if missing_symbols:
        print("Missing required symbol in namespace {}".format(namespace))
        for symbol in missing_symbols:
            print("\tsymbol: {}".format(symbol))

    assert not missing_symbols, "Missing required symbols"
    destr()


if __name__ == '__main__':
    pytest.main(sys.argv)
