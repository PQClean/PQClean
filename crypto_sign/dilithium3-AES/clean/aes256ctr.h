#ifndef AES256CTR_H
#define AES256CTR_H

#include "aes.h"
#include "params.h"
#include <stddef.h>
#include <stdint.h>

#define AES256CTR_BLOCKBYTES 64


typedef struct {
    aes256ctx ctx;
    uint8_t ivw[16];
    uint32_t ctr;
} aes256ctr_ctx;

#define aes256ctr_prf DILITHIUM_NAMESPACE(_aes256ctr_prf)
void aes256ctr_prf(uint8_t *out,
                   size_t outlen,
                   const uint8_t key[32],
                   const uint8_t nonce[12]);

#define aes256ctr_init DILITHIUM_NAMESPACE(_aes256ctr_init)
void aes256ctr_init(aes256ctr_ctx *state,
                    const uint8_t key[32],
                    const uint8_t nonce[12]);

#define aes256ctr_squeezeblocks DILITHIUM_NAMESPACE(_aes256ctr_squeezeblocks)
void aes256ctr_squeezeblocks(uint8_t *out,
                             size_t nblocks,
                             aes256ctr_ctx *state);

#define aes256ctr_ctx_release DILITHIUM_NAMESPACE(_aes256xof_ctx_release)
void aes256ctr_ctx_release(aes256ctr_ctx *s);



#endif
