#ifndef _HASH_H
#define _HASH_H
#include "choice_crypto.h"
#include "fips202.h"


#define CHOICE_HASH_SHA3

/* Choice of the hash function */
#define HASH sha3_512
#define SHAKE shake256
#define SIZE_DIGEST 64
#define SIZE_DIGEST_UINT 8
#define SIZE_2_DIGEST 128
#define EQUALHASH_NOCST PQCLEAN_GEMSS256_AVX2_ISEQUAL8_NOCST
#define COPYHASH COPY8

#define EQUALHASH(a,b) PQCLEAN_GEMSS256_AVX2_ISEQUAL(a,b,SIZE_DIGEST_UINT)

#define expandSeed(output,outputByteLen,seed,seedByteLen) \
    SHAKE(output,outputByteLen,seed,seedByteLen)

#define expandSeedCxtDeclaration Keccak_HashInstance hashInstance
#define expandSeedIUF Keccak_HashIUF_SHAKE
#define expandSeedSqueeze Keccak_HashSqueeze

#endif
