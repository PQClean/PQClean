/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: sampling functions
**************************************************************************************/

#include "api.h"
#include "params.h"
#include "sample.h"
#include "sp800-185.h"

#define NBLOCKS_SHAKE     (SHAKE_RATE/(((PARAM_B_BITS+1)+7)/8))
#define BPLUS1BYTES       (((PARAM_B_BITS+1)+7)/8)


void PQCLEAN_QTESLAPIII_CLEAN_sample_y(poly y, const uint8_t *seed, uint16_t nonce) {
    // Sample polynomial y, such that each coefficient is in the range [-B,B]
    size_t i = 0, pos = 0, nblocks = PARAM_N;
    uint8_t buf[PARAM_N * BPLUS1BYTES + 1];
    uint16_t dmsp = (uint16_t)(nonce << 8);
    uint8_t dmsp_bytes[2];

    dmsp_bytes[0] = (uint8_t)(dmsp & 0xff);
    dmsp_bytes[1] = (uint8_t)(dmsp >> 8);
    cSHAKE(buf, PARAM_N * BPLUS1BYTES, (uint8_t *)NULL, 0, dmsp_bytes, 2, seed, CRYPTO_RANDOMBYTES);
    ++dmsp;

    while (i < PARAM_N) {
        if (pos >= nblocks * BPLUS1BYTES) {
            nblocks = NBLOCKS_SHAKE;
            dmsp_bytes[0] = (uint8_t)(dmsp & 0xff);
            dmsp_bytes[1] = (uint8_t)(dmsp >> 8);
            cSHAKE(buf, SHAKE_RATE, (uint8_t *)NULL, 0, dmsp_bytes, 2, seed, CRYPTO_RANDOMBYTES);
            ++dmsp;
            pos = 0;
        }
        y[i] = (uint32_t)((buf[pos] | (buf[pos + 1] << 8) | (buf[pos + 2] << 16)) & ((1 << (PARAM_B_BITS + 1)) - 1));
        y[i] -= PARAM_B;
        if (y[i] != (1 << PARAM_B_BITS)) {
            i++;
        }
        pos += BPLUS1BYTES;
    }
}


void PQCLEAN_QTESLAPIII_CLEAN_encode_c(uint32_t *pos_list, int16_t *sign_list, uint8_t *c_bin) {
    // Encoding of c' by mapping the output of the hash function H to an N-element vector with entries {-1,0,1}
    size_t i, pos, cnt = 0;
    int16_t c[PARAM_N];
    uint8_t r[SHAKE128_RATE];
    uint16_t dmsp = 0;
    uint8_t dmsp_bytes[2];

    // Use the hash value as key to generate some randomness
    dmsp_bytes[0] = (uint8_t)(dmsp & 0xff);
    dmsp_bytes[1] = (uint8_t)(dmsp >> 8);
    cshake128(r, SHAKE128_RATE, (uint8_t *)NULL, 0, dmsp_bytes, 2, c_bin, CRYPTO_RANDOMBYTES);
    ++dmsp;

    // Use rejection sampling to determine positions to be set in the new vector
    for (i = 0; i < PARAM_N; i++) {
        c[i] = 0;
    }

    for (i = 0; i < PARAM_H;) { // Sample a unique position k times. Use two bytes
        if (cnt > (SHAKE128_RATE - 3)) {
            dmsp_bytes[0] = (uint8_t)(dmsp & 0xff);
            dmsp_bytes[1] = (uint8_t)(dmsp >> 8);
            cshake128(r, SHAKE128_RATE, (uint8_t *)NULL, 0, dmsp_bytes, 2, c_bin, CRYPTO_RANDOMBYTES);
            ++dmsp;
            cnt = 0;
        }
        pos = (size_t)((r[cnt] << 8) | (r[cnt + 1]));
        pos = pos & (PARAM_N - 1); // Position is in the range [0,N-1]

        if (c[pos] == 0) {        // Position has not been set yet. Determine sign
            if ((r[cnt + 2] & 1) == 1) {
                c[pos] = -1;
            } else {
                c[pos] = 1;
            }
            pos_list[i] = (uint32_t)pos;
            sign_list[i] = c[pos];
            i++;
        }
        cnt += 3;
    }
}
