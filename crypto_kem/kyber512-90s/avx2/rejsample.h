#ifndef PQCLEAN_KYBER51290S_AVX2_REJSAMPLE_H
#define PQCLEAN_KYBER51290S_AVX2_REJSAMPLE_H
#include "params.h"
#include <stdint.h>

unsigned int PQCLEAN_KYBER51290S_AVX2_rej_uniform_avx(int16_t *r,
        const unsigned char *buf);

#endif
