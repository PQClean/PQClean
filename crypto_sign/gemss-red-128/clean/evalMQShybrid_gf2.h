#ifndef _EVALMQSHYBRID_GF2_H
#define _EVALMQSHYBRID_GF2_H
#include "arch.h"
#include "config_HFE.h"
#include "matrix_gf2.h"



void PQCLEAN_GEMSSRED128_CLEAN_evalMQShybrid8_uncomp_nocst_gf2_m(vecm_gf2 res,
        cst_vecnv_gf2 x,
        const uint8_t *mq_quo,
        const UINT *mq_rem);



#define evalMQShybrid8_nocst_gf2_m PQCLEAN_GEMSSRED128_CLEAN_evalMQShybrid8_uncomp_nocst_gf2_m


#endif
