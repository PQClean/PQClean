#include "evalMQSv_gf2.h"
#include "sizes_HFE.h"
#include "tools_gf2m.h"
#include "tools_gf2n.h"
#include "tools_gf2v.h"


/* This file is a modified copy of evalMQS_gf2.c, used for the constant-time
   evaluation of the constant of the HFEv polynomial.
   The main modification is the parameters NB_VAR and NB_EQ */

/* Input:
    m a vector of v elements of GF(2)
    pk a MQ system with n equations in GF(2)[x1,...,xv]

  Output:
    c a vector of n elements of GF(2), c is the evaluation of pk in m
*/
/* void evalMQSv_gf2(vecn_gf2 c, cst_vecv_gf2 m, cst_mqsv_gf2n pk) */


#define HYBRID_FUNCTIONS 0


/*** These variables define the number of variables and equations ***/
#define NB_VAR (HFEv)
#define NB_EQ (HFEn)


/*** Useful macro ***/

/* NB_VAR = NB_BITS_UINT*quo + rem */
#define NB_VARq (NB_VAR/NB_BITS_UINT)
#define NB_VARr (NB_VAR%NB_BITS_UINT)


/* NB_EQ = NB_BITS_UINT*quo + rem */
#define NB_EQq (NB_EQ/NB_BITS_UINT)
#define NB_EQr (NB_EQ%NB_BITS_UINT)
/* The number of word that an element of GF(2^NB_EQ) needs */
#define NB_WORD_EQ_TMP (NB_EQq+1)


/* To choose macro for NB_WORD_EQ*64 bits */
#define NB_WORD_EQ 5

#define CONCAT_NB_WORD_EQ_SUP(name) CONCAT(name,NB_WORD_EQ)








/* 64-bit version */

#define XOR_ELEM CONCAT(CONCAT_NB_WORD_EQ_SUP(ADD),_2_GF2X)
#define xorLoadMask1_gf2EQ CONCAT_NB_WORD_EQ_SUP(XORLOADMASK1_)

#define COPY_64bits_variables CONCAT_NB_WORD_EQ_SUP(COPY)




/**************************************************************************/
/**************************************************************************/
/*** Classical constant-time evaluation using a mask to multiply by xi. ***/
/**************************************************************************/
/**************************************************************************/




/* Input:
    m a vector of n+v elements of GF(2)
    pk a MQ system with m equations in GF(2)[x1,...,x_(n+v)]

  Output:
    c a vector of m elements of GF(2), c is the evaluation of pk in m
*/
void PQCLEAN_GEMSSBLUE192_AVX2_evalMQSv_classical_gf2(vecm_gf2 c, cst_vecnv_gf2 m,
        cst_mqsnv_gf2m pk) {
    UINT xi;
    UINT x[NB_VAR];
    static_vecm_gf2 tmp[NB_WORD_EQ];
    unsigned int i, j, k;

    /* Compute one time all -((xi>>1)&UINT_1) */
    i = 0;
    k = 0;
    while (k < NB_VAR) {
        xi = m[i];
        for (j = 0; (j < NB_BITS_UINT) && (k < NB_VAR); ++j) {
            x[k] = (1 + ~((xi >> j)&UINT_1));
            ++k;
        }
        ++i;
    }

    /* Constant cst_pk */
    COPY_64bits_variables(c, pk);
    pk += NB_WORD_EQ;

    /* for each row of the quadratic matrix of pk, excepted the last block */
    for (i = 0; i < NB_VAR; ++i) {
        /* for each column of the quadratic matrix of pk */

        /* xj=xi */
        COPY_64bits_variables(tmp, pk)
        pk += NB_WORD_EQ;

        for (j = i + 1; j < NB_VAR; ++j) {
            xorLoadMask1_gf2EQ(tmp, pk, x[j]);
            pk += NB_WORD_EQ;
        }
        /* Multiply by xi */
        xorLoadMask1_gf2EQ(c, tmp, x[i]);
    }
}


void PQCLEAN_GEMSSBLUE192_AVX2_evalMQSv_unrolled_gf2(vecm_gf2 c, cst_vecnv_gf2 m,
        cst_mqsnv_gf2m pk) {
    UINT xi;
    UINT x[NB_VAR];
    static_vecm_gf2 tmp[NB_WORD_EQ];
    unsigned int i, j, k;

    /* Compute one time all -((xi>>1)&UINT_1) */
    i = 0;
    k = 0;
    while (k < NB_VAR) {
        xi = m[i];
        for (j = 0; (j < NB_BITS_UINT) && (k < NB_VAR); ++j) {
            x[k] = (1 + ~((xi >> j)&UINT_1));
            ++k;
        }
        ++i;
    }

    /* Constant cst_pk */
    COPY_64bits_variables(c, pk);
    pk += NB_WORD_EQ;

    /* for each row of the quadratic matrix of pk, excepted the last block */
    for (i = 0; i < NB_VAR; ++i) {
        /* for each column of the quadratic matrix of pk */

        /* xj=xi */
        COPY_64bits_variables(tmp, pk)
        pk += NB_WORD_EQ;

        for (j = i + 1; j < (NB_VAR - 3); j += 4) {
            xorLoadMask1_gf2EQ(tmp, pk, x[j]);
            pk += NB_WORD_EQ;
            xorLoadMask1_gf2EQ(tmp, pk, x[j + 1]);
            pk += NB_WORD_EQ;
            xorLoadMask1_gf2EQ(tmp, pk, x[j + 2]);
            pk += NB_WORD_EQ;
            xorLoadMask1_gf2EQ(tmp, pk, x[j + 3]);
            pk += NB_WORD_EQ;
        }
        for (; j < NB_VAR; ++j) {
            xorLoadMask1_gf2EQ(tmp, pk, x[j]);
            pk += NB_WORD_EQ;
        }
        /* Multiply by xi */
        xorLoadMask1_gf2EQ(c, tmp, x[i]);
    }
}
