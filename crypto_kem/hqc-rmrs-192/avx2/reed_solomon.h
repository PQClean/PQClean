#ifndef REED_SOLOMON_H
#define REED_SOLOMON_H


/**
 * @file reed_solomon.h
 * Header file of reed_solomon.c
 */

#include "parameters.h"
#include "parameters.h"
#include <stddef.h>
#include <stdint.h>

void PQCLEAN_HQCRMRS192_AVX2_reed_solomon_encode(uint64_t *cdw, const uint64_t *msg);

void PQCLEAN_HQCRMRS192_AVX2_reed_solomon_decode(uint64_t *msg, uint64_t *cdw);


#endif
