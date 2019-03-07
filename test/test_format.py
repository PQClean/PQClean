import pqclean
from helpers import run_subprocess, ensure_available


def test_formatting():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_format, implementation


def check_format(implementation: pqclean.Implementation):
    ensure_available('astyle')
    cfiles = implementation.cfiles()
    hfiles = implementation.hfiles()
    run_subprocess(['astyle',
                    '--dry-run',
                    '--options=../.astylerc',
                    *cfiles,
                    *hfiles])


if __name__ == "__main__":
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
