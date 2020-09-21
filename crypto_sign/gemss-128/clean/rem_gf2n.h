#ifndef _REM_GF2N_H
#define _REM_GF2N_H
#include "arch.h"
#include "config_gf2n.h"
#include "gf2x.h"
#include "macro.h"
#include "parameters_HFE.h"
#include "rem5_gf2n.h"
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
/************************ Without SIMD version *************************/
/***********************************************************************/
/***********************************************************************/


void PQCLEAN_GEMSS128_CLEAN_rem_gf2n_ref(static_gf2n P[NB_WORD_GFqn],
        cst_static_gf2x Pol[NB_WORD_MUL]);
void PQCLEAN_GEMSS128_CLEAN_remsqr_gf2n_ref(static_gf2n P[NB_WORD_GFqn],
        cst_static_gf2x Pol[NB_WORD_MUL]);

void PQCLEAN_GEMSS128_CLEAN_rem_gf2n_ref2(static_gf2n P[NB_WORD_GFqn],
        cst_static_gf2x Pol[NB_WORD_MUL]);


/***********************************************************************/
/***********************************************************************/
/******************* Choice of the modular reduction *******************/
/***********************************************************************/
/***********************************************************************/


/* To choose the rem_gf2n function like macro or like function in mul_gf2n.c and
   sqr_gf2n.c. */
/* It is set to 0 to use the reference implementation instead of macros. */
#define REM_MACRO 0

/* Choice of the best rem */
/* Faster */
#define rem_gf2n PQCLEAN_GEMSS128_CLEAN_rem_gf2n_ref2

/* Choice of the best rem when the input is a square */
#define remsqr_gf2n PQCLEAN_GEMSS128_CLEAN_remsqr_gf2n_ref





#endif
