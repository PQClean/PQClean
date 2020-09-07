#ifndef BCH_H
#define BCH_H


/**
 * @file bch.h
 * Header file of bch.c
 */

#include "parameters.h"
#include "parameters.h"
#include <stddef.h>
#include <stdint.h>

void PQCLEAN_HQC192_AVX2_bch_code_decode(uint64_t *message, uint64_t *vector);


size_t PQCLEAN_HQC192_AVX2_compute_bch_poly(uint16_t *bch_poly, size_t *t, const uint16_t *exp, const uint16_t *log);

void PQCLEAN_HQC192_AVX2_table_alphaij_generation(const uint16_t *exp);


#endif
