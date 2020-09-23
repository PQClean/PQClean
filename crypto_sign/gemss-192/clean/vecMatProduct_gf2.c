#include "tools_gf2m.h"
#include "tools_gf2n.h"
#include "tools_gf2nv.h"
#include "tools_gf2v.h"
#include "vecMatProduct_gf2.h"


/* Compute the vector matrix product */


/* for a block of bits of vec */
#define LOOPIR_M(NB_IT) \
    for(ir=0;ir<(NB_IT);++ir)\
    {\
        /* multiply the (iq*NB_BITS_UINT)+ir bit of vec
            by the (iq*NB_BITS_UINT)+ir row of S */\
        vec_ir=-(bit_ir&1);\
        xorLoadMask1_gf2m(res,S_cp,vec_ir);\
        /* next row of S */\
        S_cp+=NB_WORD_GFqn;\
        bit_ir>>=1;\
    }

/* for a block of bits of vec */
#define LOOPIR_N(NB_IT) \
    for(ir=0;ir<(NB_IT);++ir)\
    {\
        /* multiply the (iq*NB_BITS_UINT)+ir bit of vec
            by the (iq*NB_BITS_UINT)+ir row of S */\
        vec_ir=-(bit_ir&1);\
        xorLoadMask1_gf2n(res,S_cp,vec_ir);\
        /* next row of S */\
        S_cp+=NB_WORD_GFqn;\
        bit_ir>>=1;\
    }

/* for a block of bits of vec */
#define LOOPIR_START_N(NB_IT) \
    for(;ir<(NB_IT);++ir)\
    {\
        /* multiply the (iq*NB_BITS_UINT)+ir bit of vec
            by the (iq*NB_BITS_UINT)+ir row of S */\
        vec_ir=-(bit_ir&1);\
        xorLoadMask1_gf2n(res,S_cp,vec_ir);\
        /* next row of S */\
        S_cp+=NB_WORD_GFqn;\
        bit_ir>>=1;\
    }

/* for a block of bits of vec */
#define LOOPIR_NV(NB_IT) \
    for(ir=0;ir<(NB_IT);++ir)\
    {\
        /* multiply the (iq*NB_BITS_UINT)+ir bit of vec
            by the (iq*NB_BITS_UINT)+ir row of S */\
        vec_ir=-(bit_ir&1);\
        xorLoadMask1_gf2nv(res,S_cp,vec_ir);\
        /* next row of S */\
        S_cp+=NB_WORD_GF2nv;\
        bit_ir>>=1;\
    }


#define REM_V(LOOPIR) \
    bit_ir=vec[HFEvq]; \
    LOOPIR(HFEvr);

#define CLEAN_M \
    LOAD_UINT(vec_ir, (((unsigned char *)res)+(8*(NB_WORD_GF2m-1)))) \
    vec_ir &= MASK_GF2m; \
    STORE_UINT((((unsigned char *)res)+(8*(NB_WORD_GF2m-1))), vec_ir)

#define REM_N(LOOPIR) \
    bit_ir=vec[HFEnq]; \
    LOOPIR(HFEnr);

#define REM_START_N(LOOPIR) \
    bit_ir=vec[HFEnq]>>ir; \
    LOOPIR(HFEnr);

#define REM_M(LOOPIR) \
    bit_ir=vec[HFEnq]; \
    LOOPIR(HFEnr); \
    CLEAN_M

#define REM_NV(LOOPIR) \
    bit_ir=vec[HFEnvq]; \
    LOOPIR(HFEnvr);

#define REM_START_NV(LOOPIR) \
    bit_ir=vec[HFEnvq]>>ir; \
    LOOPIR(HFEnvr);


/*  Input:
        vec a vector of length n in GF(2)
        S a matrix n*n in GF(2)

    Output:
        res a vector of length n in GF(2)
        res = dotproduct(v,S) = v.S
*/
void PQCLEAN_GEMSS192_CLEAN_vecMatProductnv_64(vecn_gf2 res, cst_vecn_gf2 vec, cst_Mn_gf2 S) {
    cst_Mn_gf2 S_cp;
    UINT bit_ir, vec_ir;
    unsigned int iq, ir;

    /* initialization of res to 0 */
    set0_gf2nv(res);

    S_cp = S;
    /* for each bit of vec excepted the last block */
    for (iq = 0; iq < HFEnvq; ++iq) {
        bit_ir = vec[iq];
        LOOPIR_NV(NB_BITS_UINT);
    }
    /* the last block */
    REM_NV(LOOPIR_NV);
}

