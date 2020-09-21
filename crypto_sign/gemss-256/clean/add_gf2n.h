#ifndef _ADD_GF2N_H
#define _ADD_GF2N_H
#include "arch.h"
#include "gf2x.h"
#include "tools_gf2n.h"



void PQCLEAN_GEMSS256_CLEAN_add_gf2n_ref(gf2n c, cst_gf2n a, cst_gf2n b);
void PQCLEAN_GEMSS256_CLEAN_add2_gf2n_ref(gf2n c, cst_gf2n a);
void PQCLEAN_GEMSS256_CLEAN_add_product_gf2n_ref(gf2x c, cst_gf2x a, cst_gf2x b);
void PQCLEAN_GEMSS256_CLEAN_add2_product_gf2n_ref(gf2x c, cst_gf2x a);



#define add_gf2n PQCLEAN_GEMSS256_CLEAN_add_gf2n_ref
#define add2_gf2n PQCLEAN_GEMSS256_CLEAN_add2_gf2n_ref
#define add_product_gf2n PQCLEAN_GEMSS256_CLEAN_add_product_gf2n_ref
#define add2_product_gf2n PQCLEAN_GEMSS256_CLEAN_add2_product_gf2n_ref


#endif
