#ifndef REDUCE_H
#define REDUCE_H

#include <stdint.h>

#include "consts.h"
#include "params.h"


int16_t PQCLEAN_KYBER102490S_AVX2_reduce_avx(int16_t *r, const qdata_t *PQCLEAN_KYBER102490S_AVX2_qdata);

int16_t PQCLEAN_KYBER102490S_AVX2_csubq_avx(int16_t *r, const qdata_t *PQCLEAN_KYBER102490S_AVX2_qdata);

int16_t PQCLEAN_KYBER102490S_AVX2_tomont_avx(int16_t *r, const qdata_t *PQCLEAN_KYBER102490S_AVX2_qdata);

#endif
