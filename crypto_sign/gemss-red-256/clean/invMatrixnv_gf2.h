#ifndef _INVMATRIXNV_GF2_H
#define _INVMATRIXNV_GF2_H
#include "arch.h"
#include "matrix_gf2.h"



void PQCLEAN_GEMSSRED256_CLEAN_invMatrixnv_nocst_gf2(GLnv_gf2 S_inv, GLnv_gf2 S);

void PQCLEAN_GEMSSRED256_CLEAN_invMatrixnv_cst_gf2(GLnv_gf2 S_inv, GLnv_gf2 S);

void PQCLEAN_GEMSSRED256_CLEAN_invMatrixLUnv_gf2(GLnv_gf2 S_inv, cst_Tnv_gf2 L,
        cst_Tnv_gf2 U);


#define invMatrixnv_gf2 PQCLEAN_GEMSSRED256_CLEAN_invMatrixnv_cst_gf2


#endif
