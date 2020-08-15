#include "poly_r2_inv.h"
#include "poly.h"

// TODO this costs 1764 cycles.. (implementing as S3_to_bytes results in 2108)
// This can be implemented nicely in assembly using pdep / pext functions
void PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_tobytes(unsigned char *out, const poly *a) {
    int i, j, k;
    for (i = 0; i < 12; i++) {
        for (k = 0; k < 8; k++) {
            out[i * 8 + k] = 0;
            for (j = 0; j < 8; j++) {
                if ((i * 8 + k) * 8 + j < NTRU_N) {
                    out[i * 8 + k] |= (a->coeffs[(i * 8 + k) * 8 + j] & 1) << j;
                }
            }
        }
    }
}

void PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_frombytes(poly *a, const unsigned char *in) {
    int i, j, k;
    for (i = 0; i < 12; i++) {
        for (k = 0; k < 8; k++) {
            for (j = 0; j < 8; j++) {
                if ((i * 8 + k) * 8 + j < NTRU_N) {
                    a->coeffs[(i * 8 + k) * 8 + j] = (in[i * 8 + k] >> j) & 1;
                }
            }
        }
    }
}

void PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_inv(poly *r, const poly *a) {
    unsigned char squares[13][96] __attribute__((aligned(32)));

    // This relies on the following addition chain:
    // 1, 2, 3, 5, 10, 20, 21, 42, 84, 168, 336, 672, 675

    PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_tobytes(squares[0], a); // TODO alignment

    PQCLEAN_NTRUHPS2048677_AVX2_square_1_677(squares[1], squares[0]);
    PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_mul(squares[1], squares[1], squares[0]);
    PQCLEAN_NTRUHPS2048677_AVX2_square_1_677(squares[2], squares[1]);
    PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_mul(squares[2], squares[2], squares[0]);
    PQCLEAN_NTRUHPS2048677_AVX2_square_2_677(squares[3], squares[2]);
    PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_mul(squares[3], squares[3], squares[1]);
    PQCLEAN_NTRUHPS2048677_AVX2_square_5_677(squares[4], squares[3]);
    PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_mul(squares[4], squares[4], squares[3]);
    PQCLEAN_NTRUHPS2048677_AVX2_square_10_677(squares[5], squares[4]);
    PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_mul(squares[5], squares[5], squares[4]);
    PQCLEAN_NTRUHPS2048677_AVX2_square_1_677(squares[6], squares[5]);
    PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_mul(squares[6], squares[6], squares[0]);
    PQCLEAN_NTRUHPS2048677_AVX2_square_21_677(squares[7], squares[6]);
    PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_mul(squares[7], squares[7], squares[6]);
    PQCLEAN_NTRUHPS2048677_AVX2_square_42_677(squares[8], squares[7]);
    PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_mul(squares[8], squares[8], squares[7]);
    PQCLEAN_NTRUHPS2048677_AVX2_square_84_677(squares[9], squares[8]);
    PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_mul(squares[9], squares[9], squares[8]);
    PQCLEAN_NTRUHPS2048677_AVX2_square_168_677(squares[10], squares[9]);
    PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_mul(squares[10], squares[10], squares[9]);
    PQCLEAN_NTRUHPS2048677_AVX2_square_336_677(squares[11], squares[10]);
    PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_mul(squares[11], squares[11], squares[10]);
    PQCLEAN_NTRUHPS2048677_AVX2_square_3_677(squares[12], squares[11]);
    PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_mul(squares[12], squares[12], squares[2]);
    PQCLEAN_NTRUHPS2048677_AVX2_square_1_677(squares[0], squares[12]);


    PQCLEAN_NTRUHPS2048677_AVX2_poly_R2_frombytes(r, squares[0]);
}
