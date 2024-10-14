#ifndef PQCLEAN_DILITHIUM2_AVX2_API_H
#define PQCLEAN_DILITHIUM2_AVX2_API_H

#include <stddef.h>
#include <stdint.h>

#define PQCLEAN_DILITHIUM2_AVX2_CRYPTO_PUBLICKEYBYTES 1312
#define PQCLEAN_DILITHIUM2_AVX2_CRYPTO_SECRETKEYBYTES 2560
#define PQCLEAN_DILITHIUM2_AVX2_CRYPTO_BYTES 2420
#define PQCLEAN_DILITHIUM2_AVX2_CRYPTO_ALGNAME "Dilithium2"

int PQCLEAN_DILITHIUM2_AVX2_crypto_sign_keypair(uint8_t *pk, uint8_t *sk);

int PQCLEAN_DILITHIUM2_AVX2_crypto_sign_signature_ctx(uint8_t *sig, size_t *siglen,
        const uint8_t *m, size_t mlen,
        const uint8_t *ctx, size_t ctxlen,
        const uint8_t *sk);

int PQCLEAN_DILITHIUM2_AVX2_crypto_sign_ctx(uint8_t *sm, size_t *smlen,
        const uint8_t *m, size_t mlen,
        const uint8_t *ctx, size_t ctxlen,
        const uint8_t *sk);

int PQCLEAN_DILITHIUM2_AVX2_crypto_sign_verify_ctx(const uint8_t *sig, size_t siglen,
        const uint8_t *m, size_t mlen,
        const uint8_t *ctx, size_t ctxlen,
        const uint8_t *pk);

int PQCLEAN_DILITHIUM2_AVX2_crypto_sign_open_ctx(uint8_t *m, size_t *mlen,
        const uint8_t *sm, size_t smlen,
        const uint8_t *ctx, size_t ctxlen,
        const uint8_t *pk);

#define PQCLEAN_DILITHIUM2_AVX2_crypto_sign_signature(sig, siglen, m, mlen, sk) PQCLEAN_DILITHIUM2_AVX2_crypto_sign_signature_ctx(sig, siglen, m, mlen, NULL, 0, sk)
#define PQCLEAN_DILITHIUM2_AVX2_crypto_sign(sm, smlen, m, mlen, sk) PQCLEAN_DILITHIUM2_AVX2_crypto_sign_ctx(sm, smlen, m, mlen, NULL, 0, sk)
#define PQCLEAN_DILITHIUM2_AVX2_crypto_sign_verify(sig, siglen, m, mlen, pk) PQCLEAN_DILITHIUM2_AVX2_crypto_sign_verify_ctx(sig, siglen, m, mlen, NULL, 0, pk)
#define PQCLEAN_DILITHIUM2_AVX2_crypto_sign_open(m, mlen, sm, smlen, pk) PQCLEAN_DILITHIUM2_AVX2_crypto_sign_open_ctx(m, mlen, sm, smlen, NULL, 0, pk)

#endif
