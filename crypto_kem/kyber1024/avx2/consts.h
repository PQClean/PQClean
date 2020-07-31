#ifndef PQCLEAN_KYBER1024_AVX2_CONSTS_H
#define PQCLEAN_KYBER1024_AVX2_CONSTS_H

#include "cdecl.inc"

#include "params.h"
#include <immintrin.h>
#include <stdint.h>

#define ALIGNED_UINT16_T(N) \
    union   {                   \
        __m256i as_vec;         \
        uint16_t as_arr[(N)];     \
    }

typedef ALIGNED_UINT16_T(928) qdata_t;

extern const qdata_t PQCLEAN_KYBER1024_AVX2_qdata;

#endif
