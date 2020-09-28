#ifndef _ITMIA_ADDCHAINS_H
#define _ITMIA_ADDCHAINS_H


/* This file contains the addition chains for ITMIA.
    We read the bits of n-1 from higher bits to lower bits.
    The chain always starts with {1,2}. */
#include "parameters_HFE.h"


#define M_ITOH265(V,MS)\
    V##EVEN_CASE(2);\
    V##EVEN_CASE(4);\
    V##EVEN_CASE(8);\
    V##ODD_CASE(16);\
    V##EVEN_CASE(33);\
    V##EVEN_CASE(66);\
    V##EVEN_CASE(132);

#define ITOH265U M_ITOH265(,)
#define P_ITOH265U M_ITOH265(P,)
#define VP_ITOH265U M_ITOH265(VP,)
#define PMS_ITOH265U M_ITOH265(P,_MULTISQR)



#endif
