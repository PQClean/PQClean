#ifndef _RAND_GF2N_H
#define _RAND_GF2N_H
#include "randombytes.h"
#include "tools_gf2n.h"



#define rand_gf2n(A) \
    randombytes((unsigned char*)(A),NB_BYTES_GFqn);\
    /* Clean the last word (included the zero padding) */\
    (A)[NB_WORD_GFqn-1]&=MASK_GF2n;




#endif
