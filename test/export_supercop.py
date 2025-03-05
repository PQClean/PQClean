#!/usr/bin/env python3

import os
from pathlib import Path
from typing import Set, Iterator
import shutil
import itertools
import subprocess

import pqclean


BASE_DIR = Path(__file__).resolve().parent.parent
COMMON_DIR = BASE_DIR / "common"
SUPERCOP_EXPORT_DIR = Path(
    os.environ.get("SUPERCOP_DIR", BASE_DIR / "supercop_export")
).resolve()

RENAMES = {
    "kyber512-90s": "kyber90s512",
    "kyber768-90s": "kyber90s768",
    "kyber1024-90s": "kyber90s1024",
    "hqc-128-1-cca2": "hqc1281",
    "hqc-192-1-cca2": "hqc1921",
    "hqc-192-2-cca2": "hqc1922",
    "hqc-256-1-cca2": "hqc2561",
    "hqc-256-2-cca2": "hqc2562",
    "hqc-256-3-cca2": "hqc2563",
    "lightsaber": "lightsaber2",
    "saber": "saber2",
    "firesaber": "firesaber2",
}

for func, size, variant, tweak in itertools.product(
    ["shake256", "sha256", "haraka"],
    ["128", "192", "256"],
    ["s", "f"],
    ["simple", "robust"],
):
    RENAMES[
        f"sphincs-{func}-{size}{variant}-{tweak}"
    ] = f"sphincs{variant}{size}{func}{tweak}"

SKIP_FILES = ["Makefile", "Makefile.Microsoft_nmake"]

PRIMITIVES = ["fips202", "sha2", "aes", "nistseedexpander", "sp800-185"]


def get_supercop_name(scheme: pqclean.Scheme) -> str:
    return RENAMES.get(scheme.name, scheme.name.replace("-", ""))

def _get_schemes() -> Iterator[pqclean.Scheme]:
    # Instead of just returning all schemes, we generate them manually,
    # because we want to map onto the SUPERCOP schemes

    # === KEMS ===
    # Kyber
    for size, postfix in itertools.product(["512", "768", "1024"], ["", "-90s"]):
        yield pqclean.Scheme.by_name(f"kyber{size}{postfix}")
    # HQC
    for params in ["128-1", "192-1", "192-2", "256-1", "256-2", "256-3"]:
        yield pqclean.Scheme.by_name(f"hqc-{params}-cca2")
    # Frodo
    for size, func in itertools.product(["640", "976", "1344"], ["aes", "shake"]):
        yield pqclean.Scheme.by_name(f"frodokem{size}{func}")
    # Classic McEliece
    for size, variant in itertools.product(
        ["348864", "460896", "6688128", "6960119", "8192128"], ["", "f"]
    ):
        yield pqclean.Scheme.by_name(f"mceliece{size}{variant}")
    # NTRU
    for variant in ["hrss701", "hps2048509", "hps2048677", "hps4096821"]:
        yield pqclean.Scheme.by_name(f"ntru{variant}")
    # SABER
    for variant in ["light", "", "fire"]:
        yield pqclean.Scheme.by_name(f"{variant}saber")

    # === Signature schemes ===
    # Dilithium
    for size in "234":
        yield pqclean.Scheme.by_name(f"dilithium{size}")
    # SPHINCS+
    for func, size, variant, tweak in itertools.product(
        ["shake256", "sha256", "haraka"],
        ["128", "192", "256"],
        ["s", "f"],
        ["simple", "robust"],
    ):
        yield pqclean.Scheme.by_name(f"sphincs-{func}-{size}{variant}-{tweak}")
    # Falcon
    for size in ["512", "1024"]:
        yield pqclean.Scheme.by_name(f"falcon-{size}")

def supercopy(src_path: Path, dest_path: Path):
    if src_path.name in SKIP_FILES:
        return
    shutil.copy2(src_path, dest_path)


def primitives_used(impl: pqclean.Implementation) -> Set[str]:
    found = set()
    impl_dir = Path(impl.path())
    for file in impl_dir.iterdir():
        with file.open("r") as f:
            contents = f.read()
        for primitive in PRIMITIVES:
            if f'#include "{primitive}.h"\n' in contents:
                found.add(primitive)
        if "KeccakP1600" in contents:
            found.add("keccak4x")
    if "sp800-185" in found:
        found.add("fips202")
    if "nistseedexpander" in found:
        found.add("aes")
    return found


def copy_primitives(impl: pqclean.Implementation, dest_dir: Path):
    for primitive in primitives_used(impl):
        if primitive == "keccak4x":
            for filename in (COMMON_DIR / "keccak4x").iterdir():
                supercopy(filename, dest_dir / filename.name)
            continue
        for ext in ["c", "h"]:
            filename = Path(f"{primitive}.{ext}")
            supercopy(COMMON_DIR / filename, dest_dir / filename)


