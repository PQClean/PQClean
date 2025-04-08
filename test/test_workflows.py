from pathlib import Path
import sys

import helpers

@helpers.skip_windows()
def test_workflows_up_to_date():
    scriptdir = str(Path("..") / ".github" / "workflows")
    exepath = sys.executable or "python3"
    helpers.run_subprocess([exepath, "generate_workflows.py", "test"],
                           working_dir=scriptdir)

if __name__ == "__main__":
    import pytest
    import sys
    pytest.main(sys.argv)
