/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: portable, constant-time Gaussian sampler
**************************************************************************************/

#include "api.h"
#include "CDT32.h"
#include "gauss.h"
#include "sp800-185.h"
#include <string.h>

void PQCLEAN_QTESLAPIII_CLEAN_sample_gauss_poly(poly z, const uint8_t *seed, uint16_t nonce) {
    uint16_t dmsp = (uint16_t)(nonce << 8);
    int32_t samp[CHUNK_SIZE * CDT_COLS], c[CDT_COLS], borrow, sign;
    const int32_t mask = (int32_t)((uint32_t)(-1) >> 1);
    uint8_t buf[CHUNK_SIZE * CDT_COLS * sizeof(int32_t)];

    for (size_t chunk = 0; chunk < PARAM_N; chunk += CHUNK_SIZE) {
        uint8_t dmsp_bytes[2];
        dmsp_bytes[0] = (uint8_t)(dmsp & 0xff);
        dmsp_bytes[1] = (uint8_t)(dmsp >> 8);
        cSHAKE(buf, CHUNK_SIZE * CDT_COLS * sizeof(int32_t), (uint8_t *)NULL, 0, dmsp_bytes, 2, seed, CRYPTO_RANDOMBYTES);
        ++dmsp;
        for (size_t i = 0, j = 0; i < CHUNK_SIZE * CDT_COLS; i += 1, j += 4) {
            samp[i] = (int32_t)(buf[j] | (buf[j + 1] << 8) | (buf[j + 2] << 16) | (buf[j + 3] << 24));
        }
        for (size_t i = 0; i < CHUNK_SIZE; i++) {
            z[chunk + i] = 0;
            for (size_t j = 1; j < CDT_ROWS; j++) {
                borrow = 0;
                for (size_t k = CDT_COLS; k > 0; ) {
                    k--;
                    c[k] = (samp[i * CDT_COLS + k] & mask) - (cdt_v[j * CDT_COLS + k] + borrow);
                    borrow = c[k] >> (RADIX32 - 1);
                }
                z[chunk + i] += ~borrow & 1;
            }
            sign = samp[i * CDT_COLS] >> (RADIX32 - 1);
            z[chunk + i] = (sign & -z[chunk + i]) | (~sign & z[chunk + i]);
        }
    }
}

