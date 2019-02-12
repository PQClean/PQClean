"""
Checks that a LICENSE or LICENSE.txt file is present for every implementation of the specified scheme.
"""

import os
import PQClean
import Tests

def run(scheme: PQClean.Scheme):
    for implementation in scheme.implementations.values():
        found = False
        potential_filenames = ['LICENSE', 'LICENSE.txt']
        for filename in potential_filenames:
            p = os.path.join(implementation.path(), filename)
            if os.path.isfile(p):
                found = True
        if found:
            Tests.report_success("license", scheme, implementation)
        else:
            Tests.report_failure("license", scheme, implementation, "No license file found")
