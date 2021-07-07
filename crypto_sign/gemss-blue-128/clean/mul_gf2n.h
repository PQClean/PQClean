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


void PQCLEAN_GEMSSBLUE128_CLEAN_mul_no_simd_gf2x_ref(uint64_t C[NB_WORD_MUL],
        const uint64_t A[NB_WORD_GFqn],
        const uint64_t B[NB_WORD_GFqn]);


/***********************************************************************/
/***********************************************************************/
/********************** Choice of mul in GF(2)[x] **********************/
/***********************************************************************/
/***********************************************************************/


/* Choice of the multiplication in GF(2)[x] for the arithmetic in GF(2^n). */
/* The gf2x library has the priority when enabled. */
#define mul_gf2x PQCLEAN_GEMSSBLUE128_CLEAN_mul_no_simd_gf2x_ref


/***********************************************************************/
/***********************************************************************/
/************************* mul then rem version ************************/
/***********************************************************************/
/***********************************************************************/


/* Function mul in GF(2^x), then modular reduction */
void PQCLEAN_GEMSSBLUE128_CLEAN_mul_then_rem_gf2n(uint64_t res[NB_WORD_GFqn], const uint64_t A[NB_WORD_GFqn], const uint64_t B[NB_WORD_GFqn]);


/***********************************************************************/
/***********************************************************************/
/********************** Choice of mul in GF(2^n) ***********************/
/***********************************************************************/
/***********************************************************************/


/* Choice the best mul_gf2n */
#define mul_gf2n PQCLEAN_GEMSSBLUE128_CLEAN_mul_then_rem_gf2n



#endif
