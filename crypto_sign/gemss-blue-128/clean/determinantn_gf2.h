#ifndef _DETERMINANTN_GF2_H
#define _DETERMINANTN_GF2_H
#include "arch.h"
#include "gf2.h"
#include "matrix_gf2.h"



gf2 PQCLEAN_GEMSSBLUE128_CLEAN_determinantn_nocst_gf2(cst_Mn_gf2 S, Mn_gf2 S_cp);

gf2 PQCLEAN_GEMSSBLUE128_CLEAN_determinantn_cst_gf2(cst_Mn_gf2 S, Mn_gf2 S_cp);


#define determinantn_gf2 PQCLEAN_GEMSSBLUE128_CLEAN_determinantn_cst_gf2


#endif
