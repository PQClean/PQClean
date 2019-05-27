#include "gf2x_arith_mod_xPplusOne.h"
#include "rng.h"

#include <assert.h>
#include <string.h>  // memcpy(...), memset(...)


static void gf2x_mod(DIGIT out[], const DIGIT in[]) {

    int i, j, posTrailingBit, maskOffset;
    DIGIT mask, aux[2 * NUM_DIGITS_GF2X_ELEMENT];

    memcpy(aux, in, 2 * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);
    memset(out, 0x00, NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);
    if (2 * NUM_DIGITS_GF2X_ELEMENT < NUM_DIGITS_GF2X_MODULUS) {
        for (i = 0; i < 2 * NUM_DIGITS_GF2X_ELEMENT; i++) {
            out[NUM_DIGITS_GF2X_ELEMENT - 1 - i] = in[2 * NUM_DIGITS_GF2X_ELEMENT - 1 - i];
        }
        return;
    }

    for (i = 0; i < (2 * NUM_DIGITS_GF2X_ELEMENT) - NUM_DIGITS_GF2X_MODULUS; i += 1) {
        for (j = DIGIT_SIZE_b - 1; j >= 0; j--) {
            mask = ((DIGIT)0x1) << j;
            if (aux[i] & mask) {
                aux[i] ^= mask;
                posTrailingBit = (DIGIT_SIZE_b - 1 - j) + i * DIGIT_SIZE_b + P;
                maskOffset = (DIGIT_SIZE_b - 1 - (posTrailingBit % DIGIT_SIZE_b));
                mask = (DIGIT) 0x1 << maskOffset;
                aux[posTrailingBit / DIGIT_SIZE_b] ^= mask;
            }
        }
    }

    for (j = DIGIT_SIZE_b - 1; j >= MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS; j--) {
        mask = ((DIGIT)0x1) << j;
        if (aux[i] & mask) {
            aux[i] ^= mask;
            posTrailingBit = (DIGIT_SIZE_b - 1 - j) + i * DIGIT_SIZE_b + P;
            maskOffset = (DIGIT_SIZE_b - 1 - (posTrailingBit % DIGIT_SIZE_b));
            mask = (DIGIT) 0x1 << maskOffset;
            aux[posTrailingBit / DIGIT_SIZE_b] ^= mask;
        }
    }

    int to_copy = (2 * NUM_DIGITS_GF2X_ELEMENT > NUM_DIGITS_GF2X_ELEMENT) ? NUM_DIGITS_GF2X_ELEMENT : 2 * NUM_DIGITS_GF2X_ELEMENT;

    for (i = 0; i < to_copy; i++) {
        out[NUM_DIGITS_GF2X_ELEMENT - 1 - i] = aux[2 * NUM_DIGITS_GF2X_ELEMENT - 1 - i];
    }

}

static void left_bit_shift(const int length, DIGIT in[]) {

    int j;
    for (j = 0; j < length - 1; j++) {
        in[j] <<= 1;                    /* logical shift does not need clearing */
        in[j] |= in[j + 1] >> (DIGIT_SIZE_b - 1);
    }
    in[j] <<= 1;
}

static void right_bit_shift(unsigned int length, DIGIT in[]) {

    unsigned int j;
    for (j = length - 1; j > 0 ; j--) {
        in[j] >>= 1;
        in[j] |=  (in[j - 1] & (DIGIT)0x01) << (DIGIT_SIZE_b - 1);
    }
    in[j] >>= 1;
}


/* shifts by whole digits */
static inline void left_DIGIT_shift_n(unsigned int length, DIGIT in[], unsigned int amount) {
    unsigned int j;
    for (j = 0; (j + amount) < length; j++) {
        in[j] = in[j + amount];
    }
    for (; j < length; j++) {
        in[j] = (DIGIT)0;
    }
}


/* may shift by an arbitrary amount*/
static void left_bit_shift_wide_n(const int length, DIGIT in[], unsigned int amount) {
    left_DIGIT_shift_n(length, in, amount / DIGIT_SIZE_b);
    PQCLEAN_LEDAKEMLT12_CLEAN_left_bit_shift_n(length, in, amount % DIGIT_SIZE_b);
}

static uint8_t byte_reverse_with_64bitDIGIT(uint8_t b) {
    b = (b * 0x0202020202ULL & 0x010884422010ULL) % 1023;
    return b;
}

