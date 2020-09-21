#ifndef _MUL_GF2N_H
#define _MUL_GF2N_H
#include "arch.h"
#include "macro.h"
#include "mul_gf2x.h"
#include "parameters_HFE.h"
#include "rem_gf2n.h"
#include "tools_gf2n.h"

/* Really important include for the definition of
   __TRINOMIAL_GF2N__ and __PENTANOMIAL_GF2N__ */


/***********************************************************************/
/***********************************************************************/
/****** Mul in GF(2)[x] specialized in n-coefficients polynomials ******/
/***********************************************************************/
/***********************************************************************/


void PQCLEAN_GEMSS128_AVX2_mul_no_simd_gf2x_ref(uint64_t C[NB_WORD_MUL],
        const uint64_t A[NB_WORD_GFqn],
        const uint64_t B[NB_WORD_GFqn]);


/***********************************************************************/
/***********************************************************************/
/********************** Choice of mul in GF(2)[x] **********************/
/***********************************************************************/
/***********************************************************************/


#define mul_no_simd_gf2x CONCAT(CONCAT_NB_BITS_MMUL_SUP(mul),_no_simd_gf2x)
#define pmul_gf2x CONCAT(CONCAT_NB_BITS_MMUL_SUP(pmul),_gf2x)

#define pclmul_gf2x CONCAT(CONCAT_NB_BITS_MMUL_SUP(pclmul),_gf2x)

/* This macro is used only in inv_gf2n.c */
#define MUL_GF2X CONCAT(CONCAT_NB_BITS_MUL_SUP(PCLMUL),_GF2X)


/* Choice of the multiplication in GF(2)[x] for the arithmetic in GF(2^n). */
/* The gf2x library has the priority when enabled. */
#define mul_gf2x CONCAT(CONCAT_NB_BITS_MUL_SUP(best_mul),_gf2x)


/***********************************************************************/
/***********************************************************************/
/************************* mul then rem version ************************/
/***********************************************************************/
/***********************************************************************/


/* Function mul in GF(2^x), then modular reduction */
void PQCLEAN_GEMSS128_AVX2_mul_then_rem_gf2n(uint64_t res[NB_WORD_GFqn], const uint64_t A[NB_WORD_GFqn], const uint64_t B[NB_WORD_GFqn]);


/***********************************************************************/
/***********************************************************************/
/********************** Choice of mul in GF(2^n) ***********************/
/***********************************************************************/
/***********************************************************************/


/* Choice the best mul_gf2n */
#define mul_gf2n PQCLEAN_GEMSS128_AVX2_mul_then_rem_gf2n



#endif
