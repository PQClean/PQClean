
/*
 * This file is licensed
 * under Apache 2.0 (https://www.apache.org/licenses/LICENSE-2.0.html) or
 * public domain at https://github.com/pq-crystals/kyber/tree/master/ref
 */

#include <stdint.h>
#include "params.h"
#include "poly.h"
#include "polyvec.h"

/*************************************************
* Name:        polyvec_compress
*
* Description: Compress and serialize vector of polynomials
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (needs space for KYBER_POLYVECCOMPRESSEDBYTES)
*              - const polyvec *a: pointer to input vector of polynomials
**************************************************/
void polyvec_compress(uint8_t r[KYBER_POLYVECCOMPRESSEDBYTES], const int16_t b[KYBER_K][KYBER_N]) {
    unsigned int i, j, k;
    uint64_t d0;

    uint16_t t[8];
    for (i = 0; i < KYBER_K; i++) {
        for (j = 0; j < KYBER_N / 8; j++) {
            for (k = 0; k < 8; k++) {
                t[k]  = b[i][8 * j + k];
                t[k] += ((int16_t)t[k] >> 15) & KYBER_Q;
                /*      t[k]  = ((((uint32_t)t[k] << 11) + KYBER_Q/2)/KYBER_Q) & 0x7ff; */
                d0 = t[k];
                d0 <<= 11;
                d0 += 1664;
                d0 *= 645084;
                d0 >>= 31;
                t[k] = d0 & 0x7ff;

            }

            r[ 0] = (uint8_t)(t[0] >>  0);
            r[ 1] = (uint8_t)((t[0] >>  8) | (t[1] << 3));
            r[ 2] = (uint8_t)((t[1] >>  5) | (t[2] << 6));
            r[ 3] = (uint8_t)(t[2] >>  2);
            r[ 4] = (uint8_t)((t[2] >> 10) | (t[3] << 1));
            r[ 5] = (uint8_t)((t[3] >>  7) | (t[4] << 4));
            r[ 6] = (uint8_t)((t[4] >>  4) | (t[5] << 7));
            r[ 7] = (uint8_t)(t[5] >>  1);
            r[ 8] = (uint8_t)((t[5] >>  9) | (t[6] << 2));
            r[ 9] = (uint8_t)((t[6] >>  6) | (t[7] << 5));
            r[10] = (uint8_t)(t[7] >>  3);
            r += 11;
        }
    }
}

/*************************************************
* Name:        polyvec_decompress
*
* Description: De-serialize and decompress vector of polynomials;
*              approximate inverse of polyvec_compress
*
* Arguments:   - polyvec *r:       pointer to output vector of polynomials
*              - const uint8_t *a: pointer to input byte array
*                                  (of length KYBER_POLYVECCOMPRESSEDBYTES)
**************************************************/
void polyvec_decompress(int16_t r[KYBER_K][KYBER_N], const uint8_t a[KYBER_POLYVECCOMPRESSEDBYTES]) {
    unsigned int i, j, k;

    uint16_t t[8];
    for (i = 0; i < KYBER_K; i++) {
        for (j = 0; j < KYBER_N / 8; j++) {
            t[0] = (a[0] >> 0) | ((uint16_t)a[ 1] << 8);
            t[1] = (a[1] >> 3) | ((uint16_t)a[ 2] << 5);
            t[2] = (a[2] >> 6) | ((uint16_t)a[ 3] << 2) | ((uint16_t)a[4] << 10);
            t[3] = (a[4] >> 1) | ((uint16_t)a[ 5] << 7);
            t[4] = (a[5] >> 4) | ((uint16_t)a[ 6] << 4);
            t[5] = (a[6] >> 7) | ((uint16_t)a[ 7] << 1) | ((uint16_t)a[8] << 9);
            t[6] = (a[8] >> 2) | ((uint16_t)a[ 9] << 6);
            t[7] = (a[9] >> 5) | ((uint16_t)a[10] << 3);
            a += 11;

            for (k = 0; k < 8; k++) {
                r[i][8 * j + k] = ((uint32_t)(t[k] & 0x7FF) * KYBER_Q + 1024) >> 11;
            }
        }
    }
}

/*************************************************
* Name:        polyvec_tobytes
*
* Description: Serialize vector of polynomials
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (needs space for KYBER_POLYVECBYTES)
*              - const polyvec *a: pointer to input vector of polynomials
**************************************************/
void polyvec_tobytes(uint8_t r[KYBER_POLYVECBYTES], const int16_t a[KYBER_K][KYBER_N]) {
    unsigned int i;
    for (i = 0; i < KYBER_K; i++) {
        poly_tobytes(r + i * KYBER_POLYBYTES, a[i]);
    }
}

/*************************************************
* Name:        polyvec_frombytes
*
* Description: De-serialize vector of polynomials;
*              inverse of polyvec_tobytes
*
* Arguments:   - uint8_t *r:       pointer to output byte array
*              - const polyvec *a: pointer to input vector of polynomials
*                                  (of length KYBER_POLYVECBYTES)
**************************************************/
void polyvec_frombytes(int16_t r[KYBER_K][KYBER_N], const uint8_t a[KYBER_POLYVECBYTES]) {
    unsigned int i;
    for (i = 0; i < KYBER_K; i++) {
        poly_frombytes(r[i], a + i * KYBER_POLYBYTES);
    }
}
