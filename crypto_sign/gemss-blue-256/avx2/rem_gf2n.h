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
#define REM_GF2N(P,Pol,Q,R) \
    (Q)[0]=((Pol)[5]>>(KI))^((Pol)[6]<<(KI64));\
    (Q)[1]=((Pol)[6]>>(KI))^((Pol)[7]<<(KI64));\
    (Q)[2]=((Pol)[7]>>(KI))^((Pol)[8]<<(KI64));\
    (Q)[3]=((Pol)[8]>>(KI))^((Pol)[9]<<(KI64));\
    (Q)[4]=((Pol)[9]>>(KI))^((Pol)[10]<<(KI64));\
    (Q)[5]=((Pol)[10]>>(KI))^((Pol)[11]<<(KI64));\
    /* 64-((k364)+(KI)) == ((K3mod64)-(KI)) */\
    (R)=((Q)[4]>>((K364)+(KI)))^((Q)[5]<<((K3mod64)-(KI)));\
    (Q)[0]^=(R);\
    XOR6(P,Pol,Q);\
    (P)[0]^=(Q)[0]<<(K3mod64);\
    (P)[1]^=((Q)[0]>>(K364))^((Q)[1]<<(K3mod64));\
    (P)[2]^=((Q)[1]>>(K364))^((Q)[2]<<(K3mod64));\
    (P)[3]^=((Q)[2]>>(K364))^((Q)[3]<<(K3mod64));\
    (P)[4]^=((Q)[3]>>(K364))^((Q)[4]<<(K3mod64));\
    (P)[5]^=((Q)[4]>>(K364));\
    (P)[5]&=(MASK_GF2n);


/***********************************************************************/
/***********************************************************************/
/************************ Without SIMD version *************************/
/***********************************************************************/
/***********************************************************************/


void PQCLEAN_GEMSSBLUE256_AVX2_rem_gf2n_ref(static_gf2n P[NB_WORD_GFqn],
        cst_static_gf2x Pol[NB_WORD_MUL]);
void PQCLEAN_GEMSSBLUE256_AVX2_remsqr_gf2n_ref(static_gf2n P[NB_WORD_GFqn],
        cst_static_gf2x Pol[NB_WORD_MUL]);

void PQCLEAN_GEMSSBLUE256_AVX2_rem_gf2n_ref2(static_gf2n P[NB_WORD_GFqn],
        cst_static_gf2x Pol[NB_WORD_MUL]);


void PQCLEAN_GEMSSBLUE256_AVX2_rem_no_simd_gf2n(static_gf2n P[NB_WORD_GFqn],
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
#define rem_gf2n PQCLEAN_GEMSSBLUE256_AVX2_rem_no_simd_gf2n

/* Choice of the best rem when the input is a square */
#define remsqr_gf2n rem_gf2n





#endif
