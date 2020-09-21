#ifndef _ITMIA_ADDCHAINS_H
#define _ITMIA_ADDCHAINS_H


/* This file contains the addition chains for ITMIA.
    We read the bits of n-1 from higher bits to lower bits.
    The chain always starts with {1,2}. */
#include "parameters_HFE.h"


#define M_ITOH174(V,MS)        V##ODD_CASE(2);        V##EVEN_CASE(5);        V##ODD_CASE(10);        V##ODD_CASE(21);        V##EVEN_CASE(43);        V##ODD_CASE(86);

#define ITOH174U M_ITOH174(,)
#define P_ITOH174U M_ITOH174(P,)
#define VP_ITOH174U M_ITOH174(VP,)
#define PMS_ITOH174U M_ITOH174(P,_MULTISQR)



#endif
