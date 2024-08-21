#ifndef PQCLEAN_MLKEM768_AARCH64_SYMMETRIC_H
#define PQCLEAN_MLKEM768_AARCH64_SYMMETRIC_H

/*
 * This file is licensed
 * under Apache 2.0 (https://www.apache.org/licenses/LICENSE-2.0.html)
 * at https://github.com/GMUCERG/PQC_NEON/blob/main/neon/kyber or
 * public domain at https://github.com/cothan/kyber/blob/master/neon
 */

#include <stddef.h>
#include <stdint.h>
#include "params.h"

#include "fips202.h"

typedef shake128ctx xof_state;

#define kyber_shake128_absorb KYBER_NAMESPACE(kyber_shake128_absorb)
void kyber_shake128_absorb(shake128ctx *s,
                           const uint8_t seed[KYBER_SYMBYTES],
                           uint8_t x,
                           uint8_t y);

#define kyber_shake256_prf KYBER_NAMESPACE(kyber_shake256_prf)
void kyber_shake256_prf(uint8_t *out, size_t outlen, const uint8_t key[KYBER_SYMBYTES], uint8_t nonce);
#define kyber_shake256_rkprf KYBER_NAMESPACE(kyber_shake256_rkprf)
void kyber_shake256_rkprf(uint8_t out[KYBER_SSBYTES], const uint8_t key[KYBER_SYMBYTES], const uint8_t input[KYBER_CIPHERTEXTBYTES]);

#define XOF_BLOCKBYTES SHAKE128_RATE

#define hash_h(OUT, IN, INBYTES) sha3_256(OUT, IN, INBYTES)
#define hash_g(OUT, IN, INBYTES) sha3_512(OUT, IN, INBYTES)
#define xof_absorb(STATE, SEED, X, Y) kyber_shake128_absorb(STATE, SEED, X, Y)
#define xof_squeezeblocks(OUT, OUTBLOCKS, STATE) shake128_squeezeblocks(OUT, OUTBLOCKS, STATE)
#define prf(OUT, OUTBYTES, KEY, NONCE) kyber_shake256_prf(OUT, OUTBYTES, KEY, NONCE)
#define rkprf(OUT, KEY, INPUT) kyber_shake256_rkprf(OUT, KEY, INPUT)

// NEON Definition
#include "keccak2x/fips202x2.h"

typedef keccakx2_state neon_xof_state;

#define neon_kyber_shake128_absorb KYBER_NAMESPACE(neon_kyber_shake128_absorb)
void neon_kyber_shake128_absorb(keccakx2_state *s,
                                const uint8_t seed[KYBER_SYMBYTES],
                                uint8_t x1, uint8_t x2,
                                uint8_t y1, uint8_t y2);

#define neon_kyber_shake256_prf KYBER_NAMESPACE(neon_kyber_shake256_prf)
void neon_kyber_shake256_prf(uint8_t *out1, uint8_t *out2,
                             size_t outlen,
                             const uint8_t key[KYBER_SYMBYTES],
                             uint8_t nonce1, uint8_t nonce2);

#define XOF_BLOCKBYTES SHAKE128_RATE

#define neon_prf(OUT1, OUT2, OUTBYTES, KEY, NONCE1, NONCE2) \
    neon_kyber_shake256_prf(OUT1, OUT2, OUTBYTES, KEY, NONCE1, NONCE2);

#define neon_xof_absorb(STATE, SEED, X1, X2, Y1, Y2)\
    neon_kyber_shake128_absorb(STATE, SEED, X1, X2, Y1, Y2)

#define neon_xof_squeezeblocks(OUT0, OUT1, OUTBLOCKS, STATE) \
    shake128x2_squeezeblocks(OUT0, OUT1, OUTBLOCKS, STATE)

#endif /* SYMMETRIC_H */
