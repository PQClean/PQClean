#include "api.h"
#include "cbd.h"
#include "fips202.h"
#include "pack_unpack.h"
#include "poly.h"
#include "poly_mul.h"
#include <stdio.h>

void PQCLEAN_SABER_CLEAN_MatrixVectorMul(uint16_t res[SABER_L][SABER_N], const uint16_t A[SABER_L][SABER_L][SABER_N], const uint16_t s[SABER_L][SABER_N], int16_t transpose) {
    int i, j;
    for (i = 0; i < SABER_L; i++) {
        for (j = 0; j < SABER_L; j++) {
            if (transpose == 1) {
                PQCLEAN_SABER_CLEAN_poly_mul_acc(A[j][i], s[j], res[i]);
            } else {
                PQCLEAN_SABER_CLEAN_poly_mul_acc(A[i][j], s[j], res[i]);
            }
        }
    }
}

void PQCLEAN_SABER_CLEAN_InnerProd(uint16_t res[SABER_N], const uint16_t b[SABER_L][SABER_N], const uint16_t s[SABER_L][SABER_N]) {
    int j;
    for (j = 0; j < SABER_L; j++) {
        PQCLEAN_SABER_CLEAN_poly_mul_acc(b[j], s[j], res);
    }
}

void PQCLEAN_SABER_CLEAN_GenMatrix(uint16_t A[SABER_L][SABER_L][SABER_N], const uint8_t seed[SABER_SEEDBYTES]) {
    uint8_t buf[SABER_L * SABER_POLYVECBYTES];
    int i;

    shake128(buf, sizeof(buf), seed, SABER_SEEDBYTES);

    for (i = 0; i < SABER_L; i++) {
        PQCLEAN_SABER_CLEAN_BS2POLVECq(A[i], buf + i * SABER_POLYVECBYTES);
    }
}

void PQCLEAN_SABER_CLEAN_GenSecret(uint16_t s[SABER_L][SABER_N], const uint8_t seed[SABER_NOISE_SEEDBYTES]) {
    uint8_t buf[SABER_L * SABER_POLYCOINBYTES];
    size_t i;

    shake128(buf, sizeof(buf), seed, SABER_NOISE_SEEDBYTES);

    for (i = 0; i < SABER_L; i++) {
        PQCLEAN_SABER_CLEAN_cbd(s[i], buf + i * SABER_POLYCOINBYTES);
    }
}
