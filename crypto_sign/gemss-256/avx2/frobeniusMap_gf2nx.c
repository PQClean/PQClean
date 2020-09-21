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
unsigned int PQCLEAN_GEMSS256_AVX2_frobeniusMap_HFE_gf2nx(gf2nx Xqn, const
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
        PQCLEAN_GEMSS256_AVX2_sqr_HFE_gf2nx(Xqn);
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
        d += mask;
    }

    return d;
}


/**
 * @brief   Table of the X^(k*2^II) mod F, for Ceil(D/2^II) <= k < D.
 * @details We do not store X^(k*2^II) mod F when k*2^II < D, since
 * X^(k*2^II) mod F = X^(k*2^II) is already reduced.
 * @param[out]  table   A vector of KX (D-1)-degree polynomials in GF(2^n)[X].
 * @param[in]   F   A HFE polynomial in GF(2^n)[X] stored with a sparse rep.
 * @param[in]   cst The constant of F, an element of GF(2^n).
 * @return  The degree of Xqn.
 * @remark  Requires to allocate (KX*HFEDeg+POW_II)*NB_WORD_GFqn words for table.
 * @remark  Requirement: F is monic.
 * @remark  Requirement: F.L must be initialized with initListDifferences_gf2nx.
 * @remark  Constant-time implementation.
 */
static void precompute_table(vec_gf2nx table,
                             const complete_sparse_monic_gf2nx F,
                             cst_gf2n cst) {
    static_gf2n mul_coef[NB_WORD_GFqn];
    gf2n leading_coef;
    vec_gf2nx table_cp;
    unsigned int k, j, i;

    /* First element of the table: X^(KP*(2^II)) mod F. */


    /* First step: X^(KP*(2^II)) mod F = X^(KP*(2^II)-D)*(F - X^D) mod F. */

    /* The first polynomial is initialized to 0. */
    for (i = 0; i < (HFEDeg + POW_II); ++i) {
        set0_gf2n(table + i * NB_WORD_GFqn);
    }

    /* j=POW_II*KP-D, we reduce X^(D+j) mod F. */
    j = POW_II * KP - HFEDeg;

    /* i=0: constant of F */
    table_cp = table + NB_WORD_GFqn * j;
    copy_gf2n(table_cp, cst);
    for (i = 1; i < NB_COEFS_HFEPOLY; ++i) {
        table_cp += F.L[i];
        copy_gf2n(table_cp, F.poly + i * NB_WORD_GFqn);
    }

    /* Second step: we compute X^(KP*(2^II)-D)*(F - X^D) mod F */

    /* We reduce one by one the coefficients leading_coef*X^(D+j) mod F,
       by using X^(D+j) = X^j * X^D = X^j * (F-X^D) mod F. */
    leading_coef = table + (j - 1 + HFEDeg) * NB_WORD_GFqn;
    for (--j; j != (unsigned int)(-1); --j) {
        /* i=0: constant of F */
        table_cp = table + NB_WORD_GFqn * j;
        mul_gf2n(mul_coef, leading_coef, cst);
        add2_gf2n(table_cp, mul_coef);
        for (i = 1; i < NB_COEFS_HFEPOLY; ++i) {
            table_cp += F.L[i];
            mul_gf2n(mul_coef, leading_coef, F.poly + i * NB_WORD_GFqn);
            add2_gf2n(table_cp, mul_coef);
        }
        leading_coef -= NB_WORD_GFqn;
    }


    /* Computation of the other elements of the table: X^(k*(2^II)) mod F.
       X^(k*(2^II)) = (X^((k-1)*(2^II)) mod F) * X^(2^II) mod F. */
    for (k = KP + 1; k < HFEDeg; ++k) {
        /* Update the current polynomial */
        table_cp = table + HFEDeg * NB_WORD_GFqn;

        /* Multiplication of (X^((k-1)*(2^II)) mod F) by X^(2^II) */
        for (j = 0; j < POW_II; ++j) {
            set0_gf2n(table_cp + j * NB_WORD_GFqn);
        }

        table_cp += POW_II * NB_WORD_GFqn;
        for (j = 0; j < HFEDeg; ++j) {
            copy_gf2n(table_cp + j * NB_WORD_GFqn, table + j * NB_WORD_GFqn);
        }
        table_cp -= POW_II * NB_WORD_GFqn;


        /* Update the current polynomial */
        table = table_cp;


        /* Reduction of (X^((k-1)*(2^II)) mod F) * X^(2^II) modulo F */

        /* We reduce one by one the coefficients leading_coef*X^(D+j) mod F,
           by using X^(D+j) = X^j * X^D = X^j * (F-X^D) mod F. */
        leading_coef = table + (POW_II - 1 + HFEDeg) * NB_WORD_GFqn;
        for (j = POW_II - 1; j != (unsigned int)(-1); --j) {
            /* i=0: constant of F */
            table_cp = table + NB_WORD_GFqn * j;
            mul_gf2n(mul_coef, leading_coef, cst);
            add2_gf2n(table_cp, mul_coef);
            for (i = 1; i < NB_COEFS_HFEPOLY; ++i) {
                table_cp += F.L[i];
                mul_gf2n(mul_coef, leading_coef, F.poly + i * NB_WORD_GFqn);
                add2_gf2n(table_cp, mul_coef);
            }
            leading_coef -= NB_WORD_GFqn;
        }
    }
}


