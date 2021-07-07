#include "add_gf2n.h"
#include "add_gf2nx.h"
#include "arch.h"
#include "div_gf2nx.h"
#include "frobeniusMap_gf2nx.h"
#include "gf2x.h"
#include "mul_gf2n.h"
#include "rem_gf2n.h"
#include "sqr_gf2n.h"
#include "sqr_gf2nx.h"
#include "tools_gf2n.h"
#include <stdlib.h>



/**
 * @brief   Computation of (X^(2^n) - X) mod (F-U).
 * @param[out]  Xqn Xqn = (X^(2^n) - X) mod (F.poly-U) in GF(2^n)[X].
 * @param[in]   F   A HFE polynomial in GF(2^n)[X] stored with a sparse rep.
 * @param[in]   U   An element of GF(2^n).
 * @return  The degree of Xqn.
 * @remark  Requires to allocate (2*HFEDeg-1)*NB_WORD_GFqn words for Xqn,
 * initialized to zero.
 * @remark  Requirement: F is monic.
 * @remark  Requirement: D>1 (if D==1, X^(2^n)-X == 0 mod (F-U)).
 * @remark  Requirement: F.L must be initialized with initListDifferences_gf2nx.
 * @remark  Constant-time implementation when CONSTANT_TIME!=0.
 */
unsigned int PQCLEAN_GEMSSRED128_CLEAN_frobeniusMap_HFE_gf2nx(gf2nx Xqn,
        complete_sparse_monic_gf2nx F, cst_gf2n U) {
    static_gf2n cst[NB_WORD_GFqn];
    UINT b, mask;
    unsigned int d, i;

    /* Constant term of F-U */
    add_gf2n(cst, F.poly, U);

    /* For i=HFEDegI, we have X^(2^i) mod (F-U) = X^(2^i).
       The first term of degree >= HFEDeg is X^(2^(HFEDegI+1)):
       2^(HFEDegI+1) >= HFEDeg but 2^HFEDegI < HFEDeg.
       So, we begin at the step i=HFEDegI+1 */
    /* Compute X^(2^(HFEDegI+1)) mod (F-U) */

    /* Step 1: compute X^(2^(HFEDegI+1)) */
    d = 2U << HFEDegI;
    /* Xqn is initialized to 0 with calloc, so the multiprecision word is
       initialized to 1 just by setting the first word */
    Xqn[d * NB_WORD_GFqn] = 1;

    /* Step 2: reduction of X^(2^(HFEDegI+1)) modulo (F-U) */
    divsqr_r_HFE_cstdeg_gf2nx(Xqn, d, F, cst);

    for (i = HFEDegI + 1; i < HFEn; ++i) {
        /* Step 1: (X^(2^i) mod (F-U))^2 = X^(2^(i+1)) */
        PQCLEAN_GEMSSRED128_CLEAN_sqr_HFE_gf2nx(Xqn);
        /* Step 2: X^(2^(i+1)) mod (F-U) */
        divsqr_r_HFE_cst_gf2nx(Xqn, F, cst);
    }

    /* (X^(2^n) mod (F-U)) - X */
    Xqn[NB_WORD_GFqn] ^= 1;

    /* Search the degree of X^(2^n) - X mod (F-U) */
    d = 0;
    mask = 0;

    for (i = HFEDeg - 1; i > 0; --i) {
        b = isNot0_gf2n(Xqn + i * NB_WORD_GFqn);
        mask |= b;
        /* We add 1 to d as soon as we exceed all left zero coefficients */
        d += (unsigned int)mask;
    }

    return d;
}
