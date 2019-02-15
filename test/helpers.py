import subprocess

def run_subprocess(command, working_dir, expected_returncode = 0):
    """Helper function to run a shell command and report success/failure depending on the exit status of the shell command."""
    # Note we need to capture stdout/stderr from the subprocess, then print it, which nose/unittest will then capture and buffer appropriately
    result = subprocess.run(
        command,
        stdout = subprocess.PIPE,
        stderr = subprocess.STDOUT,
        cwd = working_dir
    )
    print(result.stdout.decode('utf-8'))
    assert(result.returncode == expected_returncode)
    return result.stdout.decode('utf-8')
