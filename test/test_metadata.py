"""
Verify the metadata specified in the META.yml files.
"""

import copy
import itertools

import pytest

import helpers
import pqclean


@pytest.mark.parametrize(
    'scheme',
    pqclean.Scheme.all_schemes(),
    ids=str,
)
@helpers.filtered_test
def test_metadata(scheme):
    metadata = scheme.metadata()

    specification = EXPECTED_FIELDS.items()

    if scheme.type == 'kem':
        specification = itertools.chain(specification, KEM_FIELDS.items())
    elif scheme.type == 'sign':
        specification = itertools.chain(specification,
                                        SIGNATURE_FIELDS.items())
    else:
        assert False, "Wrong type of metadata"

    check_spec(copy.deepcopy(metadata), specification)

    implementation_names_in_yaml = set(
        i['name'] for i in metadata['implementations'])
    if len(implementation_names_in_yaml) != len(metadata['implementations']):
        raise AssertionError("Implementations in YAML file are not distinct")
    implementations_on_disk = set(i.name for i in scheme.implementations)
    if implementation_names_in_yaml != implementations_on_disk:
        raise AssertionError("Implementations in YAML file {} and "
                             "implementations on disk {} do not match"
                             .format(implementation_names_in_yaml,
                                     implementations_on_disk))


EXPECTED_FIELDS = {
    'name': {'type': str},
    'type': {'type': str},
    'claimed-nist-level': {'type': int, 'min': 1, 'max': 5},
    'length-public-key': {'type': int, 'min': 1},
    'length-secret-key': {'type': int, 'min': 1},
    'nistkat-sha256': {'type': str, 'length': 64},
    'principal-submitters': {'type': list, 'elements': {'type': str}},
    'auxiliary-submitters': {
        'type': list, 'elements': {'type': str}, 'optional': True},
    'implementations': {
        'type': list,
        'elements': {
            'type': dict,
            'spec': {
                'name': {'type': str},
                'version': {'type': str},
                'supported_platforms': {
                    'type': list,
                    'optional': True,
                    'elements': {
                        'type': dict,
                        'spec': {
                            'architecture': {
                                'type': str,
                                'values': ['x86', 'x86_64', 'aarch64']},
                            'required_flags': {
                                'type': list,
                                'optional': True,
                                'elements': {'type': str},
                            },
                            'operating_systems': {
                                'type': list,
                                'optional': True,
                                'elements': {
                                    'type': str,
                                    'values': ['Linux', 'Windows', 'Darwin'],
                                },
                            },
                        },
                    },
                },
            },
        },
    },
}

KEM_FIELDS = {
    'claimed-security': {'type': str, 'values': ['IND-CPA', 'IND-CCA2']},
    'length-ciphertext': {'type': int, 'min': 1},
    'length-shared-secret': {'type': int, 'min': 1},
}

SIGNATURE_FIELDS = {
    'length-signature': {'type': int, 'min': 1},
    'testvectors-sha256': {'type': str, 'length': 64},
}


def check_spec(metadata, spec):
    for field, props in spec:
        if field not in metadata and 'optional' not in props:
            raise AssertionError("Field '{}' not present.".format(field))

        # validate element
        if field in metadata:
            check_element(field, metadata[field], props)

            # delete it to detect extras
            del metadata[field]

    # Done checking all specified fields, check if we have extras
    for field, value in metadata.items():
        raise AssertionError(
            "Unexpected item '{}' with value '{}'".format(field, value))


def check_element(field, element, props):
    type_ = props['type']
    # Validate type of element
    type_(element)

    # Strs are valid lists otherwise
    if type_ == list and type(element) != list:
        raise ValueError("Field {} not a list".format(field))

    # lists are valid dicts otherwise
    if type_ == dict and type(element) != dict:
        raise ValueError("Field {} not a dict".format(field))

    if type_ == int:
        element = int(element)
        if 'min' in props:
            if element < props['min']:
                raise ValueError("Value of field '{}' is lower than minimum "
                                 "value {}".format(field, props['min']))
        if 'max' in props:
            if element > props['max']:
                raise ValueError("Value of field '{}' is larger than maximum"
                                 " value {}"
                                 .format(field, props['max']))

    if type_ == str:
        if 'length' in props:
            actual_len = len(element)
            if actual_len != props['length']:
                raise ValueError("Value of field '{}' should be length {}"
                                 " but was length {}"
                                 .format(field, props['length'], actual_len))
        if 'values' in props and element not in props['values']:
            raise ValueError("'{}' should be in {}"
                             .format(element, props['values']))

    if type_ == list:  # recursively check the elements
        for el in element:
            check_element('element of {}'.format(field), el, props['elements'])

    if type_ == dict:
        check_spec(element, props['spec'].items())


if __name__ == '__main__':
    import sys
    pytest.main(sys.argv)
