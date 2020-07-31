#ifndef PQCLEAN_KYBER768_AVX2_CBD_H
#define PQCLEAN_KYBER768_AVX2_CBD_H

#include "params.h"
#include "poly.h"
#include <stdint.h>


void PQCLEAN_KYBER768_AVX2_cbd(poly *r, const uint8_t buf[KYBER_ETA * KYBER_N / 4]);

#endif
