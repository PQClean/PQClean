#ifndef PQCLEAN_MCELIECE8192128_SSE_FFT_TR_H
#define PQCLEAN_MCELIECE8192128_SSE_FFT_TR_H
/*
  This file is for transpose of the Gao-Mateer FFT
*/


#include "params.h"
#include "vec128.h"

void PQCLEAN_MCELIECE8192128_SSE_fft_tr(vec128  /*out*/[][GFBITS], vec128  /*in*/[][ GFBITS ]);

#endif