static DIGIT reverse_digit(const DIGIT b) {
    int i;
    union toReverse_t {
        uint8_t inByte[DIGIT_SIZE_B];
        DIGIT digitValue;
    } toReverse;

    toReverse.digitValue = b;
    for (i = 0; i < DIGIT_SIZE_B; i++) {
        toReverse.inByte[i] = byte_reverse_with_64bitDIGIT(toReverse.inByte[i]);
    }

    return __builtin_bswap64(toReverse.digitValue);
}

void PQCLEAN_LEDAKEMLT12_CLEAN_gf2x_transpose_in_place(DIGIT A[]) {
    /* it keeps the lsb in the same position and
     * inverts the sequence of the remaining bits */

    DIGIT mask = (DIGIT)0x1;
    DIGIT rev1, rev2, a00;
    int i, slack_bits_amount = NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_b - P;

    a00 = A[NUM_DIGITS_GF2X_ELEMENT - 1] & mask;
    right_bit_shift(NUM_DIGITS_GF2X_ELEMENT, A);

    for (i = NUM_DIGITS_GF2X_ELEMENT - 1; i >= (NUM_DIGITS_GF2X_ELEMENT + 1) / 2; i--) {
        rev1 = reverse_digit(A[i]);
        rev2 = reverse_digit(A[NUM_DIGITS_GF2X_ELEMENT - 1 - i]);
        A[i] = rev2;
        A[NUM_DIGITS_GF2X_ELEMENT - 1 - i] = rev1;
    }
    if (NUM_DIGITS_GF2X_ELEMENT % 2 == 1) {
        A[NUM_DIGITS_GF2X_ELEMENT / 2] = reverse_digit(A[NUM_DIGITS_GF2X_ELEMENT / 2]);
    }

    if (slack_bits_amount) {
        PQCLEAN_LEDAKEMLT12_CLEAN_right_bit_shift_n(NUM_DIGITS_GF2X_ELEMENT, A, slack_bits_amount);
    }
    A[NUM_DIGITS_GF2X_ELEMENT - 1] = (A[NUM_DIGITS_GF2X_ELEMENT - 1] & (~mask)) | a00;
}

static void rotate_bit_left(DIGIT in[]) { /*  equivalent to x * in(x) mod x^P+1 */

    DIGIT mask, rotated_bit;

    if (NUM_DIGITS_GF2X_MODULUS == NUM_DIGITS_GF2X_ELEMENT) {

        int msb_offset_in_digit = MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS - 1;
        mask = ((DIGIT)0x1) << msb_offset_in_digit;
        rotated_bit = !!(in[0] & mask);
        in[0] &= ~mask;                     /* clear shifted bit */
        left_bit_shift(NUM_DIGITS_GF2X_ELEMENT, in);
    } else {
        /* NUM_DIGITS_GF2X_MODULUS == 1 + NUM_DIGITS_GF2X_ELEMENT and
                * MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS == 0
                */
        mask =  ((DIGIT)0x1) << (DIGIT_SIZE_b - 1);
        rotated_bit = !!(in[0] & mask);
        in[0] &= ~mask;                     /* clear shifted bit */
        left_bit_shift(NUM_DIGITS_GF2X_ELEMENT, in);

    }
    in[NUM_DIGITS_GF2X_ELEMENT - 1] |= rotated_bit;
}

static void rotate_bit_right(DIGIT in[]) { /*  x^{-1} * in(x) mod x^P+1 */

    DIGIT rotated_bit = in[NUM_DIGITS_GF2X_ELEMENT - 1] & ((DIGIT)0x1);
    right_bit_shift(NUM_DIGITS_GF2X_ELEMENT, in);

    if (NUM_DIGITS_GF2X_MODULUS == NUM_DIGITS_GF2X_ELEMENT) {
        int msb_offset_in_digit = MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS - 1;
        rotated_bit = rotated_bit << msb_offset_in_digit;
    } else {
        /* NUM_DIGITS_GF2X_MODULUS == 1 + NUM_DIGITS_GF2X_ELEMENT and
                * MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS == 0
                */
        rotated_bit = rotated_bit << (DIGIT_SIZE_b - 1);
    }
    in[0] |= rotated_bit;
}

static void gf2x_swap(const int length,
                      DIGIT f[],
                      DIGIT s[]) {
    DIGIT t;
    for (int i = length - 1; i >= 0; i--) {
        t = f[i];
        f[i] = s[i];
        s[i] = t;
    }
}

