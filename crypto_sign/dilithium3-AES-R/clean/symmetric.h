#ifndef SYMMETRIC_H
#define SYMMETRIC_H

#include "fips202.h"
#include "params.h"
#include <stdint.h>


#include "aes256ctr.h"


typedef aes256ctr_ctx stream128_state;
typedef aes256ctr_ctx stream256_state;

#define dilithium_aes256ctr_init DILITHIUM_NAMESPACE(_dilithium_aes256ctr_init)
void dilithium_aes256ctr_init(aes256ctr_ctx *state,
                              const uint8_t key[32],
                              uint16_t nonce);

#define STREAM128_BLOCKBYTES AES256CTR_BLOCKBYTES
#define STREAM256_BLOCKBYTES AES256CTR_BLOCKBYTES

#define crh(OUT, IN, INBYTES) shake256(OUT, CRHBYTES, IN, INBYTES)
#define stream128_init(STATE, SEED, NONCE) \
    dilithium_aes256ctr_init(STATE, SEED, NONCE)
#define stream128_squeezeblocks(OUT, OUTBLOCKS, STATE) \
    aes256ctr_squeezeblocks(OUT, OUTBLOCKS, STATE)
#define stream128_ctx_release aes256ctr_ctx_release
#define stream256_init(STATE, SEED, NONCE) \
    dilithium_aes256ctr_init(STATE, SEED, NONCE)
#define stream256_squeezeblocks(OUT, OUTBLOCKS, STATE) \
    aes256ctr_squeezeblocks(OUT, OUTBLOCKS, STATE)
#define stream256_ctx_release aes256ctr_ctx_release


#endif
