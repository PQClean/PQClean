#ifndef POLY_MUL_H
#define POLY_MUL_H
#include "SABER_params.h"
#include <stdint.h>

void PQCLEAN_LIGHTSABER_CLEAN_poly_mul_acc(const uint16_t a[SABER_N], const uint16_t b[SABER_N], uint16_t res[SABER_N]);


#endif
