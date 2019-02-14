#ifndef POLYVEC_H
#define POLYVEC_H

#include "params.h"
#include "poly.h"
#include <stdint.h>

/* Vectors of polynomials of length L */
typedef struct {
    poly vec[L];
} polyvecl;

void PQCLEAN_DILITHIUMIII_polyvecl_freeze(polyvecl *v);

void PQCLEAN_DILITHIUMIII_polyvecl_add(polyvecl *w, const polyvecl *u,
                                       const polyvecl *v);

void PQCLEAN_DILITHIUMIII_polyvecl_ntt(polyvecl *v);
void PQCLEAN_DILITHIUMIII_polyvecl_pointwise_acc_invmontgomery(
    poly *w, const polyvecl *u, const polyvecl *v);

int PQCLEAN_DILITHIUMIII_polyvecl_chknorm(const polyvecl *v, uint32_t bound);

/* Vectors of polynomials of length K */
typedef struct {
    poly vec[K];
} polyveck;

void PQCLEAN_DILITHIUMIII_polyveck_reduce(polyveck *v);
void PQCLEAN_DILITHIUMIII_polyveck_csubq(polyveck *v);
void PQCLEAN_DILITHIUMIII_polyveck_freeze(polyveck *v);

void PQCLEAN_DILITHIUMIII_polyveck_add(polyveck *w, const polyveck *u,
                                       const polyveck *v);
void PQCLEAN_DILITHIUMIII_polyveck_sub(polyveck *w, const polyveck *u,
                                       const polyveck *v);
void PQCLEAN_DILITHIUMIII_polyveck_shiftl(polyveck *v, unsigned int k);

void PQCLEAN_DILITHIUMIII_polyveck_ntt(polyveck *v);
void PQCLEAN_DILITHIUMIII_polyveck_invntt_montgomery(polyveck *v);

int PQCLEAN_DILITHIUMIII_polyveck_chknorm(const polyveck *v, uint32_t bound);

void PQCLEAN_DILITHIUMIII_polyveck_power2round(polyveck *v1, polyveck *v0,
        const polyveck *v);
void PQCLEAN_DILITHIUMIII_polyveck_decompose(polyveck *v1, polyveck *v0,
        const polyveck *v);
unsigned int PQCLEAN_DILITHIUMIII_polyveck_make_hint(polyveck *h,
        const polyveck *u,
        const polyveck *v);
void PQCLEAN_DILITHIUMIII_polyveck_use_hint(polyveck *w, const polyveck *u,
        const polyveck *h);

#endif
