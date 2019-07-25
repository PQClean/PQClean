"""
Checks that files duplicated across schemes/implementations are consistent.
"""

import os

import yaml

import helpers
import pqclean


def pytest_generate_tests(metafunc):
    ids = []
    argvalues = []
    for scheme in pqclean.Scheme.all_schemes():
        for implementation in scheme.implementations:
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
                            argvalues.append((implementation, source, file))
                            ids.append(
                                "{scheme.name}-{source.scheme.name}: {file}"
                                .format(scheme=scheme, source=source,
                                        file=file))
    metafunc.parametrize(('implementation', 'source', 'file'),
                         argvalues,
                         ids=ids)


def file_get_contents(filename):
    with open(filename) as f:
        return f.read()


@helpers.skip_windows()
@helpers.filtered_test
def test_duplicate_consistency(implementation, source, file):
    transformed_src = helpers.run_subprocess(
        ['sed', '-e', 's/{}/{}/g'.format(source.namespace_prefix(),
         implementation.namespace_prefix()), os.path.join(source.path(), file)]
    )
    this_src = file_get_contents(os.path.join(implementation.path(), file))
    print(os.path.join(implementation.path(), file))
    print(this_src)
    assert(transformed_src == this_src)


if __name__ == '__main__':
    import pytest
    import sys
    pytest.main(sys.argv)
