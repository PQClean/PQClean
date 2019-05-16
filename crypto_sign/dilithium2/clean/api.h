#ifndef PQCLEAN_DILITHIUM2_CLEAN_API_H
#define PQCLEAN_DILITHIUM2_CLEAN_API_H

#include <stdint.h>

#define MODE 2

#if MODE == 1
#define PQCLEAN_DILITHIUM1_CLEAN_CRYPTO_PUBLICKEYBYTES 896U
#define PQCLEAN_DILITHIUM1_CLEAN_CRYPTO_SECRETKEYBYTES 2096U
#define PQCLEAN_DILITHIUM1_CLEAN_CRYPTO_BYTES 1387U

#ifndef USE_AES
#define PQCLEAN_DILITHIUM1_CLEAN_CRYPTO_ALGNAME "Dilithium1"
#else
#define PQCLEAN_DILITHIUM1_CLEAN_CRYPTO_ALGNAME "Dilithium1-AES"
#endif

#elif MODE == 2
#define PQCLEAN_DILITHIUM2_CLEAN_CRYPTO_PUBLICKEYBYTES 1184U
#define PQCLEAN_DILITHIUM2_CLEAN_CRYPTO_SECRETKEYBYTES 2800U
#define PQCLEAN_DILITHIUM2_CLEAN_CRYPTO_BYTES 2044U

#ifndef USE_AES
#define PQCLEAN_DILITHIUM2_CLEAN_CRYPTO_ALGNAME "Dilithium2"
#else
#define PQCLEAN_DILITHIUM2_CLEAN_CRYPTO_ALGNAME "Dilithium2-AES"
#endif

#elif MODE == 3
#define CRYPTO_PUBLICKEYBYTES 1472U
#define CRYPTO_SECRETKEYBYTES 3504U
#define CRYPTO_BYTES 2701U

#ifndef USE_AES
#define CRYPTO_ALGNAME "Dilithium3"
#else
#define CRYPTO_ALGNAME "Dilithium3-AES"
#endif

#elif MODE == 4
#define CRYPTO_PUBLICKEYBYTES 1760U
#define CRYPTO_SECRETKEYBYTES 3856U
#define CRYPTO_BYTES 3366U

#ifndef USE_AES
#define CRYPTO_ALGNAME "Dilithium4"
#else
#define CRYPTO_ALGNAME "Dilithium4-AES"
#endif

#endif

int PQCLEAN_DILITHIUM2_CLEAN_crypto_sign_keypair(uint8_t *pk, uint8_t *sk);

int PQCLEAN_DILITHIUM2_CLEAN_crypto_sign_signature(
    uint8_t *sig, size_t *siglen,
    const uint8_t *m, size_t mlen, const uint8_t *sk);

int PQCLEAN_DILITHIUM2_CLEAN_crypto_sign_verify(
    const uint8_t *sig, size_t siglen,
    const uint8_t *m, size_t mlen, const uint8_t *pk);

int PQCLEAN_DILITHIUM2_CLEAN_crypto_sign(uint8_t *sm, size_t *smlen,
        const uint8_t *msg, size_t len,
        const uint8_t *sk);

int PQCLEAN_DILITHIUM2_CLEAN_crypto_sign_open(uint8_t *m, size_t *mlen,
        const uint8_t *sm, size_t smlen,
        const uint8_t *pk);


#endif
