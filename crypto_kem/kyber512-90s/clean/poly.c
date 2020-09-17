#include "params.h"
#include "cbd.h"
#include "ntt.h"
#include "poly.h"
#include "reduce.h"
#include "symmetric.h"
#include <stdint.h>

/*************************************************
* Name:        PQCLEAN_KYBER51290S_CLEAN_poly_compress
*
* Description: Compression and subsequent serialization of a polynomial
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (of length KYBER_POLYCOMPRESSEDBYTES)
*              - poly *a:    pointer to input polynomial
**************************************************/
void PQCLEAN_KYBER51290S_CLEAN_poly_compress(uint8_t r[KYBER_POLYCOMPRESSEDBYTES], poly *a) {
    unsigned int i = 0, j = 0;
    uint8_t t[8];

    PQCLEAN_KYBER51290S_CLEAN_poly_csubq(a);

    for (i = 0; i < KYBER_N / 8; i++) {
        for (j = 0; j < 8; j++) {
            t[j] = ((((uint16_t)a->coeffs[8 * i + j] << 3) + KYBER_Q / 2) / KYBER_Q) & 7;
        }

        r[0] = (t[0] >> 0) | (t[1] << 3) | (t[2] << 6);
        r[1] = (t[2] >> 2) | (t[3] << 1) | (t[4] << 4) | (t[5] << 7);
        r[2] = (t[5] >> 1) | (t[6] << 2) | (t[7] << 5);
        r += 3;
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER51290S_CLEAN_poly_decompress
*
* Description: De-serialization and subsequent decompression of a polynomial;
*              approximate inverse of PQCLEAN_KYBER51290S_CLEAN_poly_compress
*
* Arguments:   - poly *r:          pointer to output polynomial
*              - const uint8_t *a: pointer to input byte array
*                                  (of length KYBER_POLYCOMPRESSEDBYTES bytes)
**************************************************/
void PQCLEAN_KYBER51290S_CLEAN_poly_decompress(poly *r, const uint8_t a[KYBER_POLYCOMPRESSEDBYTES]) {
    unsigned int i = 0;

    unsigned int j = 0;
    uint8_t t[8];
    for (i = 0; i < KYBER_N / 8; i++) {
        t[0] = (a[0] >> 0);
        t[1] = (a[0] >> 3);
        t[2] = (a[0] >> 6) | (a[1] << 2);
        t[3] = (a[1] >> 1);
        t[4] = (a[1] >> 4);
        t[5] = (a[1] >> 7) | (a[2] << 1);
        t[6] = (a[2] >> 2);
        t[7] = (a[2] >> 5);
        a += 3;

        for (j = 0; j < 8; j++) {
            r->coeffs[8 * i + j] = ((uint16_t)(t[j] & 7) * KYBER_Q + 4) >> 3;
        }
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER51290S_CLEAN_poly_tobytes
*
* Description: Serialization of a polynomial
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (needs space for KYBER_POLYBYTES bytes)
*              - poly *a:    pointer to input polynomial
**************************************************/
void PQCLEAN_KYBER51290S_CLEAN_poly_tobytes(uint8_t r[KYBER_POLYBYTES], poly *a) {
    unsigned int i = 0;
    uint16_t t0 = 0, t1 = 0;

    PQCLEAN_KYBER51290S_CLEAN_poly_csubq(a);

    for (i = 0; i < KYBER_N / 2; i++) {
        t0 = a->coeffs[2 * i];
        t1 = a->coeffs[2 * i + 1];
        r[3 * i + 0] = (uint8_t)(t0 >> 0);
        r[3 * i + 1] = (uint8_t)((t0 >> 8) | (t1 << 4));
        r[3 * i + 2] = (uint8_t)(t1 >> 4);
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER51290S_CLEAN_poly_frombytes
*
* Description: De-serialization of a polynomial;
*              inverse of PQCLEAN_KYBER51290S_CLEAN_poly_tobytes
*
* Arguments:   - poly *r:          pointer to output polynomial
*              - const uint8_t *a: pointer to input byte array
*                                  (of KYBER_POLYBYTES bytes)
**************************************************/
void PQCLEAN_KYBER51290S_CLEAN_poly_frombytes(poly *r, const uint8_t a[KYBER_POLYBYTES]) {
    unsigned int i = 0;
    for (i = 0; i < KYBER_N / 2; i++) {
        r->coeffs[2 * i]   = ((a[3 * i + 0] >> 0) | ((uint16_t)a[3 * i + 1] << 8)) & 0xFFF;
        r->coeffs[2 * i + 1] = ((a[3 * i + 1] >> 4) | ((uint16_t)a[3 * i + 2] << 4)) & 0xFFF;
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER51290S_CLEAN_poly_frommsg
*
* Description: Convert 32-byte message to polynomial
*
* Arguments:   - poly *r:            pointer to output polynomial
*              - const uint8_t *msg: pointer to input message
**************************************************/
void PQCLEAN_KYBER51290S_CLEAN_poly_frommsg(poly *r, const uint8_t msg[KYBER_INDCPA_MSGBYTES]) {
    unsigned int i = 0, j = 0;
    int16_t mask = 0;

    for (i = 0; i < KYBER_N / 8; i++) {
        for (j = 0; j < 8; j++) {
            mask = -(int16_t)((msg[i] >> j) & 1);
            r->coeffs[8 * i + j] = mask & ((KYBER_Q + 1) / 2);
        }
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER51290S_CLEAN_poly_tomsg
*
* Description: Convert polynomial to 32-byte message
*
* Arguments:   - uint8_t *msg: pointer to output message
*              - poly *a:      pointer to input polynomial
**************************************************/
void PQCLEAN_KYBER51290S_CLEAN_poly_tomsg(uint8_t msg[KYBER_INDCPA_MSGBYTES], poly *a) {
    unsigned int i = 0, j = 0;
    uint16_t t = 0;

    PQCLEAN_KYBER51290S_CLEAN_poly_csubq(a);

    for (i = 0; i < KYBER_N / 8; i++) {
        msg[i] = 0;
        for (j = 0; j < 8; j++) {
            t = ((((uint16_t)a->coeffs[8 * i + j] << 1) + KYBER_Q / 2) / KYBER_Q) & 1;
            msg[i] |= t << j;
        }
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER51290S_CLEAN_poly_getnoise
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER_ETA
*
* Arguments:   - poly *r:             pointer to output polynomial
*              - const uint8_t *seed: pointer to input seed
*                                     (of length KYBER_SYMBYTES bytes)
*              - uint8_t nonce:       one-byte input nonce
**************************************************/
void PQCLEAN_KYBER51290S_CLEAN_poly_getnoise(poly *r, const uint8_t seed[KYBER_SYMBYTES], uint8_t nonce) {
    uint8_t buf[KYBER_ETA * KYBER_N / 4];
    prf(buf, sizeof(buf), seed, nonce);
    PQCLEAN_KYBER51290S_CLEAN_cbd(r, buf);
}

/*************************************************
* Name:        PQCLEAN_KYBER51290S_CLEAN_poly_ntt
*
* Description: Computes negacyclic number-theoretic transform (NTT) of
*              a polynomial in place;
*              inputs assumed to be in normal order, output in bitreversed order
*
* Arguments:   - uint16_t *r: pointer to in/output polynomial
**************************************************/
void PQCLEAN_KYBER51290S_CLEAN_poly_ntt(poly *r) {
    PQCLEAN_KYBER51290S_CLEAN_ntt(r->coeffs);
    PQCLEAN_KYBER51290S_CLEAN_poly_reduce(r);
}

/*************************************************
* Name:        PQCLEAN_KYBER51290S_CLEAN_poly_invntt_tomont
*
* Description: Computes inverse of negacyclic number-theoretic transform (NTT)
*              of a polynomial in place;
*              inputs assumed to be in bitreversed order, output in normal order
*
* Arguments:   - uint16_t *a: pointer to in/output polynomial
**************************************************/
void PQCLEAN_KYBER51290S_CLEAN_poly_invntt_tomont(poly *r) {
    PQCLEAN_KYBER51290S_CLEAN_invntt(r->coeffs);
}

/*************************************************
* Name:        PQCLEAN_KYBER51290S_CLEAN_poly_basemul_montgomery
*
* Description: Multiplication of two polynomials in NTT domain
*
* Arguments:   - poly *r:       pointer to output polynomial
*              - const poly *a: pointer to first input polynomial
*              - const poly *b: pointer to second input polynomial
**************************************************/
void PQCLEAN_KYBER51290S_CLEAN_poly_basemul_montgomery(poly *r, const poly *a, const poly *b) {
    unsigned int i = 0;
    for (i = 0; i < KYBER_N / 4; i++) {
        PQCLEAN_KYBER51290S_CLEAN_basemul(&r->coeffs[4 * i], &a->coeffs[4 * i], &b->coeffs[4 * i], PQCLEAN_KYBER51290S_CLEAN_zetas[64 + i]);
        PQCLEAN_KYBER51290S_CLEAN_basemul(&r->coeffs[4 * i + 2], &a->coeffs[4 * i + 2], &b->coeffs[4 * i + 2],
                                          -PQCLEAN_KYBER51290S_CLEAN_zetas[64 + i]);
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER51290S_CLEAN_poly_tomont
*
* Description: Inplace conversion of all coefficients of a polynomial
*              from normal domain to Montgomery domain
*
* Arguments:   - poly *r: pointer to input/output polynomial
**************************************************/
void PQCLEAN_KYBER51290S_CLEAN_poly_tomont(poly *r) {
    unsigned int i = 0;
    const int16_t f = (1ULL << 32) % KYBER_Q;
    for (i = 0; i < KYBER_N; i++) {
        r->coeffs[i] = PQCLEAN_KYBER51290S_CLEAN_montgomery_reduce((int32_t)r->coeffs[i] * f);
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER51290S_CLEAN_poly_reduce
*
* Description: Applies Barrett reduction to all coefficients of a polynomial
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments:   - poly *r: pointer to input/output polynomial
**************************************************/
void PQCLEAN_KYBER51290S_CLEAN_poly_reduce(poly *r) {
    unsigned int i = 0;
    for (i = 0; i < KYBER_N; i++) {
        r->coeffs[i] = PQCLEAN_KYBER51290S_CLEAN_barrett_reduce(r->coeffs[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER51290S_CLEAN_poly_csubq
*
* Description: Applies conditional subtraction of q to each coefficient
*              of a polynomial. For details of conditional subtraction
*              of q see comments in reduce.c
*
* Arguments:   - poly *r: pointer to input/output polynomial
**************************************************/
void PQCLEAN_KYBER51290S_CLEAN_poly_csubq(poly *r) {
    unsigned int i = 0;
    for (i = 0; i < KYBER_N; i++) {
        r->coeffs[i] = PQCLEAN_KYBER51290S_CLEAN_csubq(r->coeffs[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER51290S_CLEAN_poly_add
*
* Description: Add two polynomials
*
* Arguments: - poly *r:       pointer to output polynomial
*            - const poly *a: pointer to first input polynomial
*            - const poly *b: pointer to second input polynomial
**************************************************/
void PQCLEAN_KYBER51290S_CLEAN_poly_add(poly *r, const poly *a, const poly *b) {
    unsigned int i = 0;
    for (i = 0; i < KYBER_N; i++) {
        r->coeffs[i] = a->coeffs[i] + b->coeffs[i];
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER51290S_CLEAN_poly_sub
*
* Description: Subtract two polynomials
*
* Arguments: - poly *r:       pointer to output polynomial
*            - const poly *a: pointer to first input polynomial
*            - const poly *b: pointer to second input polynomial
**************************************************/
void PQCLEAN_KYBER51290S_CLEAN_poly_sub(poly *r, const poly *a, const poly *b) {
    unsigned int i = 0;
    for (i = 0; i < KYBER_N; i++) {
        r->coeffs[i] = a->coeffs[i] - b->coeffs[i];
    }
}
