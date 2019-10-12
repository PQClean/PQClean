#ifndef PQCLEAN_QTESLAPIII_CLEAN_POLY_H
#define PQCLEAN_QTESLAPIII_CLEAN_POLY_H

#include "config.h"
#include "params.h"
#include <stdint.h>

typedef int64_t poly[PARAM_N];
typedef int64_t poly_k[PARAM_N * PARAM_K];

void PQCLEAN_QTESLAPIII_CLEAN_poly_ntt(poly x_ntt, const poly x);
void PQCLEAN_QTESLAPIII_CLEAN_poly_mul(poly result, const poly x, const poly y);
void PQCLEAN_QTESLAPIII_CLEAN_poly_add(poly result, const poly x, const poly y);
void PQCLEAN_QTESLAPIII_CLEAN_poly_add_correct(poly result, const poly x, const poly y);
void PQCLEAN_QTESLAPIII_CLEAN_poly_sub(poly result, const poly x, const poly y);
void PQCLEAN_QTESLAPIII_CLEAN_sparse_mul8(poly prod, const uint8_t *s, const uint32_t pos_list[PARAM_H], const int16_t sign_list[PARAM_H]);
void PQCLEAN_QTESLAPIII_CLEAN_sparse_mul32(poly prod, const int32_t *pk, const uint32_t pos_list[PARAM_H], const int16_t sign_list[PARAM_H]);
void PQCLEAN_QTESLAPIII_CLEAN_poly_uniform(poly_k a, const uint8_t *seed);

#endif
