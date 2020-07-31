#ifndef SYMMETRIC_H
#define SYMMETRIC_H

#include "params.h"
#include <stddef.h>
#include <stdint.h>


#include "aes256ctr.h"
#include "sha2.h"

typedef aes256ctr_ctx xof_state;

#define XOF_BLOCKBYTES AES256CTR_BLOCKBYTES

#define hash_h(OUT, IN, INBYTES) sha256(OUT, IN, INBYTES)
#define hash_g(OUT, IN, INBYTES) sha512(OUT, IN, INBYTES)
#define xof_absorb(STATE, SEED, X, Y) \
    PQCLEAN_KYBER76890S_AVX2_aes256ctr_init(STATE, SEED, (X) | ((uint16_t)(Y) << 8))
#define xof_squeezeblocks(OUT, OUTBLOCKS, STATE) \
    PQCLEAN_KYBER76890S_AVX2_aes256ctr_squeezeblocks(OUT, OUTBLOCKS, STATE)
#define prf(OUT, OUTBYTES, KEY, NONCE) \
    PQCLEAN_KYBER76890S_AVX2_aes256ctr_prf(OUT, OUTBYTES, KEY, NONCE)
#define kdf(OUT, IN, INBYTES) sha256(OUT, IN, INBYTES)


#endif /* SYMMETRIC_H */
