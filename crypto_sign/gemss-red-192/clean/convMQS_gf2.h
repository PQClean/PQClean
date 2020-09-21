#ifndef _CONVMQS_GF2_H
#define _CONVMQS_GF2_H
#include "MQS_gf2.h"
#include "arch.h"
#include "config_HFE.h"
#include "tools_gf2m.h"
#include "tools_gf2nv.h"



void PQCLEAN_GEMSSRED192_CLEAN_convMQS_one_eq_to_hybrid_rep8_comp_gf2(uint8_t *pk2,
        const uint8_t *pk);

void PQCLEAN_GEMSSRED192_CLEAN_convMQS_one_eq_to_hybrid_rep8_uncomp_gf2(uint8_t *pk2,
        const uint8_t *pk);

#define convMQS_one_eq_to_hybrid_rep8_gf2             PQCLEAN_GEMSSRED192_CLEAN_convMQS_one_eq_to_hybrid_rep8_uncomp_gf2


#endif
