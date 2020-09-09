#include "parameters.h"
#include "repetition.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
/**
 * @file repetition.c
 * @brief Implementation of repetition codes
 */

#define MASK_N2 ((((uint64_t) 1) << PARAM_N2) - 1)

static inline int32_t popcount(uint64_t n);

/**
 * @brief Encoding each bit in the message m using the repetition code
 *
 *
 * @param[out] em Pointer to an array that is the code word
 * @param[in] m Pointer to an array that is the message
 */
void PQCLEAN_HQC128_CLEAN_repetition_code_encode(uint64_t *em, const uint64_t *m) {
    static const uint64_t mask[2][2] = {{0x0UL, 0x0UL}, {0x7FFFFFFFUL, 0x3FFFFFFFUL}};
    for (size_t i = 0 ; i < VEC_N1_SIZE_64 - 1 ; i++) {
        for (size_t j = 0 ; j < 64 ; j++) {
            uint8_t bit = (m[i] >> j) & 0x1;
            uint32_t pos_r = PARAM_N2 * ((i << 6) + j);
            uint16_t idx_r = (pos_r & 0x3f);
            uint64_t *p64 = em;
            p64 += pos_r >> 6;
            *p64 ^= mask[bit][0] << idx_r;
            *(p64 + 1) ^= mask[bit][1] >> ((63 - idx_r));
        }
    }

    for (size_t j = 0 ; j < (PARAM_N1 & 0x3f) ; j++) {
        uint8_t bit = (m[VEC_N1_SIZE_64 - 1] >> j) & 0x1;
        uint32_t pos_r = PARAM_N2 * (((VEC_N1_SIZE_64 - 1) << 6) + j);
        uint16_t idx_r = (pos_r & 0x3f);
        uint64_t *p64 = em;
        p64 += pos_r >> 6;
        *p64 ^= mask[bit][0] << idx_r;
        *(p64 + 1) ^= mask[bit][1] >> ((63 - idx_r));
    }
}



/**
 * @brief  Compute the Hamming weight of the 64-bit integer n
 *
 * The Hamming weight is computed using a trick described in
 * Henry S. Warren  : "Hacker's Delight", chap 5., p. 66
 * @param[out] the Hamming weight of n
 * @param[in] a 64-bit integer n
 */
static inline int32_t popcount(uint64_t n) {
    n -= (n >> 1) & 0x5555555555555555UL;
    n = (n & 0x3333333333333333UL) + ((n >> 2) & 0x3333333333333333UL);
    n = (n + (n >> 4)) & 0x0f0f0f0f0f0f0f0fUL;
    return (n * 0x0101010101010101UL) >> 56;
}



/**
 * @brief Decoding the code words to a message using the repetition code
 *
 * We use a majority decoding. In fact we have that PARAM_N2 = 2 * PARAM_T + 1, thus,
 * if the Hamming weight of the vector is greater than PARAM_T, the code word is decoded
 * to 1 and 0 otherwise.
 *
 * @param[out] m Pointer to an array that is the message
 * @param[in] em Pointer to an array that is the code word
 */
void PQCLEAN_HQC128_CLEAN_repetition_code_decode(uint64_t *m, const uint64_t *em) {
    size_t t = 0, b, bn, bi, c, cn, ci;
    uint64_t cx, ones;

    for (b = 0 ; b < PARAM_N1N2 - PARAM_N2 + 1 ; b += PARAM_N2) {
        bn = b >> 6;
        bi = b & 63;
        c = b + PARAM_N2 - 1;
        cn = c >> 6;
        ci = c & 63;
        cx = em[cn] << (63 - ci);
        int64_t verif = (cn == (bn + 1));
        ones = popcount(((em[bn] >> bi) & MASK_N2) | (cx * verif));
        m[t >> 6] |= ((uint64_t) (ones > PARAM_T)) << (t & 63);
        t++;
    }
}
