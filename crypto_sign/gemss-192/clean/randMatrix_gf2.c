#include "bit.h"
#include "determinantn_gf2.h"
#include "determinantnv_gf2.h"
#include "dotProduct_gf2.h"
#include "gf2.h"
#include "macro.h"
#include "randMatrix_gf2.h"
#include "randombytes.h"
#include <stdlib.h>


void PQCLEAN_GEMSS192_CLEAN_cleanMatrix_gf2_n(Mn_gf2 S) {
    cst_Mn_gf2 S_end;

    S_end = S + MATRIXn_SIZE;
    for (S += NB_WORD_GFqn - 1; S < S_end; S += NB_WORD_GFqn) {
        /* Clean the last word */
        *S &= MASK_GF2n;
    }
}


/**
 * @brief   Generate a random matrix (n,n) in GF(2).
 * @param[out]  S   A random matrix (n,n) in GF(2).
 * @remark  Requires to allocate n elements of GF(2^n) for S.
 * @remark  Constant-time implementation.
 */
void PQCLEAN_GEMSS192_CLEAN_randMatrix_gf2_n(Mn_gf2 S) {
    randombytes((unsigned char *)S, MATRIXn_SIZE << 3);

    unsigned int i;
    --S;
    for (i = 0; i < HFEn; ++i) {
        S += NB_WORD_GFqn;
        /* Clean the last word */
        *S &= MASK_GF2n;
    }
}


void PQCLEAN_GEMSS192_CLEAN_cleanMatrix_gf2_nv(Mnv_gf2 S) {
    cst_Mnv_gf2 S_end;

    S_end = S + MATRIXnv_SIZE;
    for (S += NB_WORD_GF2nv - 1; S < S_end; S += NB_WORD_GF2nv) {
        /* Clean the last word */
        *S &= MASK_GF2nv;
    }
}


/**
 * @brief   Generate a random matrix (n+v,n+v) in GF(2).
 * @param[out]  S   A random matrix (n+v,n+v) in GF(2).
 * @remark  Requires to allocate n+v elements of GF(2^(n+v)) for S.
 * @remark  Constant-time implementation.
 */
void PQCLEAN_GEMSS192_CLEAN_randMatrix_gf2_nv(Mnv_gf2 S) {
    randombytes((unsigned char *)S, MATRIXnv_SIZE << 3);

    unsigned int i;
    --S;
    for (i = 0; i < HFEnv; ++i) {
        S += NB_WORD_GF2nv;
        /* Clean the last word */
        *S &= MASK_GF2nv;
    }
}


/**
 * @brief   Generate a random invertible matrix (n,n) in GF(2).
 * @details Trial and error method.
 * @param[out]  S   A random invertible matrix (n,n) in GF(2).
 * @param[in]   S_cp    A matrix (n,n) in GF(2).
 * @return  An integer, different from 1 if the allocation fails.
 * @remark  Requires to allocate n elements of GF(2^n) for S.
 * @remark  Requires to allocate n elements of GF(2^n) for S_cp.
 * @remark  Constant-time implementation for S, we have an information leak
 * only for the random non-invertible matrices (which are not used).
 */
void PQCLEAN_GEMSS192_CLEAN_randInvMatrix_gf2_n(GLn_gf2 S, Mn_gf2 S_cp) {
    gf2 ret;

    do {
        PQCLEAN_GEMSS192_CLEAN_randMatrix_gf2_n(S);
        ret = determinantn_gf2(S, S_cp);
    } while (!ret);
}


/**
 * @brief   Generate a random invertible matrix (n+v,n+v) in GF(2).
 * @details Trial and error method.
 * @param[out]  S   A random invertible matrix (n+v,n+v) in GF(2).
 * @param[in]   S_cp    A matrix (n+v,n+v) in GF(2).
 * @return  An integer, different from 1 if the allocation fails.
 * @remark  Requires to allocate n+v elements of GF(2^(n+v)) for S.
 * @remark  Requires to allocate n+v elements of GF(2^(n+v)) for S_cp.
 * @remark  Constant-time implementation for S, we have an information leak
 * only for the random non-invertible matrices (which are not used).
 */
