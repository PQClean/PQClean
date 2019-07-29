import pytest

import helpers
import pqclean


@pytest.mark.parametrize(
    'implementation',
    pqclean.Scheme.all_implementations(),
    ids=str,
)
@helpers.filtered_test
def test_format(implementation: pqclean.Implementation):
    helpers.ensure_available('astyle')
    cfiles = implementation.cfiles()
    hfiles = implementation.hfiles()
    result = helpers.run_subprocess(
        ['astyle',
         '--dry-run',
         '--options=../.astylerc',
         *cfiles,
         *hfiles])
    assert 'Formatted' not in result


if __name__ == '__main__':
    import sys
    pytest.main(sys.argv)
