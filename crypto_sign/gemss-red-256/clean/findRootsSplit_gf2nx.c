#include "add_gf2nx.h"
#include "arch.h"
#include "div_gf2nx.h"
#include "findRootsSplit_gf2nx.h"
#include "gcd_gf2nx.h"
#include "inv_gf2n.h"
#include "mul_gf2n.h"
#include "rand_gf2n.h"
#include "sqr_gf2n.h"
#include "sqr_gf2nx.h"
#include "tools_gf2n.h"
#include <stdlib.h>
#include <string.h>


/**
 * @brief   Computation of the sum from i=0 to n-1 of ((rX)^(2^i)) mod f
 * @param[in,out]    poly_trace  An element of GF(2^n)[X], initialized to rX
 * with r in GF(2^n). The ouput value is the sum from i=0 to n-1 of
 * ((rX)^(2^i) mod f), in GF(2^n)[X].
 * @param[in]   poly_frob   An intermediate buffer, becomes (rX)^(2^(n-1)) mod f
 * @param[in]   f   A monic polynomial in GF(2^n)[X].
 * @param[in]   deg The degree of f.
 * @remark  Requires to allocate deg elements of GF(2^n) for poly_trace.
 * @remark  Requires to allocate ((deg<<1)-1)*NB_WORD_GFqn words for poly_frob,
 * initialized to zero.
 * @remark  Requirement: f is monic.
 * @remark  Requirement: deg>1.
 * @remark  Constant-time implementation when deg is not secret and when
 * CONSTANT_TIME!=0.
 */
static void traceMap_gf2nx(gf2nx poly_trace, gf2nx poly_frob,
                           cst_gf2nx f, unsigned int deg) {
    unsigned int j, i;

    i = 1;
    /* (2^i) < deg does not require modular reduction by f */
    while ((1U << i) < deg) {
        /* poly_trace += ((rX)^(2^i)) mod f
           Here, ((rX)^(2^i)) mod f == (rX)^(2^i) since (2^i) < deg */
        sqr_gf2n(poly_trace + (NB_WORD_GFqn << i), poly_trace + (NB_WORD_GFqn << (i - 1)));
        ++i;
    }

    /* Here, (rX)^(2^i) is the first time where we need modular reduction */
    if (i < HFEn) {
        /* poly_frob = (rX)^(2^i) = ((rX)^(2^(i-1)))^2 */
        sqr_gf2n(poly_frob + (NB_WORD_GFqn << i), poly_trace + (NB_WORD_GFqn << (i - 1)));

        /* poly_frob = ((rX)^(2^i)) mod f */
        PQCLEAN_GEMSSRED256_CLEAN_div_r_monic_cst_gf2nx(poly_frob, 1U << i, f, deg);
        /* poly_trace += ((rX)^(2^i)) mod f */
        add2_gf2nx(poly_trace, poly_frob, deg, j);
        ++i;

        for (; i < HFEn; ++i) {
            /* poly_frob = (rX)^(2^i) = ((rX)^(2^(i-1)) mod f)^2 */
            PQCLEAN_GEMSSRED256_CLEAN_sqr_gf2nx(poly_frob, deg - 1);
            /* poly_frob = ((rX)^(2^i)) mod f */
            PQCLEAN_GEMSSRED256_CLEAN_div_r_monic_cst_gf2nx(poly_frob, (deg - 1) << 1U, f, deg);
            /* poly_trace += ((rX)^(2^i)) mod f */
            add2_gf2nx(poly_trace, poly_frob, deg, j);
        }
    }
}


/**
 * @brief   Computation of the roots of f, when f is split and monic.
 * @param[out]  roots   The vector of the roots of f, in GF(2^n).
 * @param[in]   f   A monic and split polynomial in GF(2^n)[X].
 * @param[in]   deg The degree of f.
 * @remark  Requires to allocate deg elements of GF(2^n) for roots.
 * @remark  Requirement: f is monic and split.
 * @remark  Requirement: deg>0.
 * @remark  This implementation is not in constant-time.
 */
void PQCLEAN_GEMSSRED256_CLEAN_findRootsSplit_gf2nx(vec_gf2n roots, gf2nx f,
        unsigned int deg) {
    UINT poly_frob[((HFEDeg << 1) - 1)*NB_WORD_GFqn] = {0};
    UINT p1[HFEDeg * NB_WORD_GFqn] = {0};
    UINT p2[(HFEDeg + 1)*NB_WORD_GFqn] = {0};
    UINT *poly_trace = p1;
    UINT *f_cp = p2;
    static_gf2n inv[NB_WORD_GFqn] = {0};
    unsigned int b, i, l, d;

    if (deg == 1) {
        /* Extract the unique root which is the constant of f */
        copy_gf2n(roots, f);
        return;
    }

    do {
        /* Set poly_frob to zero */
        set0_gf2nx(poly_frob, (deg << 1) - 1, l);
        /* Set poly_trace to zero */
        set0_gf2nx(poly_trace, deg, l);

        /* Initialization to rX */
        /* Probability 2^(-n) to find 0 with a correct RNG */
        do {
            rand_gf2n(poly_trace + NB_WORD_GFqn);
        } while (is0_gf2n(poly_trace + NB_WORD_GFqn));

        /* copy of f because the gcd modifies f */
        copy_gf2nx(f_cp, f, deg + 1, l);

        traceMap_gf2nx(poly_trace, poly_frob, f_cp, deg);
        /* Degree of poly_trace */
        d = deg - 1;
        while (is0_gf2n(poly_trace + d * NB_WORD_GFqn) && d) {
            --d;
        }
        l = PQCLEAN_GEMSSRED256_CLEAN_gcd_gf2nx(&b, f_cp, deg, poly_trace, d);

    } while ((!l) || (l == deg));

    if (b) {
        f_cp = poly_trace;
    }
    /* Here, f_cp is a non-trivial divisor of degree l */

    /* f_cp is the gcd */
    /* Here, it becomes monic */
    inv_gf2n(inv, f_cp + l * NB_WORD_GFqn);
    set1_gf2n(f_cp + l * NB_WORD_GFqn);
    for (i = l - 1; i != (unsigned int)(-1); --i) {
        mul_gf2n(f_cp + i * NB_WORD_GFqn, f_cp + i * NB_WORD_GFqn, inv);
    }


    /* f = f_cp * Q */
    /* This function destroyes f */
    PQCLEAN_GEMSSRED256_CLEAN_div_q_monic_gf2nx(f, deg, f_cp, l);
    /* Necessarily, the polynomial f is null here */

    /* f_cp is monic */
    /* We can apply PQCLEAN_GEMSSRED256_CLEAN_findRootsSplit_gf2nx recursively */
    PQCLEAN_GEMSSRED256_CLEAN_findRootsSplit_gf2nx(roots, f_cp, l);

    /* f is monic and f_cp is monic so Q is monic */
    /* We can apply PQCLEAN_GEMSSRED256_CLEAN_findRootsSplit_gf2nx recursively */
    PQCLEAN_GEMSSRED256_CLEAN_findRootsSplit_gf2nx(roots + l * NB_WORD_GFqn, f + l * NB_WORD_GFqn, deg - l);
}
