#ifndef PQCLEAN_QTESLAPI_CLEAN_POLY_H
#define PQCLEAN_QTESLAPI_CLEAN_POLY_H

#include "config.h"
#include "params.h"
#include <stdint.h>

typedef int64_t poly[PARAM_N];
typedef int64_t poly_k[PARAM_N * PARAM_K];

// int64_t PQCLEAN_QTESLAPI_CLEAN_reduce(int64_t a);
// int64_t PQCLEAN_QTESLAPI_CLEAN_barr_reduce(int64_t a);
// void PQCLEAN_QTESLAPI_CLEAN_ntt(poly a, const poly w);
// void PQCLEAN_QTESLAPI_CLEAN_nttinv(poly a, const poly w);
void PQCLEAN_QTESLAPI_CLEAN_poly_ntt(poly x_ntt, const poly x);
void PQCLEAN_QTESLAPI_CLEAN_poly_mul(poly result, const poly x, const poly y);
void PQCLEAN_QTESLAPI_CLEAN_poly_add(poly result, const poly x, const poly y);
void PQCLEAN_QTESLAPI_CLEAN_poly_add_correct(poly result, const poly x, const poly y);
void PQCLEAN_QTESLAPI_CLEAN_poly_sub(poly result, const poly x, const poly y);
void PQCLEAN_QTESLAPI_CLEAN_sparse_mul8(poly prod, const unsigned char *s, const uint32_t pos_list[PARAM_H], const int16_t sign_list[PARAM_H]);
void PQCLEAN_QTESLAPI_CLEAN_sparse_mul32(poly prod, const int32_t *pk, const uint32_t pos_list[PARAM_H], const int16_t sign_list[PARAM_H]);
void PQCLEAN_QTESLAPI_CLEAN_poly_uniform(poly_k a, const unsigned char *seed);

#endif
