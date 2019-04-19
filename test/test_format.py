import helpers
import pqclean


def test_formatting():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_format, implementation


@helpers.filtered_test
def check_format(implementation: pqclean.Implementation):
    helpers.ensure_available('astyle')
    cfiles = implementation.cfiles()
    hfiles = implementation.hfiles()
    result = helpers.run_subprocess(
        ['astyle',
         '--dry-run',
         '--options=../.astylerc',
         *cfiles,
         *hfiles])
    assert(not('Formatted' in result))


if __name__ == "__main__":
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
