#include "dotProduct_gf2.h"
#include "mixEquationsMQS_gf2.h"
#include "vecMatProduct_gf2.h"



/* To mix equations in a MQ System */


/**
 * @brief   Mix the equations of a MQS with a linear transformation. The MQS is
 * stored with a monomial representation.
 * @details MQS = (c,Q), with c the constant part in GF(2^n) and Q is an upper
 * triangular matrix of size (n+v)*(n+v) in GF(2^n). We have MQS = c + xQxt
 * with x =  (x0 x1 ... x_(n+v)). At the end of the function, we have
 * MQS = c' + xQ'xt with c' in GF(2^m) and Q' is an upper triangular matrix of
 * size (n+v)*(n+v) in GF(2^m).
 * Here, the equations vector is multiplied by T with a right multiplication.
 * We take only the m first equations of the result (the other equations are not
 * computed).
 * @param[in]   MQS A MQS in GF(2^n)[x1,...,x_(n+v)] (n equations,
 * n+v variables).
 * @param[in]   T   A matrix (n+v)*(n+v) in GF(2). T should be invertible.
 * @param[out]  pk  A MQS in GF(2^m)[x1,...,x_(n+v)] (m equations,
 * n+v variables).
 * @remark  Constant-time implementation.
 */
void PQCLEAN_GEMSSRED192_CLEAN_mixEquationsMQS8_gf2_right(mqsnv8_gf2m pk, cst_mqsnv_gf2n MQS,
        cst_Mn_gf2 T) {
    unsigned int i;

    /* for each monomial of MQS and pk */
    for (i = 1; i < NB_MONOMIAL_PK; ++i) {
        vecMatProductm_gf2((UINT *)pk, MQS, T);

        /* next monomial */
        MQS += NB_WORD_GFqn;
        pk += NB_BYTES_GFqm;
    }

    /* Last monomial: we fill the last bytes of pk without 64-bit cast. */
    UINT pk_last[NB_WORD_GF2m];

    vecMatProductm_gf2(pk_last, MQS, T);

    for (i = 0; i < HFEmq; ++i) {
        *((UINT *)pk) = pk_last[i];
        pk += 8;
    }

    /* We fill the last bytes of pk without 64-bit cast */
    for (i = 0; i < (7); ++i) {
        pk[i] = pk_last[NB_WORD_GF2m - 1] >> (i << 3);
    }
}
