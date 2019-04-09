#include "cbd.h"

/*************************************************
 * Name:        load_littleendian
 *
 * Description: load bytes into a 64-bit integer
 *              in little-endian order
 *
 * Arguments:   - const unsigned char *x: pointer to input byte array
 *              - bytes:                  number of bytes to load, has to be <=
 *8
 *
 * Returns 64-bit unsigned integer loaded from x
 **************************************************/
static uint64_t load_littleendian(const unsigned char *x, int bytes) {
    int i;
    uint64_t r = x[0];
    for (i = 1; i < bytes; i++) {
        r |= (uint64_t)x[i] << (8 * i);
    }
    return r;
}

/*************************************************
 * Name:        cbd
 *
 * Description: Given an array of uniformly random bytes, compute
 *              polynomial with coefficients distributed according to
 *              a centered binomial distribution with parameter KYBER_ETA
 *
 * Arguments:   - poly *r:                  pointer to output polynomial
 *              - const unsigned char *buf: pointer to input byte array
 **************************************************/
void PQCLEAN_KYBER768_CLEAN_cbd(poly *r, const unsigned char *buf) {
    uint32_t t, d, a[4], b[4];
    int i, j;

    for (i = 0; i < KYBER_N / 4; i++) {
        t = (uint32_t)load_littleendian(buf + 4 * i, 4);
        d = 0;
        for (j = 0; j < 4; j++) {
            d += (t >> j) & 0x11111111;
        }

        a[0] =  d        & 0xf;
        b[0] = (d >>  4) & 0xf;
        a[1] = (d >>  8) & 0xf;
        b[1] = (d >> 12) & 0xf;
        a[2] = (d >> 16) & 0xf;
        b[2] = (d >> 20) & 0xf;
        a[3] = (d >> 24) & 0xf;
        b[3] = (d >> 28);

        r->coeffs[4 * i + 0] = (uint16_t)(a[0] + KYBER_Q - b[0]);
        r->coeffs[4 * i + 1] = (uint16_t)(a[1] + KYBER_Q - b[1]);
        r->coeffs[4 * i + 2] = (uint16_t)(a[2] + KYBER_Q - b[2]);
        r->coeffs[4 * i + 3] = (uint16_t)(a[3] + KYBER_Q - b[3]);
    }
}
