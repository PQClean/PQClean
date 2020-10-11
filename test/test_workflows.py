from pathlib import Path

import helpers

@helpers.skip_windows()
def test_workflows_up_to_date():
    scriptdir = str(Path("..") / ".github" / "workflows")
    helpers.run_subprocess(["python3", "generate_workflows.py", "test"],
                           working_dir=scriptdir)

if __name__ == "__main__":
    import pytest
    import sys
    pytest.main(sys.argv)
