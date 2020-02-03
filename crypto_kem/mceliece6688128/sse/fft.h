#ifndef PQCLEAN_MCELIECE6688128_SSE_FFT_H
#define PQCLEAN_MCELIECE6688128_SSE_FFT_H
/*
  This file is for the Gao-Mateer FFT
  sse http://www.math.clemson.edu/~sgao/papers/GM10.pdf
*/


#include "params.h"
#include "vec128.h"
#include <stdint.h>

void PQCLEAN_MCELIECE6688128_SSE_fft(vec128  /*out*/[][GFBITS], vec128 * /*in*/);

#endif

