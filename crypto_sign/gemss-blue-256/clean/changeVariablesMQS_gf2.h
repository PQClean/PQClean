#ifndef _CHANGEVARIABLESMQS_GF2_H
#define _CHANGEVARIABLESMQS_GF2_H
#include "MQS_gf2.h"
#include "arch.h"
#include "matrix_gf2.h"



int PQCLEAN_GEMSSBLUE256_CLEAN_changeVariablesMQS_simd_gf2(mqsnv_gf2n MQS, cst_GLnv_gf2 S);


#define changeVariablesMQS_gf2 PQCLEAN_GEMSSBLUE256_CLEAN_changeVariablesMQS_simd_gf2



#endif
