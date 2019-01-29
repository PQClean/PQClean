"""
Verify the metadata specified in the META.yml files.
"""

import yaml
import glob
import sys
import itertools
import copy

import os.path

def eprint(*args, **kwargs):
    """Write to stderr"""
    global errors

    print(*args, file=sys.stderr, **kwargs)
    errors += 1


EXPECTED_FIELDS = {
    'name': {'type': str},
    'type': {'type': str},
    'claimed-nist-level': {'type': int, 'min': 1, 'max': 5},
    'length-public-key': {'type': int, 'min': 1},
    'testvectors-sha256': {'type': str, 'length': 64},
    'principal-submitter': {'type': str},
    'auxiliary-submitters': {'type': list, 'elements': {'type': str}},
    'implementations': {
        'type': list,
        'elements': {
            'type': dict,
            'spec': {
                'name': {'type': str},
                'version': {'type': str},
            },
        },
    },
}

KEM_FIELDS = {
    'length-ciphertext': {'type': int, 'min': 1},
}

SIGNATURE_FIELDS = {
    'length-signature': {'type': int, 'min': 1},
}

errors = 0


def check_spec(metadata, spec, metafile):
    for field, props in spec:
        if field not in metadata:
            eprint("Field '{}' not present in '{}'.".format(
                field, metafile))
            continue

        # validate element
        check_element(field, metadata[field], props, metafile)

        # delete it to detect extras
        del metadata[field]

    # Done checking all specified fields, check if we have extras
    for field, value in metadata.items():
        eprint("Unexpected item '{}' in '{}' with value '{}'"
               .format(field, metafile, value))


def check_element(field, element, props, metafile):
    type_ = props['type']
    # Validate type of element
    try:
        type_(element)

        # Strs are valid lists otherwise
        if type_ == list and type(element) != list:
            raise ValueError("Not a list")
        # lists are valid dicts otherwise
        if type_ == dict and type(element) != dict:
            raise ValueError("Not a dict")
    except ValueError as e:
        eprint("Field '{}' in '{}' seems to be of incorrect type. "
               "Expected '{}'. Got error '{}'."
               .format(field, metafile, type_.__name__, e))
        return

    if type_ == int:
        element = int(element)
        if 'min' in props:
            if element < props['min']:
                eprint("Value of field '{}' in '{}' is lower than minimum "
                       "value {}".format(field, metafile, props['min']))
        if 'max' in props:
            if element > props['max']:
                eprint("Value of field '{}' in '{}' is larger than maximum"
                       " value {}".format(field, metafile, props['max']))
    if type_ == str:
        if 'length' in props:
            actual_len = len(element)
            if actual_len != props['length']:
                eprint("Value of field '{}' in '{}' should be length {}"
                       " but was length {}"
                       .format(field, metafile,
                               props['length'], actual_len))

    if type_ == list:  # recursively check the elements
        for el in element:
            check_element('element of {}'.format(field),
                          el, props['elements'], metafile)
    if type_ == dict:
        check_spec(element, props['spec'].items(), metafile)


for directory in glob.iglob('crypto_*/*/'):
    metafile = os.path.join(directory, 'META.yml')
    try:
        with open(metafile) as f:
            metadata = yaml.load(f.read())
    except Exception as e:
        eprint("Can't open {}: {}".format(metafile, e))
        continue

    specification = EXPECTED_FIELDS.items()
    if 'crypto_kem' in metafile:
        specification = itertools.chain(specification, KEM_FIELDS.items())
    elif 'crypto_sign' in metafile:
        specification = itertools.chain(specification,
                                        SIGNATURE_FIELDS.items())

    check_spec(copy.deepcopy(metadata), specification, metafile)

    unspecified_impls = glob.glob(directory + '*/')
    if 'implementations' in metadata:
        for implementation in metadata['implementations']:
            if 'name' not in implementation:  # problem is reported elsewhere
                continue
            implpath = os.path.join(directory, implementation['name'])
            if not os.path.isdir(implpath):
                eprint("There is no implementation at '{}' but '{}' was "
                       "specified in {}".format(
                           implpath, implementation['name'], metafile))
        implpath += '/'  # adjust for trailing / in unspecified_impls
        if implpath in unspecified_impls:
            unspecified_impls.remove(implpath)

    for impl in unspecified_impls:
        eprint("Implementation '{}' is not specified in '{}'."
               .format(impl, metafile))





exit(errors)
