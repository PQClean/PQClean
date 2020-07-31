#ifndef SYMMETRIC_H
#define SYMMETRIC_H

#include "params.h"
#include <stddef.h>
#include <stdint.h>


#include "sha2.h"
#include "symmetric-aes.h"

typedef aes256xof_ctx xof_state;

#define XOF_BLOCKBYTES 64

#define hash_h(OUT, IN, INBYTES) sha256(OUT, IN, INBYTES)
#define hash_g(OUT, IN, INBYTES) sha512(OUT, IN, INBYTES)
#define xof_absorb(STATE, SEED, X, Y) PQCLEAN_KYBER102490S_CLEAN_aes256xof_absorb(STATE, SEED, X, Y)
#define xof_squeezeblocks(OUT, OUTBLOCKS, STATE) PQCLEAN_KYBER102490S_CLEAN_aes256xof_squeezeblocks(OUT, OUTBLOCKS, STATE)
#define xof_ctx_release(STATE) PQCLEAN_KYBER102490S_CLEAN_aes256xof_ctx_release(STATE)
#define prf(OUT, OUTBYTES, KEY, NONCE) PQCLEAN_KYBER102490S_CLEAN_aes256_prf(OUT, OUTBYTES, KEY, NONCE)
#define kdf(OUT, IN, INBYTES) sha256(OUT, IN, INBYTES)


#endif /* SYMMETRIC_H */
