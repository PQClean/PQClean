#include "gf.h"
#include "parameters.h"
#include <stdint.h>
/**
 * @file gf.c
 * Galois field implementation with multiplication using lookup tables
 */


/**
 * @brief Returns the integer i such that elt = a^i where a is the primitive element of GF(2^PARAM_M).
 *
 * @returns the logarithm of the given element
 */
uint16_t PQCLEAN_HQC256_CLEAN_gf_log(uint16_t elt) {
    return log[elt];
}



/**
 * @brief Multiplies nonzero element a by element b
 *
 * @returns the product a*b
 * @param[in] a First element of GF(2^PARAM_M) to multiply (cannot be zero)
 * @param[in] b Second element of GF(2^PARAM_M) to multiply (cannot be zero)
 */
uint16_t PQCLEAN_HQC256_CLEAN_gf_mul(uint16_t a, uint16_t b) {
    // mask = 0xffff if neither a nor b is zero. Otherwise mask is 0.
    int16_t mask = ((log[a] | log[b]) >> PARAM_M) - 1;
    return mask & exp[PQCLEAN_HQC256_CLEAN_gf_mod(log[a] + log[b])];
}



/**
 * @brief Squares an element of GF(2^PARAM_M)
 *
 * @returns a^2
 * @param[in] a Element of GF(2^PARAM_M)
 */
uint16_t PQCLEAN_HQC256_CLEAN_gf_square(uint16_t a) {
    int16_t mask = (log[a] >> PARAM_M) - 1;
    return mask & exp[PQCLEAN_HQC256_CLEAN_gf_mod(2 * log[a])];
}



/**
 * @brief Computes the inverse of an element of GF(2^PARAM_M)
 *
 * @returns the inverse of a
 * @param[in] a Element of GF(2^PARAM_M)
 */
uint16_t PQCLEAN_HQC256_CLEAN_gf_inverse(uint16_t a) {
    return exp[PARAM_GF_MUL_ORDER - log[a]];
}



/**
 * @brief Returns i modulo 2^PARAM_M-1
 *
 * i must be less than 2*(2^PARAM_M-1).
 * Therefore, the return value is either i or i-2^PARAM_M+1.
 *
 * @returns i mod (2^PARAM_M-1)
 * @param[in] i The integer whose modulo is taken
 */
uint16_t PQCLEAN_HQC256_CLEAN_gf_mod(uint16_t i) {
    uint16_t tmp = i - PARAM_GF_MUL_ORDER;

    // mask = 0xffff if(i < PARAM_GF_MUL_ORDER)
    int16_t mask = -(tmp >> 15);

    return tmp + (mask & PARAM_GF_MUL_ORDER);
}
