#ifndef GF2X_ARITH_MOD_XPLUSONE_H
#define GF2X_ARITH_MOD_XPLUSONE_H

#include "qc_ldpc_parameters.h"

#include "gf2x_arith.h"
#include "rng.h"

#define NUM_BITS_GF2X_ELEMENT                   (P) // 152267
#define NUM_DIGITS_GF2X_ELEMENT                 ((P+DIGIT_SIZE_b-1)/DIGIT_SIZE_b) // 2380
#define MSb_POSITION_IN_MSB_DIGIT_OF_ELEMENT    ((P % DIGIT_SIZE_b) ? (P % DIGIT_SIZE_b)-1 : DIGIT_SIZE_b-1)
#define NUM_BITS_GF2X_MODULUS                   (P+1)
#define NUM_DIGITS_GF2X_MODULUS                 ((P+1+DIGIT_SIZE_b-1)/DIGIT_SIZE_b) // 2380
#define MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS    (P-DIGIT_SIZE_b*(NUM_DIGITS_GF2X_MODULUS-1))
#define INVALID_POS_VALUE                       (P)
#define P_BITS                                  (18) // log_2(p) = 17.216243783


static inline void gf2x_copy(DIGIT dest[], const DIGIT in[]) {
    for (int i = NUM_DIGITS_GF2X_ELEMENT - 1; i >= 0; i--) {
        dest[i] = in[i];
    }
}

/* returns the coefficient of the x^exponent term as the LSB of a digit */
static inline DIGIT gf2x_get_coeff(const DIGIT poly[], unsigned int exponent) {
    unsigned int straightIdx = (NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_b - 1) - exponent;
    unsigned int digitIdx = straightIdx / DIGIT_SIZE_b;
    unsigned int inDigitIdx = straightIdx % DIGIT_SIZE_b;
    return (poly[digitIdx] >> (DIGIT_SIZE_b - 1 - inDigitIdx)) & ((DIGIT) 1) ;
}

/* sets the coefficient of the x^exponent term as the LSB of a digit */
static inline void gf2x_set_coeff(DIGIT poly[], unsigned int exponent, DIGIT value) {
    unsigned int straightIdx = (NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_b - 1) - exponent;
    unsigned int digitIdx = straightIdx / DIGIT_SIZE_b;
    unsigned int inDigitIdx = straightIdx % DIGIT_SIZE_b;

    /* clear given coefficient */
    DIGIT mask = ~( ((DIGIT) 1) << (DIGIT_SIZE_b - 1 - inDigitIdx));
    poly[digitIdx] = poly[digitIdx] & mask;
    poly[digitIdx] = poly[digitIdx] | (( value & ((DIGIT) 1)) << (DIGIT_SIZE_b - 1 - inDigitIdx));
}

/* toggles (flips) the coefficient of the x^exponent term as the LSB of a digit */
static inline void gf2x_toggle_coeff(DIGIT poly[], unsigned int exponent) {
    unsigned int straightIdx = (NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_b - 1) - exponent;
    unsigned int digitIdx = straightIdx / DIGIT_SIZE_b;
    unsigned int inDigitIdx = straightIdx % DIGIT_SIZE_b;

    /* clear given coefficient */
    DIGIT mask = ( ((DIGIT) 1) << (DIGIT_SIZE_b - 1 - inDigitIdx));
    poly[digitIdx] = poly[digitIdx] ^ mask;
}

/* population count for an unsigned 64-bit integer
   Source: Hacker's delight, p.66  */
static int popcount_uint64t(uint64_t x) {
    x -= (x >> 1) & 0x5555555555555555;
    x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
    x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0f;
    return (int)((x * 0x0101010101010101) >> 56);
}

/* population count for a single polynomial */
static inline int population_count(DIGIT *poly) {
    int ret = 0;
    for (int i = NUM_DIGITS_GF2X_ELEMENT - 1; i >= 0; i--) {
        ret += popcount_uint64t(poly[i]);
    }
    return ret;
}

static inline void gf2x_mod_add(DIGIT Res[], const DIGIT A[], const DIGIT B[]) {
    gf2x_add(Res, A, B, NUM_DIGITS_GF2X_ELEMENT);
}

static inline int partition(POSITION_T arr[], int lo, int hi) {
    POSITION_T x = arr[hi];
    POSITION_T tmp;
    int i = (lo - 1);
    for (int j = lo; j <= hi - 1; j++)  {
        if (arr[j] <= x) {
            i++;
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
    }
    tmp = arr[i + 1];
    arr[i + 1] = arr[hi];
    arr[hi] = tmp;

    return i + 1;
}

static inline void quicksort_sparse(POSITION_T Res[]) {
    int stack[DV * M];
    int hi, lo, pivot, tos = -1;
    stack[++tos] = 0;
    stack[++tos] = (DV * M) - 1;
    while (tos >= 0 ) {
        hi = stack[tos--];
        lo = stack[tos--];
        pivot = partition(Res, lo, hi);
        if ( (pivot - 1) > lo) {
            stack[++tos] = lo;
            stack[++tos] = pivot - 1;
        }
        if ( (pivot + 1) < hi) {
            stack[++tos] = pivot + 1;
            stack[++tos] = hi;
        }
    }
}

void PQCLEAN_LEDAKEMLT52_CLEAN_gf2x_mod_mul(DIGIT Res[], const DIGIT A[], const DIGIT B[]);
void PQCLEAN_LEDAKEMLT52_CLEAN_gf2x_transpose_in_place(DIGIT A[]);
void PQCLEAN_LEDAKEMLT52_CLEAN_rand_circulant_sparse_block(POSITION_T *pos_ones, int countOnes, AES_XOF_struct *seed_expander_ctx);
void PQCLEAN_LEDAKEMLT52_CLEAN_rand_circulant_blocks_sequence(DIGIT *sequence, AES_XOF_struct *seed_expander_ctx);
void PQCLEAN_LEDAKEMLT52_CLEAN_gf2x_mod_add_sparse(int sizeR, POSITION_T Res[], int sizeA, const POSITION_T A[], int sizeB, const POSITION_T B[]);
void PQCLEAN_LEDAKEMLT52_CLEAN_gf2x_transpose_in_place_sparse(int sizeA, POSITION_T A[]);
void PQCLEAN_LEDAKEMLT52_CLEAN_gf2x_mod_mul_sparse(size_t sizeR, POSITION_T Res[], size_t sizeA, const POSITION_T A[], size_t sizeB, const POSITION_T B[]);
void PQCLEAN_LEDAKEMLT52_CLEAN_gf2x_mod_mul_dense_to_sparse(DIGIT Res[], const DIGIT dense[], POSITION_T sparse[], unsigned int nPos);
void PQCLEAN_LEDAKEMLT52_CLEAN_gf2x_tobytes(uint8_t *bytes, const DIGIT *poly);

int PQCLEAN_LEDAKEMLT52_CLEAN_gf2x_mod_inverse(DIGIT out[], const DIGIT in[]);

#endif
