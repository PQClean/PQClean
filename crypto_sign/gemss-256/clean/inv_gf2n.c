#include "add_gf2n.h"
#include "inv_gf2n.h"
#include "mul_gf2n.h"
#include "rem_gf2n.h"
#include "sqr_gf2n.h"
#include "tools_gf2n.h"



/**************************************************************************/
/**************************************************************************/
/********************************* Fermat *********************************/
/**************************************************************************/
/**************************************************************************/


/**
 * @brief   Inverse in GF(2^n)*.
 * @details A^(-1) = A^((2^n)-2) = (A^(2^(n-1)-1))^2.
 * @param[in]   A   An element of GF(2^n).
 * @param[out]  res res=A^(-1) in GF(2^n)*, and res=0 when A=0.
 * @remark  Constant-time implementation.
 * @remark  Complexity: (n-2) multiplications and (n-1) squarings in GF(2^n).
 */
void PQCLEAN_GEMSS256_CLEAN_inv_Fermat_gf2n_ref(uint64_t res[NB_WORD_GFqn],
        const uint64_t A[NB_WORD_GFqn]) {
    unsigned int i;

    /* A^2 */
    sqr_gf2n(res, A);
    for (i = 2; i < HFEn; ++i) {
        mul_gf2n(res, res, A);
        sqr_gf2n(res, res);
    }
}


/*******************************************************************/
/*** Computation of the modular inverse with Itoh-Tsujii (ITMIA) ***/
/*******************************************************************/


/**
 * @brief   Inverse in GF(2^n)*.
 * @details A^(-1) = A^((2^n)-2) = (A^(2^(n-1)-1))^2.
 * The ITMIA algorithm is used. The power is computed with an addition chain.
 * Example for n=21:
 * n-1 = 20 = 10100.
 * So, we compute:
 * - A^((2^1)-1) (=A), the power of the power of two of A is 1,
 * - A^((2^2)-1), the power of the power of two of A is 10,
 * - A^((2^4)-1), the power of the power of two of A is 100,
 * - A^((2^5)-1), the power of the power of two of A is 101,
 * - A^((2^10)-1), the power of the power of two of A is 1010,
 * - A^((2^20)-1), the power of the power of two of A is 10100,
 * and finally, (A^((2^20)-1))^2 = A^((2^21)-2) = A^(-1).
 * @param[in]   A   An element of GF(2^n).
 * @param[out]  res res=A^(-1) in GF(2^n)*, and res=0 when A=0.
 * @remark  Constant-time implementation.
 * @remark  Complexity: (Floor(Log2(n-1)) + HW(n-1)-1) multiplications and
 * (n-1) squarings in GF(2^n), with HW the Hamming Weight.
 */
void PQCLEAN_GEMSS256_CLEAN_inv_ITMIA_gf2n_ref(static_gf2n res[NB_WORD_GFqn],
        cst_static_gf2n A[NB_WORD_GFqn]) {
    static_gf2n multi_sqr[NB_WORD_GFqn];
    unsigned int pos, nb_sqr, i, j;

    /* Search the position of the MSB of n-1 */
    pos = 31;
    while (!((HFEn - 1) >> pos)) {
        --pos;
    }
    /* i=pos */
    copy_gf2n(res, A);
    for (i = pos - 1; i != (unsigned int)(-1); --i) {
        nb_sqr = (HFEn - 1) >> (i + 1);
        /* j=0 */
        sqr_gf2n(multi_sqr, res);
        for (j = 1; j < nb_sqr; ++j) {
            sqr_gf2n(multi_sqr, multi_sqr);
        }
        mul_gf2n(res, res, multi_sqr);

        if (((HFEn - 1) >> i)&ONE32) {
            sqr_gf2n(multi_sqr, res);
            mul_gf2n(res, A, multi_sqr);
        }
    }
    sqr_gf2n(res, res);
}
