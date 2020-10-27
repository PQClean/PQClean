#ifndef PQCLEAN_KYBER768_AVX2_REDUCE_H
#define PQCLEAN_KYBER768_AVX2_REDUCE_H
#include "consts.h"
#include <stdint.h>

int16_t PQCLEAN_KYBER768_AVX2_reduce_avx(int16_t *r, const qdata_t *PQCLEAN_KYBER768_AVX2_qdata);
int16_t PQCLEAN_KYBER768_AVX2_csubq_avx(int16_t *r, const qdata_t *PQCLEAN_KYBER768_AVX2_qdata);
int16_t PQCLEAN_KYBER768_AVX2_tomont_avx(int16_t *r, const qdata_t *PQCLEAN_KYBER768_AVX2_qdata);

#endif
