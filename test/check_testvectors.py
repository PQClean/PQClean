import yaml
import sys
import os
import subprocess
import hashlib

"""
For a given SCHEME, this script verifies that the hash of the testvectors
produced on this platform matches the one provided in the META file.
"""

if len(sys.argv) != 2:
    print("Provide a scheme name (e.g. crypto_kem/kyber768) as argv[1]")
    exit(1)

SCHEME = sys.argv[1]
SCHEMEFULL = SCHEME.replace('/', '_')  # e.g. crypto_kem_kyber768
SCHEMESHORT = SCHEME.split('/')[1].upper()

def get_hash(scheme):
    with open(f"{scheme}/META.yml", 'r') as stream:
        meta = yaml.load(stream)
        return hex(meta['testvectors-sha3-256']).replace('0x', '')


expectedTestvectorsHash = get_hash(SCHEME)

subprocess.run(["make", "testvectors", f"SCHEME={SCHEME}"])
implementations = [x for x in os.listdir('bin') if 'testvectors' in x and SCHEMEFULL in x]

for impl in implementations:
    testvectors = subprocess.run([f"bin/{impl}"],stdout=subprocess.PIPE)
    testvectorsHash = hashlib.sha3_256(testvectors.stdout).hexdigest()
    if testvectorsHash.lower() != expectedTestvectorsHash.lower():
        print(f"testvectors of {SCHEME} should be {expectedTestvectorsHash}, but is {testvectorsHash}")
        sys.exit(1)
    else:
        print(f"testvectors of {SCHEME} matched expected hash")


