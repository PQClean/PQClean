"""
Checks that the all exported symbols are properly namespaced, i.e., all
start with "PQCLEAN_SCHEMENAME_".
"""

import pqclean
import helpers
import sys
import unittest


def test_symbol_namespace():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_symbol_namespace, implementation


@helpers.filtered_test
def check_symbol_namespace(implementation):
    if sys.platform not in ['linux', 'darwin']:
        raise unittest.SkipTest("Unsupported platform")
    helpers.make(working_dir=implementation.path())
    out = helpers.run_subprocess(
        ['nm', '-g', implementation.libname()],
        implementation.path()
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
            if (not symbol.startswith(namespace) and
                    # weird things on i386
                    not symbol.startswith('__x86.get_pc_thunk.') and
                    not symbol.startswith('_' + namespace)):
                non_namespaced.append(symbol)

    if non_namespaced:
        print("Missing namespace literal {}".format(namespace))
        for symbol in non_namespaced:
            print("\ttype: {}, symbol: {}".format(symtype, symbol))
        assert(False)


if __name__ == '__main__':
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
