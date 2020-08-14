#include "poly_r2_inv.h"
#include "poly.h"
#include <immintrin.h>

// Using pdep/pext for these two functions is faster but not a lot since they work on uint64_t which means
// we can only do 4 coefficients at a time. Per byte (where we store 8 coefficients) we will thus need 2 pdeps/pexts
// and an additional shift. In the case of tobytes we also need a logical or.
// On AMD Ryzen pdep/pext are quite slow and the naive solution (looping through and setting each bit individually)
// is preferred.
void PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_tobytes(unsigned char *out, const poly *a) {
    // Since pext works on a uint64_t we view the coefficient pointer as a 64-bit pointer
    // so that we can extract 4 coefficient at a time. It also makes arithmetic a little easier.
    uint64_t *coeff_pointer = (void *) a->coeffs;

    int i;
    for (i = 0; i < 63; i++) {
        out[i] = _pext_u64(coeff_pointer[2 * i], 0x1000100010001);
        out[i] |= _pext_u64(coeff_pointer[2 * i + 1], 0x1000100010001) << 4;
    }
    out[i] = _pext_u64(coeff_pointer[2 * 63], 0x1000100010001);
    out[i] |= _pext_u64(coeff_pointer[2 * 63 + 1], 0x1) << 4;
}

void PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_frombytes(poly *a, const unsigned char *in) {
    // Since pdep results in a uint64_t we view the coefficient pointer as a 64-bit pointer
    // so that we can store 4 coefficient at a time. It also makes arithmetic a little easier.
    uint64_t *coeff_pointer = (void *) a->coeffs;

    int i;
    for (i = 0; i < 63; i++) {
        coeff_pointer[2 * i] = _pdep_u64(in[i], 0x1000100010001);
        coeff_pointer[2 * i + 1] = _pdep_u64(in[i] >> 4, 0x1000100010001);
    }
    // From the last byte we only want 5 bits (since we have 509 total, not 512).
    coeff_pointer[2 * 63] = _pdep_u64(in[i], 0x1000100010001);
    coeff_pointer[2 * 63 + 1] = _pdep_u64(in[i] >> 4, 0x1);
}

void PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_inv(poly *r, const poly *a) {
    unsigned char squares[13][64] __attribute__((aligned(32)));

    // This relies on the following addition chain:
    // 1, 2, 3, 6, 12, 15, 30, 60, 63, 126, 252, 504, 507

    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_tobytes(squares[0], a); // TODO alignment

    PQCLEAN_NTRUHPS2048509_AVX2_square_1_509(squares[1], squares[0]);
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(squares[1], squares[1], squares[0]);
    PQCLEAN_NTRUHPS2048509_AVX2_square_1_509(squares[2], squares[1]);
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(squares[2], squares[2], squares[0]);
    PQCLEAN_NTRUHPS2048509_AVX2_square_3_509(squares[3], squares[2]);
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(squares[3], squares[3], squares[2]);
    PQCLEAN_NTRUHPS2048509_AVX2_square_6_509(squares[4], squares[3]);
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(squares[4], squares[4], squares[3]);
    PQCLEAN_NTRUHPS2048509_AVX2_square_3_509(squares[5], squares[4]);
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(squares[5], squares[5], squares[2]);
    PQCLEAN_NTRUHPS2048509_AVX2_square_15_509(squares[6], squares[5]);
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(squares[6], squares[6], squares[5]);
    PQCLEAN_NTRUHPS2048509_AVX2_square_30_509(squares[7], squares[6]);
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(squares[7], squares[7], squares[6]);
    PQCLEAN_NTRUHPS2048509_AVX2_square_3_509(squares[8], squares[7]);
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(squares[8], squares[8], squares[2]);
    PQCLEAN_NTRUHPS2048509_AVX2_square_63_509(squares[9], squares[8]);
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(squares[9], squares[9], squares[8]);
    PQCLEAN_NTRUHPS2048509_AVX2_square_126_509(squares[10], squares[9]);
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(squares[10], squares[10], squares[9]);
    PQCLEAN_NTRUHPS2048509_AVX2_square_252_509(squares[11], squares[10]);
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(squares[11], squares[11], squares[10]);
    PQCLEAN_NTRUHPS2048509_AVX2_square_3_509(squares[12], squares[11]);
    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_mul(squares[12], squares[12], squares[2]);
    PQCLEAN_NTRUHPS2048509_AVX2_square_1_509(squares[0], squares[12]);

    PQCLEAN_NTRUHPS2048509_AVX2_poly_R2_frombytes(r, squares[0]);
}
