#ifndef REJSAMPLE_H
#define REJSAMPLE_H

#include <stdint.h>

unsigned int PQCLEAN_KYBER768_AVX2_rej_uniform(int16_t *r,
        unsigned int len,
        const uint8_t *buf,
        unsigned int buflen);

#endif
