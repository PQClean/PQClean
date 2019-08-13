"""
Checks that the archive library can be successfully built for every
scheme/implementation.
"""
import pytest

import helpers
import pqclean


@pytest.mark.parametrize(
    'implementation,test_dir,impl_dir, init, destr',
    [(impl, *helpers.isolate_test_files(impl.path(), 'test_functest_'))
     for impl in pqclean.Scheme.all_supported_implementations()],
    ids=[str(impl) for impl in pqclean.Scheme.all_supported_implementations()],
)
@helpers.filtered_test
def test_compile_lib(implementation, test_dir, impl_dir, init, destr):
    init()
    helpers.make('clean', working_dir=impl_dir)
    helpers.make(working_dir=impl_dir)
    destr()


if __name__ == '__main__':
    import sys
    pytest.main(sys.argv)
