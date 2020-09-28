#ifndef _ITMIA_ADDCHAINS_H
#define _ITMIA_ADDCHAINS_H


/* This file contains the addition chains for ITMIA.
    We read the bits of n-1 from higher bits to lower bits.
    The chain always starts with {1,2}. */
#include "parameters_HFE.h"


#define M_ITOH175(V,MS)\
    V##ODD_CASE(2);\
    V##EVEN_CASE(5);\
    V##ODD_CASE(10);\
    V##ODD_CASE(21);\
    V##ODD_CASE(43);\
    V##EVEN_CASE(87);

#define ITOH175U M_ITOH175(,)
#define P_ITOH175U M_ITOH175(P,)
#define VP_ITOH175U M_ITOH175(VP,)
#define PMS_ITOH175U M_ITOH175(P,_MULTISQR)



#endif
