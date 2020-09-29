#include "arch.h"
#include "mul_gf2n.h"
#include "parameters_HFE.h"
#include "rem_gf2n.h"
#include "simd.h"
#include "tools_gf2n.h"



/**
 * @brief   Multiplication in GF(2)[x].
 * @param[in]   A   An element of GF(2^n).
 * @param[in]   B   An element of GF(2^n).
 * @param[out]  C   C=A*B in GF(2)[x] (the result is not reduced).
 * @remark  Constant-time implementation.
 */
void PQCLEAN_GEMSSRED192_CLEAN_mul_no_simd_gf2x_ref(uint64_t C[NB_WORD_MUL],
        const uint64_t A[NB_WORD_GFqn],
        const uint64_t B[NB_WORD_GFqn]) {
    uint64_t mask_B, tmp_A;
    unsigned int i, j, k, jc;

    for (i = 0; i < NB_WORD_MUL; ++i) {
        C[i] = 0;
    }

    for (i = 0; i < HFEnq; ++i) {
        /* j=0 */
        mask_B = (1 + ~((*B)&ONE64));
        for (k = 0; k < NB_WORD_GFqn; ++k) {
            C[k] ^= A[k] & mask_B;
        }

        /* The last 64-bit block BL of A contains HFEnr bits.
           So, there is not overflow for BL<<j while j<=(64-HFEnr). */
        for (j = 1; j <= (64 - HFEnr); ++j) {
            jc = 64 - j;
            mask_B = (1 + ~(((*B) >> j)&ONE64));
            /* k=0 */
            tmp_A = (*A)&mask_B;
            C[0] ^= tmp_A << j;
            for (k = 1; k < NB_WORD_GFqn; ++k) {
                C[k] ^= tmp_A >> jc;
                tmp_A = A[k] & mask_B;
                C[k] ^= tmp_A << j;
            }
        }
        for (; j < 64; ++j) {
            jc = 64 - j;
            mask_B = (1 + ~(((*B) >> j)&ONE64));
            /* k=0 */
            tmp_A = (*A)&mask_B;
            C[0] ^= tmp_A << j;
            for (k = 1; k < NB_WORD_GFqn; ++k) {
                C[k] ^= tmp_A >> jc;
                tmp_A = A[k] & mask_B;
                C[k] ^= tmp_A << j;
            }
            /* "overflow" from the last 64-bit block of A */
            C[k] ^= tmp_A >> jc;
        }

        ++B;
        ++C;
    }

    /* j=0 */
    mask_B = (1 + ~((*B)&ONE64));
    for (k = 0; k < NB_WORD_GFqn; ++k) {
        C[k] ^= A[k] & mask_B;
    }

    /* The last 64-bit block BL of A contains HFEnr bits.
       So, there is not overflow for BL<<j while j<=(64-HFEnr). */
    for (j = 1; j < HFEnr; ++j) {
        jc = 64 - j;
        mask_B = (1 + ~(((*B) >> j)&ONE64));
        /* k=0 */
        tmp_A = (*A)&mask_B;
        C[0] ^= tmp_A << j;
        for (k = 1; k < NB_WORD_GFqn; ++k) {
            C[k] ^= tmp_A >> jc;
            tmp_A = A[k] & mask_B;
            C[k] ^= tmp_A << j;
        }
    }

}


/***********************************************************************/
/***********************************************************************/
/************************* mul then rem version ************************/
/***********************************************************************/
/***********************************************************************/

#define INIT_Q(size2) uint64_t Q[size2];


void PQCLEAN_GEMSSRED192_CLEAN_mul_then_rem_gf2n(uint64_t res[NB_WORD_GFqn], const uint64_t A[NB_WORD_GFqn], const uint64_t B[NB_WORD_GFqn]) {
    uint64_t res_mul[NB_WORD_MUL];
    mul_gf2x(res_mul, A, B);
    rem_gf2n(res, res_mul);
}
