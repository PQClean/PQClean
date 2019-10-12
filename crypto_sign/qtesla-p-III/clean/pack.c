/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: packing functions
**************************************************************************************/

#include "api.h"
#include "pack.h"
#include "params.h"
#include "poly.h"
#include <string.h>

void PQCLEAN_QTESLAPIII_CLEAN_pack_sk(uint8_t *sk, const poly s, const poly_k e, uint8_t *seeds) {
    // Pack secret key sk
    size_t i, k;

    for (i = 0; i < PARAM_N; i++) {
        sk[i] = (uint8_t)s[i];
    }

    sk += PARAM_N;
    for (k = 0; k < PARAM_K; k++) {
        for (i = 0; i < PARAM_N; i++) {
            sk[k * PARAM_N + i] = (uint8_t)e[k * PARAM_N + i];
        }
    }

    memcpy(&sk[PARAM_K * PARAM_N], seeds, 2 * CRYPTO_SEEDBYTES);
}

void PQCLEAN_QTESLAPIII_CLEAN_encode_pk(uint8_t *pk, const poly_k t, const uint8_t *seedA) {
    // Encode public key pk
    size_t i, j = 0;
    uint32_t *pt = (uint32_t *)pk;

    for (i = 0; i < (PARAM_N * PARAM_K * PARAM_Q_LOG / 32); i += 15) {
        pt[i + 0] = (uint32_t)( t[j + 0]        | (t[j + 1] << 30));
        pt[i + 1] = (uint32_t)((t[j + 1] >>  2) | (t[j + 2] << 28));
        pt[i + 2] = (uint32_t)((t[j + 2] >>  4) | (t[j + 3] << 26));
        pt[i + 3] = (uint32_t)((t[j + 3] >>  6) | (t[j + 4] << 24));
        pt[i + 4] = (uint32_t)((t[j + 4] >>  8) | (t[j + 5] << 22));
        pt[i + 5] = (uint32_t)((t[j + 5] >> 10) | (t[j + 6] << 20));
        pt[i + 6] = (uint32_t)((t[j + 6] >> 12) | (t[j + 7] << 18));
        pt[i + 7] = (uint32_t)((t[j + 7] >> 14) | (t[j + 8] << 16));
        pt[i + 8] = (uint32_t)((t[j + 8] >> 16) | (t[j + 9] << 14));
        pt[i + 9] = (uint32_t)((t[j + 9] >> 18) | (t[j + 10] << 12));
        pt[i + 10] = (uint32_t)((t[j + 10] >> 20) | (t[j + 11] << 10));
        pt[i + 11] = (uint32_t)((t[j + 11] >> 22) | (t[j + 12] <<  8));
        pt[i + 12] = (uint32_t)((t[j + 12] >> 24) | (t[j + 13] <<  6));
        pt[i + 13] = (uint32_t)((t[j + 13] >> 26) | (t[j + 14] <<  4));
        pt[i + 14] = (uint32_t)((t[j + 14] >> 28) | (t[j + 15] <<  2));
        j += 16;
    }
    memcpy(&pk[PARAM_N * PARAM_K * PARAM_Q_LOG / 8], seedA, CRYPTO_SEEDBYTES);
}


#define maskq ((1<<PARAM_Q_LOG)-1)

void PQCLEAN_QTESLAPIII_CLEAN_decode_pk(int32_t *pk, uint8_t *seedA, const uint8_t *pk_in) {
    // Decode public key pk
    size_t i, j = 0;
    uint32_t *pt = (uint32_t *)pk_in, *t = (uint32_t *)pk;

    for (i = 0; i < PARAM_N * PARAM_K; i += 16) {
        t[i + 0] = ( pt[j + 0]       ) & maskq;
        t[i + 1] = ((pt[j + 0] >> 30) | (pt[j + 1] <<  2)) & maskq;
        t[i + 2] = ((pt[j + 1] >> 28) | (pt[j + 2] <<  4)) & maskq;
        t[i + 3] = ((pt[j + 2] >> 26) | (pt[j + 3] <<  6)) & maskq;
        t[i + 4] = ((pt[j + 3] >> 24) | (pt[j + 4] <<  8)) & maskq;
        t[i + 5] = ((pt[j + 4] >> 22) | (pt[j + 5] << 10)) & maskq;
        t[i + 6] = ((pt[j + 5] >> 20) | (pt[j + 6] << 12)) & maskq;
        t[i + 7] = ((pt[j + 6] >> 18) | (pt[j + 7] << 14)) & maskq;
        t[i + 8] = ((pt[j + 7] >> 16) | (pt[j + 8] << 16)) & maskq;
        t[i + 9] = ((pt[j + 8] >> 14) | (pt[j + 9] << 18)) & maskq;
        t[i + 10] = ((pt[j + 9] >> 12) | (pt[j + 10] << 20)) & maskq;
        t[i + 11] = ((pt[j + 10] >> 10) | (pt[j + 11] << 22)) & maskq;
        t[i + 12] = ((pt[j + 11] >>  8) | (pt[j + 12] << 24)) & maskq;
        t[i + 13] = ((pt[j + 12] >>  6) | (pt[j + 13] << 26)) & maskq;
        t[i + 14] = ((pt[j + 13] >>  4) | (pt[j + 14] << 28)) & maskq;
        t[i + 15] = ((pt[j + 14] >>  2)) & maskq;
        j += 15;
    }
    memcpy(seedA, &pk_in[PARAM_N * PARAM_K * PARAM_Q_LOG / 8], CRYPTO_SEEDBYTES);
}


