#include "poly.h"
#include "poly_r2_inv.h"

// TODO this costs 1764 cycles.. (implementing as S3_to_bytes results in 2108)
// This can be implemented nicely in assembly using pdep / pext functions
void PQCLEAN_NTRUHPS4096821_AVX2_poly_R2_tobytes(unsigned char *out, const poly *a) {
    int i, j, k;
    for (i = 0; i < 16; i++) {
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

void PQCLEAN_NTRUHPS4096821_AVX2_poly_R2_frombytes(poly *a, const unsigned char *in) {
    int i, j, k;
    for (i = 0; i < 16; i++) {
        for (k = 0; k < 8; k++) {
            for (j = 0; j < 8; j++) {
                if ((i * 8 + k) * 8 + j < NTRU_N) {
                    a->coeffs[(i * 8 + k) * 8 + j] = (in[i * 8 + k] >> j) & 1;
                }
            }
        }
    }
}


static void __poly_R2_mul(unsigned char r[128], unsigned char a[128], unsigned char b[128]) {
    // TODO: Try to avoid copying input
    int i;
    unsigned char rt[128] __attribute__((aligned(32)));
    PQCLEAN_NTRUHPS4096821_AVX2_poly_R2_mul(rt, a, b);
    for (i = 0; i < 128; i++) {
        r[i] = rt[i];
    }
}

void PQCLEAN_NTRUHPS4096821_AVX2_poly_R2_inv(poly *r, const poly *a) {
    unsigned char squares[13][128] __attribute__((aligned(32)));

    // This relies on the following addition chain:
    // 1, 2, 3, 6, 12, 24, 48, 51, 102, 204, 408, 816, 819

    PQCLEAN_NTRUHPS4096821_AVX2_poly_R2_tobytes(squares[0], a); // TODO alignment

    PQCLEAN_NTRUHPS4096821_AVX2_square_1_821(squares[1], squares[0]);
    __poly_R2_mul(squares[1], squares[1], squares[0]);
    PQCLEAN_NTRUHPS4096821_AVX2_square_1_821(squares[2], squares[1]);
    __poly_R2_mul(squares[2], squares[2], squares[0]);
    PQCLEAN_NTRUHPS4096821_AVX2_square_3_821(squares[3], squares[2]);
    __poly_R2_mul(squares[3], squares[3], squares[2]);
    PQCLEAN_NTRUHPS4096821_AVX2_square_6_821(squares[4], squares[3]);
    __poly_R2_mul(squares[4], squares[4], squares[3]);
    PQCLEAN_NTRUHPS4096821_AVX2_square_12_821(squares[5], squares[4]);
    __poly_R2_mul(squares[5], squares[5], squares[4]);
    PQCLEAN_NTRUHPS4096821_AVX2_square_24_821(squares[6], squares[5]);
    __poly_R2_mul(squares[6], squares[6], squares[5]);
    PQCLEAN_NTRUHPS4096821_AVX2_square_3_821(squares[7], squares[6]);
    __poly_R2_mul(squares[7], squares[7], squares[2]);
    PQCLEAN_NTRUHPS4096821_AVX2_square_51_821(squares[8], squares[7]);
    __poly_R2_mul(squares[8], squares[8], squares[7]);
    PQCLEAN_NTRUHPS4096821_AVX2_square_102_821(squares[9], squares[8]);
    __poly_R2_mul(squares[9], squares[9], squares[8]);
    PQCLEAN_NTRUHPS4096821_AVX2_square_204_821(squares[10], squares[9]);
    __poly_R2_mul(squares[10], squares[10], squares[9]);
    PQCLEAN_NTRUHPS4096821_AVX2_square_408_821(squares[11], squares[10]);
    __poly_R2_mul(squares[11], squares[11], squares[10]);
    PQCLEAN_NTRUHPS4096821_AVX2_square_3_821(squares[12], squares[11]);
    __poly_R2_mul(squares[12], squares[12], squares[2]);
    PQCLEAN_NTRUHPS4096821_AVX2_square_1_821(squares[0], squares[12]);


    PQCLEAN_NTRUHPS4096821_AVX2_poly_R2_frombytes(r, squares[0]);
}
