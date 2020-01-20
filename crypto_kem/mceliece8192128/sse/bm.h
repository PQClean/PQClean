#ifndef BM_H
#define BM_H
/*
  This file is for the inversion-free Berlekamp-Massey algorithm
  see https://ieeexplore.ieee.org/document/87857
*/


#include "params.h"
#include "vec128.h"

void PQCLEAN_MCELIECE8192128_SSE_bm(vec128 * /*out*/, vec128  /*in*/[][ GFBITS ]);

#endif

