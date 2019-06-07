#include "gf2x_arith.h"

#include <assert.h>
#include <string.h>  // memset(...)

/* allows the second operand to be shorter than the first */
/* the result should be as large as the first operand*/
static inline void gf2x_add_asymm(const size_t nr, DIGIT Res[],
                                  const size_t na, const DIGIT A[],
                                  const size_t nb, const DIGIT B[]) {
    assert(nr >= na && na >= nb);
    size_t i;
    size_t delta = na - nb;
    for (i = 0; i < delta; i++) {
        Res[i] = A[i];
    }
    for (i = 0; i < nb; i++) {
        Res[i + delta] = A[i + delta] ^ B[i];
    }
}

/* PRE: MAX ALLOWED ROTATION AMOUNT : DIGIT_SIZE_b */
void PQCLEAN_LEDAKEMLT12_CLEAN_right_bit_shift_n(size_t length, DIGIT in[], unsigned int amount) {
    assert(amount < DIGIT_SIZE_b);
    if ( amount == 0 ) {
        return;
    }
    unsigned int j;
    DIGIT mask;
    mask = ((DIGIT)0x01 << amount) - 1;
    for (j = length - 1; j > 0 ; j--) {
        in[j] >>= amount;
        in[j] |=  (in[j - 1] & mask) << (DIGIT_SIZE_b - amount);
    }
    in[j] >>= amount;
}

/* PRE: MAX ALLOWED ROTATION AMOUNT : DIGIT_SIZE_b */
void PQCLEAN_LEDAKEMLT12_CLEAN_left_bit_shift_n(size_t length, DIGIT in[], unsigned int amount) {
    assert(amount < DIGIT_SIZE_b);
    if ( amount == 0 ) {
        return;
    }
    size_t j;
    DIGIT mask;
    mask = ~(((DIGIT)0x01 << (DIGIT_SIZE_b - amount)) - 1);
    for (j = 0 ; j < length - 1 ; j++) {
        in[j] <<= amount;
        in[j] |=  (in[j + 1] & mask) >> (DIGIT_SIZE_b - amount);
    }
    in[j] <<= amount;
}

void PQCLEAN_LEDAKEMLT12_CLEAN_gf2x_mul_comb(int nr, DIGIT Res[],
        int na, const DIGIT A[],
        int nb, const DIGIT B[]) {
    int i, j, k;
    DIGIT u, h;

    memset(Res, 0x00, nr * sizeof(DIGIT));

    for (k = DIGIT_SIZE_b - 1; k > 0; k--) {
        for (i = na - 1; i >= 0; i--) {
            if ( A[i] & (((DIGIT)0x1) << k) ) {
                for (j = nb - 1; j >= 0; j--) {
                    Res[i + j + 1] ^= B[j];
                }
            }
        }

        u = Res[na + nb - 1];
        Res[na + nb - 1] = u << 0x1;
        for (j = 1; j < na + nb; ++j) {
            h = u >> (DIGIT_SIZE_b - 1);
            u = Res[na + nb - 1 - j];
            Res[na + nb - 1 - j] = h ^ (u << 0x1);
        }
    }
    for (i = na - 1; i >= 0; i--) {
        if ( A[i] & ((DIGIT)0x1) ) {
            for (j = nb - 1; j >= 0; j--) {
                Res[i + j + 1] ^= B[j];
            }
        }
    }
}

static inline void gf2x_exact_div_x_plus_one(const int na, DIGIT A[]) {
    DIGIT t = 0;
    for (int i = na - 1; i >= 0; i--) {

        t ^= A[i];

        for (int j = 1; j <= DIGIT_SIZE_b / 2; j = j * 2) {
            t ^= t << (unsigned) j;
        }
        A[i] = t;
        t >>= DIGIT_SIZE_b - 1;
    }
}
