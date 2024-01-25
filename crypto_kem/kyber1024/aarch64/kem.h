#ifndef KEM_H
#define KEM_H

/*
 * This file is licensed
 * under Apache 2.0 (https://www.apache.org/licenses/LICENSE-2.0.html) or
 * public domain at https://github.com/pq-crystals/kyber/tree/master/ref
 */

#include "params.h"
#include <stdint.h>

#define CRYPTO_SECRETKEYBYTES  KYBER_SECRETKEYBYTES
#define CRYPTO_PUBLICKEYBYTES  KYBER_PUBLICKEYBYTES
#define CRYPTO_CIPHERTEXTBYTES KYBER_CIPHERTEXTBYTES
#define CRYPTO_BYTES           KYBER_SSBYTES

#define CRYPTO_ALGNAME "Kyber1024"

#define crypto_kem_keypair_derand KYBER_NAMESPACE(keypair_derand)
int crypto_kem_keypair_derand(uint8_t *pk, uint8_t *sk, const uint8_t *coins);

#define crypto_kem_keypair KYBER_NAMESPACE(crypto_kem_keypair)
int crypto_kem_keypair(uint8_t *pk, uint8_t *sk);

#define crypto_kem_enc_derand KYBER_NAMESPACE(enc_derand)
int crypto_kem_enc_derand(uint8_t *ct, uint8_t *ss, const uint8_t *pk, const uint8_t *coins);

#define crypto_kem_enc KYBER_NAMESPACE(crypto_kem_enc)
int crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);

#define crypto_kem_dec KYBER_NAMESPACE(crypto_kem_dec)
int crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif
