#ifndef _INVMATRIXN_GF2_H
#define _INVMATRIXN_GF2_H
#include "arch.h"
#include "matrix_gf2.h"



void PQCLEAN_GEMSSRED128_AVX2_invMatrixn_nocst_gf2(GLn_gf2 S_inv, GLn_gf2 S);

void PQCLEAN_GEMSSRED128_AVX2_invMatrixn_cst_gf2(GLn_gf2 S_inv, GLn_gf2 S);

void PQCLEAN_GEMSSRED128_AVX2_invMatrixLUn_gf2(GLn_gf2 S_inv, cst_Tn_gf2 L, cst_Tn_gf2 U);


#define invMatrixn_gf2 PQCLEAN_GEMSSRED128_AVX2_invMatrixn_cst_gf2


#endif
