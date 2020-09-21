#ifndef _GCD_GF2NX_H
#define _GCD_GF2NX_H
#include "arch.h"
#include "gf2nx.h"



unsigned int PQCLEAN_GEMSSRED256_AVX2_gcd_gf2nx(unsigned int *b, gf2nx A, unsigned int da,
        gf2nx B, unsigned int db);


#endif
