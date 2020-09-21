#include "add_gf2n.h"
#include "chooseRootHFE_gf2nx.h"
#include "findRoots_gf2nx.h"
#include "hash.h"
#include "sort_gf2n.h"
#include "tools_gf2n.h"
#include <stdlib.h>






/**
 * @brief   Find the roots of a monic HFE polynomial, then choose one.
 * @param[in]   F   A monic HFE polynomial in GF(2^n)[X] stored with a sparse
 * representation.
 * @param[in]   U   An element of GF(2^n).
 * @param[out]  root    An element of GF(2^n), one of the root of (F.poly-U).
 * @return  The number of roots of (F.poly-U), without the multiplicity.
 * @remark  Requirement: F.poly is monic.
 * @remark  Requirement: F.L must be initialized with initListDifferences_gf2nx.
 * @remark  A part of the implementation is not in constant-time.
 */
int PQCLEAN_GEMSS128_CLEAN_chooseRootHFE_gf2nx(gf2n root,
        const complete_sparse_monic_gf2nx F,
        cst_gf2n U) {

    UINT hash[SIZE_DIGEST_UINT];

    vec_gf2n roots;
    int l;

    l = PQCLEAN_GEMSS128_CLEAN_findRootsHFE_gf2nx(&roots, F, U);

    if (l == 0) {
        /* Zero root */
        return 0;
    }
    if (l == 1) {
        /* One root */
        copy_gf2n(root, roots);
    } else {
        /* Several roots */

        /* Sort the roots */
        sort_gf2n(roots, l);

        /* Choose a root with a determinist hash */
        HASH((unsigned char *)hash,
             (unsigned char *)U, NB_BYTES_GFqn);
        copy_gf2n(root, roots + (hash[0] % l)*NB_WORD_GFqn);
    }
    free(roots);
    return l;
}
