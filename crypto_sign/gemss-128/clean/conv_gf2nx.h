#ifndef _CONV_GF2NX_H
#define _CONV_GF2NX_H
#include "gf2nx.h"



void PQCLEAN_GEMSS128_CLEAN_convMonic_gf2nx(gf2nx F, unsigned int d);


void PQCLEAN_GEMSS128_CLEAN_convHFEpolynomialSparseToDense_gf2nx(gf2nx F_dense,
        complete_sparse_monic_gf2nx F);


#endif