/*
 * Optimized extended GCD algorithm to compute the multiplicative inverse of
 * a non-zero element in GF(2)[x] mod x^P+1, in polyn. representation.
 *
 * H. Brunner, A. Curiger, and M. Hofstetter. 1993.
 * On Computing Multiplicative Inverses in GF(2^m).
 * IEEE Trans. Comput. 42, 8 (August 1993), 1010-1015.
 * DOI=http://dx.doi.org/10.1109/12.238496
 *
 *
 * Henri Cohen, Gerhard Frey, Roberto Avanzi, Christophe Doche, Tanja Lange,
 * Kim Nguyen, and Frederik Vercauteren. 2012.
 * Handbook of Elliptic and Hyperelliptic Curve Cryptography,
 * Second Edition (2nd ed.). Chapman & Hall/CRC.
 * (Chapter 11 -- Algorithm 11.44 -- pag 223)
 *
 */

int PQCLEAN_LEDAKEMLT12_CLEAN_gf2x_mod_inverse(DIGIT out[], const DIGIT in[]) {   /* in^{-1} mod x^P-1 */

    int i;
    long int delta = 0;
    DIGIT u[NUM_DIGITS_GF2X_ELEMENT] = {0};
    DIGIT v[NUM_DIGITS_GF2X_ELEMENT] = {0};
    DIGIT s[NUM_DIGITS_GF2X_MODULUS] = {0};
    DIGIT f[NUM_DIGITS_GF2X_MODULUS] = {0}; // alignas(32)?

    DIGIT mask;
    u[NUM_DIGITS_GF2X_ELEMENT - 1] = 0x1;
    v[NUM_DIGITS_GF2X_ELEMENT - 1] = 0x0;

    s[NUM_DIGITS_GF2X_MODULUS - 1] = 0x1;
    if (MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS == 0) {
        mask = 0x1;
    } else {
        mask = (((DIGIT)0x1) << MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS);
    }
    s[0] |= mask;

    for (i = NUM_DIGITS_GF2X_ELEMENT - 1; i >= 0 && in[i] == 0; i--) { };
    if (i < 0) {
        return 0;
    }

    if (NUM_DIGITS_GF2X_MODULUS == 1 + NUM_DIGITS_GF2X_ELEMENT) {
        for (i = NUM_DIGITS_GF2X_MODULUS - 1; i >= 1 ; i--) {
            f[i] = in[i - 1];
        }
    } else { /* they are equal */
        for (i = NUM_DIGITS_GF2X_MODULUS - 1; i >= 0 ; i--) {
            f[i] = in[i];
        }
    }

    for (i = 1; i <= 2 * P; i++) {
        if ( (f[0] & mask) == 0 ) {
            left_bit_shift(NUM_DIGITS_GF2X_MODULUS, f);
            rotate_bit_left(u);
            delta += 1;
        } else {
            if ( (s[0] & mask) != 0) {
                gf2x_add(s, s, f, NUM_DIGITS_GF2X_MODULUS);
                gf2x_mod_add(v, v, u);
            }
            left_bit_shift(NUM_DIGITS_GF2X_MODULUS, s);
            if ( delta == 0 ) {
                gf2x_swap(NUM_DIGITS_GF2X_MODULUS, f, s);
                gf2x_swap(NUM_DIGITS_GF2X_ELEMENT, u, v);
                rotate_bit_left(u);
                delta = 1;
            } else {
                rotate_bit_right(u);
                delta = delta - 1;
            }
        }
    }

    for (i = NUM_DIGITS_GF2X_ELEMENT - 1; i >= 0 ; i--) {
        out[i] = u[i];
    }

    return (delta == 0);
}

void PQCLEAN_LEDAKEMLT12_CLEAN_gf2x_mod_mul(DIGIT Res[], const DIGIT A[], const DIGIT B[]) {

    DIGIT aux[2 * NUM_DIGITS_GF2X_ELEMENT];
    GF2X_MUL(2 * NUM_DIGITS_GF2X_ELEMENT, aux,
             NUM_DIGITS_GF2X_ELEMENT, A,
             NUM_DIGITS_GF2X_ELEMENT, B);
    gf2x_mod(Res, aux);

}

/*PRE: the representation of the sparse coefficients is sorted in increasing
 order of the coefficients themselves */
