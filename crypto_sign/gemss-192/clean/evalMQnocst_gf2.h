#ifndef _EVALMQNOCST_GF2_H
#define _EVALMQNOCST_GF2_H
#include "arch.h"
#include "matrix_gf2.h"
#include "tools_gf2nv.h"



UINT PQCLEAN_GEMSS192_CLEAN_evalMQnocst_unrolled_no_simd_gf2(const UINT *m,
        const UINT *mq);



#define evalMQnocst_gf2 PQCLEAN_GEMSS192_CLEAN_evalMQnocst_unrolled_no_simd_gf2



#endif
