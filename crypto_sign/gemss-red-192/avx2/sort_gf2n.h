#ifndef _SORT_GF2N_H
#define _SORT_GF2N_H
#include "arch.h"
#include "matrix_gf2n.h"



void PQCLEAN_GEMSSRED192_AVX2_quickSort_nocst_gf2n(vec_gf2n tab_start, vec_gf2n tab_last);
void PQCLEAN_GEMSSRED192_AVX2_selectionSort_nocst_gf2n(vec_gf2n tab, int l);
void PQCLEAN_GEMSSRED192_AVX2_fast_sort_gf2n(vec_gf2n tab, int l);
void PQCLEAN_GEMSSRED192_AVX2_selectionSort_gf2n(vec_gf2n tab, int l);


/* The fastest function in variable-time */
#define sort_nocst_gf2n(tab,l)                 PQCLEAN_GEMSSRED192_AVX2_quickSort_nocst_gf2n(tab,(tab)+((l)-1)*NB_WORD_GFqn)

/* Constant-time */
#define sort_gf2n PQCLEAN_GEMSSRED192_AVX2_fast_sort_gf2n


#endif
