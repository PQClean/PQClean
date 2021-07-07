#include "add_gf2x.h"
#include "evalMQSnocst8_gf2.h"
#include "init.h"
#include "simd.h"
#include "tools_gf2m.h"



/***************************************************************/
/***************************************************************/
/**** Classical variable-time evaluation using accumulator. ****/
/***************************************************************/
/***************************************************************/


/*** These variables define the number of variables and equations ***/
#define NB_VAR (HFEnv)
#define NB_EQ (HFEm)



/*** Useful macro ***/

/* NB_VAR = NB_BITS_UINT*quo + rem */
#define NB_VARq (NB_VAR/NB_BITS_UINT)
#define NB_VARr (NB_VAR%NB_BITS_UINT)


/* NB_EQ = 8*quo + rem */
#define NB_EQq (NB_EQ>>3)
#define NB_EQr (NB_EQ&7)
/* The number of bytes that an element of GF(2^NB_EQ) needs */
#define NB_BYTES_EQ (NB_EQq+1)

#define MASK_EQ ((UINT_1<<(34))-UINT_1)
#define MASK_64(c) (c)&=MASK_EQ;
#define MASK2_64(c,a) (c)=(a)&MASK_EQ;


/* To choose macro for NB_WORD_EQ*64 bits */
#define NB_WORD_EQ 3

#define CONCAT_NB_WORD_EQ_SUP(name) CONCAT(name,NB_WORD_EQ)



/*** Tuning ***/
#define LEN_UNROLLED_64 4







/* 64-bit version */

#define XOR_ELEM CONCAT(CONCAT_NB_WORD_EQ_SUP(ADD),_2_GF2X)

#define COPY_64bits_variables CONCAT_NB_WORD_EQ_SUP(COPY)



#define LOOPJR_NOCST_64(START,NB_IT) \
    for(jr=(START);jr<(NB_IT);++jr)\
    {\
        if(xj&UINT_1)\
        {\
            XOR_ELEM(c,(const UINT*)pk);\
        }\
        pk+=NB_BYTES_EQ;\
        xj>>=1;\
    }



#define LOOPJR_UNROLLED_64(START,NB_IT) \
    for(jr=(START);jr<((NB_IT)-LEN_UNROLLED_64+1);jr+=LEN_UNROLLED_64)\
    {\
        for(h=0;h<LEN_UNROLLED_64;++h)\
        {\
            if(xj&UINT_1)\
            {\
                XOR_ELEM(c,(const UINT*)pk);\
            }\
            pk+=NB_BYTES_EQ;\
            xj>>=1;\
        }\
    }\
    for(;jr<(NB_IT);++jr)\
    {\
        if(xj&UINT_1)\
        {\
            XOR_ELEM(c,(const UINT*)pk);\
        }\
        pk+=NB_BYTES_EQ;\
        xj>>=1;\
    }



/* Requirement:
            to increase the size of pk by (8-(NB_BYTES_EQ&7))&7 bytes */
void PQCLEAN_GEMSSRED128_AVX2_evalMQSnocst8_unrolled_gf2(vecm_gf2 c, cst_vecnv_gf2 m,
        cst_mqsnv8_gf2m pk) {
    UINT xi, xj;
    unsigned int iq, ir, i = NB_VAR, jq, jr;
    unsigned int h;

    /* Constant cst_pk */
    COPY_64bits_variables(c, (const UINT *)pk);
    pk += NB_BYTES_EQ;

    /* for each row of the quadratic matrix of pk, excepted the last block */
    for (iq = 0; iq < NB_VARq; ++iq) {
        xi = m[iq];
        for (ir = 0; ir < NB_BITS_UINT; ++ir, --i) {
            if (xi & UINT_1) {

                /* for each column of the quadratic matrix of pk */

                /* xj=xi=1 */
                XOR_ELEM(c, (const UINT *)pk);
                pk += NB_BYTES_EQ;

                xj = xi >> 1;
                LOOPJR_UNROLLED_64(ir + 1, NB_BITS_UINT);
                for (jq = iq + 1; jq < NB_VARq; ++jq) {
                    xj = m[jq];
                    LOOPJR_UNROLLED_64(0, NB_BITS_UINT);
                }
            } else {
                pk += i * NB_BYTES_EQ;
            }
            xi >>= 1;
        }
    }

    /* the last block */

    MASK_64(c[NB_WORD_EQ - 1]);
}
