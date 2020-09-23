#ifndef _HASH_H
#define _HASH_H
#include "choice_crypto.h"
#include "fips202.h"


#define CHOICE_HASH_SHA3

/* Choice of the hash function */
#define HASH sha3_256
#define SHAKE shake128
#define SIZE_DIGEST 32
#define SIZE_DIGEST_UINT 4
#define SIZE_2_DIGEST 64
#define EQUALHASH_NOCST PQCLEAN_GEMSSRED128_CLEAN_ISEQUAL4_NOCST
#define COPYHASH COPY4

#define EQUALHASH(a,b) PQCLEAN_GEMSSRED128_CLEAN_ISEQUAL(a,b,SIZE_DIGEST_UINT)

#define expandSeed(output,outputByteLen,seed,seedByteLen)         SHAKE(output,outputByteLen,seed,seedByteLen)

#define expandSeedCxtDeclaration Keccak_HashInstance hashInstance
#define expandSeedIUF Keccak_HashIUF_SHAKE
#define expandSeedSqueeze Keccak_HashSqueeze

#endif
