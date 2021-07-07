#include "sqr_gf2n.h"
#include "sqr_gf2nx.h"
#include "tools_gf2n.h"




/**
 * @brief   Square in GF(2^n)[X], in-place.
 * @details Use of the linearity of the Frobenius endomorphism,
 * (a+bX)^2 = a^2 + b^2 X^2.
 * In particular, the odd degree terms are null.
 * @param[in,out]    poly    An element of GF(2^n)[X].
 * @param[in]   d   The degree of poly is at most d.
 * @remark  Requires to allocate 2d+1 elements of GF(2^n) for poly.
 * @remark  Constant-time implementation when d is not secret.
 * @todo    A complete SSE2 implementation would be faster.
 */
void PQCLEAN_GEMSSRED128_AVX2_sqr_gf2nx(gf2nx poly, unsigned int d) {
    /* A pointer on X^(2*(d-i)) */
    gf2nx poly_2i;
    unsigned int i = NB_WORD_GFqn * d;

    /* Pointer on the last coefficient of poly */
    poly += i;
    /* Pointer on the last coefficient of the square of poly */
    poly_2i = poly + i;

    /* Square of each coefficient, a_i X^i becomes a_i^2 X^(2i).
       Order: X^d X^(d-1) X^(d-2) ... X^(d-i) ... X^2 X^1 */
    for (i = 0; i < d; ++i) {
        sqr_gf2n(poly_2i, poly);
        poly -= NB_WORD_GFqn;
        poly_2i -= NB_WORD_GFqn;
        /* The coefficient of X^(2(d-i)-1) is set to 0 (odd exponent) */
        set0_gf2n(poly_2i);
        poly_2i -= NB_WORD_GFqn;
    }

    /* Square of the coefficient of X^0 */
    sqr_gf2n(poly, poly);
}


/**
 * @brief   Square in GF(2^n)[X], in-place, specialized for d=HFEDeg-1.
 * @details Use of the linearity of the Frobenius endomorphism,
 * (a+bX)^2 = a^2 + b^2 X^2.
 * In particular, the odd degree terms are null.
 * @param[in,out]    poly    An element of GF(2^n)[X], its degree is at most
 * HFEDeg-1.
 * @remark  Requires to allocate 2*HFEDeg-1 elements of GF(2^n) for poly.
 * @remark  Constant-time implementation.
 * @todo    A complete SSE2 implementation would be faster.
 */
void PQCLEAN_GEMSSRED128_AVX2_sqr_HFE_gf2nx(gf2nx poly) {
    /* A pointer on X^(2*(HFEDeg-1-i)) */
    gf2nx poly_2i;
    unsigned int i = NB_WORD_GFqn * (HFEDeg - 1);

    /* Pointer on the last coefficient of poly */
    poly += i;
    /* Pointer on the last coefficient of the square of poly */
    poly_2i = poly + i;

    /* Square of each coefficient, a_i X^i becomes a_i^2 X^(2i).
       Order: X^d X^(d-1) X^(d-2) ... X^(d-i) ... X^2 X^1 for d=HFEDeg-1 */
    for (i = 0; i < (HFEDeg - 1); ++i) {
        sqr_gf2n(poly_2i, poly);
        poly -= NB_WORD_GFqn;
        poly_2i -= NB_WORD_GFqn;
        /* The coefficient of X^(2(d-i)-1) is set to 0 (odd exponent) */
        set0_gf2n(poly_2i);
        poly_2i -= NB_WORD_GFqn;
    }

    /* Square of the coefficient of X^0 */
    sqr_gf2n(poly, poly);
}
