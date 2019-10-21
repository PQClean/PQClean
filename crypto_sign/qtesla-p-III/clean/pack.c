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
    size_t i, j;

    for (i = 0, j = 0; i < PARAM_N * PARAM_K; i += 4, j += 15) {
        pk[j   ] = (uint8_t)( t[i  ]     );
        pk[j + 1] = (uint8_t)( t[i  ] >> 8 );
        pk[j + 2] = (uint8_t)( t[i  ] >> 16 );
        pk[j + 3] = (uint8_t)((t[i  ] >> 24) | (t[i + 1] << 6));
        pk[j + 4] = (uint8_t)( t[i + 1] >> 2 );
        pk[j + 5] = (uint8_t)( t[i + 1] >> 10 );
        pk[j + 6] = (uint8_t)( t[i + 1] >> 18 );
        pk[j + 7] = (uint8_t)((t[i + 1] >> 26) | (t[i + 2] << 4));
        pk[j + 8] = (uint8_t)( t[i + 2] >> 4 );
        pk[j + 9] = (uint8_t)( t[i + 2] >> 12 );
        pk[j + 10] = (uint8_t)( t[i + 2] >> 20 );
        pk[j + 11] = (uint8_t)((t[i + 2] >> 28) | (t[i + 3] << 2));
        pk[j + 12] = (uint8_t)( t[i + 3] >> 6 );
        pk[j + 13] = (uint8_t)( t[i + 3] >> 14 );
        pk[j + 14] = (uint8_t)( t[i + 3] >> 22 );
    }

    memcpy(&pk[j], seedA, CRYPTO_SEEDBYTES);
}


void PQCLEAN_QTESLAPIII_CLEAN_decode_pk(int32_t *pk, uint8_t *seedA, const uint8_t *pk_in) {
    // Decode public key pk
    size_t i, j;
    int32_t mask30 = (1 << PARAM_Q_LOG) - 1;
    const uint8_t *a = pk_in;

    for (i = 0, j = 0; i < PARAM_N * PARAM_K; i += 4, j += 15) {
        pk[i  ] = (int32_t)(( a[j   ]     | (a[j + 1] << 8) | (a[j + 2] << 16) | (a[j + 3] << 24)                ) & mask30);
        pk[i + 1] = (int32_t)(((a[j + 3] >> 6) | (a[j + 4] << 2) | (a[j + 5] << 10) | (a[j + 6] << 18) | (a[j + 7] << 26)) & mask30);
        pk[i + 2] = (int32_t)(((a[j + 7] >> 4) | (a[j + 8] << 4) | (a[j + 9] << 12) | (a[j + 10] << 20) | (a[j + 11] << 28)) & mask30);
        pk[i + 3] = (int32_t)( (a[j + 11] >> 2) | (a[j + 12] << 6) | (a[j + 13] << 14) | (a[j + 14] << 22)                          );
    }

    memcpy(seedA, &pk_in[j], CRYPTO_SEEDBYTES);
}


void PQCLEAN_QTESLAPIII_CLEAN_encode_sig(uint8_t *sm, uint8_t *c, const poly z) {
    // Encode signature sm
    size_t i, j;

    for (i = 0, j = 0; i < PARAM_N; i += 4, j += 11) {
        sm[j   ] = (uint8_t)(  z[i  ]    );
        sm[j + 1] = (uint8_t)(  z[i  ] >> 8);
        sm[j + 2] = (uint8_t)(((z[i  ] >> 16) & 0x3F) | (z[i + 1] << 6));
        sm[j + 3] = (uint8_t)(  z[i + 1] >> 2);
        sm[j + 4] = (uint8_t)(  z[i + 1] >> 10);
        sm[j + 5] = (uint8_t)(((z[i + 1] >> 18) & 0x0F) | (z[i + 2] << 4));
        sm[j + 6] = (uint8_t)(  z[i + 2] >> 4);
        sm[j + 7] = (uint8_t)(  z[i + 2] >> 12);
        sm[j + 8] = (uint8_t)(((z[i + 2] >> 20) & 0x03) | (z[i + 3] << 2));
        sm[j + 9] = (uint8_t)(  z[i + 3] >> 6);
        sm[j + 10] = (uint8_t)(  z[i + 3] >> 14);
    }

    memcpy(&sm[j], c, CRYPTO_C_BYTES);
}


void PQCLEAN_QTESLAPIII_CLEAN_decode_sig(uint8_t *c, poly z, const uint8_t *sm) {
    // Decode signature sm
    size_t i, j;

    for (i = 0, j = 0; i < PARAM_N; i += 4, j += 11) {
        z[i  ] =  sm[j  ]     | (sm[j + 1] << 8) |                 (((int64_t)sm[j + 2] << 58) >> 42);
        z[i + 1] = (sm[j + 2] >> 6) | (sm[j + 3] << 2) | (sm[j + 4] << 10) | (((int64_t)sm[j + 5] << 60) >> 42);
        z[i + 2] = (sm[j + 5] >> 4) | (sm[j + 6] << 4) | (sm[j + 7] << 12) | (((int64_t)sm[j + 8] << 62) >> 42);
        z[i + 3] = (sm[j + 8] >> 2) | (sm[j + 9] << 6) |                 (((int64_t)sm[j + 10] << 56) >> 42);
    }

    memcpy(c, &sm[j], CRYPTO_C_BYTES);
}
