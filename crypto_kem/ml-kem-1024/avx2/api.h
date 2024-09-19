#ifndef PQCLEAN_MLKEM1024_AVX2_API_H
#define PQCLEAN_MLKEM1024_AVX2_API_H

#include <stdint.h>

#define PQCLEAN_MLKEM1024_AVX2_CRYPTO_SECRETKEYBYTES  3168
#define PQCLEAN_MLKEM1024_AVX2_CRYPTO_PUBLICKEYBYTES  1568
#define PQCLEAN_MLKEM1024_AVX2_CRYPTO_CIPHERTEXTBYTES 1568
#define PQCLEAN_MLKEM1024_AVX2_CRYPTO_BYTES           32
#define PQCLEAN_MLKEM1024_AVX2_CRYPTO_ALGNAME "ML-KEM-1024"

int PQCLEAN_MLKEM1024_AVX2_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);

int PQCLEAN_MLKEM1024_AVX2_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);

int PQCLEAN_MLKEM1024_AVX2_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif
