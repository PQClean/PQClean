#ifndef _RANDMATRIX_GF2_H
#define _RANDMATRIX_GF2_H
#include "matrix_gf2.h"
#include "parameters_HFE.h"



void PQCLEAN_GEMSS128_CLEAN_cleanMatrix_gf2_n(Mn_gf2 S);
void PQCLEAN_GEMSS128_CLEAN_cleanMatrix_gf2_nv(Mnv_gf2 S);


void PQCLEAN_GEMSS128_CLEAN_randMatrix_gf2_n(Mn_gf2 S);
void PQCLEAN_GEMSS128_CLEAN_randMatrix_gf2_nv(Mnv_gf2 S);

void PQCLEAN_GEMSS128_CLEAN_cleanLowerMatrixn(Tn_gf2 L);
void PQCLEAN_GEMSS128_CLEAN_genLowerMatrixn(Tn_gf2 L);
void PQCLEAN_GEMSS128_CLEAN_cleanLowerMatrixnv(Tnv_gf2 L);
void PQCLEAN_GEMSS128_CLEAN_genLowerMatrixnv(Tnv_gf2 L);

#define GEN_LU_Tn(L,U) \
    PQCLEAN_GEMSS128_CLEAN_genLowerMatrixn(L);\
    /* Generate the transpose of U */\
    PQCLEAN_GEMSS128_CLEAN_genLowerMatrixn(U);

#define GEN_LU_Tnv(L,U) \
    PQCLEAN_GEMSS128_CLEAN_genLowerMatrixnv(L);\
    /* Generate the transpose of U */\
    PQCLEAN_GEMSS128_CLEAN_genLowerMatrixnv(U);

void PQCLEAN_GEMSS128_CLEAN_randInvMatrix_gf2_n(GLn_gf2 S, Mn_gf2 S_cp);
void PQCLEAN_GEMSS128_CLEAN_randInvMatrix_gf2_nv(GLnv_gf2 S, Mnv_gf2 S_cp);

void PQCLEAN_GEMSS128_CLEAN_mulMatricesLU_gf2_n(Mn_gf2 S, cst_Tn_gf2 L, cst_Tn_gf2 U);
void PQCLEAN_GEMSS128_CLEAN_mulMatricesLU_gf2_nv(Mnv_gf2 S, cst_Tnv_gf2 L,
        cst_Tnv_gf2 U);


#define randInvMatrixLU_gf2_n(S,L,U) \
    GEN_LU_Tn(L,U);\
    PQCLEAN_GEMSS128_CLEAN_mulMatricesLU_gf2_n(S,L,U);

#define randInvMatrixLU_gf2_nv(S,L,U) \
    GEN_LU_Tnv(L,U);\
    PQCLEAN_GEMSS128_CLEAN_mulMatricesLU_gf2_nv(S,L,U);


#endif
