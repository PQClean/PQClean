#ifndef PQCLEAN_MLKEM1024_AVX2_REDUCE_H
#define PQCLEAN_MLKEM1024_AVX2_REDUCE_H
#include "params.h"
#include <immintrin.h>

void PQCLEAN_MLKEM1024_AVX2_reduce_avx(__m256i *r, const __m256i *PQCLEAN_MLKEM1024_AVX2_qdata);
void PQCLEAN_MLKEM1024_AVX2_tomont_avx(__m256i *r, const __m256i *PQCLEAN_MLKEM1024_AVX2_qdata);

#endif
