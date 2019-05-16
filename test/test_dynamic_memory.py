"""
Checks that no dynamic memory functions are used
"""

import pqclean
import helpers


def test_dynamic_memory():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            # Keep this loop outside, to allow multiple assertions
            for function in ['malloc', 'free', 'realloc', 'calloc']:
                yield (check_dynamic_memory, implementation, function)


@helpers.filtered_test
@helpers.skip_windows()
def check_dynamic_memory(implementation, function):
    # 'make' will take care of not rebuilding existing library files
    helpers.make(working_dir=implementation.path())
    scheme_name = implementation.scheme.name
    out = helpers.run_subprocess(
        ['nm', '-g', 'lib{}_{}.a'.format(scheme_name, implementation.name)],
        implementation.path()
    )

    lines = out.strip().split("\n")

    for line in lines:
        if line.endswith('U {}'.format(function)):
            raise AssertionError(
                "Illegal use of dynamic memory function '{}'".format(function))


if __name__ == '__main__':
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
