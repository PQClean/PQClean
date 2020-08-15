#ifndef POLY_H
#define POLY_H

#include <stdint.h>

#include "params.h"

#define MODQ(X) ((X) & (NTRU_Q-1))

typedef struct {
    // round to nearest multiple of 32 to make it easier to load into vector
    //   registers without having to do bound checks
#define NTRU_N_32 PAD32(NTRU_N)
    uint16_t coeffs[NTRU_N_32] __attribute__((aligned(32)));
} poly;

void PQCLEAN_NTRUHPS2048677_AVX2_poly_mod_3_Phi_n(poly *r);
void PQCLEAN_NTRUHPS2048677_AVX2_poly_mod_q_Phi_n(poly *r);

void PQCLEAN_NTRUHPS2048677_AVX2_poly_Sq_tobytes(unsigned char *r, const poly *a);
void PQCLEAN_NTRUHPS2048677_AVX2_poly_Sq_frombytes(poly *r, const unsigned char *a);

void PQCLEAN_NTRUHPS2048677_AVX2_poly_Rq_sum_zero_tobytes(unsigned char *r, const poly *a);
void PQCLEAN_NTRUHPS2048677_AVX2_poly_Rq_sum_zero_frombytes(poly *r, const unsigned char *a);

void PQCLEAN_NTRUHPS2048677_AVX2_poly_S3_tobytes(unsigned char msg[NTRU_PACK_TRINARY_BYTES], const poly *a);
void PQCLEAN_NTRUHPS2048677_AVX2_poly_S3_frombytes(poly *r, const unsigned char msg[NTRU_PACK_TRINARY_BYTES]);

void PQCLEAN_NTRUHPS2048677_AVX2_poly_Sq_mul(poly *r, const poly *a, const poly *b);
void PQCLEAN_NTRUHPS2048677_AVX2_poly_Rq_mul(poly *r, const poly *a, const poly *b);
void PQCLEAN_NTRUHPS2048677_AVX2_poly_S3_mul(poly *r, const poly *a, const poly *b);
void PQCLEAN_NTRUHPS2048677_AVX2_poly_lift(poly *r, const poly *a);
void PQCLEAN_NTRUHPS2048677_AVX2_poly_Rq_to_S3(poly *r, const poly *a);

void PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_inv(poly *r, const poly *a);
void PQCLEAN_NTRUHPS2048677_AVX2_poly_Rq_inv(poly *r, const poly *a);
void PQCLEAN_NTRUHPS2048677_AVX2_poly_S3_inv(poly *r, const poly *a);

void PQCLEAN_NTRUHPS2048677_AVX2_poly_Z3_to_Zq(poly *r);
void PQCLEAN_NTRUHPS2048677_AVX2_poly_trinary_Zq_to_Z3(poly *r);

#endif