def generate_supercop_api(impl: pqclean.Implementation) -> str:
    namespace = impl.namespace_prefix()
    defines = [
        f'#define CRYPTO_VERSION "PQCleaned {impl.metadata()["version"]}"',
    ]
    items = ["PUBLICKEYBYTES", "SECRETKEYBYTES", "BYTES"]
    if impl.scheme.type == "kem":
        items.append("CIPHERTEXTBYTES")
    for item in items:
        defines.append(f"#define CRYPTO_{item} {namespace}CRYPTO_{item}")

    defines_str = "\n".join(defines)

    return f"""\
/// Generated for the SUPERCOP api
/// We recommend using the namespaced PQClean api
#ifndef SUPERCOP_API_H
#define SUPERCOP_API_H

#include "pqclean_api.h"

{defines_str}

#endif
"""


def generate_supercop_kem(impl: pqclean.Implementation) -> str:
    namespace = impl.namespace_prefix()

    return f"""\
/// Generated for the SUPERCOP api
/// We recommend using the namespaced PQClean api
#include "pqclean_api.h"
#include "crypto_kem.h"

int crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {{
    return {namespace}crypto_kem_keypair(pk, sk);
}}

int crypto_kem_enc(unsigned char* c, unsigned char *k, const unsigned char *pk) {{
    return {namespace}crypto_kem_enc(c, k, pk);
}}

int crypto_kem_dec(unsigned char* k, const unsigned char *c, const unsigned char *sk) {{
    return {namespace}crypto_kem_dec(k, c, sk);
}}
"""


def generate_supercop_sign(impl) -> str:
    namespace = impl.namespace_prefix()
    return f"""\
/// Generated for the SUPERCOP api
/// We recommend using the namespaced PQClean api
#include "pqclean_api.h"
#include "crypto_sign.h"

int crypto_sign_keypair(unsigned char *pk, unsigned char *sk) {{
    return {namespace}crypto_sign_keypair(pk, sk);
}}

int crypto_sign(unsigned char *sm,unsigned long long *smlen,
                const unsigned char *m,unsigned long long mlen,
                const unsigned char *sk) {{
    return {namespace}crypto_sign(sm, (size_t*)smlen, m, (size_t)mlen, sk);
}}

int crypto_sign_open(unsigned char *m,unsigned long long *mlen,
                     const unsigned char *sm,unsigned long long smlen,
                     const unsigned char *pk) {{
    return {namespace}crypto_sign_open(m, (size_t*)mlen, sm, (size_t)smlen, pk);
}}
"""


def export_implementation(base_dir: Path, impl: pqclean.Implementation):
    impl_dest_dir = base_dir / impl.name
    if impl_dest_dir.exists():
        shutil.rmtree(impl_dest_dir)
    impl_dest_dir.mkdir(exist_ok=False)
    print(f"Exporting {impl} to {impl_dest_dir}")
    impl_src_dir = Path(impl.path())
    for src_file in (item for item in impl_src_dir.iterdir()):
        if src_file.name == "api.h":
            dest_file = impl_dest_dir / "pqclean_api.h"
        elif src_file.name == "kem.c":
            dest_file = impl_dest_dir / "pqclean_kem.c"
        elif src_file.name == "sign.c":
            dest_file = impl_dest_dir / "pqclean_sign.c"
        else:
            dest_file = impl_dest_dir / str(src_file.name)
        supercopy(src_file, dest_file)

    with open(impl_dest_dir / "api.h", "w") as file:
        file.write(generate_supercop_api(impl))
    if impl.scheme.type == "kem":
        with open(impl_dest_dir / "kem.c", "w") as file:
            file.write(generate_supercop_kem(impl))
    elif impl.scheme.type == "sign":
        with open(impl_dest_dir / "sign.c", "w") as file:
            file.write(generate_supercop_sign(impl))
    copy_primitives(impl, impl_dest_dir)


def export_scheme(scheme: pqclean.Scheme):
    dir = (
        SUPERCOP_EXPORT_DIR
        / f"crypto_{scheme.type}"
        / get_supercop_name(scheme)
        / "pqclean"
    )
    dir.mkdir(parents=True, exist_ok=True)

    for impl in scheme.implementations:
        export_implementation(dir, impl)


def main():
    for scheme in _get_schemes():
        export_scheme(scheme)

        # Also test it using SUPERCOP
        if (SUPERCOP_EXPORT_DIR / "do-part").exists():
            subprocess.run(
                ["./do-part", f"crypto_{scheme.type}", get_supercop_name(scheme)],
                cwd=SUPERCOP_EXPORT_DIR,
            )
            resultfile = next(SUPERCOP_EXPORT_DIR.glob("bench/*/data"))
            with resultfile.open() as file:
                for line in file.readlines():
                    if "fails" in line:
                        print(f"{scheme.name} failed")
                        print(line)


if __name__ == "__main__":
    main()
