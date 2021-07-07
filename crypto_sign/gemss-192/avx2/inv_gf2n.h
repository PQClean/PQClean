#ifndef _INV_GF2N_H
#define _INV_GF2N_H
#include "arch.h"
#include "config_gf2n.h"
#include "parameters_HFE.h"
#include "tools_gf2n.h"




#define CONCAT4(a,b) a ## b
#define CONCAT3(a,b) CONCAT4(a,b)

#define ITOH CONCAT3(ITOH,HFEn)


#define CHOOSE_ITMIA


#define inv_gf2n PQCLEAN_GEMSS192_AVX2_inv_ITMIA_gf2n


void PQCLEAN_GEMSS192_AVX2_inv_Fermat_gf2n_ref(uint64_t res[NB_WORD_GFqn],
        const uint64_t A[NB_WORD_GFqn]);


void PQCLEAN_GEMSS192_AVX2_inv_ITMIA_gf2n_ref(static_gf2n res[NB_WORD_GFqn],
        cst_static_gf2n A[NB_WORD_GFqn]);


void PQCLEAN_GEMSS192_AVX2_inv_ITMIA_gf2n(static_gf2n res[NB_WORD_GFqn],
        cst_static_gf2n A[NB_WORD_GFqn]);


#endif
