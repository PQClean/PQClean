/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: matrix arithmetic functions used by the KEM
*********************************************************************************************/

#include <stdint.h>
#include <string.h>

#include "aes.h"

#include "api.h"
#include "common.h"
#include "params.h"

int PQCLEAN_FRODOKEM640AES_OPT_mul_add_as_plus_e(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A) {
    // Generate-and-multiply: generate matrix A (N x N) row-wise, multiply by s on the right.
    // Inputs: s, e (N x N_BAR)
    // Output: out = A*s + e (N x N_BAR)
    int k;
    uint16_t i, j;
    int16_t a_row[4 * PARAMS_N];

    for (i = 0; i < (PARAMS_N * PARAMS_NBAR); i += 2) {
        *((uint32_t *)&out[i]) = *((uint32_t *)&e[i]);
    }

    int16_t a_row_temp[4 * PARAMS_N] = {0};                     // Take four lines of A at once
    aes128ctx ctx128;

    aes128_keyexp(&ctx128, seed_A);

    for (j = 0; j < PARAMS_N; j += PARAMS_STRIPE_STEP) {
        a_row_temp[j + 1 + 0 * PARAMS_N] = PQCLEAN_FRODOKEM640AES_OPT_UINT16_TO_LE(j);     // Loading values in the little-endian order
        a_row_temp[j + 1 + 1 * PARAMS_N] = PQCLEAN_FRODOKEM640AES_OPT_UINT16_TO_LE(j);
        a_row_temp[j + 1 + 2 * PARAMS_N] = PQCLEAN_FRODOKEM640AES_OPT_UINT16_TO_LE(j);
        a_row_temp[j + 1 + 3 * PARAMS_N] = PQCLEAN_FRODOKEM640AES_OPT_UINT16_TO_LE(j);
    }

    for (i = 0; i < PARAMS_N; i += 4) {
        for (j = 0; j < PARAMS_N; j += PARAMS_STRIPE_STEP) {    // Go through A, four rows at a time
            a_row_temp[j + 0 * PARAMS_N] = PQCLEAN_FRODOKEM640AES_OPT_UINT16_TO_LE(i + 0); // Loading values in the little-endian order
            a_row_temp[j + 1 * PARAMS_N] = PQCLEAN_FRODOKEM640AES_OPT_UINT16_TO_LE(i + 1);
            a_row_temp[j + 2 * PARAMS_N] = PQCLEAN_FRODOKEM640AES_OPT_UINT16_TO_LE(i + 2);
            a_row_temp[j + 3 * PARAMS_N] = PQCLEAN_FRODOKEM640AES_OPT_UINT16_TO_LE(i + 3);
        }
        aes128_ecb((uint8_t *)a_row, (uint8_t *)a_row_temp, 4 * PARAMS_N * sizeof(int16_t) / AES_BLOCKBYTES, &ctx128);
        for (k = 0; k < 4 * PARAMS_N; k++) {
            a_row[k] = PQCLEAN_FRODOKEM640AES_OPT_LE_TO_UINT16(a_row[k]);
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
    aes128_ctx_release(&ctx128);
    return 1;
}




int PQCLEAN_FRODOKEM640AES_OPT_mul_add_sa_plus_e(uint16_t *out, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A) {
    // Generate-and-multiply: generate matrix A (N x N) column-wise, multiply by s' on the left.
    // Inputs: s', e' (N_BAR x N)
    // Output: out = s'*A + e' (N_BAR x N)
    int j;
    uint16_t i, kk;
    for (i = 0; i < (PARAMS_N * PARAMS_NBAR); i += 2) {
        *((uint32_t *)&out[i]) = *((uint32_t *)&e[i]);
    }

    int k;
    uint16_t a_cols[PARAMS_N * PARAMS_STRIPE_STEP] = {0};
    uint16_t a_cols_t[PARAMS_N * PARAMS_STRIPE_STEP];
    uint16_t a_cols_temp[PARAMS_N * PARAMS_STRIPE_STEP] = {0};
    aes128ctx ctx128;

    aes128_keyexp(&ctx128, seed_A);

    for (i = 0, j = 0; i < PARAMS_N; i++, j += PARAMS_STRIPE_STEP) {
        a_cols_temp[j] = PQCLEAN_FRODOKEM640AES_OPT_UINT16_TO_LE(i);                       // Loading values in the little-endian order
    }

    for (kk = 0; kk < PARAMS_N; kk += PARAMS_STRIPE_STEP) {     // Go through A's columns, 8 (== PARAMS_STRIPE_STEP) columns at a time.
        for (i = 0; i < (PARAMS_N * PARAMS_STRIPE_STEP); i += PARAMS_STRIPE_STEP) {
            a_cols_temp[i + 1] = PQCLEAN_FRODOKEM640AES_OPT_UINT16_TO_LE(kk);              // Loading values in the little-endian order
        }

        aes128_ecb((uint8_t *)a_cols, (uint8_t *)a_cols_temp, PARAMS_N * PARAMS_STRIPE_STEP * sizeof(int16_t) / AES_BLOCKBYTES, &ctx128);

        for (i = 0; i < PARAMS_N; i++) {                        // Transpose a_cols to have access to it in the column-major order.
            for (k = 0; k < PARAMS_STRIPE_STEP; k++) {
                a_cols_t[k * PARAMS_N + i] = PQCLEAN_FRODOKEM640AES_OPT_LE_TO_UINT16(a_cols[i * PARAMS_STRIPE_STEP + k]);
            }
        }

        for (i = 0; i < PARAMS_NBAR; i++) {
            for (k = 0; k < PARAMS_STRIPE_STEP; k += PARAMS_PARALLEL) {
                uint16_t sum[PARAMS_PARALLEL] = {0};
                for (j = 0; j < PARAMS_N; j++) {                // Matrix-vector multiplication
                    uint16_t sp = s[i * PARAMS_N + j];
                    sum[0] += sp * a_cols_t[(k + 0) * PARAMS_N + j];
                    sum[1] += sp * a_cols_t[(k + 1) * PARAMS_N + j];
                    sum[2] += sp * a_cols_t[(k + 2) * PARAMS_N + j];
                    sum[3] += sp * a_cols_t[(k + 3) * PARAMS_N + j];
                }
                out[i * PARAMS_N + kk + k + 0] += sum[0];
                out[i * PARAMS_N + kk + k + 2] += sum[2];
                out[i * PARAMS_N + kk + k + 1] += sum[1];
                out[i * PARAMS_N + kk + k + 3] += sum[3];
            }
        }
    }
    aes128_ctx_release(&ctx128);
    return 1;
}
