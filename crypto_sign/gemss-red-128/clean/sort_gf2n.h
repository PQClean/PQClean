#ifndef _SORT_GF2N_H
#define _SORT_GF2N_H
#include "arch.h"
#include "matrix_gf2n.h"



void PQCLEAN_GEMSSRED128_CLEAN_selectionSort_nocst_gf2n(vec_gf2n tab, int l);
void PQCLEAN_GEMSSRED128_CLEAN_selectionSort_gf2n(vec_gf2n tab, int l);


/* The selection sort is the reference implementation. */
#define sort_nocst_gf2n PQCLEAN_GEMSSRED128_CLEAN_selectionSort_nocst_gf2n

/* Constant-time */
#define sort_gf2n PQCLEAN_GEMSSRED128_CLEAN_selectionSort_gf2n


#endif
