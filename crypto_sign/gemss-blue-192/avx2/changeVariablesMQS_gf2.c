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
int PQCLEAN_GEMSSBLUE192_AVX2_changeVariablesMQS_simd_gf2(mqsnv_gf2n MQS, cst_GLnv_gf2 S) {
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


/***************************************/
/*** Special fast version for 64-bit ***/
/***************************************/


/* Compute a dot product with one word of S */
#define LOOPKR(START,NB_IT) \
    for(kr=(START);kr<(NB_IT);++kr)\
    {\
        /* multiply one bit of S by one element of MQS_cpj */\
        mask=-(bit_kr&UINT_1); \
        xorLoadMask1_gf2n(MQS2_cp,MQS_cpj,mask);\
        MQS_cpj+=NB_WORD_GFqn;\
        bit_kr>>=1;\
    }

/* Compute a dot product for the last word of S */
#define LOOPKR_REMAINDER \
    bit_kr=S_cpj[kq];\
    LOOPKR(0,HFEnvr);

/* Compute a dot product for the last word of S */
#define LOOPK_REM \
    bit_kr=(*S_cpj)>>ir;\
    LOOPKR(ir,HFEnvr);


/* Compute a dot product with iq words equal to zero at the begin of the rows */
#define LOOPK \
    bit_kr=(*S_cpj)>>ir;\
    LOOPKR(ir,NB_BITS_UINT);\
    \
    for(kq=1;kq<(HFEnvq-iq);++kq)\
    {\
        bit_kr=S_cpj[kq];\
        LOOPKR(0,NB_BITS_UINT);\
    }\
    \
    LOOPKR_REMAINDER;


/* Compute a dot product with complete rows */
#define LOOPK_COMPLETE \
    for(kq=0;kq<HFEnvq;++kq)\
    {\
        bit_kr=S_cpj[kq];\
        LOOPKR(0,NB_BITS_UINT);\
    }\
    LOOPKR_REMAINDER;


/* Loop for a block of rows */
#define LOOPIR(STARTIR,NB_ITIR,LOOPK) \
    for(ir=(STARTIR);ir<(NB_ITIR);++ir)\
    {\
        /* Compute a dot product */\
        LOOPK;\
        /* update the next element to compute */\
        MQS2_cp+=NB_WORD_GFqn;\
    }


/* Loop for a block of rows */
/* Init to 0 the res */
#define LOOPIR_INIT(STARTIR,NB_ITIR) \
    for(ir=(STARTIR);ir<(NB_ITIR);++ir)\
    {\
        set0_gf2n(MQS2_cp);\
        MQS_cpj=MQS_cpi;\
        /* Compute a dot product */\
        LOOPK_COMPLETE;\
        /* update the next element to compute */\
        MQS2_cp+=NB_WORD_GFqn;\
        /* update the next row of S to use */\
        S_cpj+=NB_WORD_GF2nv;\
    }


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
 * @remark  This function should be faster than PQCLEAN_GEMSSBLUE192_AVX2_changeVariablesMQS_simd_gf2
 * when SIMD is not used.
 * @remark  Constant-time implementation.
 */
int PQCLEAN_GEMSSBLUE192_AVX2_changeVariablesMQS64_gf2(mqsnv_gf2n MQS, cst_GLnv_gf2 S) {
    /* Tmp matrix (n+v)*(n+v) of quadratic terms to compute S*Q */
    UINT MQS2[HFEnv * HFEnv * NB_WORD_GFqn] = {0};
    UINT *MQS2_cp;
    UINT bit_kr, mask;
    cst_mqsnv_gf2n MQS_cpi, MQS_cpj;
    cst_GLnv_gf2 S_cpi, S_cpj;
    unsigned int iq, ir, j, jq, jr, kq, kr;

    /* To avoid the constant of MQS */
    MQS_cpi = MQS + NB_WORD_GFqn;

    MQS2_cp = MQS2;
    S_cpj = S;

    /* Step 1 : compute MQS2 = S*Q */
    /* Use multiplication by transpose (so by rows of Q) */
    /* It is possible because X*Q*tX = X*tQ*tX (with X = (x1 ... xn)) */
    /* Warning : Q is a upper triangular matrix in GF(q^n) */

    /* In this code, we have : */
    /* i = iq*NB_BITS_UINT + ir */
    /* k = kq*NB_BITS_UINT + kr */

    /* *MQS_cpi = MQS[NB_WORD_GFqn] */
    /* *MQS_cpj = MQS_cpi[(((i*(2n-i+1))/2) + k)*NB_WORD_GFqn] */
    /* The previous formula is a bit complicated, so the idea is :
       *MQS_cpj would equal MQS_cpi[i][i+k] if MQS used n*n in memory */
    /* *MQS2_cp = MQS2[i*NB_WORD_GFqn] */
    /* *S_cpj = S[j*NB_WORD_GFqn+iq] */

    /* for each row j of S */
    for (j = 0; j < HFEnv; ++j) {
        /* initialisation at the first row of Q */
        MQS_cpj = MQS_cpi;
        /* for each row of Q excepted the last block */
        for (iq = 0; iq < HFEnvq; ++iq) {
            LOOPIR(0, NB_BITS_UINT, LOOPK);
            /* 64 bits of zero in Q */
            ++S_cpj;
        }
        /* the last block */
        LOOPIR(0, HFEnvr, LOOPK_REM);
        /* Next row of S */
        ++S_cpj;
    }

    /* Step 2 : compute MQS = MQS2*tS = (S*Q)*tS */
    /* Use multiplication by transpose (so by rows of S) */

    /* Permute MQS and MQS2 */
    MQS_cpi = MQS2;
    MQS2_cp = MQS + NB_WORD_GFqn;
    S_cpi = S;

    /* First : compute upper triangular result */


    /* In this code, we have : */
    /* *MQS_cpi = MQS2[j*n*NB_WORD_GFqn] */
    /* *MQS_cpj = MQS2[(j*n+k)*NB_WORD_GFqn] */
    /* *MQS2_cp = MQS[(((j*(2n-j+1))/2) + i-j)*NB_WORD_GFqn] */
    /* The previous formula is a bit complicated, so the idea is :
       *MQS2_cp would equal MQS[j][i] if MQS used n*n in memory */
    /* *S_cpi = S[j*NB_WORD_GFqn] */
    /* *S_cpj = S[i*NB_WORD_GFqn] */


    /* for each row j of MQS2 excepted the last block */
    for (jq = 0; jq < HFEnvq; ++jq) {
        for (jr = 0; jr < NB_BITS_UINT; ++jr) {
            S_cpj = S_cpi;
            /* for each row >=j of S */
            LOOPIR_INIT(jr, NB_BITS_UINT);
            for (iq = jq + 1; iq < HFEnvq; ++iq) {
                LOOPIR_INIT(0, NB_BITS_UINT);
            }
            /* the last block */
            LOOPIR_INIT(0, HFEnvr);
            /* Next row of MQS2 */
            MQS_cpi = MQS_cpj;
            /* Next row of S because of upper triangular */
            S_cpi += NB_WORD_GF2nv;
        }
    }
    /* the last block */
    for (jr = 0; jr < HFEnvr; ++jr) {
        S_cpj = S_cpi;
        MQS_cpj = MQS_cpi;
        /* for each row >=j of S, the last block */
        LOOPIR_INIT(jr, HFEnvr);
        MQS_cpi = MQS_cpj;
        S_cpi += NB_WORD_GF2nv;
    }

    /* Second : compute lower triangular result */

    MQS_cpi = MQS2;
    MQS2_cp = MQS + NB_WORD_GFqn;
    S_cpj = S;


    /* In this code, we have : */
    /* *MQS_cpi = MQS2[(j+1)*n*NB_WORD_GFqn] */
    /* *MQS_cpj = MQS2[(j+1)*n+k)*NB_WORD_GFqn] */
    /* *MQS2_cp = MQS[(((j*(2n-j+1))/2) + i-j)*NB_WORD_GFqn] */
    /* The previous formula is a bit complicated, so the idea is :
       *MQS2_cp would equal MQS[j][i] if MQS used n*n in memory */
    /* *S_cpj = S[j*NB_WORD_GFqn] */


    /* for each row j of S excepted the last block */
    for (jq = 0; jq < HFEnvq; ++jq) {
        for (jr = 0; jr < NB_BITS_UINT; ++jr) {
            /* i=j : the diagonal is already computing */
            MQS2_cp += NB_WORD_GFqn;
            /* The line j of MQS2 is useless */
            MQS_cpi += HFEnv * NB_WORD_GFqn;
            MQS_cpj = MQS_cpi;
            /* for each row >j of MQS2 */
            LOOPIR(jr + 1, NB_BITS_UINT, LOOPK_COMPLETE);
            for (iq = jq + 1; iq < HFEnvq; ++iq) {
                LOOPIR(0, NB_BITS_UINT, LOOPK_COMPLETE);
            }
            /* the last block */
            LOOPIR(0, HFEnvr, LOOPK_COMPLETE);
            /* Next row of S */
            S_cpj += NB_WORD_GF2nv;
        }
    }
    /* the last block excepted the last row */
    for (jr = 0; jr < HFEnvr - 1; ++jr) {
        /* i=j : the diagonal is already computing */
        MQS2_cp += NB_WORD_GFqn;
        /* The line j of MQS2 is useless */
        MQS_cpi += HFEnv * NB_WORD_GFqn;
        MQS_cpj = MQS_cpi;
        /* for each row >=j of S */
        /* the last block */
        LOOPIR(jr + 1, HFEnvr, LOOPK_COMPLETE);
        /* Next row of S */
        S_cpj += NB_WORD_GF2nv;
    }

    return 0;
}
