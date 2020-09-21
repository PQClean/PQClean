#ifndef _ITMIA_ADDCHAINS_H
#define _ITMIA_ADDCHAINS_H


/* This file contains the addition chains for ITMIA.
    We read the bits of n-1 from higher bits to lower bits.
    The chain always starts with {1,2}. */
#include "parameters_HFE.h"


#define M_ITOH177(V,MS)        V##ODD_CASE(2);        V##ODD_CASE(5);        V##EVEN_CASE(11);        V##EVEN_CASE(22);        V##EVEN_CASE(44);        V##EVEN_CASE(88);

#define ITOH177U M_ITOH177(,)
#define P_ITOH177U M_ITOH177(P,)
#define VP_ITOH177U M_ITOH177(VP,)
#define PMS_ITOH177U M_ITOH177(P,_MULTISQR)



#endif
