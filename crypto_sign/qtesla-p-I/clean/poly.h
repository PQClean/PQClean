#ifndef PQCLEAN_QTESLAPI_CLEAN_POLY_H
#define PQCLEAN_QTESLAPI_CLEAN_POLY_H

#include "config.h"
#include "params.h"
#include <stdint.h>

typedef int64_t poly[PARAM_N];
typedef int64_t poly_k[PARAM_N * PARAM_K];

int64_t reduce(int64_t a);
int64_t barr_reduce(int64_t a);
void ntt(poly a, const poly w);
void nttinv(poly a, const poly w);
void poly_ntt(poly x_ntt, const poly x);
void poly_mul(poly result, const poly x, const poly y);
void poly_add(poly result, const poly x, const poly y);
void poly_add_correct(poly result, const poly x, const poly y);
void poly_sub(poly result, const poly x, const poly y);
void sparse_mul8(poly prod, const unsigned char *s, const uint32_t pos_list[PARAM_H], const int16_t sign_list[PARAM_H]);
void sparse_mul32(poly prod, const int32_t *pk, const uint32_t pos_list[PARAM_H], const int16_t sign_list[PARAM_H]);
void poly_uniform(poly_k a, const unsigned char *seed);

#endif