void PQCLEAN_GEMSS192_CLEAN_randInvMatrix_gf2_nv(GLnv_gf2 S, Mnv_gf2 S_cp) {
    gf2 ret;

    do {
        PQCLEAN_GEMSS192_CLEAN_randMatrix_gf2_nv(S);
        ret = determinantnv_gf2(S, S_cp);
    } while (!ret);
}


void PQCLEAN_GEMSS192_CLEAN_cleanLowerMatrixn(Tn_gf2 L) {
    UINT mask;
    unsigned int iq, ir;

    /* for each row */
    for (iq = 1; iq <= HFEnq; ++iq) {
        mask = 0;
        for (ir = 0; ir < NB_BITS_UINT; ++ir) {
            /* Put the bit of diagonal to 1 + zeros after the diagonal */
            *L &= mask;
            *L ^= UINT_1 << ir;
            mask <<= 1;
            ++mask;

            L += iq;
        }
        /* Next column */
        ++L;
    }

    /* iq = HFEnq */
    mask = 0;
    for (ir = 0; ir < HFEnr; ++ir) {
        /* Put the bit of diagonal to 1 + zeros after the diagonal */
        *L &= mask;
        *L ^= UINT_1 << ir;
        mask <<= 1;
        ++mask;

        L += iq;
    }
}
void PQCLEAN_GEMSS192_CLEAN_cleanLowerMatrixnv(Tn_gf2 L) {
    UINT mask;
    unsigned int iq, ir;

    /* for each row */
    for (iq = 1; iq <= HFEnvq; ++iq) {
        mask = 0;
        for (ir = 0; ir < NB_BITS_UINT; ++ir) {
            /* Put the bit of diagonal to 1 + zeros after the diagonal */
            *L &= mask;
            *L ^= UINT_1 << ir;
            mask <<= 1;
            ++mask;

            L += iq;
        }
        /* Next column */
        ++L;
    }

    /* iq = HFEnq */
    mask = 0;
    for (ir = 0; ir < HFEnvr; ++ir) {
        /* Put the bit of diagonal to 1 + zeros after the diagonal */
        *L &= mask;
        *L ^= UINT_1 << ir;
        mask <<= 1;
        ++mask;

        L += iq;
    }
}


/**
 * @brief   Generate a random invertible lower triangular matrix (n,n) L in
 * GF(2).
 * @param[out]  L   A random invertible lower triangular matrix (n,n) in GF(2).
 * @remark  Requires to allocate LTRIANGULAR_N_SIZE words for L.
 * @remark  Only the lower triangular part of L is stored. In particular, the
 * words equal to zero in the upper trigular part are not stored.
 * @remark  Constant-time implementation.
 */
void PQCLEAN_GEMSS192_CLEAN_genLowerMatrixn(Tn_gf2 L) {
    UINT mask;
    unsigned int iq, ir;

    randombytes((unsigned char *)L, LTRIANGULAR_N_SIZE << 3);

    /* for each row */
    for (iq = 1; iq <= HFEnq; ++iq) {
        mask = 0;
        for (ir = 0; ir < NB_BITS_UINT; ++ir) {
            /* Put the bit of diagonal to 1 + zeros after the diagonal */
            *L &= mask;
            *L ^= UINT_1 << ir;
            mask <<= 1;
            ++mask;

            L += iq;
        }
        /* Next column */
        ++L;
    }

    /* iq = HFEnq */
    mask = 0;
    for (ir = 0; ir < HFEnr; ++ir) {
        /* Put the bit of diagonal to 1 + zeros after the diagonal */
        *L &= mask;
        *L ^= UINT_1 << ir;
        mask <<= 1;
        ++mask;

        L += iq;
    }
}

/**
 * @brief   Generate a random invertible lower triangular matrix (n+v,n+v) L
 * in GF(2).
 * @param[out]  L   A random invertible lower triangular matrix (n+v,n+v) in
 * GF(2).
 * @remark  Requires to allocate LTRIANGULAR_NV_SIZE words for L.
 * @remark  Only the lower triangular part of L is stored. In particular,
 * the words equal to zero in the upper trigular part are not stored.
 * @remark  Constant-time implementation.
 */
