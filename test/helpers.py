import functools
import os
import subprocess
import unittest
import shutil
import sys

import pqclean


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


def permit_test(testname, thing, **args):
    if 'PQCLEAN_ONLY_TESTS' in os.environ:
        if not(testname.lower() in os.environ['PQCLEAN_ONLY_TESTS'].lower().split(',')):
            return False
    if 'PQCLEAN_SKIP_TESTS' in os.environ:
        if testname.lower() in os.environ['PQCLEAN_SKIP_TESTS'].lower().split(','):
            return False

    if isinstance(thing, pqclean.Implementation):
        scheme = thing.scheme
    else:
        scheme = thing

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

    return True
