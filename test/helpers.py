import functools
import os
import subprocess
import unittest
import shutil
import sys

import pqclean

import logging


def run_subprocess(command, working_dir='.', env=None, expected_returncode=0):
    """
    Helper function to run a shell command and report success/failure
    depending on the exit status of the shell command.
    """
    if env is not None:
        env_ = os.environ.copy()
        env_.update(env)
        env = env_

    # Note we need to capture stdout/stderr from the subprocess,
    # then print it, which nose/unittest will then capture and
    # buffer appropriately
    print(working_dir + " > " + " ".join(command))
    result = subprocess.run(
        command,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        cwd=working_dir,
        env=env,
    )
    print(result.stdout.decode('utf-8'))
    if expected_returncode is not None:
        assert result.returncode == expected_returncode, \
            "Got unexpected return code {}".format(result.returncode)
    else:
        return (result.returncode, result.stdout.decode('utf-8'))
    return result.stdout.decode('utf-8')


def make(*args, working_dir='.', env=None, expected_returncode=0, **kwargs):
    """
    Runs a make target in the specified working directory

    Usage:
        make('clean', 'targetb', SCHEME='bla')
    """
    if os.name == 'nt':
        make_command = ['nmake', '/f', 'Makefile.Microsoft_nmake',
                        '/NOLOGO', '/E']
        # we need SCHEME_UPPERCASE and IMPLEMENTATION_UPPERCASE with nmake
        for envvar in ['IMPLEMENTATION', 'SCHEME']:
            if envvar in kwargs:
                kwargs['{}_UPPERCASE'.format(envvar)] = (
                        kwargs[envvar].upper().replace('-', ''))
    else:
        make_command = ['make']

    return run_subprocess(
        [
            *make_command,
            *['{}={}'.format(k, v) for k, v in kwargs.items()],
            *args,
        ],
        working_dir=working_dir,
        env=env,
        expected_returncode=expected_returncode,
    )


def skip_windows(message="This test is not supported on Windows"):
    def wrapper(f):
        @functools.wraps(f)
        def skip_windows(*args, **kwargs):
            raise unittest.SkipTest(message)
        if os.name == 'nt':
            return skip_windows
        else:
            return f
    return wrapper


def slow_test(f):
    @functools.wraps(f)
    def wrapper(*args, **kwargs):
        if ('CI' in os.environ and 'RUN_SLOW' not in os.environ and
                os.environ.get('TRAVIS_EVENT_TYPE') != 'cron'):
            raise unittest.SkipTest("Slow test skipped on CI run")
        return f(*args, **kwargs)
    return wrapper


def ensure_available(executable):
    """
    Checks if a command is available.

    If a command MUST be available, because we are in a CI environment,
    raises an AssertionError.

    In the docker containers, on Travis and on Windows, CI=true is set.
    """
    path = shutil.which(executable)
    if path:
        return path

    # Installing clang-tidy on LLVM will be too much of a mess.
    if ((executable == 'clang-tidy' and sys.platform == 'darwin')
            or 'CI' not in os.environ):
        raise unittest.SkipTest(
                "{} is not available on PATH. Install it to run this test.{}"
                .format(executable, "" if not os.name == 'nt'
                        else "On Windows, make sure to add it to PATH")
                )
    raise AssertionError("{} not available on CI".format(executable))


def permit_test(testname, thing, *args, **kwargs):
    if 'PQCLEAN_ONLY_TESTS' in os.environ:
        if not(testname.lower() in os.environ['PQCLEAN_ONLY_TESTS'].lower().split(',')):
            return False
    if 'PQCLEAN_SKIP_TESTS' in os.environ:
        if testname.lower() in os.environ['PQCLEAN_SKIP_TESTS'].lower().split(','):
            return False

    if isinstance(thing, pqclean.Implementation):
        scheme = thing.scheme
    elif isinstance(thing, pqclean.Scheme):
        scheme = thing
    else:
        return True

    if 'PQCLEAN_ONLY_TYPES' in os.environ:
        if not(scheme.type.lower() in os.environ['PQCLEAN_ONLY_TYPES'].lower().split(',')):
            return False
    if 'PQCLEAN_SKIP_TYPES' in os.environ:
        if scheme.type.lower() in os.environ['PQCLEAN_SKIP_TYPES'].lower().split(','):
            return False
    if 'PQCLEAN_ONLY_SCHEMES' in os.environ:
        if not(scheme.name.lower() in os.environ['PQCLEAN_ONLY_SCHEMES'].lower().split(',')):
            return False
    if 'PQCLEAN_SKIP_SCHEMES' in os.environ:
        if scheme.name.lower() in os.environ['PQCLEAN_SKIP_SCHEMES'].lower().split(','):
            return False

    if 'PQCLEAN_ONLY_DIFF' in os.environ:
        if shutil.which('git') is not None:
            # if we're on a non-master branch, and the only changes are in schemes,
            # only run tests on those schemes
            branch_result = subprocess.run(
                ['git', 'status', '--porcelain=2', '--branch'],
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                cwd="..",
            )
            # ensure we're in a working directory
            if branch_result.returncode != 0:
                return True
            # ensure we're not on master branch
            for branch_line in branch_result.stdout.decode('utf-8').splitlines():
                tokens = branch_line.split(' ')
                if tokens[0] == '#' and tokens[1] == 'branch.head':
                    if tokens[2] == 'master':
                        return True
            # where are there changes?
            diff_result = subprocess.run(
                ['git', 'diff', '--name-only', 'origin/master'],
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT
            )
            assert diff_result.returncode == 0, \
                "Got unexpected return code {}".format(diff_result.returncode)
            for diff_line in diff_result.stdout.decode('utf-8').splitlines():
                # don't skip test if there are any changes outside schemes
                if (not diff_line.startswith('crypto_kem') and
                        not diff_line.startswith('crypto_sign') and
                        not diff_line.startswith(os.path.join('test', 'duplicate_consistency'))):
                    logging.info("Running all tests as there are changes "
                                 "outside of schemes")
                    return True
                # do test if the scheme in question has been changed
                if diff_line.startswith(thing.path(base='')):
                    return True
                # do test if the scheme's duplicate_consistency files have been changed
                if diff_line.startswith(os.path.join('test', 'duplicate_consistency', scheme.name.lower())):
                    return True
            # there were no changes outside schemes, and the scheme in question had no diffs
            return False

    return True


def filtered_test(func):
    funcname = func.__name__[len("check_"):]

    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        if permit_test(funcname, *args, **kwargs):
            return func(*args, **kwargs)
        else:
            raise unittest.SkipTest("Test disabled by filter")
    return wrapper
