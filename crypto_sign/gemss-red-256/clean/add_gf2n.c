#include "add_gf2n.h"



/**
 * @brief   Addition in GF(2^n).
 * @param[in]   a   An element of GF(2^n).
 * @param[in]   b   An element of GF(2^n).
 * @param[out]  c   c=a+b in GF(2^n).
 * @remark  Constant-time implementation.
 */
void PQCLEAN_GEMSSRED256_CLEAN_add_gf2n_ref(gf2n c, cst_gf2n a, cst_gf2n b) {
    unsigned int i;
    for (i = 0; i < NB_WORD_GFqn; ++i) {
        c[i] = a[i] ^ b[i];
    }
}

/**
 * @brief   Addition in GF(2^n).
 * @param[in]   a   An element of GF(2^n).
 * @param[out]  c   c=a+c in GF(2^n).
 * @remark  Constant-time implementation.
 */
void PQCLEAN_GEMSSRED256_CLEAN_add2_gf2n_ref(gf2n c, cst_gf2n a) {
    unsigned int i;
    for (i = 0; i < NB_WORD_GFqn; ++i) {
        c[i] ^= a[i];
    }
}


/**
 * @brief   Addition in GF(2)[x] for (2n-1)-coefficients polynomials.
 * @param[in]   a   A (2n-1)-coefficients polynomial in GF(2)[x].
 * @param[in]   b   A (2n-1)-coefficients polynomial in GF(2)[x].
 * @param[out]  c   c=a+b in GF(2)[x].
 * @remark  Constant-time implementation.
 */
void PQCLEAN_GEMSSRED256_CLEAN_add_product_gf2n_ref(gf2x c, cst_gf2x a, cst_gf2x b) {
    unsigned int i;
    for (i = 0; i < NB_WORD_MMUL; ++i) {
        c[i] = a[i] ^ b[i];
    }
}

/**
 * @brief   Addition in GF(2)[x] for (2n-1)-coefficients polynomials.
 * @param[in]   a   A (2n-1)-coefficients polynomial in GF(2)[x].
 * @param[out]  c   c=a+c in GF(2)[x].
 * @remark  Constant-time implementation.
 */
void PQCLEAN_GEMSSRED256_CLEAN_add2_product_gf2n_ref(gf2x c, cst_gf2x a) {
    unsigned int i;
    for (i = 0; i < NB_WORD_MMUL; ++i) {
        c[i] ^= a[i];
    }
}
