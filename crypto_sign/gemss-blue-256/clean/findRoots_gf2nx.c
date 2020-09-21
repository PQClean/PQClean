#include "add_gf2n.h"
#include "add_gf2nx.h"
#include "arch.h"
#include "conv_gf2nx.h"
#include "findRootsSplit_gf2nx.h"
#include "findRoots_gf2nx.h"
#include "frobeniusMap_gf2nx.h"
#include "gcd_gf2nx.h"
#include "inv_gf2n.h"
#include "mul_gf2n.h"
#include "tools_gf2n.h"
#include "tools_gf2nv.h"
#include <stdlib.h>



/**
 * @brief   Computation of the roots of a monic HFE polynomial in GF(2^n)[X].
 * @param[in]   F   A monic HFE polynomial in GF(2^n)[X] stored with a sparse
 * representation.
 * @param[in]   U   An element of GF(2^n).
 * @param[out]  roots   A pointer on the vector of the roots of (F.poly-U), in
 * GF(2^n). This function initializes the memory of *roots when the number of
 * roots is not zero, so you will need to use free() to desallocate.
 * @return  The number of roots of (F.poly-U), without the multiplicity.
 * @remark  Requirement: F.poly is monic.
 * @remark  Requirement: F.L must be initialized with initListDifferences_gf2nx.
 * @remark  A part of the implementation is not in constant-time.
 */
int PQCLEAN_GEMSSBLUE256_CLEAN_findRootsHFE_gf2nx(vec_gf2n *roots,
        const complete_sparse_monic_gf2nx F,
        cst_gf2n U) {

    gf2nx tmp_p, poly, poly2;
    unsigned int i, l, d2;

    d2 = HFEDeg;
    poly = (UINT *)calloc(((HFEDeg << 1) - 1) * NB_WORD_GFqn, sizeof(UINT));
    if (!poly) {
        return ERROR_ALLOC;
    }

    poly2 = (UINT *)calloc((HFEDeg + 1) * NB_WORD_GFqn, sizeof(UINT));
    if (!poly2) {
        free(poly);
        return ERROR_ALLOC;
    }

    /* X^(2^n) - X mod (F-U) */
    l = best_frobeniusMap_HFE_gf2nx(poly, F, U);

    /* Initialize to F */
    PQCLEAN_GEMSSBLUE256_CLEAN_convHFEpolynomialSparseToDense_gf2nx(poly2, F);
    /* Initialize to F-U */
    add2_gf2n(poly2, U);

    /* GCD(F-U, X^(2^n)-X mod (F-U)) */
    l = PQCLEAN_GEMSSBLUE256_CLEAN_gcd_gf2nx(&i, poly2, d2, poly, l);

    if (i) {
        tmp_p = poly;
        poly = poly2;
        poly2 = tmp_p;
    }

    if (!is0_gf2n(poly)) {
        /* The gcd is a constant (!=0) */
        /* Irreducible: 0 root */
        /* l=0; */
        free(poly);
        free(poly2);
        return 0;
    }
    /* poly2 is the gcd */
    /* Here, it becomes monic */
    PQCLEAN_GEMSSBLUE256_CLEAN_convMonic_gf2nx(poly2, l);
    free(poly);


    *roots = (UINT *)malloc(l * NB_WORD_GFqn * sizeof(UINT));
    if (!roots) {
        free(poly2);
        return ERROR_ALLOC;
    }
    PQCLEAN_GEMSSBLUE256_CLEAN_findRootsSplit_gf2nx(*roots, poly2, l);
    free(poly2);

    return (int)l;
}


/**
 * @brief   Computation of the roots of a monic HFE polynomial in GF(2^n)[X],
 * but the process is stopped if the root is not unique, without the
 * multiplicity.
 * @param[in]   F   A monic HFE polynomial in GF(2^n)[X] stored with a sparse
 * representation.
 * @param[in]   U   An element of GF(2^n).
 * @param[out]  root    The root of F.poly-U when it exists and is unique, in
 * GF(2^n).
 * @return  1 is the root of F.poly-U exists and is unique, else 0.
 * @remark  Requirement: F.poly is monic.
 * @remark  Requirement: F.L must be initialized with initListDifferences_gf2nx.
 * @remark  A part of the implementation is not in constant-time.
 */
int PQCLEAN_GEMSSBLUE256_CLEAN_findUniqRootHFE_gf2nx(gf2n root,
        const complete_sparse_monic_gf2nx F,
        cst_gf2n U) {

    static_gf2n inv[NB_WORD_GFqn];
    gf2nx tmp_p, poly, poly2;
    unsigned int i, l, d2;

    d2 = HFEDeg;
    poly = (UINT *)calloc(((HFEDeg << 1) - 1) * NB_WORD_GFqn, sizeof(UINT));
    if (!poly) {
        return ERROR_ALLOC;
    }

    poly2 = (UINT *)calloc((HFEDeg + 1) * NB_WORD_GFqn, sizeof(UINT));
    if (!poly2) {
        free(poly);
        return ERROR_ALLOC;
    }

    /* X^(2^n) - X mod (F-U) */
    l = best_frobeniusMap_HFE_gf2nx(poly, F, U);

    /* Initialize to F */
    PQCLEAN_GEMSSBLUE256_CLEAN_convHFEpolynomialSparseToDense_gf2nx(poly2, F);
    /* Initialize to F-U */
    add2_gf2n(poly2, U);

    /* GCD(F-U, X^(2^n)-X mod (F-U)) */
    l = PQCLEAN_GEMSSBLUE256_CLEAN_gcd_gf2nx(&i, poly2, d2, poly, l);

    if (i) {
        tmp_p = poly;
        poly = poly2;
        poly2 = tmp_p;
    }

    if ((!is0_gf2n(poly)) || (l != 1)) {
        /* if (!is0_gf2n(poly)): the gcd is a constant (!=0) */
        /* Irreducible: 0 root */
        /* l=0; */

        /* else, l roots */

        free(poly);
        free(poly2);
        return 0;
    }
    /* poly2 is the gcd such that poly2 = a*x+b. */
    /* The root is b*a^(-1). */
    inv_gf2n(inv, poly2 + NB_WORD_GFqn);
    mul_gf2n(root, inv, poly2);

    free(poly);
    free(poly2);
    return 1;
}
