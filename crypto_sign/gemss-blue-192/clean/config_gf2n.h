#ifndef _CONFIG_GF2N_H
#define _CONFIG_GF2N_H


/* An irreducible polynomial f(x) of degree n in GF(2)[x] is chosen.
    f is a trinomial s.t. f(x) = x^n + x^K3 + 1,
    or f is a pentanomial s.t. f(x) = x^n + x^K3 + x^K2 + x^K1 + 1. */
#include "parameters_HFE.h"


/* Warning: 0 < K3 <= 32 is required (excepted for special cases) */
/* Warning: 0 < K1 < K2 < K3 <= 32 is required (excepted for special cases) */

/* The value before (HFEn==2) are special cases */

#define K3 42U



/* Choice of trinomial for modular reduction in GF(2^n) */
#define __TRINOMIAL_GF2N__



#endif
