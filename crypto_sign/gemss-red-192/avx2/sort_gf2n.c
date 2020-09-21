#include "add_gf2n.h"
#include "sort_gf2n.h"
#include "tools_gf2n.h"



/**
 * @brief   Sort in ascending order the elements of a vector in GF(2^n).
 * @details Quick sort.
 * The elements of GF(2^n) are seen as unsigned integers.
 * @param[in,out]   tab_start   A pointer on the first element of the vector.
 * @param[in,out]   tab_last    A pointer on the last element of the vector.
 * @remark  This implementation is not in constant-time.
 */
void PQCLEAN_GEMSSRED192_AVX2_quickSort_nocst_gf2n(vec_gf2n tab_start, vec_gf2n tab_last) {
    vec_gf2n pivot, end;

    pivot = tab_start;
    end = tab_last;

    while (1) {
        while (cmp_lt_nocst_gf2n(tab_start, pivot)) {
            tab_start += NB_WORD_GFqn;
        }
        while (cmp_gt_nocst_gf2n(tab_last, pivot)) {
            tab_last -= NB_WORD_GFqn;
        }

        if (tab_start < tab_last) {
            swap_gf2n(tab_start, tab_last);
        } else {
            break;
        }
    }

    /* Particular case, pivot is the beginning of the table.
       Else, we must replace pivot by the beginning of the table. */
    if (pivot < tab_last) {
        PQCLEAN_GEMSSRED192_AVX2_quickSort_nocst_gf2n(pivot, tab_last);
    }
    tab_last += NB_WORD_GFqn;
    if (tab_last < end) {
        PQCLEAN_GEMSSRED192_AVX2_quickSort_nocst_gf2n(tab_last, end);
    }
}


/**
 * @brief   Sort in ascending order of a vector in GF(2^n), in-place.
 * @details Selection sort.
 * The elements of GF(2^n) are seen as unsigned integers.
 * @param[in,out]   tab A vector of l elements of GF(2^n). Will be sorted.
 * @param[in]   l   The length of tab.
 * @remark  This implementation is not in constant-time.
 * @remark  Complexity: l(l-1)/2 comparisons and at most l(l-1)/2 swaps.
 */
void PQCLEAN_GEMSSRED192_AVX2_selectionSort_nocst_gf2n(vec_gf2n tab, int l) {
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
 * @details The fastest constant-time sort of this library.
 * The elements of GF(2^n) are seen as unsigned integers.
 * @param[in,out]   tab A vector of l elements of GF(2^n). Will be sorted.
 * @param[in]   l   The length of tab.
 * @remark  Requirement: l>1.
 * @remark  Constant-time implementation when l is not secret.
 */
void PQCLEAN_GEMSSRED192_AVX2_fast_sort_gf2n(vec_gf2n tab, int l) {
    static_gf2n tmp[NB_WORD_GFqn], sum[NB_WORD_GFqn], prod[NB_WORD_GFqn];
    vec_gf2n tab_i, tab_ipa;
    UINT mask;
    /* pow2_prev,pa,pb,pc are powers of two */
    unsigned int i, j, quo, rem, pow2_prev, pa, pb, pc;

    /* The power of 2 before l, which is 1<<position(MSB(l-1)). */
    pow2_prev = 2;
    while (pow2_prev < (unsigned int)l) {
        pow2_prev <<= 1;
    }
    pow2_prev >>= 1;

    for (pa = pow2_prev; pa > 1; pa >>= 1) {
        /* Number of complete blocks */
        quo = l / (pa << 1);
        /* Size of the remainder block */
        rem = l - (pa << 1) * quo;
        /* Impact on the sort */
        rem = (rem <= pa) ? 0 : (rem - pa);

        tab_i = tab;
        tab_ipa = tab + pa * NB_WORD_GFqn;

        for (i = 0; i < quo; ++i) {
            for (j = 0; j < pa; ++j) {
                CMP_AND_SWAP_CST_TIME(tab_i, tab_ipa);
                tab_i += NB_WORD_GFqn;
                tab_ipa += NB_WORD_GFqn;
            }
            tab_i += pa * NB_WORD_GFqn;
            tab_ipa += pa * NB_WORD_GFqn;
        }

        for (j = 0; j < rem; ++j) {
            CMP_AND_SWAP_CST_TIME(tab_i, tab_ipa);
            tab_i += NB_WORD_GFqn;
            tab_ipa += NB_WORD_GFqn;
        }

        i = 0;
        for (pb = pow2_prev; pb > pa; pb >>= 1) {
            /* l>1 implies pb<l. */
            for (; i < (l - pb); ++i) {
                if (!(i & pa)) {
                    tab_ipa = tab + (i + pa) * NB_WORD_GFqn;
                    copy_gf2n(tmp, tab_ipa);
                    for (pc = pb; pc > pa; pc >>= 1) {
                        tab_i = tab + (i + pc) * NB_WORD_GFqn;
                        CMP_AND_SWAP_CST_TIME(tmp, tab_i);
                    }
                    copy_gf2n(tab_ipa, tmp);
                }
            }
        }
    }

    /* pa=1 */
    tab_i = tab;
    tab_ipa = tab + NB_WORD_GFqn;
    for (i = 0; i < (unsigned int)(l - 1); i += 2) {
        CMP_AND_SWAP_CST_TIME(tab_i, tab_ipa);
        tab_i += NB_WORD_GFqn << 1;
        tab_ipa += NB_WORD_GFqn << 1;
    }

    i = 0;
    tab_ipa = tab + NB_WORD_GFqn;
    for (pb = pow2_prev; pb > 1; pb >>= 1) {
        /* l>1 implies pb<l. */
        for (; i < (l - pb); i += 2) {
            copy_gf2n(tmp, tab_ipa);
            for (pc = pb; pc > 1; pc >>= 1) {
                tab_i = tab + (i + pc) * NB_WORD_GFqn;
                CMP_AND_SWAP_CST_TIME(tmp, tab_i);
            }
            copy_gf2n(tab_ipa, tmp);
            tab_ipa += NB_WORD_GFqn << 1;
        }
    }
}


/**
 * @brief   Sort in ascending order of a vector in GF(2^n), in-place.
 * @details Constant-time selection sort.
 * The elements of GF(2^n) are seen as unsigned integers.
 * @param[in,out]   tab A vector of l elements of GF(2^n). Will be sorted.
 * @param[in]   l   The length of tab.
 * @remark  Constant-time implementation when l is not secret.
 * @remark  Complexity: l(l-1)/2 steps.
 */
void PQCLEAN_GEMSSRED192_AVX2_selectionSort_gf2n(vec_gf2n tab, int l) {
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
