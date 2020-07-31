#ifndef PQCLEAN_KYBER768_AVX2_POLY_H
#define PQCLEAN_KYBER768_AVX2_POLY_H

#include "params.h"
#include <immintrin.h>
#include <stdint.h>

/*
 * Elements of R_q = Z_q[X]/(X^n + 1). Represents polynomial
 * coeffs[0] + X*coeffs[1] + X^2*xoeffs[2] + ... + X^{n-1}*coeffs[n-1]
 */
typedef union {
    __m256i dummy;
    int16_t coeffs[KYBER_N];
} poly;


void PQCLEAN_KYBER768_AVX2_poly_compress(uint8_t r[KYBER_POLYCOMPRESSEDBYTES], poly *a);

void PQCLEAN_KYBER768_AVX2_poly_decompress(poly *r, const uint8_t a[KYBER_POLYCOMPRESSEDBYTES]);


void PQCLEAN_KYBER768_AVX2_poly_tobytes(uint8_t r[KYBER_POLYBYTES], poly *a);

void PQCLEAN_KYBER768_AVX2_poly_frombytes(poly *r, const uint8_t a[KYBER_POLYBYTES]);


void PQCLEAN_KYBER768_AVX2_poly_frommsg(poly *r, const uint8_t msg[KYBER_INDCPA_MSGBYTES]);

void PQCLEAN_KYBER768_AVX2_poly_tomsg(uint8_t msg[KYBER_INDCPA_MSGBYTES], poly *a);


void PQCLEAN_KYBER768_AVX2_poly_getnoise(poly *r, const uint8_t seed[KYBER_SYMBYTES], uint8_t nonce);

void PQCLEAN_KYBER768_AVX2_poly_getnoise4x(poly *r0,
        poly *r1,
        poly *r2,
        poly *r3,
        const uint8_t *seed,
        uint8_t nonce0,
        uint8_t nonce1,
        uint8_t nonce2,
        uint8_t nonce3);


void PQCLEAN_KYBER768_AVX2_poly_ntt(poly *r);

void PQCLEAN_KYBER768_AVX2_poly_invntt_tomont(poly *r);

void PQCLEAN_KYBER768_AVX2_poly_nttunpack(poly *r);

void PQCLEAN_KYBER768_AVX2_poly_basemul_montgomery(poly *r, const poly *a, const poly *b);

void PQCLEAN_KYBER768_AVX2_poly_tomont(poly *r);


void PQCLEAN_KYBER768_AVX2_poly_reduce(poly *r);

void PQCLEAN_KYBER768_AVX2_poly_csubq(poly *r);


void PQCLEAN_KYBER768_AVX2_poly_add(poly *r, const poly *a, const poly *b);

void PQCLEAN_KYBER768_AVX2_poly_sub(poly *r, const poly *a, const poly *b);

#endif