void PQCLEAN_LEDAKEMLT12_CLEAN_gf2x_mod_mul_dense_to_sparse(
    DIGIT Res[],
    const DIGIT dense[],
    POSITION_T sparse[], unsigned int nPos) {

    DIGIT aux[2 * NUM_DIGITS_GF2X_ELEMENT] = {0x00};
    DIGIT resDouble[2 * NUM_DIGITS_GF2X_ELEMENT] = {0x00};
    memcpy(aux + NUM_DIGITS_GF2X_ELEMENT, dense, NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);
    memcpy(resDouble + NUM_DIGITS_GF2X_ELEMENT, dense,
           NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);

    if (sparse[0] != INVALID_POS_VALUE) {
        left_bit_shift_wide_n(2 * NUM_DIGITS_GF2X_ELEMENT, resDouble, sparse[0]);
        left_bit_shift_wide_n(2 * NUM_DIGITS_GF2X_ELEMENT, aux, sparse[0]);

        for (unsigned int i = 1; i < nPos; i++) {
            if (sparse[i] != INVALID_POS_VALUE) {
                left_bit_shift_wide_n(2 * NUM_DIGITS_GF2X_ELEMENT, aux, (sparse[i] - sparse[i - 1]) );
                gf2x_add(resDouble, aux, resDouble, 2 * NUM_DIGITS_GF2X_ELEMENT);
            }
        }
    }

    gf2x_mod(Res, resDouble);

}

void PQCLEAN_LEDAKEMLT12_CLEAN_gf2x_transpose_in_place_sparse(int sizeA, POSITION_T A[]) {

    POSITION_T t;
    int i = 0, j;

    if (A[i] == 0) {
        i = 1;
    }
    j = i;

    for (; i < sizeA && A[i] != INVALID_POS_VALUE; i++) {
        A[i] = P - A[i];
    }

    for (i -= 1; j < i; j++, i--) {
        t = A[j];
        A[j] = A[i];
        A[i] = t;
    }

}

void PQCLEAN_LEDAKEMLT12_CLEAN_gf2x_mod_mul_sparse(size_t sizeR, POSITION_T Res[],
        size_t sizeA, const POSITION_T A[],
        size_t sizeB, const POSITION_T B[]) {

    /* compute all the coefficients, filling invalid positions with P*/
    size_t lastFilledPos = 0;
    for (size_t i = 0 ; i < sizeA ; i++) {
        for (size_t j = 0 ; j < sizeB ; j++) {
            uint32_t prod = A[i] + B[j];
            prod = ( (prod >= P) ? prod - P : prod);
            if ((A[i] != INVALID_POS_VALUE) &&
                    (B[j] != INVALID_POS_VALUE)) {
                Res[lastFilledPos] = prod;
            } else {
                Res[lastFilledPos] = INVALID_POS_VALUE;
            }
            lastFilledPos++;
        }
    }
    while (lastFilledPos < sizeR) {
        Res[lastFilledPos] = INVALID_POS_VALUE;
        lastFilledPos++;
    }
    quicksort_sparse(Res);
    /* eliminate duplicates */
    POSITION_T lastReadPos = Res[0];
    int duplicateCount;
    size_t write_idx = 0;
    size_t read_idx = 0;
    while (read_idx < sizeR  && Res[read_idx] != INVALID_POS_VALUE) {
        lastReadPos = Res[read_idx];
        read_idx++;
        duplicateCount = 1;
        while ( (Res[read_idx] == lastReadPos) && (Res[read_idx] != INVALID_POS_VALUE)) {
            read_idx++;
            duplicateCount++;
        }
        if (duplicateCount % 2) {
            Res[write_idx] = lastReadPos;
            write_idx++;
        }
    }
    /* fill remaining cells with INVALID_POS_VALUE */
    for (; write_idx < sizeR; write_idx++) {
        Res[write_idx] = INVALID_POS_VALUE;
    }
}

