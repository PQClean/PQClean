#include "arch.h"
#include "parameters_HFE.h"
#include "rem_gf2n.h"
#include "simd.h"
#include "sqr_gf2n.h"
#include "tools_gf2n.h"



/***********************************************************************/
/***********************************************************************/
/*************************** sqr without rem ***************************/
/***********************************************************************/
/***********************************************************************/


/**
 * @brief   Squaring in GF(2)[x].
 * @details For each 32-bit block on the input, we use the following strategy:
   Assume we want to insert a null bit between each bit of 0x00000000FFFFFFFF.
   We do as following:
    0x00000000FFFFFFFF (it is already an insertion of a zero 32-bit packed)
    0x0000FFFF0000FFFF (insertion by pack of 16 bits)
    0x00FF00FF00FF00FF (insertion by pack of 8 bits)
    0x0F0F0F0F0F0F0F0F (insertion by pack of 4 bits)
    0x3333333333333333 (insertion by pack of 2 bits)
    0x5555555555555555 (insertion by pack of 1 bit).
 * @param[in]   A   An element of GF(2^n).
 * @param[out]  C   C=A*A in GF(2)[x] (the result is not reduced).
 * @remark  Constant-time implementation.
 */
void PQCLEAN_GEMSSRED128_AVX2_sqr_no_simd_gf2x_ref2(static_gf2x C[NB_WORD_MUL],
        cst_static_gf2n A[NB_WORD_GFqn]) {
    uint64_t Ci;
    unsigned int i;

    C += NB_WORD_MUL - 1;
    for (i = NB_WORD_GFqn - 1; i != (unsigned int)(-1); --i) {
        /* Higher 32 bits of A[i] */
        Ci = ((A[i] >> 32) ^ ((A[i] >> 32) << 16)) & ((uint64_t)0x0000FFFF0000FFFF);
        Ci = (Ci ^ (Ci << 8)) & ((uint64_t)0x00FF00FF00FF00FF);
        Ci = (Ci ^ (Ci << 4)) & ((uint64_t)0x0F0F0F0F0F0F0F0F);
        Ci = (Ci ^ (Ci << 2)) & ((uint64_t)0x3333333333333333);
        Ci = (Ci ^ (Ci << 1)) & ((uint64_t)0x5555555555555555);
        *C = Ci;
        --C;

        /* Lower 32 bits of A[i] */
        Ci = ((A[i] & ((uint64_t)0xFFFFFFFF)) ^ (A[i] << 16))
             & ((uint64_t)0x0000FFFF0000FFFF);
        Ci = (Ci ^ (Ci << 8)) & ((uint64_t)0x00FF00FF00FF00FF);
        Ci = (Ci ^ (Ci << 4)) & ((uint64_t)0x0F0F0F0F0F0F0F0F);
        Ci = (Ci ^ (Ci << 2)) & ((uint64_t)0x3333333333333333);
        Ci = (Ci ^ (Ci << 1)) & ((uint64_t)0x5555555555555555);
        *C = Ci;
        --C;
    }
}


/***********************************************************************/
/***********************************************************************/
/************************* sqr then rem version ************************/
/***********************************************************************/
/***********************************************************************/

#define INIT_Q(size2) uint64_t Q[size2];

void PQCLEAN_GEMSSRED128_AVX2_sqr_then_rem_gf2n(uint64_t res[NB_WORD_GFqn], const uint64_t A[NB_WORD_GFqn]) {
    uint64_t res_mul[NB_WORD_MUL];
    sqr_gf2x(res_mul, A);
    remsqr_gf2n(res, res_mul);
}

void PQCLEAN_GEMSSRED128_AVX2_sqr_nocst_then_rem_gf2n(uint64_t res[NB_WORD_GFqn], const uint64_t A[NB_WORD_GFqn]) {
    uint64_t res_mul[NB_WORD_MUL];
    sqr_nocst_gf2x(res_mul, A);
    remsqr_gf2n(res, res_mul);
}
