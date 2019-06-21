#ifndef SPX_THASH_H
#define SPX_THASH_H

#include <stdint.h>
#include "primitive.h"

void PQCLEAN_SPHINCSHARAKA128SSIMPLE_CLEAN_thash_1(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    hash_state *state_seeded);

void PQCLEAN_SPHINCSHARAKA128SSIMPLE_CLEAN_thash_2(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    hash_state *state_seeded);

void PQCLEAN_SPHINCSHARAKA128SSIMPLE_CLEAN_thash_WOTS_LEN(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    hash_state *state_seeded);

void PQCLEAN_SPHINCSHARAKA128SSIMPLE_CLEAN_thash_FORS_TREES(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    hash_state *state_seeded);

#endif
