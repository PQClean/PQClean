/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: NTT, modular reduction and polynomial functions
**************************************************************************************/

#include "api.h"
#include "poly.h"
#include "sp800-185.h"

extern const poly PQCLEAN_QTESLAPI_CLEAN_zeta;
extern const poly PQCLEAN_QTESLAPI_CLEAN_zetainv;



static int64_t reduce(int64_t a) {
    // Montgomery reduction
    int64_t u;

    u = ((uint64_t)a * PARAM_QINV) & 0xFFFFFFFF;
    u *= PARAM_Q;
    a += u;
    return a >> 32;
}


static int64_t barr_reduce(int64_t a) {
    // Barrett reduction
    int64_t u = (a * PARAM_BARR_MULT) >> PARAM_BARR_DIV;
    return a - u * PARAM_Q;
}


static void ntt(poly a, const poly w) {
    // Forward NTT transform
    size_t NumoProblems = PARAM_N >> 1, jTwiddle = 0;

    for (; NumoProblems > 0; NumoProblems >>= 1) {
        size_t jFirst, j = 0;
        for (jFirst = 0; jFirst < PARAM_N; jFirst = j + NumoProblems) {
            sdigit_t W = (sdigit_t)w[jTwiddle++];
            for (j = jFirst; j < jFirst + NumoProblems; j++) {
                int64_t temp = reduce((int64_t)W * a[j + NumoProblems]);
                a[j + NumoProblems] = a[j] + (PARAM_Q - temp);
                a[j] = temp + a[j];
            }
        }
    }
}


static void nttinv(poly a, const poly w) {
    // Inverse NTT transform
    size_t NumoProblems = 1, jTwiddle = 0;
    for (; NumoProblems < PARAM_N; NumoProblems *= 2) {
        size_t jFirst, j = 0;
        for (jFirst = 0; jFirst < PARAM_N; jFirst = j + NumoProblems) {
            sdigit_t W = (sdigit_t)w[jTwiddle++];
            for (j = jFirst; j < jFirst + NumoProblems; j++) {
                int64_t temp = a[j];
                a[j] = (temp + a[j + NumoProblems]);
                a[j + NumoProblems] = reduce((int64_t)W * (temp + (2 * PARAM_Q - a[j + NumoProblems])));
            }
        }
        NumoProblems *= 2;
        for (jFirst = 0; jFirst < PARAM_N; jFirst = j + NumoProblems) {
            sdigit_t W = (sdigit_t)w[jTwiddle++];
            for (j = jFirst; j < jFirst + NumoProblems; j++) {
                int64_t temp = a[j];
                a[j] = barr_reduce(temp + a[j + NumoProblems]);
                a[j + NumoProblems] = reduce((int64_t)W * (temp + (2 * PARAM_Q - a[j + NumoProblems])));
            }
        }
    }
}


static void poly_pointwise(poly result, const poly x, const poly y) {
    // Pointwise polynomial multiplication result = x.y

    for (size_t i = 0; i < PARAM_N; i++) {
        result[i] = reduce(x[i] * y[i]);
    }
}

void PQCLEAN_QTESLAPI_CLEAN_poly_ntt(poly x_ntt, const poly x) {
    // Call to NTT function. Avoids input destruction

    for (size_t i = 0; i < PARAM_N; i++) {
        x_ntt[i] = x[i];
    }
    ntt(x_ntt, PQCLEAN_QTESLAPI_CLEAN_zeta);
}


void PQCLEAN_QTESLAPI_CLEAN_poly_mul(poly result, const poly x, const poly y) {
    // Polynomial multiplication result = x*y, with in place reduction for (X^N+1)
    // The inputs x and y are assumed to be in NTT form

    poly_pointwise(result, x, y);
    nttinv(result, PQCLEAN_QTESLAPI_CLEAN_zetainv);
}


void PQCLEAN_QTESLAPI_CLEAN_poly_add(poly result, const poly x, const poly y) {
    // Polynomial addition result = x+y

    for (size_t i = 0; i < PARAM_N; i++) {
        result[i] = x[i] + y[i];
    }
}


void PQCLEAN_QTESLAPI_CLEAN_poly_add_correct(poly result, const poly x, const poly y) {
    // Polynomial addition result = x+y with correction

    for (size_t i = 0; i < PARAM_N; i++) {
        result[i] = x[i] + y[i];
        result[i] -= PARAM_Q;
        result[i] += (result[i] >> (RADIX32 - 1)) & PARAM_Q;  // If result[i] >= q then subtract q
    }
}


void PQCLEAN_QTESLAPI_CLEAN_poly_sub(poly result, const poly x, const poly y) {
    // Polynomial subtraction result = x-y

    for (size_t i = 0; i < PARAM_N; i++) {
        result[i] = barr_reduce(x[i] - y[i]);
    }
}