void PQCLEAN_GEMSS192_CLEAN_genLowerMatrixnv(Tn_gf2 L) {
    UINT mask;
    unsigned int iq, ir;

    randombytes((unsigned char *)L, LTRIANGULAR_NV_SIZE << 3);

    /* for each row */
    for (iq = 1; iq <= HFEnvq; ++iq) {
        mask = 0;
        for (ir = 0; ir < NB_BITS_UINT; ++ir) {
            /* Put the bit of diagonal to 1 + zeros after the diagonal */
            *L &= mask;
            *L ^= UINT_1 << ir;
            mask <<= 1;
            ++mask;

            L += iq;
        }
        /* Next column */
        ++L;
    }

    /* iq = HFEnq */
    mask = 0;
    for (ir = 0; ir < HFEnvr; ++ir) {
        /* Put the bit of diagonal to 1 + zeros after the diagonal */
        *L &= mask;
        *L ^= UINT_1 << ir;
        mask <<= 1;
        ++mask;

        L += iq;
    }
}


#define LOOPJR(NB_IT) \
    if (iq<jq) mini=iq; \
    else mini=jq; \
    *S=0;\
    for(jr=0;jr<(NB_IT);++jr)\
    {\
        /* Dot product */\
        tmp=L_cp[0]&U_cp[0];\
        for(k=1;k<mini;++k)\
        {\
            tmp^=L_cp[k]&U_cp[k];\
        }\
        XORBITS_UINT(tmp);\
        *S^=tmp<<jr;\
        U_cp+=jq;\
    }\
    ++S;


#define LOOPIR(NB_IT,nq,nr,REM) \
    for(ir=0;ir<(NB_IT);++ir)\
    {\
        U_cp=U;\
        /* for each row of U (multiply by the transpose) */\
        for(jq=1;jq<=(nq);++jq)\
        {\
            LOOPJR(NB_BITS_UINT);\
        }\
        { REM };\
        L_cp+=iq;\
    }


/**
 * @brief   Compute the matrix (n,n) L*U in GF(2).
 * @param[out]  S   S=L*U is a matrix (n,n) in GF(2).
 * @remark  Requires to allocate n elements of GF(2^n) for S.
 * @remark  Constant-time implementation.
 */
void PQCLEAN_GEMSS192_CLEAN_mulMatricesLU_gf2_n(Mn_gf2 S, cst_Tn_gf2 L, cst_Tn_gf2 U) {
    cst_Tn_gf2 L_cp, U_cp;
    UINT tmp;
    unsigned int iq, ir, jq, jr, k, mini;

    /* Computation of S = L*U */
    L_cp = L;
    /* for each row of L (and S) */
    for (iq = 1; iq <= HFEnq; ++iq) {
        LOOPIR(NB_BITS_UINT, HFEnq, HFEnr, LOOPJR(HFEnr));
    }
    LOOPIR(HFEnr, HFEnq, HFEnr, LOOPJR(HFEnr));
}

/**
 * @brief   Compute the matrix (n+v,n+v) L*U in GF(2).
 * @param[out]  S   S=L*U is a matrix (n+v,n+v) in GF(2).
 * @remark  Requires to allocate n+v elements of GF(2^(n+v)) for S.
 * @remark  Constant-time implementation.
 */
void PQCLEAN_GEMSS192_CLEAN_mulMatricesLU_gf2_nv(Mn_gf2 S, cst_Tn_gf2 L, cst_Tn_gf2 U) {
    cst_Tn_gf2 L_cp, U_cp;
    UINT tmp;
    unsigned int iq, ir, jq, jr, k, mini;

    /* Computation of S = L*U */
    L_cp = L;
    /* for each row of L (and S) */
    for (iq = 1; iq <= HFEnvq; ++iq) {
        LOOPIR(NB_BITS_UINT, HFEnvq, HFEnvr, LOOPJR(HFEnvr));
    }
    LOOPIR(HFEnvr, HFEnvq, HFEnvr, LOOPJR(HFEnvr));
}
