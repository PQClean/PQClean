import subprocess
import sys

"""
For a given SCHEME, this script verifies that all exported symbols are properly
namespaced, i.e., all start with "PQCLEAN_SCHEMENAME_"
"""

if len(sys.argv) != 2:
    print("Provide a scheme name (e.g. crypto_kem/kyber768) as argv[1]")
    exit(1)

SCHEME = sys.argv[1]
SCHEMEFULL = SCHEME.replace('/', '_')  # e.g. crypto_kem_kyber768
SCHEMESHORT = SCHEME.split('/')[1].upper()
namespace = "PQCLEAN_{}_".format(SCHEMESHORT).replace('-', '')

# TODO can we do this using object files instead, to preserve file origin?
sharedlib = "bin/shared_{}_clean.so".format(SCHEMEFULL)
subprocess.run(["make", sharedlib, "SCHEME={}".format(SCHEME)])
p = subprocess.run(["nm", "-D", sharedlib], stdout=subprocess.PIPE)

symbols = p.stdout.decode('utf-8').strip().split("\n")
non_namespaced = []

for symbolstr in symbols:
    *_, symtype, symbol = symbolstr.split()
    if symtype in 'TR':
        if not symbol.startswith(namespace):
            non_namespaced.append(symbol)

if non_namespaced:
    print("! Not all symbols were properly namespaced.", file=sys.stderr)
    print("! Missing namespace literal {}".format(namespace), file=sys.stderr)
    for symbol in non_namespaced:
        print("\t{}".format(symbol), file=sys.stderr)
    sys.exit(1)
else:
    print("Checking {} succeeded".format(SCHEME))
