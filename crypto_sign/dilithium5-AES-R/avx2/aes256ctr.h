#ifndef AES256CTR_H
#define AES256CTR_H

#include "params.h"
#include <immintrin.h>
#include <stddef.h>
#include <stdint.h>

#define AES256CTR_BLOCKBYTES 64

typedef struct {
    __m128i rkeys[16];
    __m128i n;
} aes256ctr_ctx;

#define aes256ctr_init DILITHIUM_NAMESPACE(_init)
void aes256ctr_init(aes256ctr_ctx *state,
                    const uint8_t key[32],
                    uint64_t nonce);

#define aes256ctr_squeezeblocks DILITHIUM_NAMESPACE(_squeezeblocks)
void aes256ctr_squeezeblocks(uint8_t *out,
                             size_t nblocks,
                             aes256ctr_ctx *state);

#define aes256ctr_prf DILITHIUM_NAMESPACE(_prf)
void aes256ctr_prf(uint8_t *out,
                   size_t outlen,
                   const uint8_t key[32],
                   uint64_t nonce);

#define aes256ctr_ctx_release DILITHIUM_NAMESPACE(_aes256xof_ctx_release)
void aes256ctr_ctx_release(aes256ctr_ctx *s);


#endif
