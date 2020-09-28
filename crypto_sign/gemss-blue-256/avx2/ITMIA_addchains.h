#ifndef _ITMIA_ADDCHAINS_H
#define _ITMIA_ADDCHAINS_H


/* This file contains the addition chains for ITMIA.
    We read the bits of n-1 from higher bits to lower bits.
    The chain always starts with {1,2}. */
#include "parameters_HFE.h"


#define M_ITOH358(V,MS)\
    V##ODD_CASE(2);\
    V##ODD_CASE(5);\
    V##EVEN_CASE(11);\
    V##EVEN_CASE(22);\
    V##ODD_CASE(44);\
    V##EVEN_CASE(89);\
    V##ODD_CASE(178);

#define ITOH358U M_ITOH358(,)
#define P_ITOH358U M_ITOH358(P,)
#define VP_ITOH358U M_ITOH358(VP,)
#define PMS_ITOH358U M_ITOH358(P,_MULTISQR)



#endif
