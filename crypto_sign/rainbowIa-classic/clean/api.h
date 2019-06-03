#ifndef PQCLEAN_RAINBOWIACLASSIC_CLEAN_API_H
#define PQCLEAN_RAINBOWIACLASSIC_CLEAN_API_H

#include <stddef.h>
#include <stdint.h>

#define _RAINBOW_CLASSIC
//#define _RAINBOW_CYCLIC
//#define _RAINBOW_CYCLIC_COMPRESSED

#define PQCLEAN_RAINBOWIACLASSIC_CLEAN_CRYPTO_SECRETKEYBYTES 92960
#define PQCLEAN_RAINBOWIACLASSIC_CLEAN_CRYPTO_PUBLICKEYBYTES 148992
#define PQCLEAN_RAINBOWIACLASSIC_CLEAN_CRYPTO_BYTES 64
#define PQCLEAN_RAINBOWIACLASSIC_CLEAN_CRYPTO_ALGNAME "RAINBOW(16,32,32,32) - classic"

//TODO: remove this after creating the other parameter sets

//#if defined _RAINBOW_CLASSIC
//
//#define PQCLEAN_RAINBOWIACLASSIC_CLEAN_CRYPTO_SECRETKEYBYTES sizeof(sk_t)
//#define PQCLEAN_RAINBOWIACLASSIC_CLEAN_CRYPTO_PUBLICKEYBYTES sizeof(pk_t)
//
//#elif defined _RAINBOW_CYCLIC
//
//#define CRYPTO_SECRETKEYBYTES sizeof(sk_t)
//#define CRYPTO_PUBLICKEYBYTES sizeof(cpk_t)
//
//#elif defined _RAINBOW_CYCLIC_COMPRESSED
//
//#define CRYPTO_SECRETKEYBYTES sizeof(csk_t)
//#define CRYPTO_PUBLICKEYBYTES sizeof(cpk_t)
//
//#else
//error here
//#endif
//
//
//#define PQCLEAN_RAINBOWIACLASSIC_CLEAN_CRYPTO_BYTES _SIGNATURE_BYTE
//
//#define PQCLEAN_RAINBOWIACLASSIC_CLEAN_CRYPTO_ALGNAME _S_NAME _SUFFIX

int PQCLEAN_RAINBOWIACLASSIC_CLEAN_crypto_sign_keypair(uint8_t *pk, uint8_t *sk);


int PQCLEAN_RAINBOWIACLASSIC_CLEAN_crypto_sign_signature(
    uint8_t *sig, size_t *siglen,
    const uint8_t *m, size_t mlen, const uint8_t *sk);

int PQCLEAN_RAINBOWIACLASSIC_CLEAN_crypto_sign_verify(
    const uint8_t *sig, size_t siglen,
    const uint8_t *m, size_t mlen, const uint8_t *pk);

int PQCLEAN_RAINBOWIACLASSIC_CLEAN_crypto_sign(uint8_t *sm, size_t *smlen,
        const uint8_t *m, size_t mlen,
        const uint8_t *sk);

int PQCLEAN_RAINBOWIACLASSIC_CLEAN_crypto_sign_open(uint8_t *m, size_t *mlen,
        const uint8_t *sm, size_t smlen,
        const uint8_t *pk);


#endif
