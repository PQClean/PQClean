#ifndef _ADD_GF2N_H
#define _ADD_GF2N_H
#include "add_gf2x.h"
#include "arch.h"
#include "gf2x.h"
#include "macro.h"
#include "tools_gf2n.h"



/* The following macros are used only in this file. */

/* Addition in GF(2^n). */
#define ADD_GF2N CONCAT(CONCAT_NB_WORD_GFqn_SUP(ADD),_GF2X)
#define ADD2_GF2N CONCAT(CONCAT_NB_WORD_GFqn_SUP(ADD),_2_GF2X)


/* Addition in GF(2)[x] for (2n-1)-coefficients polynomials. */
#define ADD_PRODUCT_GF2N CONCAT(CONCAT_NB_WORD_MMUL(ADD),_GF2X)
#define ADD2_PRODUCT_GF2N CONCAT(CONCAT_NB_WORD_MMUL(ADD),_2_GF2X)


void PQCLEAN_GEMSSBLUE192_AVX2_add_gf2n_ref(gf2n c, cst_gf2n a, cst_gf2n b);
void PQCLEAN_GEMSSBLUE192_AVX2_add2_gf2n_ref(gf2n c, cst_gf2n a);
void PQCLEAN_GEMSSBLUE192_AVX2_add_product_gf2n_ref(gf2x c, cst_gf2x a, cst_gf2x b);
void PQCLEAN_GEMSSBLUE192_AVX2_add2_product_gf2n_ref(gf2x c, cst_gf2x a);



/* The addition function is inlined. */
#define add_gf2n ADD_GF2N
#define add2_gf2n ADD2_GF2N
#define add_product_gf2n ADD_PRODUCT_GF2N
#define add2_product_gf2n ADD2_PRODUCT_GF2N




#endif
