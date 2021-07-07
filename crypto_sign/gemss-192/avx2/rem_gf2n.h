#ifndef _REM_GF2N_H
#define _REM_GF2N_H
#include "arch.h"
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
/****************** Macro without SIMD version *************************/
/***********************************************************************/
/***********************************************************************/


/* Automatic choice of REM_GF2N */
/* Assumes KI < K3, which it is for {Blue,Red,}GeMSS192 */
#define REM_GF2N(P,Pol,Q,R)\
    (Q)[0]=((Pol)[4]>>(KI))^((Pol)[5]<<(KI64));\
    (Q)[1]=((Pol)[5]>>(KI))^((Pol)[6]<<(KI64));\
    (Q)[2]=((Pol)[6]>>(KI))^((Pol)[7]<<(KI64));\
    (Q)[3]=((Pol)[7]>>(KI))^((Pol)[8]<<(KI64));\
    (Q)[4]=((Pol)[8]>>(KI));\
    XOR5(P,Pol,Q);\
    (P)[0]^=(Q)[0]<<(K3);\
    (P)[1]^=((Q)[0]>>(K364))^((Q)[1]<<(K3));\
    (P)[2]^=((Q)[1]>>(K364))^((Q)[2]<<(K3));\
    (P)[3]^=((Q)[2]>>(K364))^((Q)[3]<<(K3));\
    (P)[4]^=((Q)[3]>>(K364))^((Q)[4]<<(K3));\
    /* 64-((K364)+(KI)) == ((K3)-(KI)) */\
    (R)=((Q)[3]>>((K364)+(KI)))^((Q)[4]<<((K3)-(KI)));\
    (P)[0]^=(R);\
    (P)[0]^=(R)<<(K3);\
    /* This row is the unique difference with REM288_TRINOMIAL_GF2X */\
    (P)[1]^=(R)>>(K364);\
    (P)[4]&=(MASK_GF2n);


/***********************************************************************/
/***********************************************************************/
/************************ Without SIMD version *************************/
/***********************************************************************/
/***********************************************************************/


void PQCLEAN_GEMSS192_AVX2_rem_gf2n_ref(static_gf2n P[NB_WORD_GFqn],
                                        cst_static_gf2x Pol[NB_WORD_MUL]);
void PQCLEAN_GEMSS192_AVX2_remsqr_gf2n_ref(static_gf2n P[NB_WORD_GFqn],
        cst_static_gf2x Pol[NB_WORD_MUL]);

void PQCLEAN_GEMSS192_AVX2_rem_gf2n_ref2(static_gf2n P[NB_WORD_GFqn],
        cst_static_gf2x Pol[NB_WORD_MUL]);


void PQCLEAN_GEMSS192_AVX2_rem_no_simd_gf2n(static_gf2n P[NB_WORD_GFqn],
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
#define rem_gf2n PQCLEAN_GEMSS192_AVX2_rem_no_simd_gf2n

/* Choice of the best rem when the input is a square */
#define remsqr_gf2n rem_gf2n





#endif
