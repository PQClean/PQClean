#include "api.h"
#include "signHFE.h"
#include "sign_keypairHFE.h"
#include "sign_openHFE.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>



/**
 * @brief   Generation of a keypair for the digital signature.
 * @param[out]   pk  The public-key.
 * @param[out]   sk  The secret-key.
 * @return  Zero if the function runs correctly, non-zero else.
 */
int PQCLEAN_GEMSSRED256_CLEAN_crypto_sign_keypair(uint8_t *pk, uint8_t *sk) {
    return PQCLEAN_GEMSSRED256_CLEAN_sign_keypairHFE(pk, (UINT *)sk);
}


/**
 * @brief   Generation of the signature of a document.
 * @param[out]  sm  The signature, the document will be stored at the end.
 * @param[out]  smlen   The length of the signature (including the document).
 * @param[in]   m   The document.
 * @param[in]   mlen    The length of the document.
 * @param[in]   sk  The secret-key.
 * @return  Zero if the function runs correctly, non-zero else.
 */
int PQCLEAN_GEMSSRED256_CLEAN_crypto_sign(
    uint8_t *sm, size_t *smlen,
    const uint8_t *m, size_t mlen,
    const uint8_t *sk) {
    *smlen = mlen + PQCLEAN_GEMSSRED256_CLEAN_CRYPTO_BYTES;
    memcpy(sm + PQCLEAN_GEMSSRED256_CLEAN_CRYPTO_BYTES, m, mlen);
    return signHFE(sm, m, mlen, (UINT *)sk);
}


/**
 * @brief   Verification of the signature of a document.
 * @param[out]   m   The original document.
 * @param[out]   mlen    The length of the document.
 * @param[in]  sm  The signature, the document is stored at the end.
 * @param[in]  smlen   The length of the signature (including the document).
 * @param[in]   pk  The public-key.
 * @return  Zero if the function runs correctly, non-zero else.
 */
int PQCLEAN_GEMSSRED256_CLEAN_crypto_sign_open(
    uint8_t *m, size_t *mlen,
    const uint8_t *sm, size_t smlen,
    const uint8_t *pk) {
    int result;
    *mlen = smlen - PQCLEAN_GEMSSRED256_CLEAN_CRYPTO_BYTES;
    result = PQCLEAN_GEMSSRED256_CLEAN_sign_openHFE(sm + PQCLEAN_GEMSSRED256_CLEAN_CRYPTO_BYTES, (size_t)(*mlen), sm, pk);
    /* For compatibily with SUPERCOP, the memcpy is done only after sign_open */
    memcpy(m, sm + PQCLEAN_GEMSSRED256_CLEAN_CRYPTO_BYTES, (size_t)(*mlen));
    return result;
}


int PQCLEAN_GEMSSRED256_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk) {
    int result;
    *siglen = PQCLEAN_GEMSSRED256_CLEAN_CRYPTO_BYTES;
    result = signHFE(sig, m, mlen, (UINT *)sk);
    return result;
}

int PQCLEAN_GEMSSRED256_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk) {
    int result;
    if (siglen == PQCLEAN_GEMSSRED256_CLEAN_CRYPTO_BYTES) {
        result = PQCLEAN_GEMSSRED256_CLEAN_sign_openHFE(m, mlen, sig, pk);
    } else {
        result = -1;
    }
    return result;
}
