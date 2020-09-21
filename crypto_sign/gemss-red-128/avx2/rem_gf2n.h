#ifndef _REM_GF2N_H
#define _REM_GF2N_H
#include "arch.h"
#include "gf2x.h"
#include "macro.h"
#include "parameters_HFE.h"
#include "rem5_gf2n.h"
#include "rem_gf2x.h"
#include "tools_gf2n.h"



/* Compute the modular reduction */


#define KI (HFEn&63U)
#define KI64 (64U-KI)

#define K3mod64 K3

#define K364 (64U-K3mod64)
/* Only to remove the warnings */
#define K364mod64 (K364&63U)


/***********************************************************************/
/***********************************************************************/
/****************** Macro without SIMD version *************************/
/***********************************************************************/
/***********************************************************************/


/* Automatic choice of REM_GF2N */

/* Example: REM192_TRINOMIAL_GF2X(P,Pol,K3mod64,KI,KI64,K364mod64,
                                  Q,R,MASK_GF2n) */
#define REM_GF2N(P,Pol,Q,R) CONCAT(CONCAT_NB_BITS_MMUL_SUP(REM),_TRINOMIAL_GF2X(P,Pol,K3mod64,KI,KI64,K364mod64,Q,R,MASK_GF2n))


/***********************************************************************/
/***********************************************************************/
/************************ Without SIMD version *************************/
/***********************************************************************/
/***********************************************************************/


void PQCLEAN_GEMSSRED128_AVX2_rem_gf2n_ref(static_gf2n P[NB_WORD_GFqn],
        cst_static_gf2x Pol[NB_WORD_MUL]);
void PQCLEAN_GEMSSRED128_AVX2_remsqr_gf2n_ref(static_gf2n P[NB_WORD_GFqn],
        cst_static_gf2x Pol[NB_WORD_MUL]);

void PQCLEAN_GEMSSRED128_AVX2_rem_gf2n_ref2(static_gf2n P[NB_WORD_GFqn],
        cst_static_gf2x Pol[NB_WORD_MUL]);


void PQCLEAN_GEMSSRED128_AVX2_rem_no_simd_gf2n(static_gf2n P[NB_WORD_GFqn],
        cst_static_gf2x Pol[NB_WORD_MUL]);



/***********************************************************************/
/***********************************************************************/
/******************* Choice of the modular reduction *******************/
/***********************************************************************/
/***********************************************************************/


/* To choose the rem_gf2n function like macro or like function in mul_gf2n.c and
   sqr_gf2n.c. */
/* The user can set to 0 ou 1 this macro. */
#define REM_MACRO 1

/* Choice of the best rem */
#define rem_gf2n PQCLEAN_GEMSSRED128_AVX2_rem_no_simd_gf2n

/* Choice of the best rem when the input is a square */
#define remsqr_gf2n rem_gf2n





#endif
