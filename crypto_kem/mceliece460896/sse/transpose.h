#ifndef PQCLEAN_MCELIECE460896_SSE_TRANSPOSE_H
#define PQCLEAN_MCELIECE460896_SSE_TRANSPOSE_H
/*
  This file is for matrix transposition
*/


#include "vec128.h"

void PQCLEAN_MCELIECE460896_SSE_transpose_64x128_sp_asm(vec128 *);

inline static void PQCLEAN_MCELIECE460896_SSE_transpose_64x128_sp(vec128 *in) {
    PQCLEAN_MCELIECE460896_SSE_transpose_64x128_sp_asm(in);
}

#endif

