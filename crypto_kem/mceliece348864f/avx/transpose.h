#ifndef TRANSPOSE_H
#define TRANSPOSE_H
/*
  This file is for matrix transposition
*/


#include "vec256.h"

#include <stdint.h>

extern void PQCLEAN_MCELIECE348864F_AVX_transpose_64x64_asm(uint64_t *);

static inline void PQCLEAN_MCELIECE348864F_AVX_transpose_64x64(uint64_t *in) {
    PQCLEAN_MCELIECE348864F_AVX_transpose_64x64_asm(in);
}

extern void PQCLEAN_MCELIECE348864F_AVX_transpose_64x256_sp_asm(vec256 *);

static inline void PQCLEAN_MCELIECE348864F_AVX_transpose_64x256_sp(vec256 *in) {
    PQCLEAN_MCELIECE348864F_AVX_transpose_64x256_sp_asm(in);
}

#endif

