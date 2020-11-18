/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: matrix arithmetic functions used by the KEM
*********************************************************************************************/

#include <stdint.h>
#include <string.h>

#include "fips202.h"

#include "api.h"
#include "common.h"
#include "params.h"

int PQCLEAN_FRODOKEM1344SHAKE_OPT_mul_add_as_plus_e(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A) {
    // Generate-and-multiply: generate matrix A (N x N) row-wise, multiply by s on the right.
    // Inputs: s, e (N x N_BAR)
    // Output: out = A*s + e (N x N_BAR)
    int j, k;
    uint16_t i;
    int16_t a_row[4 * PARAMS_N];

    for (i = 0; i < (PARAMS_N * PARAMS_NBAR); i += 2) {
        *((uint32_t *)&out[i]) = *((uint32_t *)&e[i]);
    }

    uint8_t seed_A_separated[2 + BYTES_SEED_A];
    uint16_t *seed_A_origin = (uint16_t *)&seed_A_separated;
    memcpy(&seed_A_separated[2], seed_A, BYTES_SEED_A);
    for (i = 0; i < PARAMS_N; i += 4) {
        seed_A_origin[0] = PQCLEAN_FRODOKEM1344SHAKE_OPT_UINT16_TO_LE(i + 0);
        shake128((unsigned char *)(a_row + 0 * PARAMS_N), (unsigned long long)(2 * PARAMS_N), seed_A_separated, 2 + BYTES_SEED_A);
        seed_A_origin[0] = PQCLEAN_FRODOKEM1344SHAKE_OPT_UINT16_TO_LE(i + 1);
        shake128((unsigned char *)(a_row + 1 * PARAMS_N), (unsigned long long)(2 * PARAMS_N), seed_A_separated, 2 + BYTES_SEED_A);
        seed_A_origin[0] = PQCLEAN_FRODOKEM1344SHAKE_OPT_UINT16_TO_LE(i + 2);
        shake128((unsigned char *)(a_row + 2 * PARAMS_N), (unsigned long long)(2 * PARAMS_N), seed_A_separated, 2 + BYTES_SEED_A);
        seed_A_origin[0] = PQCLEAN_FRODOKEM1344SHAKE_OPT_UINT16_TO_LE(i + 3);
        shake128((unsigned char *)(a_row + 3 * PARAMS_N), (unsigned long long)(2 * PARAMS_N), seed_A_separated, 2 + BYTES_SEED_A);
        for (k = 0; k < 4 * PARAMS_N; k++) {
            a_row[k] = PQCLEAN_FRODOKEM1344SHAKE_OPT_LE_TO_UINT16(a_row[k]);
        }
        for (k = 0; k < PARAMS_NBAR; k++) {
            uint16_t sum[4] = {0};
            for (j = 0; j < PARAMS_N; j++) {                    // Matrix-vector multiplication
                uint16_t sp = s[k * PARAMS_N + j];
                sum[0] += a_row[0 * PARAMS_N + j] * sp;         // Go through four lines with same s
                sum[1] += a_row[1 * PARAMS_N + j] * sp;
                sum[2] += a_row[2 * PARAMS_N + j] * sp;
                sum[3] += a_row[3 * PARAMS_N + j] * sp;
            }
            out[(i + 0)*PARAMS_NBAR + k] += sum[0];
            out[(i + 2)*PARAMS_NBAR + k] += sum[2];
            out[(i + 1)*PARAMS_NBAR + k] += sum[1];
            out[(i + 3)*PARAMS_NBAR + k] += sum[3];
        }
    }
    return 1;
}




int PQCLEAN_FRODOKEM1344SHAKE_OPT_mul_add_sa_plus_e(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A) {
    // Generate-and-multiply: generate matrix A (N x N) column-wise, multiply by s' on the left.
    // Inputs: s', e' (N_BAR x N)
    // Output: out = s'*A + e' (N_BAR x N)
    int i, j;
    uint16_t kk;
    for (i = 0; i < (PARAMS_N * PARAMS_NBAR); i += 2) {
        *((uint32_t *)&out[i]) = *((uint32_t *)&e[i]);
    }

    int t = 0;
    uint16_t a_cols[4 * PARAMS_N];

    int k;
    uint8_t seed_A_separated[2 + BYTES_SEED_A];
    uint16_t *seed_A_origin = (uint16_t *)&seed_A_separated;
    memcpy(&seed_A_separated[2], seed_A, BYTES_SEED_A);
    for (kk = 0; kk < PARAMS_N; kk += 4) {
        seed_A_origin[0] = PQCLEAN_FRODOKEM1344SHAKE_OPT_UINT16_TO_LE(kk + 0);
        shake128((unsigned char *)(a_cols + 0 * PARAMS_N), (unsigned long long)(2 * PARAMS_N), seed_A_separated, 2 + BYTES_SEED_A);
        seed_A_origin[0] = PQCLEAN_FRODOKEM1344SHAKE_OPT_UINT16_TO_LE(kk + 1);
        shake128((unsigned char *)(a_cols + 1 * PARAMS_N), (unsigned long long)(2 * PARAMS_N), seed_A_separated, 2 + BYTES_SEED_A);
        seed_A_origin[0] = PQCLEAN_FRODOKEM1344SHAKE_OPT_UINT16_TO_LE(kk + 2);
        shake128((unsigned char *)(a_cols + 2 * PARAMS_N), (unsigned long long)(2 * PARAMS_N), seed_A_separated, 2 + BYTES_SEED_A);
        seed_A_origin[0] = PQCLEAN_FRODOKEM1344SHAKE_OPT_UINT16_TO_LE(kk + 3);
        shake128((unsigned char *)(a_cols + 3 * PARAMS_N), (unsigned long long)(2 * PARAMS_N), seed_A_separated, 2 + BYTES_SEED_A);
        for (i = 0; i < 4 * PARAMS_N; i++) {
            a_cols[i] = PQCLEAN_FRODOKEM1344SHAKE_OPT_LE_TO_UINT16(a_cols[i]);
        }

        for (i = 0; i < PARAMS_NBAR; i++) {
            uint16_t sum[PARAMS_N] = {0};
            for (j = 0; j < 4; j++) {
                uint16_t sp = s[i * PARAMS_N + kk + j];
                for (k = 0; k < PARAMS_N; k++) {                // Matrix-vector multiplication
                    sum[k] += (uint16_t)(sp * (uint32_t)a_cols[(t + j) * PARAMS_N + k]);
                }
            }
            for (k = 0; k < PARAMS_N; k++) {
                out[i * PARAMS_N + k] += sum[k];
            }
        }
    }
    return 1;
}
