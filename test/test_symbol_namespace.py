"""
Checks that the all exported symbols are properly namespaced, i.e., all
start with "PQCLEAN_SCHEMENAME_".
"""

import pqclean
import helpers
import sys
import unittest


def test_symbol_namespace():
    if sys.platform not in ['linux', 'darwin']:
        raise unittest.SkipTest()
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_symbol_namespace, scheme.name, implementation.name


def check_symbol_namespace(scheme_name, implementation_name):
    implementation = pqclean.Implementation.by_name(
        scheme_name, implementation_name)
    helpers.run_subprocess(
        ['make'],
        implementation.path()
    )
    out = helpers.run_subprocess(
        ['nm', '-g', 'lib{}_{}.a'.format(scheme_name, implementation_name)],
        implementation.path()
    )

    lines = out.strip().split("\n")
    symbols = []
    for line in lines:
        if ' T ' in line or ' D ' in line or ' S ' in line:
            symbols.append(line)

    namespace = implementation.scheme.namespace_prefix()
    non_namespaced = []
    for symbolstr in symbols:
        *_, symtype, symbol = symbolstr.split()
        if symtype in 'TR':
            if (not symbol.startswith(namespace) and
                    not symbol.startswith('_' + namespace)):
                non_namespaced.append(symbol)

    if non_namespaced:
        print("Missing namespace literal {}".format(namespace))
        for symbol in non_namespaced:
            print("\ttype: {}, symbol: {}".format(symtype, symbol))
        assert(False)
