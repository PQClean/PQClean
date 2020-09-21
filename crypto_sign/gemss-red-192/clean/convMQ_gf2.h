#ifndef _CONVMQ_GF2_H
#define _CONVMQ_GF2_H
#include "MQ_gf2.h"
#include "arch.h"
#include "tools_gf2m.h"
#include "tools_gf2nv.h"



void PQCLEAN_GEMSSRED192_CLEAN_convMQ_UL_gf2(uint8_t *pk2, const uint8_t *pk);
UINT PQCLEAN_GEMSSRED192_CLEAN_convMQ_uncompressL_gf2(uint64_t *pk2, const uint8_t *pk);
UINT PQCLEAN_GEMSSRED192_CLEAN_convMQ_last_uncompressL_gf2(uint64_t *pk2, const uint8_t *pk);


uint64_t PQCLEAN_GEMSSRED192_CLEAN_convMQ_last_UL_gf2(uint8_t *pk2, const uint8_t *pk);



#endif
