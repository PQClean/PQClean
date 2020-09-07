#include "gf.h"
#include "parameters.h"
#include <emmintrin.h>
#include <immintrin.h>
#include <stdint.h>
/**
 * @file gf.c
 * Galois field implementation with multiplication using the pclmulqdq instruction
 */


static uint16_t gf_reduce(uint64_t x, size_t deg_x);


/**
 * Returns the integer i such that elt = a^i
 * where a is the primitive element of GF(2^GF_M).
 *@returns the logarithm of the given element
 */
uint16_t PQCLEAN_HQCRMRS256_AVX2_gf_log(uint16_t elt) {
    return log[elt];
}



/**
 * Reduces polynomial x modulo primitive polynomial GF_POLY.
 * @returns x mod GF_POLY
 * @param[in] x Polynomial of degree less than 64
 * @param[in] deg_x The degree of polynomial x
 */
static uint16_t gf_reduce(uint64_t x, size_t deg_x) {
    // Compute the distance between the primitive polynomial first two set bits
    size_t lz1 = __builtin_clz(PARAM_GF_POLY);
    size_t lz2 = __builtin_clz(PARAM_GF_POLY ^ 1 << PARAM_M);
    size_t dist = lz2 - lz1;

    // Deduce the number of steps of reduction
    size_t steps = CEIL_DIVIDE(deg_x - (PARAM_M - 1), dist);

    // Reduce
    for (size_t i = 0; i < steps; ++i) {
        uint64_t mod = x >> PARAM_M;
        x &= (1 << PARAM_M) - 1;
        x ^= mod;

        size_t tz1 = 0;
        uint16_t rmdr = PARAM_GF_POLY ^ 1;
        for (size_t j = __builtin_popcount(PARAM_GF_POLY) - 2; j; --j) {
            size_t tz2 = __builtin_ctz(rmdr);
            size_t shift = tz2 - tz1;
            mod <<= shift;
            x ^= mod;
            rmdr ^= 1 << tz2;
            tz1 = tz2;
        }
    }

    return x;
}



/**
 * Multiplies two elements of GF(2^GF_M).
 * @returns the product a*b
 * @param[in] a Element of GF(2^GF_M)
 * @param[in] b Element of GF(2^GF_M)
 */
uint16_t PQCLEAN_HQCRMRS256_AVX2_gf_mul(uint16_t a, uint16_t b) {
    __m128i va = _mm_cvtsi32_si128(a);
    __m128i vb = _mm_cvtsi32_si128(b);
    __m128i vab = _mm_clmulepi64_si128(va, vb, 0);
    uint32_t ab = _mm_cvtsi128_si32(vab);

    return gf_reduce(ab, 2 * (PARAM_M - 1));
}



/**
 * Squares an element of GF(2^GF_M).
 * @returns a^2
 * @param[in] a Element of GF(2^GF_M)
 */
uint16_t PQCLEAN_HQCRMRS256_AVX2_gf_square(uint16_t a) {
    uint32_t b = a;
    uint32_t s = b & 1;
    for (size_t i = 1; i < PARAM_M; ++i) {
        b <<= 1;
        s ^= b & (1 << 2 * i);
    }

    return gf_reduce(s, 2 * (PARAM_M - 1));
}


/**
 * Computes the inverse of an element of GF(2^GF_M) by fast exponentiation.
 * @returns the inverse of a
 * @param[in] a Element of GF(2^GF_M)
 */
uint16_t PQCLEAN_HQCRMRS256_AVX2_gf_inverse(uint16_t a) {
    size_t pow = (1 << PARAM_M) - 2;
    uint16_t inv = 1;

    do {
        if (pow & 1) {
            inv = PQCLEAN_HQCRMRS256_AVX2_gf_mul(inv, a);
        }
        a = PQCLEAN_HQCRMRS256_AVX2_gf_square(a);
        pow >>= 1;
    } while (pow);

    return inv;
}



/**
 * Returns i modulo 2^GF_M-1.
 * i must be less than 2*(2^GF_M-1).
 * Therefore, the return value is either i or i-2^GF_M+1.
 * @returns i mod (2^GF_M-1)
 * @param[in] i The integer whose modulo is taken
 */
uint16_t PQCLEAN_HQCRMRS256_AVX2_gf_mod(uint16_t i) {
    uint16_t tmp = i - PARAM_GF_MUL_ORDER;

    // mask = 0xffff if (i < GF_MUL_ORDER)
    int16_t mask = -(tmp >> 15);

    return tmp + (mask & PARAM_GF_MUL_ORDER);
}
