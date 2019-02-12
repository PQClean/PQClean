"""
Checks that the archive library can be successfully built for every implementation of the specified scheme.
"""

import os
import PQClean
import Tests
import subprocess

def run(scheme: PQClean.Scheme):
    for implementation in scheme.implementations.values():
        Tests.run_subprocess(
            "compile_lib",
            scheme,
            implementation,
            ['make'],
            implementation.path(),
            "Compilation failed"
        )
