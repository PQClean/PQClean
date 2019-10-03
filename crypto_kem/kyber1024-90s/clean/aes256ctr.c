#include "aes256ctr.h"
#include "aes.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

static inline uint32_t br_swap32(uint32_t x) {
    x = ((x & (uint32_t)0x00FF00FF) << 8)
        | ((x >> 8) & (uint32_t)0x00FF00FF);
    return (x << 16) | (x >> 16);
}


static inline void inc1_be(uint32_t *x) {
    uint32_t t = br_swap32(*x) + 1;
    *x = br_swap32(t);
}

static void aes256_ctr_xof(unsigned char *out, size_t outlen, const unsigned char *iv, uint32_t ctr, const aes256ctx *ctx) {
    uint32_t ivw[4] = {0};
    ivw[3] = br_swap32(ctr);
    uint8_t buf[AES_BLOCKBYTES];
    size_t i;

    memcpy(ivw, iv, AESCTR_NONCEBYTES);

    while (outlen > AES_BLOCKBYTES) {
        aes256_ecb(out, (uint8_t *) ivw, 1, ctx);
        inc1_be(ivw + 3);
        out += AES_BLOCKBYTES;
        outlen -= AES_BLOCKBYTES;
    }
    if (outlen > 0) {
        aes256_ecb(buf, (uint8_t *) ivw, 1, ctx);
        for (i = 0; i < outlen; i++) {
            out[i] = buf[i];
        }
    }
}

/*************************************************
* Name:        aes256_prf
*
* Description: AES256 stream generation in CTR mode using 32-bit counter,
*              nonce is zero-padded to 12 bytes, counter starts at zero
*
* Arguments:   - uint8_t *output:      pointer to output
*              - size_t outlen:        length of requested output in bytes
*              - const uint8_t *key:   pointer to 32-byte key
*              - uint8_t nonce:        1-byte nonce (will be zero-padded to 12 bytes)
**************************************************/
void PQCLEAN_KYBER102490S_CLEAN_aes256_prf(uint8_t *output, size_t outlen, const uint8_t *key, uint8_t nonce) {
    uint8_t iv[12];
    for (int i = 1; i < 12; i++) {
        iv[i] = 0;
    }
    iv[0] = nonce;

    aes256ctx ctx;
    aes256_keyexp(&ctx, key);
    aes256_ctr(output, outlen, iv, &ctx);
    aes256_ctx_release(&ctx);
}

/*************************************************
* Name:        aes256xof_absorb
*
* Description: AES256 CTR used as a replacement for a XOF; this function
*              "absorbs" a 32-byte key and two additional bytes that are zero-padded
*              to a 12-byte nonce
*
* Arguments:   - aes256xof_ctx *s:    pointer to state to "absorb" key and IV into
*              - const uint8_t *key:  pointer to 32-byte key
*              - uint8_t x:           first additional byte to "absorb"
*              - uint8_t y:           second additional byte to "absorb"
**************************************************/
void PQCLEAN_KYBER102490S_CLEAN_aes256xof_absorb(aes256xof_ctx *s, const uint8_t *key, uint8_t x, uint8_t y) {
    aes256_keyexp(&s->sk_exp, key);
    for (int i = 2; i < 12; i++) {
        s->iv[i] = 0;
    }
    s->iv[0] = x;
    s->iv[1] = y;
    s->ctr = 0;
}

/*************************************************
* Name:        aes256xof_squeezeblocks
*
* Description: AES256 CTR used as a replacement for a XOF; this function
*              generates 4 blocks out AES256-CTR output
*
* Arguments:   - uint8_t *out:          pointer to output
*              - size_t nblocks:        number of reqested 64-byte output blocks
*              - aes256xof_ctx *s:      AES "state", i.e. expanded key and IV
**************************************************/
void PQCLEAN_KYBER102490S_CLEAN_aes256xof_squeezeblocks(uint8_t *out, size_t nblocks, aes256xof_ctx *s) {
    aes256_ctr_xof(out, nblocks * 64, s->iv, s->ctr, &s->sk_exp);
    s->ctr += 4 * nblocks;
}
