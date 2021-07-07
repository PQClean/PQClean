#ifndef PQCLEAN_GEMSSRED128_AVX2_API_H
#define PQCLEAN_GEMSSRED128_AVX2_API_H

#include <stddef.h>
#include <stdint.h>

#define PQCLEAN_GEMSSRED128_AVX2_CRYPTO_ALGNAME                      "gemss-red-128"

#define PQCLEAN_GEMSSRED128_AVX2_CRYPTO_SECRETKEYBYTES               16
#define PQCLEAN_GEMSSRED128_AVX2_CRYPTO_PUBLICKEYBYTES               375213
#define PQCLEAN_GEMSSRED128_AVX2_CRYPTO_BYTES                        36

int PQCLEAN_GEMSSRED128_AVX2_crypto_sign_keypair(uint8_t *pk, uint8_t *sk);
int PQCLEAN_GEMSSRED128_AVX2_crypto_sign(uint8_t *sm, size_t *smlen, const uint8_t *msg, size_t len, const uint8_t *sk);
int PQCLEAN_GEMSSRED128_AVX2_crypto_sign_open(uint8_t *m, size_t *mlen, const uint8_t *sm, size_t smlen, const uint8_t *pk);
int PQCLEAN_GEMSSRED128_AVX2_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
int PQCLEAN_GEMSSRED128_AVX2_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

#endif
