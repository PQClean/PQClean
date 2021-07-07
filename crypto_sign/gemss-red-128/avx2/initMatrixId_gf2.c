#include "initMatrixId_gf2.h"


/**
 * @brief   Generate the identity matrix (n,n) in GF(2).
 * @param[out]  S   The identity matrix (n,n) in GF(2).
 * @remark  Requires to allocate n elements of GF(2^n) for S.
 * @remark  This function does not require a constant-time implementation.
 */
void PQCLEAN_GEMSSRED128_AVX2_initMatrixIdn_gf2(Mn_gf2 S) {
    UINT bit_ir;
    unsigned int iq, ir;

    /* Initialize to 0 */
    for (iq = 0; iq < MATRIXn_SIZE; ++iq) {
        S[iq] = 0;
    }

    /* for each row excepted the last block */
    for (iq = 0; iq < HFEnq; ++iq) {
        bit_ir = 1;
        for (ir = 0; ir < NB_BITS_UINT; ++ir) {
            *S = bit_ir;
            /* Next row */
            S += NB_WORD_GFqn;
            bit_ir <<= 1;
        }
        /* Next block of column */
        ++S;
    }

    /* for each row of the last block */
    bit_ir = 1;
    for (ir = 0; ir < HFEnr; ++ir) {
        *S = bit_ir;
        /* Next row */
        S += NB_WORD_GFqn;
        bit_ir <<= 1;
    }
}

/**
 * @brief   Generate the identity matrix (n+v,n+v) in GF(2).
 * @param[out]  S   The identity matrix (n+v,n+v) in GF(2).
 * @remark  Requires to allocate n+v elements of GF(2^(n+v)) for S.
 * @remark  This function does not require a constant-time implementation.
 */
void PQCLEAN_GEMSSRED128_AVX2_initMatrixIdnv_gf2(Mn_gf2 S) {
    UINT bit_ir;
    unsigned int iq, ir;

    /* Initialize to 0 */
    for (iq = 0; iq < MATRIXnv_SIZE; ++iq) {
        S[iq] = 0;
    }

    /* for each row excepted the last block */
    for (iq = 0; iq < HFEnvq; ++iq) {
        bit_ir = 1;
        for (ir = 0; ir < NB_BITS_UINT; ++ir) {
            *S = bit_ir;
            /* Next row */
            S += NB_WORD_GF2nv;
            bit_ir <<= 1;
        }
        /* Next block of column */
        ++S;
    }

    /* for each row of the last block */
}
