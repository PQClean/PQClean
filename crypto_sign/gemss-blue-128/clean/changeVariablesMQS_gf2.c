#include "add_gf2n.h"
#include "changeVariablesMQS_gf2.h"
#include "tools_gf2n.h"
#include "vecMatProduct_gf2.h"
#include <stdlib.h>

/* To do a change of variables in a MQ System */


/**
 * @brief   Apply the change of variables x'=xS to a MQS stored with a monomial
 * representation.
 * @details MQS = (c,Q), with c the constant part in GF(2^n) and Q is an upper
 * triangular matrix of size (n+v)*(n+v) in GF(2^n). We have MQS = c + xQxt
 * with x =  (x0 x1 ... x_(n+v)). At the end of the function, we have
 * MQS = c + xQ'xt with Q' = SQSt. We multiply S by Q, then SQ by St.
 * @param[in,out]   MQS A MQS in GF(2^n)[x1,...,x_(n+v)] (n equations,
 * n+v variables).
 * @param[in]   S   A matrix (n+v)*(n+v) in GF(2). S should be invertible
 * (by definition of a change of variables).
 * @return   0 if the result is correct, ERROR_ALLOC for error from
 * malloc/calloc functions.
 * @remark  This function uses SIMD only when the vecMatProduct function uses
 * it.
 * @remark  Constant-time implementation.
 */
int PQCLEAN_GEMSSBLUE128_CLEAN_changeVariablesMQS_simd_gf2(mqsnv_gf2n MQS, cst_GLnv_gf2 S) {
    UINT tmp[NB_WORD_GFqn] = {0};
    /* Tmp matrix (n+v)*(n+v) of quadratic terms to compute S*Q */
    UINT MQS2[HFEnv * HFEnv * NB_WORD_GFqn] = {0};
    UINT *MQS2_cp;
    cst_mqsnv_gf2n MQS_cpi, MQS_cpj;
    cst_GLnv_gf2 S_cpi, S_cpj;
    unsigned int i, j;

    /* To avoid the constant of MQS */
    MQS += NB_WORD_GFqn;

    /* Step 1: compute MQS2 = S*Q */
    /* Use multiplication by transpose (so by rows of Q) */
    /* It is possible because X*Q*tX = X*tQ*tX (with X = (x1 ... xn)) */
    /* Warning : Q is a upper triangular matrix in GF(q^n) */


    MQS2_cp = MQS2;
    S_cpj = S;

    /* for each row j of S */
    for (j = 0; j < HFEnv; ++j) {
        /* initialisation at the first row of Q */
        MQS_cpi = MQS;
        /* for each row of Q */
        for (i = 0; i < HFEnv; ++i) {
            /* Compute a dot product */
            vecMatProductnvn_start_gf2(MQS2_cp, S_cpj, MQS_cpi, i);
            MQS_cpi += NB_WORD_GFqn * (HFEnv - i);
            /* update the next element to compute */
            MQS2_cp += NB_WORD_GFqn;
        }
        S_cpj += NB_WORD_GF2nv;
    }


    /* Step 2: compute MQS = MQS2*tS = (S*Q)*tS */
    /* Use multiplication by transpose (so by rows of S) */

    /* Permute MQS and MQS2 */
    MQS_cpj = MQS2;
    MQS2_cp = MQS;
    S_cpj = S;

    /* First: compute upper triangular result */

    /* for each row j of MQS2 */
    for (j = 0; j < HFEnv; ++j) {
        S_cpi = S_cpj;

        /* for each row >=j of S */
        for (i = j; i < HFEnv; ++i) {
            /* Compute a dot product with complete rows */
            /* (Init to 0 the res) */
            vecMatProductnvn_gf2(MQS2_cp, S_cpi, MQS_cpj);
            /* update the next element to compute */
            MQS2_cp += NB_WORD_GFqn;
            /* update the next row of S to use */
            S_cpi += NB_WORD_GF2nv;
        }

        /* Next row of MQS2 */
        MQS_cpj += NB_WORD_GFqn * HFEnv;
        /* Next row of S because of upper triangular */
        S_cpj += NB_WORD_GF2nv;
    }

    /* Second: compute lower triangular result */

    MQS_cpj = MQS2;
    MQS2_cp = MQS;
    S_cpj = S;

    /* for each row j of S */
    for (j = 0; j < HFEnv; ++j) {
        /* i=j : the diagonal is already computing */
        MQS2_cp += NB_WORD_GFqn;
        /* The line j of MQS2 is useless */
        MQS_cpj += HFEnv * NB_WORD_GFqn;
        MQS_cpi = MQS_cpj;

        /* for each row >j of MQS2 */
        for (i = j + 1; i < HFEnv; ++i) {
            /* Compute a dot product with complete rows */
            vecMatProductnvn_gf2(tmp, S_cpj, MQS_cpi);
            add2_gf2n(MQS2_cp, tmp);
            MQS_cpi += NB_WORD_GFqn * HFEnv;
            /* update the next element to compute */
            MQS2_cp += NB_WORD_GFqn;
        }

        /* Next row of S */
        S_cpj += NB_WORD_GF2nv;
    }

    return 0;
}