/* the implementation is safe even in case A or B alias with the result */
/* PRE: A and B should be sorted and have INVALID_POS_VALUE at the end */
void PQCLEAN_LEDAKEMLT12_CLEAN_gf2x_mod_add_sparse(
    int sizeR, POSITION_T Res[],
    int sizeA, const POSITION_T A[],
    int sizeB, const POSITION_T B[]) {

    POSITION_T tmpRes[DV * M]; // TODO: now function only works for adding (disjunct) DV and M positions
    int idxA = 0, idxB = 0, idxR = 0;
    while ( idxA < sizeA  &&
            idxB < sizeB  &&
            A[idxA] != INVALID_POS_VALUE &&
            B[idxB] != INVALID_POS_VALUE ) {

        if (A[idxA] == B[idxB]) {
            idxA++;
            idxB++;
        } else {
            if (A[idxA] < B[idxB]) {
                tmpRes[idxR] = A[idxA];
                idxA++;
            } else {
                tmpRes[idxR] = B[idxB];
                idxB++;
            }
            idxR++;
        }
    }

    while (idxA < sizeA && A[idxA] != INVALID_POS_VALUE) {
        tmpRes[idxR] = A[idxA];
        idxA++;
        idxR++;
    }

    while (idxB < sizeB && B[idxB] != INVALID_POS_VALUE) {
        tmpRes[idxR] = B[idxB];
        idxB++;
        idxR++;
    }

    while (idxR < sizeR) {
        tmpRes[idxR] = INVALID_POS_VALUE;
        idxR++;
    }
    memcpy(Res, tmpRes, sizeof(POSITION_T)*sizeR);

}

/* Return a uniform random value in the range 0..n-1 inclusive,
 * applying a rejection sampling strategy and exploiting as a random source
 * the NIST seedexpander seeded with the proper key.
 * Assumes that the maximum value for the range n is 2^32-1
 */
static uint32_t rand_range(const unsigned int n, const int logn, AES_XOF_struct *seed_expander_ctx) {
    unsigned long required_rnd_bytes = (logn + 7) / 8;
    unsigned char rnd_char_buffer[4];
    uint32_t rnd_value;
    uint32_t mask = ( (uint32_t)1 << logn) - 1;

    do {
        PQCLEAN_LEDAKEMLT12_CLEAN_seedexpander(seed_expander_ctx, rnd_char_buffer, required_rnd_bytes);
        /* obtain an endianness independent representation of the generated random
         bytes into an unsigned integer */
        rnd_value =  ((uint32_t)rnd_char_buffer[3] << 24) +
                     ((uint32_t)rnd_char_buffer[2] << 16) +
                     ((uint32_t)rnd_char_buffer[1] <<  8) +
                     ((uint32_t)rnd_char_buffer[0] <<  0) ;
        rnd_value = mask & rnd_value;
    } while (rnd_value >= n);

    return rnd_value;
}

/* Obtains fresh randomness and seed-expands it until all the required positions
 * for the '1's in the circulant block are obtained */
void PQCLEAN_LEDAKEMLT12_CLEAN_rand_circulant_sparse_block(POSITION_T *pos_ones,
        int countOnes,
        AES_XOF_struct *seed_expander_ctx) {

    int duplicated, placedOnes = 0;
    uint32_t p;

    while (placedOnes < countOnes) {
        p = rand_range(NUM_BITS_GF2X_ELEMENT,
                       BITS_TO_REPRESENT(P),
                       seed_expander_ctx);
        duplicated = 0;
        for (int j = 0; j < placedOnes; j++) {
            if (pos_ones[j] == p) {
                duplicated = 1;
            }
        }
        if (duplicated == 0) {
            pos_ones[placedOnes] = p;
            placedOnes++;
        }
    }
}

/* Returns random weight-t circulant block */
void PQCLEAN_LEDAKEMLT12_CLEAN_rand_circulant_blocks_sequence(DIGIT sequence[N0 * NUM_DIGITS_GF2X_ELEMENT],
        AES_XOF_struct *seed_expander_ctx) {

    int rndPos[NUM_ERRORS_T],  duplicated, counter = 0;
    memset(sequence, 0x00, N0 * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);


    while (counter < NUM_ERRORS_T) {
        int p = rand_range(N0 * NUM_BITS_GF2X_ELEMENT, BITS_TO_REPRESENT(P),
                           seed_expander_ctx);
        duplicated = 0;
        for (int j = 0; j < counter; j++) {
            if (rndPos[j] == p) {
                duplicated = 1;
            }
        }
        if (duplicated == 0) {
            rndPos[counter] = p;
            counter++;
        }
    }
    for (int j = 0; j < counter; j++) {
        int polyIndex = rndPos[j] / P;
        int exponent = rndPos[j] % P;
        gf2x_set_coeff( sequence + NUM_DIGITS_GF2X_ELEMENT * polyIndex, exponent,
                        ( (DIGIT) 1));
    }

}
