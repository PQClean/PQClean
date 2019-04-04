"""
Checks that files duplicated across schemes/implementations are consistent.
"""

import os
import pqclean
import helpers


def test_duplicate_consistency():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            yield check_duplicate_consistency, implementation


def file_get_contents(filename):
    with open(filename) as f:
        return f.read()

def check_duplicate_consistency(implementation):
    helpers.ensure_available('sed')
    if 'duplicate-consistency' in implementation.metadata():
        dc = implementation.metadata()['duplicate-consistency']
        for pairs in dc['files']:
            transformed_src = helpers.run_subprocess(
                ['sed', '-e', 's/{}/{}/g'.format(dc['source_namespace'], dc['target_namespace']), pairs['source_file']],
                '..',
            )
            this_src = file_get_contents(os.path.join(implementation.path(), pairs['target_file']))
            print(this_src)
            assert(transformed_src == this_src)


if __name__ == '__main__':
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
