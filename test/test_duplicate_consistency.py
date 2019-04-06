"""
Checks that files duplicated across schemes/implementations are consistent.
"""

import os
import pqclean
import helpers
import unittest
import yaml

def test_duplicate_consistency():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            if os.path.isfile(os.path.join('duplicate_consistency', '{}_{}.yml'.format(scheme.name, implementation.name))):
                yield check_duplicate_consistency, implementation

def file_get_contents(filename):
    with open(filename) as f:
        return f.read()

def check_duplicate_consistency(implementation):
    helpers.skip_windows()
    metafile = os.path.join('duplicate_consistency', '{}_{}.yml'.format(implementation.scheme.name, implementation.name))
    with open(metafile, encoding='utf-8') as f:
        metadata = yaml.load(f.read())
        source = pqclean.Implementation.by_name(metadata['source']['scheme'], metadata['source']['implementation'])
        for file in metadata['files']:
            transformed_src = helpers.run_subprocess(
                ['sed', '-e', 's/{}/{}/g'.format(source.namespace_prefix(), implementation.namespace_prefix()), os.path.join(source.path(), file)]
            )
            this_src = file_get_contents(os.path.join(implementation.path(), file))
            print(os.path.join(implementation.path(), file))
            print(this_src)
            assert(transformed_src == this_src)

if __name__ == '__main__':
    try:
        import nose2
        nose2.main()
    except ImportError:
        import nose
        nose.runmodule()
