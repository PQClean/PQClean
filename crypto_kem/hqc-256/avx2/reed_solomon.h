#ifndef REED_SOLOMON_H
#define REED_SOLOMON_H


/**
 * @file reed_solomon.h
 * @brief Header file of reed_solomon.c
 */
#include "parameters.h"
#include <stddef.h>
#include <stdint.h>


void PQCLEAN_HQC256_AVX2_reed_solomon_encode(uint64_t *cdw, const uint64_t *msg);

void PQCLEAN_HQC256_AVX2_reed_solomon_decode(uint64_t *msg, uint64_t *cdw);


void PQCLEAN_HQC256_AVX2_compute_generator_poly(uint16_t *poly);


#endif