/**
 * @brief   Computation of (X^(2^n) - X) mod (F-U).
 * @param[out]  Xqn Xqn = (X^(2^n) - X) mod (F.poly-U) in GF(2^n)[X].
 * @param[in]   F   A HFE polynomial in GF(2^n)[X] stored with a sparse rep.
 * @param[in]   U   An element of GF(2^n).
 * @return  The degree of Xqn.
 * @remark  Requires to allocate (2*HFEDeg-1)*NB_WORD_GFqn words for Xqn.
 * @remark  Requirement: F is monic.
 * @remark  Requirement: F.L must be initialized with initListDifferences_gf2nx.
 * @remark  Constant-time implementation.
 */
unsigned int PQCLEAN_GEMSS256_AVX2_frobeniusMap_multisqr_HFE_gf2nx(gf2nx Xqn, const
        complete_sparse_monic_gf2nx F, cst_gf2n U) {
    static_gf2n cst[NB_WORD_GFqn];
    static_gf2n mul_coef[NB_WORD_GFqn];
    gf2nx Xqn_cp;
    vec_gf2nx table, table_cp;
    gf2nx Xqn_sqr;
    gf2n current_coef;
    UINT b, mask;
    unsigned int d, i, j, k;

    /* Constant of F-U */
    add_gf2n(cst, F.poly, U);

    /* Table of the X^(k*2^II) mod F. */
    table = (UINT *)malloc((KX * HFEDeg + POW_II) * NB_WORD_GFqn * sizeof(UINT));
    precompute_table(table, F, cst);

    /* X^(2^(HFEDegI+II)) = X^( (2^HFEDegI) * (2^II)) */
    /* We take the polynomial from the table */
    table += (((ONE32 << HFEDegI) - KP) * HFEDeg) * NB_WORD_GFqn;
    copy_gf2nx(Xqn, table, HFEDeg, i);
    table -= (((ONE32 << HFEDegI) - KP) * HFEDeg) * NB_WORD_GFqn;

    Xqn_sqr = (UINT *)calloc(HFEDeg * NB_WORD_GFqn, sizeof(UINT));
    for (i = 0; i < ((HFEn - HFEDegI - II) / II); ++i) {
        /* Step 1: Xqn^(2^II) with II squarings */
        /* Xqn_sqr is the list of the coefficients of Xqn at the power 2^II */

        /* j=0, first squaring */
        for (k = 0; k < HFEDeg; ++k) {
            sqr_gf2n(Xqn_sqr + k * NB_WORD_GFqn, Xqn + k * NB_WORD_GFqn);
        }

        /* The other squarings */
        for (j = 1; j < II; ++j) {
            for (k = 0; k < HFEDeg; ++k) {
                sqr_gf2n(Xqn_sqr + k * NB_WORD_GFqn, Xqn_sqr + k * NB_WORD_GFqn);
            }
        }

        /* Step 2: Reduction of Xqn^(2^II) modulo F, by using the table.
           Multiplication of ((X^(k*2^II)) mod F) by the current coefficient. */

        /* j=KP, initialization of the new Xqn */
        current_coef = Xqn_sqr + KP * NB_WORD_GFqn;
        table_cp = table;
        Xqn_cp = Xqn;
        for (k = 0; k < HFEDeg; ++k) {
            mul_gf2n(Xqn_cp, table_cp, current_coef);
            Xqn_cp += NB_WORD_GFqn;
            table_cp += NB_WORD_GFqn;
        }

        for (j = KP + 1; j < HFEDeg; ++j) {
            current_coef += NB_WORD_GFqn;
            Xqn_cp = Xqn;
            for (k = 0; k < HFEDeg; ++k) {
                mul_gf2n(mul_coef, table_cp, current_coef);
                add2_gf2n(Xqn_cp, mul_coef);
                Xqn_cp += NB_WORD_GFqn;
                table_cp += NB_WORD_GFqn;
            }
        }

        /* The coefficients such as X^(k*2^II) mod F = X^(k*2^II). */
        for (j = 0; j < KP; ++j) {
            /* (X^j)^II */
            add2_gf2n(Xqn + j * POW_II * NB_WORD_GFqn, Xqn_sqr + j * NB_WORD_GFqn);
        }
    }

    free(table);
    free(Xqn_sqr);

    for (i = 0; i < (2); ++i) {
        /* Step 1: (X^(2^i) mod (F-U))^2 = X^(2^(i+1)) */
        PQCLEAN_GEMSS256_AVX2_sqr_HFE_gf2nx(Xqn);
        /* Step 2: X^(2^(i+1)) mod (F-U) */
        divsqr_r_HFE_cst_gf2nx(Xqn, F, cst);
    }

    /* X^(2^n) - X */
    Xqn[NB_WORD_GFqn] ^= 1;

    d = 0;
    mask = 0;

    for (i = HFEDeg - 1; i > 0; --i) {
        b = isNot0_gf2n(Xqn + i * NB_WORD_GFqn);
        mask |= b;
        /* We add 1 to d as soon as we exceed all left zero coefficients */
        d += mask;
    }

    return d;
}
