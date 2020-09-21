#ifndef _DOTPRODUCT_GF2N_H
#define _DOTPRODUCT_GF2N_H
#include "arch.h"
#include "macro.h"
#include "parameters_HFE.h"
#include "rem_gf2n.h"
#include "tools_gf2n.h"

/* Really important include for the definition of
   __TRINOMIAL_GF2N__ and __PENTANOMIAL_GF2N__ */


/* Choice the best dotProduct */
#define dotProduct_gf2n PQCLEAN_GEMSSRED192_CLEAN_dotProduct_no_simd_gf2n
#define doubleDotProduct_gf2n PQCLEAN_GEMSSRED192_CLEAN_doubleDotProduct_no_simd_gf2n


void PQCLEAN_GEMSSRED192_CLEAN_dotProduct_no_simd_gf2n(uint64_t res[NB_WORD_GFqn],
        const UINT *vec_x, const UINT *vec_y, unsigned int len);
void PQCLEAN_GEMSSRED192_CLEAN_doubleDotProduct_no_simd_gf2n(uint64_t res[NB_WORD_GFqn],
        const UINT *vec_x, const UINT *vec_y,
        const UINT *vec2_x, const UINT *vec2_y, unsigned int len);



#endif
