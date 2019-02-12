"""
Utility functions for running tests and logging the results.
"""

import colorama
import PQClean
import subprocess

colorama.init()

globalreturnvalue = 0

successes = []

def report_success(check: str, scheme: PQClean.Scheme, implementation: PQClean.Implementation, capture: str = None):
    """Report a successful test.  Note that successful test results are not immediately output, and instead are saved for output via print_successess."""
    global successess
    entry = {'check': check, 'scheme': scheme, 'implementation': implementation}
    if entry not in successes: successes.append(entry)

def print_successes():
    """Print table listing all successful tests."""
    global successes
    print(colorama.Fore.GREEN + "=======================SUCCESSES=========================")
    print("{:18}, {:18}, {:15}".format("check", "scheme", "implementation"))
    print("---------------------------------------------------------")
    for entry in successes:
        print("{:18}, ".format(entry["check"]), end='')
        print("{:18}, ".format(entry["scheme"].name if entry["scheme"] else ""), end='')
        print("{:15}".format(entry["implementation"].name if entry["implementation"] else ""))
    print("=========================================================")
    print(colorama.Style.RESET_ALL)

def report_failure(check: str, scheme: PQClean.Scheme, implementation: PQClean.Implementation, message: str, capture: str = None):
    """Report a failed test.  Note that failed test results are immediately output."""
    global successess
    entry = {'check': check, 'scheme': scheme, 'implementation': implementation}
    if entry in successes: successes.remove(entry)
    global globalreturnvalue
    globalreturnvalue = -1
    print(colorama.Fore.RED + "=========================================================")
    print("====                    FAILURE                      ====")
    print("=========================================================")
    print("Check:            " + check)
    if scheme: print("Scheme:           " + scheme.name)
    if implementation: print("Implementation:   " + implementation.name)
    if message: print("Error message:    " + message)
    if capture != None:
        print("")
        print(colorama.Fore.BLACK + capture)
    print(colorama.Fore.RED + "=========================================================")
    print(colorama.Style.RESET_ALL)

def run_subprocess(check_name, scheme, implementation, command, working_dir, error_message):
    """Helper function to run a shell command and report success/failure depending on the exit status of the shell command."""
    try:
        result = subprocess.run(
            command,
            stdout = subprocess.PIPE,
            stderr = subprocess.STDOUT,
            cwd = working_dir
        )
        if result.returncode == 0:
            report_success(check_name, scheme, implementation)
        else:
            report_failure(check_name, scheme, implementation, error_message, result.stdout.decode('utf-8'));
    except Exception as e:
        report_failure(check_name, scheme, implementation, "Python error during execution", e.strerror)
