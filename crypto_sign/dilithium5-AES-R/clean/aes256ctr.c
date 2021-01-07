#include "aes.h"
#include "symmetric.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>


static inline void br_enc32be(unsigned char *dst, uint32_t x) {
    dst[3] = (unsigned char)x;
    dst[2] = (unsigned char)(x >> 8);
    dst[1] = (unsigned char)(x >> 16);
    dst[0] = (unsigned char)(x >> 24);
}

void aes256ctr_prf(uint8_t *out, size_t outlen, const uint8_t *key, const uint8_t *nonce) {
    aes256ctx ctx;
    aes256_ctr_keyexp(&ctx, key);
    aes256_ctr(out, outlen, nonce, &ctx);
    aes256_ctx_release(&ctx);
}

void aes256ctr_init(aes256ctr_ctx *s, const uint8_t *key, const uint8_t *nonce) {
    aes256_ecb_keyexp(&s->ctx, key);
    memcpy(s->ivw, nonce, AESCTR_NONCEBYTES);
    s->ctr = 0;
}

void aes256ctr_squeezeblocks(uint8_t *out, size_t nblocks, aes256ctr_ctx *s) {
    br_enc32be(s->ivw + AESCTR_NONCEBYTES, s->ctr);
    while (nblocks > 0) {
        for (int i = 0; i < AES256CTR_BLOCKBYTES / AES_BLOCKBYTES; i++) {
            aes256_ecb(out, s->ivw, 1, &s->ctx);
            br_enc32be(s->ivw + AESCTR_NONCEBYTES, ++s->ctr);
            out += AES_BLOCKBYTES;
        }
        nblocks--;
    }
}

void aes256ctr_ctx_release(aes256ctr_ctx *s) {
    aes256_ctx_release(&s->ctx);
}
