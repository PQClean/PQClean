#include "conv_gf2nx.h"
#include "inv_gf2n.h"
#include "mul_gf2n.h"
#include "tools_gf2n.h"



/**
 * @brief   Convert a polynomial in GF(2^n)[X] in a monic polynomial, in-place.
 * @param[in]   d   The degree of F.
 * @param[in,out]    F   An element of GF(2^n)[X], becomes monic.
 * @remark  Requires to allocate d+1 elements of GF(2^n) for F.
 * @remark  Constant-time implementation when d is not secret.
 */
void PQCLEAN_GEMSS192_AVX2_convMonic_gf2nx(gf2nx F, unsigned int d) {
    static_gf2n inv[NB_WORD_GFqn];
    unsigned int i;

    F += d * NB_WORD_GFqn;
    /* At this step, F is the pointer on the term X^d of F */
    inv_gf2n(inv, F);
    set1_gf2n(F);

    for (i = d - 1; i != (unsigned int)(-1); --i) {
        F -= NB_WORD_GFqn;
        /* At this step, F is the pointer on the term X^i of F */
        mul_gf2n(F, F, inv);
    }
}


/**
 * @brief   Convert a sparse HFE polynomial in GF(2^n)[X] in its dense
 * representation.
 * @param[in]   F   A HFE polynomial in GF(2^n)[X] stored with a sparse rep.
 * @param[out]  F_dense The dense representation of F in GF(2^n)[X].
 * @remark  Requires to allocate D+1 elements of GF(2^n) for F_dense,
 * initialized to 0.
 * @remark  Requirement: F.L must be initialized with initListDifferences_gf2nx.
 * @remark  Constant-time implementation.
 */
void PQCLEAN_GEMSS192_AVX2_convHFEpolynomialSparseToDense_gf2nx(gf2nx F_dense,
        const complete_sparse_monic_gf2nx F) {
    cst_sparse_monic_gf2nx F_cp = F.poly;
    gf2nx F_dense_cp = F_dense;
    unsigned int i;

    /* i=0: constant of F */
    copy_gf2n(F_dense, F_cp);

    for (i = 1; i < NB_COEFS_HFEPOLY; ++i) {
        F_dense_cp += F.L[i];
        copy_gf2n(F_dense_cp, F_cp + i * NB_WORD_GFqn);
    }

    /* Leading term: 1 */
    F_dense[HFEDeg * NB_WORD_GFqn] = 1;
}
