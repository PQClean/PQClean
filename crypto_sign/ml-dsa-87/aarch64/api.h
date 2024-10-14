#ifndef PQCLEAN_MLDSA87_AARCH64_API_H
#define PQCLEAN_MLDSA87_AARCH64_API_H

/*
 * This file is dual licensed
 * under Apache 2.0 (https://www.apache.org/licenses/LICENSE-2.0.html)
 * or public domain at https://github.com/pq-crystals/dilithium
 */

#include <stddef.h>
#include <stdint.h>

#define PQCLEAN_MLDSA87_AARCH64_CRYPTO_PUBLICKEYBYTES 2592
#define PQCLEAN_MLDSA87_AARCH64_CRYPTO_SECRETKEYBYTES 4896
#define PQCLEAN_MLDSA87_AARCH64_CRYPTO_BYTES          4627
#define PQCLEAN_MLDSA87_AARCH64_CRYPTO_ALGNAME       "ML-DSA-87"

int PQCLEAN_MLDSA87_AARCH64_crypto_sign_keypair(uint8_t *pk, uint8_t *sk);

int PQCLEAN_MLDSA87_AARCH64_crypto_sign_signature_ctx(
    uint8_t *sig, size_t *siglen,
    const uint8_t *m, size_t mlen,
    const uint8_t *ctx, size_t ctxlen,
    const uint8_t *sk);

int PQCLEAN_MLDSA87_AARCH64_crypto_sign_verify_ctx(
    const uint8_t *sig, size_t siglen,
    const uint8_t *m, size_t mlen,
    const uint8_t *ctx, size_t ctxlen,

    const uint8_t *pk);

int PQCLEAN_MLDSA87_AARCH64_crypto_sign_ctx(
    uint8_t *sm, size_t *smlen,
    const uint8_t *m, size_t mlen,
    const uint8_t *ctx, size_t ctxlen,
    const uint8_t *sk);

int PQCLEAN_MLDSA87_AARCH64_crypto_sign_open_ctx(
    uint8_t *m, size_t *mlen,
    const uint8_t *sm, size_t smlen,
    const uint8_t *ctx, size_t ctxlen,
    const uint8_t *pk);

#define PQCLEAN_MLDSA87_AARCH64_crypto_sign_signature(sig, siglen, m, mlen, sk) PQCLEAN_MLDSA87_AARCH64_crypto_sign_signature_ctx(sig, siglen, m, mlen, NULL, 0, sk)
#define PQCLEAN_MLDSA87_AARCH64_crypto_sign(sm, smlen, m, mlen, sk) PQCLEAN_MLDSA87_AARCH64_crypto_sign_ctx(sm, smlen, m, mlen, NULL, 0, sk)
#define PQCLEAN_MLDSA87_AARCH64_crypto_sign_verify(sig, siglen, m, mlen, pk) PQCLEAN_MLDSA87_AARCH64_crypto_sign_verify_ctx(sig, siglen, m, mlen, NULL, 0, pk)
#define PQCLEAN_MLDSA87_AARCH64_crypto_sign_open(m, mlen, sm, smlen, pk) PQCLEAN_MLDSA87_AARCH64_crypto_sign_open_ctx(m, mlen, sm, smlen, NULL, 0, pk)

#endif
