"""
Checks that the archive library can be successfully built for every
scheme/implementation.
"""

import pqclean
import helpers


def test_compile_lib():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_compile_lib, scheme.name, implementation.name


def check_compile_lib(scheme_name, implementation_name):
    implementation = pqclean.Implementation.by_name(
            scheme_name, implementation_name)
    helpers.run_subprocess(
        ['make', 'clean'],
        implementation.path()
    )
    helpers.run_subprocess(
        ['make'],
        implementation.path()
    )


if __name__ == '__main__':
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
