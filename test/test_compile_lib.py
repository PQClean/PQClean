"""
Checks that the archive library can be successfully built for every
scheme/implementation.
"""

import pqclean
import helpers


def test_compile_lib():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_compile_lib, implementation


@helpers.filtered_test
def check_compile_lib(implementation):
    helpers.make('clean', working_dir=implementation.path())
    helpers.make(working_dir=implementation.path())


if __name__ == '__main__':
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
