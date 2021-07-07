#ifndef _GF2X_H
#define _GF2X_H
#include "arch.h"



/* This type stores an element of GF(2)[x]. Their coefficients are stored in
   the lexicographic order (in little-endian). */
typedef UINT *gf2x;
typedef const UINT *cst_gf2x;
/* to use data[length]. */
typedef UINT static_gf2x;
typedef const UINT cst_static_gf2x;


#endif
