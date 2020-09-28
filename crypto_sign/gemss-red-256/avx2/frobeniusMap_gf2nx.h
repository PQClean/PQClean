#ifndef _FROBENIUSMAP_GF2NX_H
#define _FROBENIUSMAP_GF2NX_H
#include "arch.h"
#include "gf2nx.h"
#include "parameters_HFE.h"
#include "tools_gf2n.h"



unsigned int PQCLEAN_GEMSSRED256_AVX2_frobeniusMap_HFE_gf2nx(gf2nx Xqn,
        complete_sparse_monic_gf2nx F, cst_gf2n U);


/* Multi-squaring at the power 2^II */
/* A script has been used to find the value of II which minimizes the number
   of multiplication in GF(2^n). */
#define II 4

#define POW_II (ONE32<<II)

/* First k s.t. k*(2^II) >= D, i.e. k = Ceil(D/2^II) */
#define KP ((HFEDeg>>II)+1)
/* Number of coefficients k s.t. k*(2^II) >= D and k < D */
#define KX (HFEDeg-KP)


unsigned int PQCLEAN_GEMSSRED256_AVX2_frobeniusMap_multisqr_HFE_gf2nx(gf2nx Xqn,
        complete_sparse_monic_gf2nx F, cst_gf2n U);


/* Heuristic to choose the fastest function */
/* In particular, it is optimal for GeMSS, Gui and DualModeMS parameters */
/* It is the best choice for all n>=17 */
#define best_frobeniusMap_HFE_gf2nx PQCLEAN_GEMSSRED256_AVX2_frobeniusMap_multisqr_HFE_gf2nx


#endif
