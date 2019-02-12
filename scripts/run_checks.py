#!/usr/bin/env python3

"""
Run all (or requested) checks on all (or requested) schemes.
"""

import glob
import sys
import PQClean
import Tests

# populate all checks from files named check_*.py, and import those modules
all_check_names = []
for check in glob.iglob('check_*.py'):
    check = check[6:] # remove check_ prefix
    check = check[:-3] # remove .py extension
    exec("import check_" + check)
    all_check_names.append(check)

all_schemes = PQClean.Scheme.all_schemes()
all_scheme_names = all_schemes.keys()

def run(check_names: list, scheme_names: list):
    """Run the requested checks for the requested schemes."""
    global all_schemes
    for scheme in scheme_names:
        for check in check_names:
            eval("check_" + check).run(all_schemes[scheme])
    Tests.print_successes()
    exit(Tests.globalreturnvalue)

if __name__ == "__main__":
    # if any arguments were specified, filter to just those checks / schemes
    requested_check_names = []
    requested_scheme_names = []
    for a in sys.argv[1:]:
        if a in all_check_names: requested_check_names.append(a)
        elif a in all_scheme_names: requested_scheme_names.append(a)
        else:
            print("Invalid argument {}.  Must be a check or scheme".format(a))
            print("Known checks: " + ", ".join(all_check_names))
            print("Known schemes: " + ", ".join(all_scheme_names))
            exit(-1)
    # if no checks / schemes were explicitly requested, use all of them
    if len(requested_check_names) == 0: requested_check_names = all_check_names
    if len(requested_scheme_names) == 0: requested_scheme_names = all_scheme_names
    # run the requested checks for the requested schemes
    run(requested_check_names, requested_scheme_names)
