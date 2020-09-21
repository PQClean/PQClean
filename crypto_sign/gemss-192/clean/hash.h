#ifndef _HASH_H
#define _HASH_H
#include "choice_crypto.h"
#include "fips202.h"


#define CHOICE_HASH_SHA3

/* Choice of the hash function */
#define HASH sha3_384
#define SHAKE shake256
#define SIZE_DIGEST 48
#define SIZE_DIGEST_UINT 6
#define SIZE_2_DIGEST 96
#define EQUALHASH_NOCST PQCLEAN_GEMSS192_CLEAN_ISEQUAL6_NOCST
#define COPYHASH COPY6

#define EQUALHASH(a,b) f_ISEQUAL(a,b,SIZE_DIGEST_UINT)

#define expandSeed(output,outputByteLen,seed,seedByteLen)         SHAKE(output,outputByteLen,seed,seedByteLen)

#define expandSeedCxtDeclaration Keccak_HashInstance hashInstance
#define expandSeedIUF Keccak_HashIUF_SHAKE
#define expandSeedSqueeze Keccak_HashSqueeze

#endif
