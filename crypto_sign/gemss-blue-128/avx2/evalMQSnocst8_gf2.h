#ifndef _EVALMQSNOCST8_GF2_H
#define _EVALMQSNOCST8_GF2_H
#include "MQS_gf2.h"
#include "arch.h"
#include "matrix_gf2.h"
#include "tools_gf2m.h"



void PQCLEAN_GEMSSBLUE128_AVX2_evalMQSnocst8_unrolled_gf2(vecm_gf2 c, cst_vecnv_gf2 m,
        cst_mqsnv8_gf2m pk);


/* Choose the best evaluation */
#define evalMQSnocst8_gf2 PQCLEAN_GEMSSBLUE128_AVX2_evalMQSnocst8_unrolled_gf2


#endif
