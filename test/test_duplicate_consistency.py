"""
Checks that files duplicated across schemes/implementations are consistent.
"""

import os
import pqclean
import helpers
import unittest
import yaml


def _skipped_test(*args, **kwargs):
    raise unittest.SkipTest("Skipped consistency check")


def test_duplicate_consistency():
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
            if not helpers.permit_test('duplicate_consistency',
                                       implementation):
                yield _skipped_test, implementation
                continue

            if os.path.isfile(
                    os.path.join(
                        'duplicate_consistency',
                        '{}_{}.yml'.format(scheme.name, implementation.name))):
                metafile = os.path.join(
                    'duplicate_consistency',
                    '{}_{}.yml'.format(scheme.name, implementation.name))
                with open(metafile, encoding='utf-8') as f:
                    metadata = yaml.safe_load(f.read())
                    for group in metadata['consistency_checks']:
                        source = pqclean.Implementation.by_name(
                                group['source']['scheme'],
                                group['source']['implementation'])
                        for file in group['files']:
                            yield (check_duplicate_consistency, implementation,
                                   source, file)


def file_get_contents(filename):
    with open(filename) as f:
        return f.read()


@helpers.skip_windows()
def check_duplicate_consistency(implementation, source, file):
    transformed_src = helpers.run_subprocess(
        ['sed', '-e', 's/{}/{}/g'.format(source.namespace_prefix(),
         implementation.namespace_prefix()), os.path.join(source.path(), file)]
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
