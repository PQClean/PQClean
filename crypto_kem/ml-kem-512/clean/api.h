#ifndef PQCLEAN_MLKEM512_CLEAN_API_H
#define PQCLEAN_MLKEM512_CLEAN_API_H

#include <stdint.h>

#define PQCLEAN_MLKEM512_CLEAN_CRYPTO_SECRETKEYBYTES  1632
#define PQCLEAN_MLKEM512_CLEAN_CRYPTO_PUBLICKEYBYTES  800
#define PQCLEAN_MLKEM512_CLEAN_CRYPTO_CIPHERTEXTBYTES 768
#define PQCLEAN_MLKEM512_CLEAN_CRYPTO_BYTES           32
#define PQCLEAN_MLKEM512_CLEAN_CRYPTO_ALGNAME "ML-KEM-512"

int PQCLEAN_MLKEM512_CLEAN_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);

int PQCLEAN_MLKEM512_CLEAN_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);

int PQCLEAN_MLKEM512_CLEAN_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif
