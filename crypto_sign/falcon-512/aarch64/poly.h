#ifndef POLY_H
#define POLY_H

#include "inner.h"
#include "config.h"

typedef enum ntt_domain {
    NTT_NONE = 0,
    NTT_MONT = 1,
    NTT_MONT_INV = 2,
} ntt_domain_t;

typedef enum invntt_domain {
    INVNTT_NONE = 0,
    INVNTT_NINV = 1,
} invntt_domain_t;

void ZfN(poly_ntt)(int16_t a[FALCON_N], ntt_domain_t mont);

void ZfN(poly_invntt)(int16_t a[FALCON_N], invntt_domain_t ninv);

void ZfN(poly_int8_to_int16)(int16_t out[FALCON_N], const int8_t in[FALCON_N]);

void ZfN(poly_div_12289)(int16_t f[FALCON_N], const int16_t g[FALCON_N]);

void ZfN(poly_convert_to_unsigned)(int16_t f[FALCON_N]);

uint16_t ZfN(poly_compare_with_zero)(int16_t f[FALCON_N]);

void ZfN(poly_montmul_ntt)(int16_t f[FALCON_N], const int16_t g[FALCON_N]);

void ZfN(poly_sub_barrett)(int16_t f[FALCON_N], const int16_t g[FALCON_N], const int16_t s[FALCON_N]);

int ZfN(poly_int16_to_int8)(int8_t G[FALCON_N], const int16_t t[FALCON_N]);

int ZfN(poly_check_bound_int8)(const int8_t t[FALCON_N],
                               const int8_t low, const int8_t high);

int ZfN(poly_check_bound_int16)(const int16_t t[FALCON_N],
                                const int16_t low, const int16_t high);

#endif
