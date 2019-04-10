import helpers
import pqclean


def test_formatting():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            if helpers.permit_test('format', implementation): 
                yield check_format, implementation


def check_format(implementation: pqclean.Implementation):
    helpers.ensure_available('astyle')
    cfiles = implementation.cfiles()
    hfiles = implementation.hfiles()
    helpers.run_subprocess(['astyle',
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
