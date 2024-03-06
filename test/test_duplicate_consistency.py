"""
Checks that files duplicated across schemes/implementations are consistent.
"""

import difflib
import os
import sys

import yaml

import helpers
import pqclean

sys.tracebacklimit = 0


def pytest_generate_tests(metafunc):
    ids = []
    argvalues = []
    if 'duplicate_consistency' not in os.environ.get('PQCLEAN_SKIP_TESTS', '').split(','):
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
                            argvalues.append(
                                (implementation, source, group['files']))
                            ids.append(
                                "{metafile}: {scheme.name} {implementation.name}"
                                .format(scheme=scheme,
                                        implementation=implementation,
                                        metafile=metafile))
    else:
        # Placeholders so we don't crash
        ids = ['dummy']
        argvalues = [('dummy', 'dummy', 'dummy')]

    metafunc.parametrize(('implementation', 'source', 'files'),
                         argvalues,
                         ids=ids)


def file_get_contents(filename):
    with open(filename) as file:
        return file.read()


@helpers.filtered_test
def test_duplicate_consistency(implementation, source, files):
    """Test sets of files to be identical modulo namespacing"""
    messages = []
    for file in files:
        target_path = os.path.join(source.path(), file)
        this_path = os.path.join(implementation.path(), file)
        target_src = file_get_contents(target_path)\
                        .replace(source.namespace_prefix(), '')\
                        .replace(source.padded_namespace_prefix(), 'PADDED')\
                        .replace(' ', '') # the padded replace must come after the namespace replace
        this_src = file_get_contents(this_path)\
                        .replace(implementation.namespace_prefix(), '')\
                        .replace(implementation.padded_namespace_prefix(), 'PADDED')\
                        .replace(' ', '') # the padded replace must come after the namespace replace

        if not this_src == target_src:
            diff = difflib.unified_diff(
                this_src.splitlines(keepends=True),
                target_src.splitlines(keepends=True),
                fromfile=this_path,
                tofile=target_path)
            messages.append("{} differed:\n{}".format(file, ''.join(diff)))
    if messages:
        messages.append("prefix: {}".format(source.namespace_prefix()))
        messages.append("prefix: {}".format(implementation.namespace_prefix()))
        raise AssertionError("Files differed:\n{}".format('\n'.join(messages)))


if __name__ == '__main__':
    import pytest
    pytest.main(sys.argv)
