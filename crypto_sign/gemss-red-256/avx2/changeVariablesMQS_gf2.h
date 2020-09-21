#ifndef _CHANGEVARIABLESMQS_GF2_H
#define _CHANGEVARIABLESMQS_GF2_H
#include "MQS_gf2.h"
#include "arch.h"
#include "matrix_gf2.h"



int PQCLEAN_GEMSSRED256_AVX2_changeVariablesMQS_simd_gf2(mqsnv_gf2n MQS, cst_GLnv_gf2 S);
int PQCLEAN_GEMSSRED256_AVX2_changeVariablesMQS64_gf2(mqsnv_gf2n MQS, cst_GLnv_gf2 S);



#define changeVariablesMQS_gf2 PQCLEAN_GEMSSRED256_AVX2_changeVariablesMQS64_gf2



#endif
