#ifndef PQCLEAN_KYBER768_AVX2_KEM_H
#define PQCLEAN_KYBER768_AVX2_KEM_H
#include "params.h"
#include <stdint.h>

#define PQCLEAN_KYBER768_AVX2_CRYPTO_SECRETKEYBYTES  KYBER_SECRETKEYBYTES
#define PQCLEAN_KYBER768_AVX2_CRYPTO_PUBLICKEYBYTES  KYBER_PUBLICKEYBYTES
#define PQCLEAN_KYBER768_AVX2_CRYPTO_CIPHERTEXTBYTES KYBER_CIPHERTEXTBYTES
#define PQCLEAN_KYBER768_AVX2_CRYPTO_BYTES           KYBER_SSBYTES

#define PQCLEAN_KYBER768_AVX2_CRYPTO_ALGNAME "Kyber768"

int PQCLEAN_KYBER768_AVX2_crypto_kem_keypair_derand(uint8_t *pk, uint8_t *sk, const uint8_t *coins);

int PQCLEAN_KYBER768_AVX2_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);

int PQCLEAN_KYBER768_AVX2_crypto_kem_enc_derand(uint8_t *ct, uint8_t *ss, const uint8_t *pk, const uint8_t *coins);

int PQCLEAN_KYBER768_AVX2_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);

int PQCLEAN_KYBER768_AVX2_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif
