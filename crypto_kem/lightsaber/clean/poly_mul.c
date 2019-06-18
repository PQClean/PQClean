#include "poly_mul.h"
#include "SABER_params.h"
#include <stdint.h>
#include <string.h>


void PQCLEAN_LIGHTSABER_CLEAN_pol_mul(const uint16_t *a, const uint16_t *b, uint16_t *res, uint16_t p, uint32_t n) {
    // Polynomial multiplication using the schoolbook method, c[x] = a[x]*b[x]

    // normal multiplication
    uint16_t c[2 * SABER_N] = {0};
    for (size_t i = 0; i < SABER_N; i++) {
        for (size_t j = 0; j < SABER_N; j++) {
            c[i + j] += a[i] * b[j];
        }
    }

    // reduction
    for (size_t i = n; i < 2 * n; i++) {
        res[i - n] = (c[i - n] - c[i]) & (p - 1);
    }
}
