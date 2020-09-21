#ifndef _FINDROOTS_GF2NX_H
#define _FINDROOTS_GF2NX_H
#include "conv_gf2nx.h"
#include "gf2nx.h"
#include "matrix_gf2n.h"
#include "tools_gf2n.h"





/* GCD(F-U, X^(2^n)-X mod (F-U)) */
/* To know the number of roots of F-U */
#define firstStepHFE_gf2nx(l,i,poly,poly2,F,U)     /* X^(2^n) - X mod (F-U) */    l=best_frobeniusMap_HFE_gf2nx(poly,F,U);    /* Initialize to F */    PQCLEAN_GEMSSRED128_CLEAN_convHFEpolynomialSparseToDense_gf2nx(poly2,F);    /* Initialize to F-U */    add2_gf2n(poly2,U);    (l)=PQCLEAN_GEMSSRED128_CLEAN_gcd_gf2nx(&(i),poly2,d2,poly,l);


int PQCLEAN_GEMSSRED128_CLEAN_findRootsHFE_gf2nx(vec_gf2n *roots,
        complete_sparse_monic_gf2nx F,
        cst_gf2n U);
int PQCLEAN_GEMSSRED128_CLEAN_findUniqRootHFE_gf2nx(gf2n root,
        complete_sparse_monic_gf2nx F,
        cst_gf2n U);


#endif
