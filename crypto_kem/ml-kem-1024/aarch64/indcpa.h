#ifndef PQCLEAN_MLKEM1024_AARCH64_INDCPA_H
#define PQCLEAN_MLKEM1024_AARCH64_INDCPA_H

/*
 * This file is licensed
 * under Apache 2.0 (https://www.apache.org/licenses/LICENSE-2.0.html) or
 * public domain at https://github.com/pq-crystals/kyber/tree/master/ref
 */

#include <stdint.h>
#include "params.h"
#include "polyvec.h"

#define gen_matrix KYBER_NAMESPACE(gen_matrix)
void gen_matrix(int16_t a[KYBER_K][KYBER_K][KYBER_N], const uint8_t seed[KYBER_SYMBYTES], int transposed);
#define indcpa_keypair_derand KYBER_NAMESPACE(indcpa_keypair_derand)
void indcpa_keypair_derand(uint8_t pk[KYBER_INDCPA_PUBLICKEYBYTES],
                           uint8_t sk[KYBER_INDCPA_SECRETKEYBYTES],
                           const uint8_t coins[KYBER_SYMBYTES]);

#define indcpa_enc KYBER_NAMESPACE(indcpa_enc)
void indcpa_enc(uint8_t c[KYBER_INDCPA_BYTES],
                const uint8_t m[KYBER_INDCPA_MSGBYTES],
                const uint8_t pk[KYBER_INDCPA_PUBLICKEYBYTES],
                const uint8_t coins[KYBER_SYMBYTES]);

#define indcpa_dec KYBER_NAMESPACE(indcpa_dec)
void indcpa_dec(uint8_t m[KYBER_INDCPA_MSGBYTES],
                const uint8_t c[KYBER_INDCPA_BYTES],
                const uint8_t sk[KYBER_INDCPA_SECRETKEYBYTES]);

#endif
