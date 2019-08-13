"""
Checks that no dynamic memory functions are used
"""

import pytest

import helpers
import pqclean


@pytest.mark.parametrize(
    'implementation,test_dir,impl_path, init, destr',
    [(impl, *helpers.isolate_test_files(impl.path(), 'test_functest_'))
     for impl in pqclean.Scheme.all_supported_implementations()],
    ids=[str(impl) for impl in pqclean.Scheme.all_supported_implementations()],
)
@helpers.skip_windows()
@helpers.filtered_test
def test_dynamic_memory(implementation, test_dir, impl_path, init, destr):
    init()
    # 'make' will take care of not rebuilding existing library files
    helpers.make(working_dir=impl_path)
    scheme_name = implementation.scheme.name
    out = helpers.run_subprocess(
        ['nm', '-g', 'lib{}_{}.a'.format(scheme_name, implementation.name)],
        impl_path,
    )

    lines = out.strip().split("\n")

    for line in lines:
        for function in ['malloc', 'free', 'realloc', 'calloc']:
            if line.endswith('U {}'.format(function)):
                raise AssertionError(
                    "Illegal use of dynamic memory function "
                    "'{function}'".format(function=function))

    destr()


if __name__ == '__main__':
    import sys
    pytest.main(sys.argv)