/********************************************************************************************
* Name:        sparse_mul8
* Description: performs sparse polynomial multiplication
* Parameters:  inputs:
*              - const uint8_t *s: part of the secret key
*              - const uint32_t pos_list[PARAM_H]: list of indices of nonzero elements in c
*              - const int16_t sign_list[PARAM_H]: list of signs of nonzero elements in c
*              outputs:
*              - poly prod: product of 2 polynomials
*
* Note: pos_list[] and sign_list[] contain public information since c is public
*********************************************************************************************/
void PQCLEAN_QTESLAPI_CLEAN_sparse_mul8(poly prod, const uint8_t *s, const uint32_t pos_list[PARAM_H], const int16_t sign_list[PARAM_H]) {
    size_t i, j, pos;
    int8_t *t = (int8_t *)s;

    for (i = 0; i < PARAM_N; i++) {
        prod[i] = 0;
    }

    for (i = 0; i < PARAM_H; i++) {
        pos = pos_list[i];
        for (j = 0; j < pos; j++) {
            prod[j] = prod[j] - sign_list[i] * t[j + PARAM_N - pos];
        }
        for (j = pos; j < PARAM_N; j++) {
            prod[j] = prod[j] + sign_list[i] * t[j - pos];
        }
    }
}

/********************************************************************************************
* Name:        sparse_mul32
* Description: performs sparse polynomial multiplication
* Parameters:  inputs:
*              - const int32_t* pk: part of the public key
*              - const uint32_t pos_list[PARAM_H]: list of indices of nonzero elements in c
*              - const int16_t sign_list[PARAM_H]: list of signs of nonzero elements in c
*              outputs:
*              - poly prod: product of 2 polynomials
*********************************************************************************************/
void PQCLEAN_QTESLAPI_CLEAN_sparse_mul32(poly prod, const int32_t *pk, const uint32_t pos_list[PARAM_H], const int16_t sign_list[PARAM_H]) {
    size_t i, j, pos;

    for (i = 0; i < PARAM_N; i++) {
        prod[i] = 0;
    }

    for (i = 0; i < PARAM_H; i++) {
        pos = pos_list[i];
        for (j = 0; j < pos; j++) {
            prod[j] = prod[j] - sign_list[i] * pk[j + PARAM_N - pos];
        }
        for (j = pos; j < PARAM_N; j++) {
            prod[j] = prod[j] + sign_list[i] * pk[j - pos];
        }
    }
    for (i = 0; i < PARAM_N; i++) {
        prod[i] = barr_reduce(prod[i]);
    }
}

void PQCLEAN_QTESLAPI_CLEAN_poly_uniform(poly_k a, const uint8_t *seed) {
    // Generation of polynomials "a_i"
    size_t pos = 0, i = 0, nbytes = (PARAM_Q_LOG + 7) / 8;
    size_t nblocks = PARAM_GEN_A;
    uint32_t val1, val2, val3, val4, mask = (uint32_t)(1 << PARAM_Q_LOG) - 1;
    uint8_t buf[SHAKE128_RATE * PARAM_GEN_A];
    uint16_t dmsp = 0;
    uint8_t dmsp_bytes[2];
    dmsp_bytes[0] = (uint8_t)(dmsp & 0xff);
    dmsp_bytes[1] = (uint8_t)(dmsp >> 8);
    cshake128(buf, SHAKE128_RATE * PARAM_GEN_A, (uint8_t *)NULL, 0, dmsp_bytes, 2, seed, CRYPTO_RANDOMBYTES);
    ++dmsp;

    while (i < PARAM_K * PARAM_N) {
        if (pos > SHAKE128_RATE * nblocks - 4 * nbytes) {
            nblocks = 1;
            dmsp_bytes[0] = (uint8_t)(dmsp & 0xff);
            dmsp_bytes[1] = (uint8_t)(dmsp >> 8);
            cshake128(buf, SHAKE128_RATE * nblocks, (uint8_t *)NULL, 0, dmsp_bytes, 2, seed, CRYPTO_RANDOMBYTES);
            ++dmsp;
            pos = 0;
        }
        val1 = ((uint32_t)(buf[pos])
                |  (uint32_t)(buf[pos + 1] << 8)
                |  (uint32_t)(buf[pos + 2] << 16)
                |  (uint32_t)(buf[pos + 3] << 24))
               & mask;
        pos += nbytes;
        val2 = ((uint32_t)(buf[pos])
                |  (uint32_t)(buf[pos + 1] << 8)
                |  (uint32_t)(buf[pos + 2] << 16)
                |  (uint32_t)(buf[pos + 3] << 24))
               & mask;
        pos += nbytes;
        val3 = ((uint32_t)(buf[pos])
                |  (uint32_t)(buf[pos + 1] << 8)
                |  (uint32_t)(buf[pos + 2] << 16)
                |  (uint32_t)(buf[pos + 3] << 24))
               & mask;
        pos += nbytes;
        val4 = ((uint32_t)(buf[pos])
                |  (uint32_t)(buf[pos + 1] << 8)
                |  (uint32_t)(buf[pos + 2] << 16)
                |  (uint32_t)(buf[pos + 3] << 24))
               & mask;
        pos += nbytes;
        if (val1 < PARAM_Q && i < PARAM_K * PARAM_N) {
            a[i++] = reduce((int64_t)val1 * PARAM_R2_INVN);
        }
        if (val2 < PARAM_Q && i < PARAM_K * PARAM_N) {
            a[i++] = reduce((int64_t)val2 * PARAM_R2_INVN);
        }
        if (val3 < PARAM_Q && i < PARAM_K * PARAM_N) {
            a[i++] = reduce((int64_t)val3 * PARAM_R2_INVN);
        }
        if (val4 < PARAM_Q && i < PARAM_K * PARAM_N) {
            a[i++] = reduce((int64_t)val4 * PARAM_R2_INVN);
        }
    }
}

