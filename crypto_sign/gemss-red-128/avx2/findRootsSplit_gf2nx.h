#ifndef _FINDROOTSSPLIT_GF2NX_H
#define _FINDROOTSSPLIT_GF2NX_H
#include "gf2nx.h"
#include "matrix_gf2n.h"



void PQCLEAN_GEMSSRED128_AVX2_findRootsSplit_gf2nx(vec_gf2n roots, gf2nx f,
        unsigned int deg);


#endif
