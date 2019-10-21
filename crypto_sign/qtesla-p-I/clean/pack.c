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

void PQCLEAN_QTESLAPI_CLEAN_pack_sk(uint8_t *sk, const poly s, const poly_k e, uint8_t *seeds) {
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

void PQCLEAN_QTESLAPI_CLEAN_encode_pk(uint8_t *pk, const poly_k t, const uint8_t *seedA) {
    // Encode public key pk
    size_t i, j;

    for (i = 0, j = 0; i < PARAM_N * PARAM_K; i += 8, j += 29) {
        pk[j   ] = (uint8_t)( t[i  ]    );
        pk[j + 1] = (uint8_t)( t[i  ] >> 8);
        pk[j + 2] = (uint8_t)( t[i  ] >> 16);
        pk[j + 3] = (uint8_t)((t[i  ] >> 24) | (t[i + 1] << 5));
        pk[j + 4] = (uint8_t)( t[i + 1] >> 3);
        pk[j + 5] = (uint8_t)( t[i + 1] >> 11);
        pk[j + 6] = (uint8_t)( t[i + 1] >> 19);
        pk[j + 7] = (uint8_t)((t[i + 1] >> 27) | (t[i + 2] << 2));
        pk[j + 8] = (uint8_t)( t[i + 2] >> 6);
        pk[j + 9] = (uint8_t)( t[i + 2] >> 14);
        pk[j + 10] = (uint8_t)((t[i + 2] >> 22) | (t[i + 3] << 7));
        pk[j + 11] = (uint8_t)( t[i + 3] >> 1);
        pk[j + 12] = (uint8_t)( t[i + 3] >> 9);
        pk[j + 13] = (uint8_t)( t[i + 3] >> 17);
        pk[j + 14] = (uint8_t)((t[i + 3] >> 25) | (t[i + 4] << 4));
        pk[j + 15] = (uint8_t)( t[i + 4] >> 4);
        pk[j + 16] = (uint8_t)( t[i + 4] >> 12);
        pk[j + 17] = (uint8_t)( t[i + 4] >> 20);
        pk[j + 18] = (uint8_t)((t[i + 4] >> 28) | (t[i + 5] << 1));
        pk[j + 19] = (uint8_t)( t[i + 5] >> 7);
        pk[j + 20] = (uint8_t)( t[i + 5] >> 15);
        pk[j + 21] = (uint8_t)((t[i + 5] >> 23) | (t[i + 6] << 6));
        pk[j + 22] = (uint8_t)( t[i + 6] >> 2);
        pk[j + 23] = (uint8_t)( t[i + 6] >> 10);
        pk[j + 24] = (uint8_t)( t[i + 6] >> 18);
        pk[j + 25] = (uint8_t)((t[i + 6] >> 26) | (t[i + 7] << 3));
        pk[j + 26] = (uint8_t)( t[i + 7] >> 5);
        pk[j + 27] = (uint8_t)( t[i + 7] >> 13);
        pk[j + 28] = (uint8_t)( t[i + 7] >> 21);
    }

    memcpy(&pk[j], seedA, CRYPTO_SEEDBYTES);
}


void PQCLEAN_QTESLAPI_CLEAN_decode_pk(int32_t *pk, uint8_t *seedA, const uint8_t *pk_in) {
    // Decode public key pk
    size_t i, j;
    int32_t mask29 = (1 << PARAM_Q_LOG) - 1;
    const uint8_t *a = pk_in;

    for (i = 0, j = 0; i < PARAM_N * PARAM_K; i += 8, j += 29) {
        pk[i  ] = (int32_t)(( a[j   ]     | (a[j + 1] << 8) | (a[j + 2] << 16) | (a[j + 3] << 24)                ) & mask29);
        pk[i + 1] = (int32_t)(((a[j + 3] >> 5) | (a[j + 4] << 3) | (a[j + 5] << 11) | (a[j + 6] << 19) | (a[j + 7] << 27)) & mask29);
        pk[i + 2] = (int32_t)(((a[j + 7] >> 2) | (a[j + 8] << 6) | (a[j + 9] << 14) | (a[j + 10] << 22)                ) & mask29);
        pk[i + 3] = (int32_t)(((a[j + 10] >> 7) | (a[j + 11] << 1) | (a[j + 12] << 9) | (a[j + 13] << 17) | (a[j + 14] << 25)) & mask29);
        pk[i + 4] = (int32_t)(((a[j + 14] >> 4) | (a[j + 15] << 4) | (a[j + 16] << 12) | (a[j + 17] << 20) | (a[j + 18] << 28)) & mask29);
        pk[i + 5] = (int32_t)(((a[j + 18] >> 1) | (a[j + 19] << 7) | (a[j + 20] << 15) | (a[j + 21] << 23)                ) & mask29);
        pk[i + 6] = (int32_t)(((a[j + 21] >> 6) | (a[j + 22] << 2) | (a[j + 23] << 10) | (a[j + 24] << 18) | (a[j + 25] << 26)) & mask29);
        pk[i + 7] = (int32_t)( (a[j + 25] >> 3) | (a[j + 26] << 5) | (a[j + 27] << 13) | (a[j + 28] << 21)                          );
    }

    memcpy(seedA, &pk_in[j], CRYPTO_SEEDBYTES);
}


void PQCLEAN_QTESLAPI_CLEAN_encode_sig(uint8_t *sm, uint8_t *c, const poly z) {
    // Encode signature sm
    size_t i, j;

    for (i = 0, j = 0; i < PARAM_N; i += 2, j += 5) {
        sm[j  ] = (uint8_t)(  z[i  ]    );
        sm[j + 1] = (uint8_t)(  z[i  ] >> 8);
        sm[j + 2] = (uint8_t)(((z[i  ] >> 16) & 0x0F) | (z[i + 1] << 4));
        sm[j + 3] = (uint8_t)(  z[i + 1] >> 4);
        sm[j + 4] = (uint8_t)(  z[i + 1] >> 12);
    }

    memcpy(&sm[j], c, CRYPTO_C_BYTES);
}

void PQCLEAN_QTESLAPI_CLEAN_decode_sig(uint8_t *c, poly z, const uint8_t *sm) {
    // Decode signature sm
    size_t i, j;

    for (i = 0, j = 0; i < PARAM_N; i += 2, j += 5) {
        z[i  ] =  sm[j  ]     | (sm[j + 1] << 8) | (((int64_t)sm[j + 2] << 60) >> 44);
        z[i + 1] = (sm[j + 2] >> 4) | (sm[j + 3] << 4) | (((int64_t)sm[j + 4] << 56) >> 44);
    }

    memcpy(c, &sm[j], CRYPTO_C_BYTES);
}

