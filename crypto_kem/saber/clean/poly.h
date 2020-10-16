#ifndef POLY_H
#define POLY_H
#include "SABER_params.h"
#include <stdint.h>

void PQCLEAN_SABER_CLEAN_MatrixVectorMul(uint16_t res[SABER_L][SABER_N], const uint16_t a[SABER_L][SABER_L][SABER_N], const uint16_t s[SABER_L][SABER_N], int16_t transpose);

void PQCLEAN_SABER_CLEAN_InnerProd(uint16_t res[SABER_N], const uint16_t b[SABER_L][SABER_N], const uint16_t s[SABER_L][SABER_N]);

void PQCLEAN_SABER_CLEAN_GenMatrix(uint16_t a[SABER_L][SABER_L][SABER_N], const uint8_t seed[SABER_SEEDBYTES]);

void PQCLEAN_SABER_CLEAN_GenSecret(uint16_t s[SABER_L][SABER_N], const uint8_t seed[SABER_NOISE_SEEDBYTES]);


#endif
