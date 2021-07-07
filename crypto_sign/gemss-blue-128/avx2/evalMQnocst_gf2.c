#include "MQ_gf2.h"
#include "bit.h"
#include "dotProduct_gf2.h"
#include "evalMQnocst_gf2.h"
#include "parameters_HFE.h"
#include "tools_gf2nv.h"




/**
 * @brief   Variable-time evaluation of a MQ equation in a vector. The equation
 * is uncompressed.
 * @details mq = (c,Q).
 * c is in GF(2).
 * Q is lower triangular of size (n+v)*(n+v) in GF(2).
 * The rows of Q are stored separately (we take new words for each new row).
 * Evaluation: accumulation of x_i * sum p_i,j xj.
 * If x_i==0, the sum is not computed (variable-time optimization).
 * Else, the sum is computed with a dot product of the current row of Q by m.
 * @param[in]   m   A vector of n+v elements in GF(2).
 * @param[in]   mq  A MQ equation in GF(2)[x1,...,x_(n+v)].
 * @return  The evaluation of mq in m, in GF(2).
 * @remark  Variable-time implementation.
 */
UINT PQCLEAN_GEMSSBLUE128_AVX2_evalMQnocst_unrolled_no_simd_gf2(const UINT *m,
        const UINT *mq) {
    UINT acc = 0;
    unsigned int i;

    for (i = 0; i < 64; ++i) {
        if ((m[0] >> i)&UINT_1) {
            acc ^= mq[i] & m[0];
        }
    }

    mq += 64;
    for (i = 0; i < 64; ++i) {
        if ((m[1] >> i)&UINT_1) {
            acc ^= mq[0] & m[0];
            acc ^= mq[1] & m[1];
        }
        mq += 2;
    }

    for (i = 0; i < HFEnvr; ++i) {
        if ((m[2] >> i)&UINT_1) {
            acc ^= mq[0] & m[0];
            acc ^= mq[1] & m[1];
            acc ^= mq[2] & m[2];
        }
        mq += 3;
    }

    XORBITS_UINT(acc);
    return acc;
}
