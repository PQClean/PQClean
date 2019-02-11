#ifndef POLY_H
#define POLY_H

#include "fips202.h"
#include "params.h"
#include <stdint.h>

typedef struct {
    uint32_t coeffs[N];
} poly;

void PQCLEAN_DILITHIUMIII_poly_reduce(poly *a);
void PQCLEAN_DILITHIUMIII_poly_csubq(poly *a);
void PQCLEAN_DILITHIUMIII_poly_freeze(poly *a);

void PQCLEAN_DILITHIUMIII_poly_add(poly *c, const poly *a, const poly *b);
void PQCLEAN_DILITHIUMIII_poly_sub(poly *c, const poly *a, const poly *b);
void PQCLEAN_DILITHIUMIII_poly_neg(poly *a);
void PQCLEAN_DILITHIUMIII_poly_shiftl(poly *a, unsigned int k);

void PQCLEAN_DILITHIUMIII_poly_ntt(poly *a);
void PQCLEAN_DILITHIUMIII_poly_invntt_montgomery(poly *a);
void PQCLEAN_DILITHIUMIII_poly_pointwise_invmontgomery(poly *c, const poly *a,
                                                       const poly *b);

void PQCLEAN_DILITHIUMIII_poly_power2round(poly *a1, poly *a0, const poly *a);
void PQCLEAN_DILITHIUMIII_poly_decompose(poly *a1, poly *a0, const poly *a);
unsigned int PQCLEAN_DILITHIUMIII_poly_make_hint(poly *h, const poly *a,
                                                 const poly *b);
void PQCLEAN_DILITHIUMIII_poly_use_hint(poly *a, const poly *b, const poly *h);

int PQCLEAN_DILITHIUMIII_poly_chknorm(const poly *a, uint32_t B);
void PQCLEAN_DILITHIUMIII_poly_uniform(poly *a, const unsigned char *buf);
void PQCLEAN_DILITHIUMIII_poly_uniform_eta(poly *a,
                                           const unsigned char seed[SEEDBYTES],
                                           unsigned char nonce);
void PQCLEAN_DILITHIUMIII_poly_uniform_gamma1m1(
    poly *a, const unsigned char seed[SEEDBYTES + CRHBYTES], uint16_t nonce);

void PQCLEAN_DILITHIUMIII_polyeta_pack(unsigned char *r, const poly *a);
void PQCLEAN_DILITHIUMIII_polyeta_unpack(poly *r, const unsigned char *a);

void PQCLEAN_DILITHIUMIII_polyt1_pack(unsigned char *r, const poly *a);
void PQCLEAN_DILITHIUMIII_polyt1_unpack(poly *r, const unsigned char *a);

void PQCLEAN_DILITHIUMIII_polyt0_pack(unsigned char *r, const poly *a);
void PQCLEAN_DILITHIUMIII_polyt0_unpack(poly *r, const unsigned char *a);

void PQCLEAN_DILITHIUMIII_polyz_pack(unsigned char *r, const poly *a);
void PQCLEAN_DILITHIUMIII_polyz_unpack(poly *r, const unsigned char *a);

void PQCLEAN_DILITHIUMIII_polyw1_pack(unsigned char *r, const poly *a);
#endif
