import os
import yaml

class Scheme:
    def __init__(self):
        self.type = None
        self.name = None
        self.implementations = []

    def path(self, base='..'):
        return os.path.join(base, 'crypto_' + self.type, self.name)

    @staticmethod
    def all_schemes():
        schemes = dict()
        schemes.update(Scheme.all_schemes_of_type('kem'))
        schemes.update(Scheme.all_schemes_of_type('sign'))
        return schemes

    @staticmethod
    def all_schemes_of_type(type: str) -> list:
        schemes = dict()
        p = os.path.join('..', 'crypto_' + type)
        for d in os.listdir(p):
            if os.path.isdir(os.path.join(p, d)):
                if type == 'kem':
                    schemes[d] = KEM(d)
                elif type == 'sign':
                    schemes[d] = Signature(d)
                else:
                    assert('Unknown type')
        return schemes

    def metadata(self):
        metafile = os.path.join(self.path(), 'META.yml')
        try:
            with open(metafile, encoding='utf-8') as f:
                metadata = yaml.load(f.read())
                return metadata
        except Exception as e:
            print("Can't open {}: {}".format(metafile, e))
            return None

class Implementation:

    def __init__(self, scheme, name):
        self.scheme = scheme
        self.name = name

    def path(self, base='..') -> str:
        return os.path.join(self.scheme.path(), self.name)

    @staticmethod
    def all_implementations(scheme: Scheme) -> list:
        implementations = dict()
        for d in os.listdir(scheme.path()):
            if os.path.isdir(os.path.join(scheme.path(), d)):
                implementations[d] = Implementation(scheme, d)
        return implementations

class KEM(Scheme):

    def __init__(self, name: str):
        self.type = 'kem'
        self.name = name;
        self.implementations = Implementation.all_implementations(self)

    @staticmethod
    def all_kems() -> list:
        return Scheme.all_schemes_of_type('kem')

class Signature(Scheme):

    def __init__(self, name: str):
        self.type = 'sign'
        self.name = name;
        self.implementations = Implementation.all_implementations(self)

    @staticmethod
    def all_sigs():
        return Scheme.all_schemes_of_type('sig')
