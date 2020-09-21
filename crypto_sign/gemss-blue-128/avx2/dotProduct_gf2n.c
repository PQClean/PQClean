#include "add_gf2n.h"
#include "dotProduct_gf2n.h"
#include "mul_gf2n.h"
#include "rem_gf2n.h"
#include "simd.h"



/**
 * @brief   Dot product of vectors in GF(2^n).
 * @details The modular reduction is done only at the end.
 * @param[in]   vec_x   A vector of len elements in GF(2^n).
 * @param[in]   vec_y   A vector of len elements in GF(2^n).
 * @param[out]  res res=vec_x.vec_y, in GF(2^n).
 * @remark  Constant-time implementation.
 */
void PQCLEAN_GEMSSBLUE128_AVX2_dotProduct_no_simd_gf2n(uint64_t res[NB_WORD_GFqn],
        const UINT *vec_x, const UINT *vec_y, unsigned int len) {
    static_gf2n acc[NB_WORD_MUL], tmp_mul[NB_WORD_MUL];
    unsigned int i;

    /* i=0 */
    mul_gf2x(acc, vec_x, vec_y);

    for (i = 1; i < len; ++i) {
        vec_x += NB_WORD_GFqn;
        vec_y += NB_WORD_GFqn;
        mul_gf2x(tmp_mul, vec_x, vec_y);
        add2_product_gf2n(acc, tmp_mul);
    }

    rem_gf2n(res, acc);
}

/**
 * @brief   Sum of two dot products of vectors in GF(2^n).
 * @details The modular reduction is done only at the end.
 * @param[in]   vec_x   A vector of len elements in GF(2^n).
 * @param[in]   vec_y   A vector of len elements in GF(2^n).
 * @param[in]   vec2_x  A vector of len elements in GF(2^n).
 * @param[in]   vec2_y  A vector of len elements in GF(2^n).
 * @param[out]  res res=vec_x.vec_y + vec2_x.vec2_y, in GF(2^n).
 * @remark  Constant-time implementation.
 */
void PQCLEAN_GEMSSBLUE128_AVX2_doubleDotProduct_no_simd_gf2n(uint64_t res[NB_WORD_GFqn],
        const UINT *vec_x, const UINT *vec_y,
        const UINT *vec2_x, const UINT *vec2_y, unsigned int len) {
    static_gf2n acc[NB_WORD_MUL], tmp_mul[NB_WORD_MUL];
    unsigned int i;

    /* i=0 */
    mul_gf2x(acc, vec_x, vec_y);

    for (i = 1; i < len; ++i) {
        vec_x += NB_WORD_GFqn;
        vec_y += NB_WORD_GFqn;
        mul_gf2x(tmp_mul, vec_x, vec_y);
        add2_product_gf2n(acc, tmp_mul);
    }

    for (i = 0; i < len; ++i) {
        mul_gf2x(tmp_mul, vec2_x, vec2_y);
        add2_product_gf2n(acc, tmp_mul);
        vec2_x += NB_WORD_GFqn;
        vec2_y += NB_WORD_GFqn;
    }

    rem_gf2n(res, acc);
}
