#ifndef _SQR_GF2N_H
#define _SQR_GF2N_H
#include "arch.h"
#include "macro.h"
#include "parameters_HFE.h"
#include "rem_gf2n.h"
#include "sqr_gf2x.h"
#include "tools_gf2n.h"

/* Really important include for the definition of
   __TRINOMIAL_GF2N__ and __PENTANOMIAL_GF2N__ */


/***********************************************************************/
/***********************************************************************/
/********************** Choice of sqr in GF(2)[x] **********************/
/***********************************************************************/
/***********************************************************************/


void PQCLEAN_GEMSSBLUE192_AVX2_sqr_no_simd_gf2x_ref2(static_gf2x C[NB_WORD_MUL],
        cst_static_gf2n A[NB_WORD_GFqn]);


/* Here, we define the squaring functions in GF2[x] */
#define sqr_no_simd_gf2x CONCAT(CONCAT_NB_BITS_MMUL_SUP(SQR),_NO_SIMD_GF2X)
#define psqr_gf2x CONCAT(CONCAT_NB_BITS_MMUL_SUP(psqr),_gf2x)
#define psqr_shuffle_gf2x CONCAT(CONCAT_NB_BITS_MMUL_SUP(psqr),_shuffle_gf2x)
#define vpsqr_shuffle_gf2x CONCAT(CONCAT_NB_BITS_MMUL_SUP(vpsqr),_shuffle_gf2x)

#define sqr_pclmul_gf2x CONCAT(CONCAT_NB_BITS_MMUL_SUP(sqr),_pclmul_gf2x)



/* This macro is used only in inv_gf2n.c */
#define SQR_GF2X CONCAT(CONCAT_NB_BITS_MUL_SUP(SQR),_PCLMUL_GF2X)



/* Choice of the squaring in GF(2)[x] for the arithmetic in GF(2^n). */
#define sqr_nocst_gf2x                 CONCAT(CONCAT_NB_BITS_MMUL_SUP(best_sqr_nocst_),_gf2x)

#define sqr_gf2x CONCAT(CONCAT_NB_BITS_MMUL_SUP(best_sqr),_gf2x)


/***********************************************************************/
/***********************************************************************/
/************************* sqr then rem version ************************/
/***********************************************************************/
/***********************************************************************/


/* Function sqr in GF(2^x), then modular reduction */
void PQCLEAN_GEMSSBLUE192_AVX2_sqr_then_rem_gf2n(uint64_t res[NB_WORD_GFqn], const uint64_t A[NB_WORD_GFqn]);
void PQCLEAN_GEMSSBLUE192_AVX2_sqr_nocst_then_rem_gf2n(uint64_t res[NB_WORD_GFqn], const uint64_t A[NB_WORD_GFqn]);


/***********************************************************************/
/***********************************************************************/
/********************** Choice of sqr in GF(2^n) ***********************/
/***********************************************************************/
/***********************************************************************/


/* Choice the best sqr */
#define sqr_nocst_gf2n PQCLEAN_GEMSSBLUE192_AVX2_sqr_nocst_then_rem_gf2n
#define sqr_gf2n PQCLEAN_GEMSSBLUE192_AVX2_sqr_then_rem_gf2n



#endif
