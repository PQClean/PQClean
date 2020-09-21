#ifndef _SQR_GF2N_H
#define _SQR_GF2N_H

/* Really important include for the definition of
   __TRINOMIAL_GF2N__ and __PENTANOMIAL_GF2N__ */
#include "arch.h"
#include "macro.h"
#include "parameters_HFE.h"
#include "rem_gf2n.h"
#include "tools_gf2n.h"



/***********************************************************************/
/***********************************************************************/
/********************** Choice of sqr in GF(2)[x] **********************/
/***********************************************************************/
/***********************************************************************/


void PQCLEAN_GEMSS192_CLEAN_sqr_no_simd_gf2x_ref2(static_gf2x C[NB_WORD_MUL],
        cst_static_gf2n A[NB_WORD_GFqn]);


/* Choice of the squaring in GF(2)[x] for the arithmetic in GF(2^n). */
#define sqr_gf2x PQCLEAN_GEMSS192_CLEAN_sqr_no_simd_gf2x_ref2
#define sqr_nocst_gf2x sqr_gf2x


/***********************************************************************/
/***********************************************************************/
/************************* sqr then rem version ************************/
/***********************************************************************/
/***********************************************************************/


/* Function sqr in GF(2^x), then modular reduction */
void PQCLEAN_GEMSS192_CLEAN_sqr_then_rem_gf2n(uint64_t res[NB_WORD_GFqn], const uint64_t A[NB_WORD_GFqn]);
void PQCLEAN_GEMSS192_CLEAN_sqr_nocst_then_rem_gf2n(uint64_t res[NB_WORD_GFqn], const uint64_t A[NB_WORD_GFqn]);


/***********************************************************************/
/***********************************************************************/
/********************** Choice of sqr in GF(2^n) ***********************/
/***********************************************************************/
/***********************************************************************/


/* Choice the best sqr */
#define sqr_nocst_gf2n PQCLEAN_GEMSS192_CLEAN_sqr_nocst_then_rem_gf2n
#define sqr_gf2n PQCLEAN_GEMSS192_CLEAN_sqr_then_rem_gf2n



#endif