#define maskb1 ((1<<(PARAM_B_BITS+1))-1)

void PQCLEAN_QTESLAPIII_CLEAN_encode_sig(uint8_t *sm, uint8_t *c, const poly z) {
    // Encode signature sm
    size_t i, j = 0;
    const uint64_t *t = (const uint64_t *)z;
    uint32_t *pt = (uint32_t *)sm;

    for (i = 0; i < (PARAM_N * (PARAM_B_BITS + 1) / 32); i += 11) {
        pt[i + 0] = (uint32_t)( (t[j + 0]        & ((1 << 22) - 1)) |  (t[j + 1] << 22));
        pt[i + 1] = (uint32_t)(((t[j + 1] >> 10) & ((1 << 12) - 1)) |  (t[j + 2] << 12));
        pt[i + 2] = (uint32_t)(((t[j + 2] >> 20) & ((1 << 2) - 1)) | ((t[j + 3] & maskb1) <<  2) |  (t[j + 4] << 24));
        pt[i + 3] = (uint32_t)(((t[j + 4] >>  8) & ((1 << 14) - 1)) |  (t[j + 5] << 14));
        pt[i + 4] = (uint32_t)(((t[j + 5] >> 18) & ((1 << 4) - 1)) | ((t[j + 6] & maskb1) <<  4) |  (t[j + 7] << 26));
        pt[i + 5] = (uint32_t)(((t[j + 7] >>  6) & ((1 << 16) - 1)) |  (t[j + 8] << 16));
        pt[i + 6] = (uint32_t)(((t[j + 8] >> 16) & ((1 << 6) - 1)) | ((t[j + 9] & maskb1) <<  6) |  (t[j + 10] << 28));
        pt[i + 7] = (uint32_t)(((t[j + 10] >>  4) & ((1 << 18) - 1)) |  (t[j + 11] << 18));
        pt[i + 8] = (uint32_t)(((t[j + 11] >> 14) & ((1 << 8) - 1)) | ((t[j + 12] & maskb1) <<  8) |  (t[j + 13] << 30));
        pt[i + 9] = (uint32_t)(((t[j + 13] >>  2) & ((1 << 20) - 1)) |  (t[j + 14] << 20));
        pt[i + 10] = (uint32_t)(((t[j + 14] >> 12) & ((1 << 10) - 1)) |  (t[j + 15] << 10));
        j += 16;
    }
    memcpy(&sm[PARAM_N * (PARAM_B_BITS + 1) / 8], c, CRYPTO_C_BYTES);
}


void PQCLEAN_QTESLAPIII_CLEAN_decode_sig(uint8_t *c, poly z, const uint8_t *sm) {
    // Decode signature sm
    size_t i, j = 0;
    uint32_t *pt = (uint32_t *)sm;

    for (i = 0; i < PARAM_N; i += 16) {
        z[i + 0] = ((int32_t) pt[j + 0] << 10) >> 10;
        z[i + 1] =  (int32_t)(pt[j + 0] >> 22) | ((int32_t)(pt[j + 1] << 20) >> 10);
        z[i + 2] =  (int32_t)(pt[j + 1] >> 12) | ((int32_t)(pt[j + 2] << 30) >> 10);
        z[i + 3] = ((int32_t) pt[j + 2] <<  8) >> 10;
        z[i + 4] =  (int32_t)(pt[j + 2] >> 24) | ((int32_t)(pt[j + 3] << 18) >> 10);
        z[i + 5] =  (int32_t)(pt[j + 3] >> 14) | ((int32_t)(pt[j + 4] << 28) >> 10);
        z[i + 6] = ((int32_t) pt[j + 4] <<  6) >> 10;
        z[i + 7] =  (int32_t)(pt[j + 4] >> 26) | ((int32_t)(pt[j + 5] << 16) >> 10);
        z[i + 8] =  (int32_t)(pt[j + 5] >> 16) | ((int32_t)(pt[j + 6] << 26) >> 10);
        z[i + 9] = ((int32_t) pt[j + 6] <<  4) >> 10;
        z[i + 10] =  (int32_t)(pt[j + 6] >> 28) | ((int32_t)(pt[j + 7] << 14) >> 10);
        z[i + 11] =  (int32_t)(pt[j + 7] >> 18) | ((int32_t)(pt[j + 8] << 24) >> 10);
        z[i + 12] = ((int32_t) pt[j + 8] <<  2) >> 10;
        z[i + 13] =  (int32_t)(pt[j + 8] >> 30) | ((int32_t)(pt[j + 9] << 12) >> 10);
        z[i + 14] =  (int32_t)(pt[j + 9] >> 20) | ((int32_t)(pt[j + 10] << 22) >> 10);
        z[i + 15] =  (int32_t) pt[j + 10] >> 10;
        j += 11;
    }
    memcpy(c, &sm[PARAM_N * (PARAM_B_BITS + 1) / 8], CRYPTO_C_BYTES);
}
