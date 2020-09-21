#ifndef _DIV_GF2NX_H
#define _DIV_GF2NX_H
#include "gf2nx.h"
#include "parameters_HFE.h"
#include "tools_gf2n.h"



unsigned int PQCLEAN_GEMSSBLUE192_AVX2_div_qr_monic_gf2nx(gf2nx A, unsigned int da,
        cst_gf2nx B, unsigned int db);
unsigned int PQCLEAN_GEMSSBLUE192_AVX2_div_q_monic_gf2nx(gf2nx A, unsigned int da,
        cst_gf2nx B, unsigned int db);
unsigned int PQCLEAN_GEMSSBLUE192_AVX2_div_r_gf2nx(gf2nx A, unsigned int da,
        cst_gf2nx B, unsigned int db);
void PQCLEAN_GEMSSBLUE192_AVX2_div_r_monic_cst_gf2nx(gf2nx A, unsigned int da,
        cst_gf2nx B, unsigned int db);
#define div_r_monic_gf2nx PQCLEAN_GEMSSBLUE192_AVX2_div_qr_monic_gf2nx


unsigned int PQCLEAN_GEMSSBLUE192_AVX2_div_r_HFE_gf2nx(gf2nx poly, unsigned int dp,
        complete_sparse_monic_gf2nx F,
        cst_gf2n cst);
void PQCLEAN_GEMSSBLUE192_AVX2_div_r_HFE_cstdeg_gf2nx(gf2nx poly, unsigned int dp,
        complete_sparse_monic_gf2nx F,
        cst_gf2n cst);
void PQCLEAN_GEMSSBLUE192_AVX2_div_r_HFE_cst_gf2nx(gf2nx poly,
        complete_sparse_monic_gf2nx F,
        cst_gf2n cst);


#define divsqr_r_HFE_cstdeg_gf2nx PQCLEAN_GEMSSBLUE192_AVX2_div_r_HFE_cstdeg_gf2nx
#define divsqr_r_HFE_cst_gf2nx PQCLEAN_GEMSSBLUE192_AVX2_div_r_HFE_cst_gf2nx


#endif
