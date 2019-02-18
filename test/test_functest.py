"""
Checks that the functional test program (functest) can be successfully built and executed for every scheme/implementation.
"""

import os
import pqclean
import helpers

def test_functest():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_functest, scheme.name, implementation.name

def check_functest(scheme_name, implementation_name):
    implementation = pqclean.Implementation.by_name(scheme_name, implementation_name)
    helpers.run_subprocess(
        ['make', 'TYPE=' + implementation.scheme.type, 'SCHEME=' + scheme_name, 'IMPLEMENTATION=' + implementation_name],
        os.path.join('..', 'test')
    )
    helpers.run_subprocess(
        ['./functest_{}_{}'.format(scheme_name, implementation_name)],
        os.path.join('..', 'bin'),
    )


if __name__ == '__main__':
    import nose
    nose.runmodule()
