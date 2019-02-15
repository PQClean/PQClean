import os
import yaml

class Scheme:
    def __init__(self):
        self.type = None
        self.name = None
        self.implementations = []

    def path(self, base='..'):
        return os.path.join(base, 'crypto_' + self.type, self.name)

    def namespace_prefix(self):
        return 'PQCLEAN_{}_'.format(self.name.upper()).replace('-', '')

    @staticmethod
    def by_name(scheme_name):
        for scheme in Scheme.all_schemes():
            if scheme.name == scheme_name:
                return scheme
        raise KeyError()

    @staticmethod
    def all_schemes():
        schemes = []
        schemes.extend(Scheme.all_schemes_of_type('kem'))
        schemes.extend(Scheme.all_schemes_of_type('sign'))
        return schemes

    @staticmethod
    def all_implementations():
        implementations = dict()
        for scheme in Scheme.all_schemes().values():
            implementations.extend(scheme.all_implementations())
        return implementations

    @staticmethod
    def all_schemes_of_type(type: str) -> list:
        schemes = []
        p = os.path.join('..', 'crypto_' + type)
        for d in os.listdir(p):
            if os.path.isdir(os.path.join(p, d)):
                if type == 'kem':
                    schemes.append(KEM(d))
                elif type == 'sign':
                    schemes.append(Signature(d))
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
    def by_name(scheme_name, implementation_name):
        scheme = Scheme.by_name(scheme_name)
        for implementation in scheme.implementations:
            if implementation.name == implementation_name:
                return implementation
        raise KeyError()

    @staticmethod
    def all_implementations(scheme: Scheme) -> list:
        implementations = []
        for d in os.listdir(scheme.path()):
            if os.path.isdir(os.path.join(scheme.path(), d)):
                implementations.append(Implementation(scheme, d))
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
