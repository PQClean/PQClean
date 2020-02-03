#ifndef PQCLEAN_MCELIECE6688128_AVX_TRANSPOSE_H
#define PQCLEAN_MCELIECE6688128_AVX_TRANSPOSE_H
/*
  This file is for matrix transposition
*/


#include "vec128.h"
#include "vec256.h"

extern void PQCLEAN_MCELIECE6688128_AVX_transpose_64x128_sp_asm(vec128 *);

static inline void PQCLEAN_MCELIECE6688128_AVX_transpose_64x128_sp(vec128 *in) {
    PQCLEAN_MCELIECE6688128_AVX_transpose_64x128_sp_asm(in);
}

extern void PQCLEAN_MCELIECE6688128_AVX_transpose_64x256_sp_asm(vec256 *);

static inline void PQCLEAN_MCELIECE6688128_AVX_transpose_64x256_sp(vec256 *in) {
    PQCLEAN_MCELIECE6688128_AVX_transpose_64x256_sp_asm(in);
}

#endif


