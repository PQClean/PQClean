#ifndef PQCLEAN_QTESLAPIII_CLEAN_API_H
#define PQCLEAN_QTESLAPIII_CLEAN_API_H

/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: API header file
**************************************************************************************/

#include <stddef.h>
#include <stdint.h>

#define PQCLEAN_QTESLAPIII_CLEAN_CRYPTO_SECRETKEYBYTES 12352
#define PQCLEAN_QTESLAPIII_CLEAN_CRYPTO_PUBLICKEYBYTES 38432
#define PQCLEAN_QTESLAPIII_CLEAN_CRYPTO_BYTES 5664
#define PQCLEAN_QTESLAPIII_CLEAN_CRYPTO_ALGNAME "qTESLA-p-III"

int PQCLEAN_QTESLAPIII_CLEAN_crypto_sign_keypair(
    uint8_t *pk, uint8_t *sk);

int PQCLEAN_QTESLAPIII_CLEAN_crypto_sign(
    uint8_t *sm, size_t *smlen,
    const uint8_t *m, size_t mlen,
    const uint8_t *sk);

int PQCLEAN_QTESLAPIII_CLEAN_crypto_sign_open(
    uint8_t *m, size_t *mlen,
    const uint8_t *sm, size_t smlen,
    const uint8_t *pk
);

int PQCLEAN_QTESLAPIII_CLEAN_crypto_sign_signature(
    uint8_t *sig, size_t *siglen,
    const uint8_t *m, size_t mlen,
    const uint8_t *sk);

int PQCLEAN_QTESLAPIII_CLEAN_crypto_sign_verify(
    const uint8_t *sig, size_t siglen,
    const uint8_t *m, size_t mlen,
    const uint8_t *pk);

#endif
