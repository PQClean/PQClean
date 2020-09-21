#include "gf2nx.h"
#include "initListDifferences_gf2nx.h"
#include "parameters_HFE.h"
#include "tools_gf2n.h"



/**
 * @brief   Initialize L to the list of the successive differences of the
 * exponents of the monomials of a HFE polynomial, multiplied by NB_WORD_GFqn.
 * @details A HFE polynomial has the exponents 0,1,2,3,4,5,6,8,9,10,12,16,...
 * So, L = NB_WORD_GFqn*{0,1-0,2-1,3-2,4-3,5-4,6-5,8-6,9-8,10-9,12-10,16-12,...}
 *       = NB_WORD_GFqn*{0,1,
 *                         1,
 *                         1,1,
 *                         1,1,2,
 *                         1,1,2,4,
 *                         1,1,2,4,8,
 *                         1,1,2,4,8,16,
 *                         ...}
 * @param[out]    L   A list of NB_COEFS_HFEPOLY unsigned integers.
 * @remark  Requires to allocate NB_COEFS_HFEPOLY unsigned int for L.
 * @remark  This function does not require a constant-time implementation.
 */
void PQCLEAN_GEMSS256_CLEAN_initListDifferences_gf2nx(unsigned int *L) {
    unsigned int i, j, k = 2U;

    L[0] = 0U;
    L[1] = NB_WORD_GFqn;
    for (i = 0; i < HFEDegI; ++i) {
        /* j=0 */
        L[k] = NB_WORD_GFqn;
        ++k;

        /* j=1 to j=i */
        for (j = 0; j < i; ++j) {
            L[k] = NB_WORD_GFqn << j;
            ++k;
        }
    }
}
