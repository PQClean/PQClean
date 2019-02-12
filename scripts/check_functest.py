"""
Checks that the functional test program (functest) can be successfully built and executed for every implementation of the specified scheme.
"""

import os
import PQClean
import Tests

def run(scheme: PQClean.Scheme):
    for implementation in scheme.implementations.values():
        Tests.run_subprocess(
            "functest",
            scheme,
            implementation,
            ['make', 'TYPE=' + scheme.type, 'SCHEME=' + scheme.name, 'IMPLEMENTATION=' + implementation.name],
            os.path.join('..', 'test'),
            "Compilation failed"
        )
        Tests.run_subprocess(
            "functest",
            scheme,
            implementation,
            ['./functest_{}_{}'.format(scheme.name, implementation.name)],
            os.path.join('..', 'bin'),
            "Functional test failed"
        )