void PQCLEAN_GEMSS192_CLEAN_vecMatProductnvn_64(vecn_gf2 res, cst_vecn_gf2 vec, cst_Mn_gf2 S) {
    cst_Mn_gf2 S_cp;
    UINT bit_ir, vec_ir;
    unsigned int iq, ir;

    /* initialization of res to 0 */
    set0_gf2n(res);

    S_cp = S;
    /* for each bit of vec excepted the last block */
    for (iq = 0; iq < HFEnvq; ++iq) {
        bit_ir = vec[iq];
        LOOPIR_N(NB_BITS_UINT);
    }
    /* the last block */
    REM_NV(LOOPIR_N);
}

void PQCLEAN_GEMSS192_CLEAN_vecMatProductv_64(vecn_gf2 res, cst_vecn_gf2 vec, cst_Mn_gf2 S) {
    cst_Mn_gf2 S_cp;
    UINT bit_ir, vec_ir;
    unsigned int iq, ir;

    /* initialization of res to 0 */
    set0_gf2n(res);

    S_cp = S;
    /* for each bit of vec excepted the last block */
    iq = 0;
    ir = 0;
    while (NB_BITS_UINT * iq + ir < HFEv) {
        bit_ir = vec[iq];
        for (ir = 0; (NB_BITS_UINT * iq + ir < HFEv) && (ir < NB_BITS_UINT); ++ir) {
            /* multiply the (iq*NB_BITS_UINT)+ir bit of vec
                by the (iq*NB_BITS_UINT)+ir row of S */
            vec_ir = -(bit_ir & 1);
            xorLoadMask1_gf2n(res, S_cp, vec_ir);
            /* next row of S */
            S_cp += NB_WORD_GFqn;
            bit_ir >>= 1;
        }
        ++iq;
    }
}

void PQCLEAN_GEMSS192_CLEAN_vecMatProductn_64(vecn_gf2 res, cst_vecn_gf2 vec, cst_Mn_gf2 S) {
    cst_Mn_gf2 S_cp;
    UINT bit_ir, vec_ir;
    unsigned int iq, ir;

    /* initialization of res to 0 */
    set0_gf2n(res);

    S_cp = S;
    /* for each bit of vec excepted the last block */
    for (iq = 0; iq < HFEnq; ++iq) {
        bit_ir = vec[iq];
        LOOPIR_N(NB_BITS_UINT);
    }
    /* the last block */
    REM_N(LOOPIR_N);
}

void PQCLEAN_GEMSS192_CLEAN_vecMatProductm_64(vecn_gf2 res, cst_vecn_gf2 vec, cst_Mn_gf2 S) {
    cst_Mn_gf2 S_cp;
    UINT bit_ir, vec_ir;
    unsigned int iq, ir;

    /* initialization of res to 0 */
    set0_gf2m(res);

    S_cp = S;
    /* for each bit of vec excepted the last block */
    for (iq = 0; iq < HFEnq; ++iq) {
        bit_ir = vec[iq];
        LOOPIR_M(NB_BITS_UINT);
    }
    /* the last block */
    REM_M(LOOPIR_M);
}


void PQCLEAN_GEMSS192_CLEAN_vecMatProductnvn_start_64(vecn_gf2 res, cst_vecn_gf2 vec, cst_Mn_gf2 S, unsigned int start) {
    cst_Mn_gf2 S_cp;
    UINT bit_ir, vec_ir;
    unsigned int iq, ir;

    /* initialization of res to 0 */
    set0_gf2n(res);

    S_cp = S;
    ir = start & 63U;
    /* for each bit of vec excepted the last block */
    for (iq = start >> 6; iq < HFEnvq; ++iq) {
        bit_ir = vec[iq] >> ir;
        LOOPIR_START_N(NB_BITS_UINT);
        ir = 0;
    }
    /* the last block */
    REM_START_NV(LOOPIR_START_N);
}
