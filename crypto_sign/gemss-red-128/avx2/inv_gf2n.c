#include "ITMIA_addchains.h"
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
void PQCLEAN_GEMSSRED128_AVX2_inv_Fermat_gf2n_ref(uint64_t res[NB_WORD_GFqn],
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
void PQCLEAN_GEMSSRED128_AVX2_inv_ITMIA_gf2n_ref(static_gf2n res[NB_WORD_GFqn],
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


#define rem_function REM_GF2N(r,r,Q,R)


#define INIT_REGISTERS __m128i r_128,r2_128,r_128_copy,r2_128_copy,\
    a_128,a2_128,ra,ra2,sum,res1,res2;
#define PINIT_GF2N_(r_128,r2_128,r3_128,r4_128,r5_128,r) \
    PINIT_GF2N(r_128,r2_128,r)
#define SQR_GF2X_(r,r_128,r2_128,r3_128,r4_128,r5_128,ra) \
    SQR_GF2X(r,r_128,r2_128,ra)
#define MUL_GF2X_(r,r_128,r2_128,r3_128,r4_128,r5_128,a_128,a2_128,a3_128,\
                  a4_128,a5_128,ra,ra2,sum,res1,res2) \
MUL_GF2X(r,r_128,r2_128,a_128,a2_128,ra,ra2,sum,res1,res2)


#define SQR_GF2N_(r,r_128,r2_128,r3_128,r4_128,r5_128,ra,rem_function) \
    PINIT_GF2N_(r_128,r2_128,r3_128,r4_128,r5_128,r);\
    SQR_GF2X_(r,r_128,r2_128,r3_128,r4_128,r5_128,ra);\
    rem_function;

#define MUL_GF2N_(r,r_128,r2_128,r3_128,r4_128,r5_128,a_128,a2_128,a3_128,\
                  a4_128,a5_128,ra,ra2,sum,res1,res2,rem_function) \
PINIT_GF2N_(r_128,r2_128,r3_128,r4_128,r5_128,r);\
MUL_GF2X_(r,r_128,r2_128,r3_128,r4_128,r5_128,a_128,a2_128,a3_128,a4_128,\
          a5_128,ra,ra2,sum,res1,res2);\
rem_function;

/* Take in input (HFEn-1)>>i (i the current bit of (HFEn-1)) */
#define EVEN_CASE(val) \
    /* res=res * res^(2^((HFEn-1)>>i)) */\
    /* res^(2^((HFEn-1)>>i)) */\
    PINIT_GF2N_(r_128_copy,r2_128_copy,r3_128_copy,r4_128_copy,r5_128_copy,r);\
    for(j=0;j<(val);++j)\
    {\
        SQR_GF2N_(r,r_128,r2_128,r3_128,r4_128,r5_128,ra,rem_function);\
    }\
    /* multiply by the old res (res=res * res^(2^((HFEn-1)>>i))) */\
    MUL_GF2N_(r,r_128,r2_128,r3_128,r4_128,r5_128,r_128_copy,r2_128_copy,\
              r3_128_copy,r4_128_copy,r5_128_copy,ra,ra2,sum,res1,res2,rem_function);

/* Take in input (HFEn-1)>>i (i the current bit of (HFEn-1)) */
#define ODD_CASE(val) \
    EVEN_CASE(val);\
    /* res = res^2 *A */\
    SQR_GF2N_(r,r_128,r2_128,r3_128,r4_128,r5_128,ra,rem_function);\
    MUL_GF2N_(r,r_128,r2_128,r3_128,r4_128,r5_128,a_128,a2_128,a3_128,a4_128,\
              a5_128,ra,ra2,sum,res1,res2,rem_function);

/* Part of the odd case */
/* Compute A^((2^3)-1 from A^((2^2)-1) */
#define ODD_INIT \
    SQR_GF2N_(r,r_128,r2_128,r3_128,r4_128,r5_128,ra,rem_function);\
    MUL_GF2N_(r,r_128,r2_128,r3_128,r4_128,r5_128,a_128,a2_128,a3_128,a4_128,\
              a5_128,ra,ra2,sum,res1,res2,rem_function);\



#define ITOH_ ITOH

/**
 * @brief   Inverse in GF(2^n)*.
 * @details A^(-1) = A^((2^n)-2) = (A^(2^(n-1)-1))^2.
 * The ITMIA algorithm is used. The power is computed with an addition chain.
 * The latter is defined in ITMIA_addchains.h.
 * @param[in]   A   An element of GF(2^n).
 * @param[out]  res res=A^(-1) in GF(2^n)*, and res=0 when A=0.
 * @remark  Constant-time implementation.
 * @remark  Complexity: O(Floor(Log2(n-1))) multiplications and (n-1) squarings
 * in GF(2^n). However, the complexity of the squarings can be improved with
 * multi-squaring tables.
 */
// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSRED128_AVX2_inv_ITMIA_gf2n(static_gf2n res[NB_WORD_GFqn],
        cst_static_gf2n A[NB_WORD_GFqn]) {
    INIT_REGISTERS;
    uint64_t r[NB_WORD_MUL];
    uint64_t R;

    uint64_t Q[NB_WORD_GFqn];

    unsigned int j;

    PINIT_GF2N_(a_128, a2_128, a3_128, a4_128, a5_128, A);

    /* The MSB of (HFEn-1) is done here : A^(2^1 -1) = A */
    PINIT_GF2N_(r_128, r2_128, r3_128, r4_128, r5_128, A);

    /* The second MSB of (HFEn-1) is done here */
    /* Compute A^(2^2 -1) = A^3 */
    SQR_GF2X_(r, r_128, r2_128, r3_128, r4_128, r5_128, ra);
    rem_function;
    MUL_GF2N_(r, r_128, r2_128, r3_128, r4_128, r5_128, a_128, a2_128, a3_128, a4_128,
              a5_128, ra, ra2, sum, res1, res2, rem_function);

    /* For the others bits of (HFEn-1): */
    /* bit Log_2(n-1)-2 to bit 0 of (n-1) */
    ITOH_;

    /* res=A^(2^(HFEn-1) -1) here */
    /* (2^(HFEn-1) -1)*2 = 2^k -2 */
    /* Square */
    SQR_GF2N_(r, r_128, r2_128, r3_128, r4_128, r5_128, ra, rem_function);
    copy_gf2n(res, r);
}
