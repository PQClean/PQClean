#include "api.h"
#include "cbd.h"
#include "fips202.h"
#include "pack_unpack.h"
#include "poly.h"
#include "poly_mul.h"
#include <stddef.h>

void PQCLEAN_LIGHTSABER_CLEAN_MatrixVectorMul(uint16_t res[SABER_L][SABER_N], const uint16_t A[SABER_L][SABER_L][SABER_N], const uint16_t s[SABER_L][SABER_N], int16_t transpose) {
    size_t i, j;
    for (i = 0; i < SABER_L; i++) {
        for (j = 0; j < SABER_L; j++) {
            if (transpose == 1) {
                PQCLEAN_LIGHTSABER_CLEAN_poly_mul_acc(res[i], A[j][i], s[j]);
            } else {
                PQCLEAN_LIGHTSABER_CLEAN_poly_mul_acc(res[i], A[i][j], s[j]);
            }
        }
    }
}

void PQCLEAN_LIGHTSABER_CLEAN_InnerProd(uint16_t res[SABER_N], const uint16_t b[SABER_L][SABER_N], const uint16_t s[SABER_L][SABER_N]) {
    size_t j;
    for (j = 0; j < SABER_L; j++) {
        PQCLEAN_LIGHTSABER_CLEAN_poly_mul_acc(res, b[j], s[j]);
    }
}

void PQCLEAN_LIGHTSABER_CLEAN_GenMatrix(uint16_t A[SABER_L][SABER_L][SABER_N], const uint8_t seed[SABER_SEEDBYTES]) {
    uint8_t buf[SABER_L * SABER_POLYVECBYTES];
    size_t i;

    shake128(buf, sizeof(buf), seed, SABER_SEEDBYTES);

    for (i = 0; i < SABER_L; i++) {
        PQCLEAN_LIGHTSABER_CLEAN_BS2POLVECq(A[i], buf + i * SABER_POLYVECBYTES);
    }
}

void PQCLEAN_LIGHTSABER_CLEAN_GenSecret(uint16_t s[SABER_L][SABER_N], const uint8_t seed[SABER_NOISE_SEEDBYTES]) {
    uint8_t buf[SABER_L * SABER_POLYCOINBYTES];
    size_t i;

    shake128(buf, sizeof(buf), seed, SABER_NOISE_SEEDBYTES);

    for (i = 0; i < SABER_L; i++) {
        PQCLEAN_LIGHTSABER_CLEAN_cbd(s[i], buf + i * SABER_POLYCOINBYTES);
    }
}
