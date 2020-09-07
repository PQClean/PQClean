#ifndef GF_H
#define GF_H


/**
 * @file gf.h
 * Header file of gf.c
 */

#include <stddef.h>

#include <stddef.h>
#include <stdint.h>

void PQCLEAN_HQC192_CLEAN_gf_generate(uint16_t *exp, uint16_t *log, int16_t m);


uint16_t PQCLEAN_HQC192_CLEAN_gf_log(uint16_t elt);

uint16_t PQCLEAN_HQC192_CLEAN_gf_mul(uint16_t a, uint16_t b);

uint16_t PQCLEAN_HQC192_CLEAN_gf_square(uint16_t a);

uint16_t PQCLEAN_HQC192_CLEAN_gf_inverse(uint16_t a);

uint16_t PQCLEAN_HQC192_CLEAN_gf_mod(uint16_t i);


#endif
