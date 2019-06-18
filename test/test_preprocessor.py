import pqclean
import helpers


def test_preprocessor():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_preprocessor, implementation


@helpers.filtered_test
def check_preprocessor(implementation: pqclean.Implementation):
    cfiles = implementation.cfiles()
    hfiles = implementation.hfiles()
    errors = []
    for file in hfiles + cfiles:
        with open(file) as f:
            for i, line in enumerate(f):
                line = line.strip()
                if file in hfiles and i == 0 and line.startswith('#ifndef'):
                    continue
                if line.startswith('#if'):
                    errors.append("\n at {}:{}".format(file, i+1))
    if errors:
        raise AssertionError(
            "Prohibited use of preprocessor conditional" + "".join(errors)
        )


if __name__ == "__main__":
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
