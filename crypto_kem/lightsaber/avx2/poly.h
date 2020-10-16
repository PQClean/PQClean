#ifndef POLY_H
#define POLY_H
/*---------------------------------------------------------------------
This file has been adapted from the implementation
(available at, Public Domain https://github.com/pq-crystals/kyber)
of "CRYSTALS – Kyber: a CCA-secure module-lattice-based KEM"
by : Joppe Bos, Leo Ducas, Eike Kiltz, Tancrede Lepoint,
Vadim Lyubashevsky, John M. Schanck, Peter Schwabe & Damien stehle
----------------------------------------------------------------------*/
#include "SABER_params.h"
#include <stdint.h>

typedef struct {
    uint16_t coeffs[SABER_N];
} poly;

typedef struct {
    poly vec[SABER_K];
} polyvec;

void PQCLEAN_LIGHTSABER_AVX2_poly_getnoise(uint16_t *r, const unsigned char *seed, unsigned char nonce);


void PQCLEAN_LIGHTSABER_AVX2_poly_getnoise4x(uint16_t *r0, uint16_t *r1, uint16_t *r2, const unsigned char *seed, unsigned char nonce0, unsigned char nonce1, unsigned char nonce2, unsigned char nonce3);


#endif
