#ifndef RNG_H
#define RNG_H

#include <stddef.h>
#include <stdint.h>

#define RNG_SUCCESS     ( 0)
#define RNG_BAD_MAXLEN  (-1)
#define RNG_BAD_OUTBUF  (-2)
#define RNG_BAD_REQ_LEN (-3)

typedef struct {
    unsigned char   buffer[16];
    unsigned int    buffer_pos;
    uint64_t        length_remaining;
    unsigned char   key[32];
    unsigned char   ctr[16];
} AES_XOF_struct;

int PQCLEAN_LEDAKEMLT12_CLEAN_seedexpander(AES_XOF_struct *ctx, unsigned char *x, size_t xlen);
void PQCLEAN_LEDAKEMLT12_CLEAN_seedexpander_from_trng(AES_XOF_struct *ctx, const unsigned char *trng_entropy);

#endif
