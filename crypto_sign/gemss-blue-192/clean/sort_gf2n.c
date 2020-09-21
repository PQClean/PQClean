#include "add_gf2n.h"
#include "sort_gf2n.h"
#include "tools_gf2n.h"



/**
 * @brief   Sort in ascending order of a vector in GF(2^n), in-place.
 * @details Selection sort.
 * The elements of GF(2^n) are seen as unsigned integers.
 * @param[in,out]   tab A vector of l elements of GF(2^n). Will be sorted.
 * @param[in]   l   The length of tab.
 * @remark  This implementation is not in constant-time.
 * @remark  Complexity: l(l-1)/2 comparisons and at most l(l-1)/2 swaps.
 */
void PQCLEAN_GEMSSBLUE192_CLEAN_selectionSort_nocst_gf2n(vec_gf2n tab, int l) {
    vec_gf2n tab_lim, tab_j;

    tab_lim = tab + NB_WORD_GFqn * (l - 1);

    for (; tab < tab_lim; tab += NB_WORD_GFqn) {
        for (tab_j = tab + NB_WORD_GFqn; tab_j <= tab_lim; tab_j += NB_WORD_GFqn) {
            /* If *tab_j < *tab, we swap both. */
            if (cmp_lt_nocst_gf2n(tab_j, tab)) {
                swap_gf2n(tab_j, tab);
            }
        }
    }
}


#define CMP_AND_SWAP_CST_TIME(tab_j1,tab_j) \
    /* If *tab_j < *tab_j1, we swap both. */\
    mask=-cmp_lt_gf2n(tab_j,tab_j1);\
    \
    add_gf2n(sum,tab_j1,tab_j);\
    set0_gf2n(prod);\
    xorLoadMask1_gf2n(prod,sum,mask);\
    add2_gf2n(tab_j,prod);\
    add2_gf2n(tab_j1,prod);


/**
 * @brief   Sort in ascending order of a vector in GF(2^n), in-place.
 * @details Constant-time selection sort.
 * The elements of GF(2^n) are seen as unsigned integers.
 * @param[in,out]   tab A vector of l elements of GF(2^n). Will be sorted.
 * @param[in]   l   The length of tab.
 * @remark  Constant-time implementation when l is not secret.
 * @remark  Complexity: l(l-1)/2 steps.
 */
void PQCLEAN_GEMSSBLUE192_CLEAN_selectionSort_gf2n(vec_gf2n tab, int l) {
    static_gf2n sum[NB_WORD_GFqn], prod[NB_WORD_GFqn];
    vec_gf2n tab_lim, tab_j;
    UINT mask;

    tab_lim = tab + NB_WORD_GFqn * (l - 1);

    for (; tab < tab_lim; tab += NB_WORD_GFqn) {
        for (tab_j = tab + NB_WORD_GFqn; tab_j <= tab_lim; tab_j += NB_WORD_GFqn) {
            CMP_AND_SWAP_CST_TIME(tab, tab_j);
        }
    }
}
