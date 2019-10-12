/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: high-level functions of the signature scheme
**************************************************************************************/

#include "api.h"
#include "fips202.h"
#include "gauss.h"
#include "pack.h"
#include "params.h"
#include "poly.h"
#include "randombytes.h"
#include "sample.h"
#include <stdlib.h>
#include <string.h>

static void hash_H(uint8_t *c_bin, const poly_k v, const uint8_t *hm) {
    // Hash-based function H to generate c'
    uint8_t t[PARAM_K * PARAM_N + HM_BYTES];
    int32_t mask, cL, temp;
    size_t i, k, index;

    for (k = 0; k < PARAM_K; k++) {
        index = k * PARAM_N;
        for (i = 0; i < PARAM_N; i++) {
            temp = (int32_t)v[index];
            // If v[i] > PARAM_Q/2 then v[i] -= PARAM_Q
            mask = (PARAM_Q / 2 - temp) >> (RADIX32 - 1);
            temp = ((temp - PARAM_Q) & mask) | (temp & ~mask);

            cL = temp & ((1 << PARAM_D) - 1);
            // If cL > 2^(d-1) then cL -= 2^d
            mask = ((1 << (PARAM_D - 1)) - cL) >> (RADIX32 - 1);
            cL = ((cL - (1 << PARAM_D)) & mask) | (cL & ~mask);
            t[index++] = (uint8_t)((temp - cL) >> PARAM_D);
        }
    }
    memcpy(&t[PARAM_K * PARAM_N], hm, HM_BYTES);
    SHAKE(c_bin, CRYPTO_C_BYTES, t, PARAM_K * PARAM_N + HM_BYTES);
}


static inline int32_t Abs(int32_t value) {
    // Compute absolute value

    int32_t mask = value >> (RADIX32 - 1);
    return (mask ^ value) - mask;
}


static int test_rejection(const poly z) {
    // Check bounds for signature vector z during signing. Returns 0 if valid, otherwise outputs 1 if invalid (rejected).
    // This function does not leak any information about the coefficient that fails the test.
    uint32_t valid = 0;

    for (size_t i = 0; i < PARAM_N; i++) {
        valid |= (PARAM_B - PARAM_S) - (uint32_t)Abs((int32_t)z[i]);
    }
    return (int)(valid >> 31);
}


static int test_correctness(const poly v) {
    // Check bounds for w = v - ec during signature verification. Returns 0 if valid, otherwise outputs 1 if invalid (rejected).
    // This function leaks the position of the coefficient that fails the test (but this is independent of the secret data).
    // It does not leak the sign of the coefficients.
    int32_t mask, left, val;
    uint32_t t0, t1;

    for (size_t i = 0; i < PARAM_N; i++) {
        // If v[i] > PARAM_Q/2 then v[i] -= PARAM_Q
        mask = (int32_t)(PARAM_Q / 2 - v[i]) >> (RADIX32 - 1);
        val = (int32_t)(((v[i] - PARAM_Q) & mask) | (v[i] & ~mask));
        // If (Abs(val) < PARAM_Q/2 - PARAM_E) then t0 = 0, else t0 = 1
        t0 = (uint32_t)(~(Abs(val) - (PARAM_Q / 2 - PARAM_E))) >> (RADIX32 - 1);

        left = val;
        val = (val + (1 << (PARAM_D - 1)) - 1) >> PARAM_D;
        val = left - (val << PARAM_D);
        // If (Abs(val) < (1<<(PARAM_D-1))-PARAM_E) then t1 = 0, else t1 = 1
        t1 = (uint32_t)(~(Abs(val) - ((1 << (PARAM_D - 1)) - PARAM_E))) >> (RADIX32 - 1);

        if ((t0 | t1) == 1) { // Returns 1 if any of the two tests failed
            return 1;
        }
    }
    return 0;
}


static int test_z(const poly z) {
    // Check bounds for signature vector z during signature verification
    // Returns 0 if valid, otherwise outputs 1 if invalid (rejected)

    for (size_t i = 0; i < PARAM_N; i++) {
        if (z[i] < -(PARAM_B - PARAM_S) || z[i] > (PARAM_B - PARAM_S)) {
            return 1;
        }
    }
    return 0;
}


static int check_ES(poly p, unsigned int bound) {
    // Checks the generated polynomial e or s
    // Returns 0 if ok, otherwise returns 1
    unsigned int sum = 0;
    size_t i, j, limit = PARAM_N;
    int32_t temp, mask, list[PARAM_N];

    for (j = 0; j < PARAM_N; j++) {
        list[j] = Abs((int32_t)p[j]);
    }

    for (j = 0; j < PARAM_H; j++) {
        for (i = 0; i < limit - 1; i++) {
            // If list[i+1] > list[i] then exchange contents
            mask = (list[i + 1] - list[i]) >> (RADIX32 - 1);
            temp = (list[i + 1] & mask) | (list[i] & ~mask);
            list[i + 1] = (list[i] & mask) | (list[i + 1] & ~mask);
            list[i] = temp;
        }
        sum += (unsigned int)list[limit - 1];
        limit -= 1;
    }

    if (sum > bound) {
        return 1;
    }
    return 0;
}


/*********************************************************
* Name:        crypto_sign_keypair
* Description: generates a public and private key pair
* Parameters:  inputs:  none
*              outputs:
*              - uint8_t *pk: public key
*              - uint8_t *sk: secret key
* Returns:     0 for successful execution
**********************************************************/
int PQCLEAN_QTESLAPIII_CLEAN_crypto_sign_keypair(uint8_t *pk, uint8_t *sk) {
    uint8_t randomness[CRYPTO_RANDOMBYTES], randomness_extended[(PARAM_K + 3)*CRYPTO_SEEDBYTES];
    poly s, s_ntt;
    poly_k e, a, t;
    size_t k;  // Initialize domain separator for error and secret polynomials
    uint16_t nonce = 0;

    // Get randomness_extended <- seed_e, seed_s, seed_a, seed_y
    randombytes(randomness, CRYPTO_RANDOMBYTES);
    SHAKE(randomness_extended, (PARAM_K + 3)*CRYPTO_SEEDBYTES, randomness, CRYPTO_RANDOMBYTES);

    for (k = 0; k < PARAM_K; k++) {
        do {  // Sample the error polynomials
            PQCLEAN_QTESLAPIII_CLEAN_sample_gauss_poly(&e[k * PARAM_N], &randomness_extended[k * CRYPTO_SEEDBYTES], ++nonce);
        } while (check_ES(&e[k * PARAM_N], PARAM_KEYGEN_BOUND_E) != 0);
    }

    do {  // Sample the secret polynomial
        PQCLEAN_QTESLAPIII_CLEAN_sample_gauss_poly(s, &randomness_extended[PARAM_K * CRYPTO_SEEDBYTES], ++nonce);
    } while (check_ES(s, PARAM_KEYGEN_BOUND_S) != 0);

    // Generate uniform polynomial "a"
    PQCLEAN_QTESLAPIII_CLEAN_poly_uniform(a, &randomness_extended[(PARAM_K + 1)*CRYPTO_SEEDBYTES]);
    PQCLEAN_QTESLAPIII_CLEAN_poly_ntt(s_ntt, s);

    // Compute the public key t = as+e
    for (k = 0; k < PARAM_K; k++) {
        PQCLEAN_QTESLAPIII_CLEAN_poly_mul(&t[k * PARAM_N], &a[k * PARAM_N], s_ntt);
        PQCLEAN_QTESLAPIII_CLEAN_poly_add_correct(&t[k * PARAM_N], &t[k * PARAM_N], &e[k * PARAM_N]);
    }

    // Pack public and private keys
    PQCLEAN_QTESLAPIII_CLEAN_pack_sk(sk, s, e, &randomness_extended[(PARAM_K + 1)*CRYPTO_SEEDBYTES]);
    PQCLEAN_QTESLAPIII_CLEAN_encode_pk(pk, t, &randomness_extended[(PARAM_K + 1)*CRYPTO_SEEDBYTES]);

    return 0;
}


/***************************************************************
* Name:        crypto_sign
* Description: outputs a signature for a given message m
* Parameters:  inputs:
*              - const uint8_t *m: message to be signed
*              - size_t mlen: message length
*              - const uint8_t* sk: secret key
*              outputs:
*              - uint8_t *sm: signature
*              - size_t *smlen: signature length*
* Returns:     0 for successful execution
***************************************************************/
int PQCLEAN_QTESLAPIII_CLEAN_crypto_sign(uint8_t *sm, size_t *smlen, const uint8_t *m, size_t mlen, const uint8_t *sk) {
    uint8_t c[CRYPTO_C_BYTES], randomness[CRYPTO_SEEDBYTES], randomness_input[CRYPTO_RANDOMBYTES + CRYPTO_SEEDBYTES + HM_BYTES];
    uint32_t pos_list[PARAM_H];
    int16_t sign_list[PARAM_H];
    poly y, y_ntt, Sc, z;
    poly_k v, Ec, a;
    size_t k;
    int rsp;
    uint16_t nonce = 0;  // Initialize domain separator for sampling y

    // Get H(seed_y, r, H(m)) to sample y
    randombytes(randomness_input + CRYPTO_RANDOMBYTES, CRYPTO_RANDOMBYTES);
    memcpy(randomness_input, &sk[PQCLEAN_QTESLAPIII_CLEAN_CRYPTO_SECRETKEYBYTES - CRYPTO_SEEDBYTES], CRYPTO_SEEDBYTES);
    SHAKE(randomness_input + CRYPTO_RANDOMBYTES + CRYPTO_SEEDBYTES, HM_BYTES, m, mlen);
    SHAKE(randomness, CRYPTO_SEEDBYTES, randomness_input, CRYPTO_RANDOMBYTES + CRYPTO_SEEDBYTES + HM_BYTES);

    PQCLEAN_QTESLAPIII_CLEAN_poly_uniform(a, &sk[PQCLEAN_QTESLAPIII_CLEAN_CRYPTO_SECRETKEYBYTES - 2 * CRYPTO_SEEDBYTES]);

    while (1) {
        PQCLEAN_QTESLAPIII_CLEAN_sample_y(y, randomness, ++nonce);           // Sample y uniformly at random from [-B,B]
        PQCLEAN_QTESLAPIII_CLEAN_poly_ntt (y_ntt, y);
        for (k = 0; k < PARAM_K; k++) {
            PQCLEAN_QTESLAPIII_CLEAN_poly_mul(&v[k * PARAM_N], &a[k * PARAM_N], y_ntt);
        }
        hash_H(c, v, randomness_input + CRYPTO_RANDOMBYTES + CRYPTO_SEEDBYTES);
        PQCLEAN_QTESLAPIII_CLEAN_encode_c(pos_list, sign_list, c);           // Generate c = Enc(c'), where c' is the hashing of v together with m
        PQCLEAN_QTESLAPIII_CLEAN_sparse_mul8(Sc, sk, pos_list, sign_list);
        PQCLEAN_QTESLAPIII_CLEAN_poly_add(z, y, Sc);                         // Compute z = y + sc

        if (test_rejection(z) != 0) {               // Rejection sampling
            continue;
        }

        for (k = 0; k < PARAM_K; k++) {
            PQCLEAN_QTESLAPIII_CLEAN_sparse_mul8(&Ec[k * PARAM_N], sk + (sizeof(int8_t)*PARAM_N * (k + 1)), pos_list, sign_list);
            PQCLEAN_QTESLAPIII_CLEAN_poly_sub(&v[k * PARAM_N], &v[k * PARAM_N], &Ec[k * PARAM_N]);
            rsp = test_correctness(&v[k * PARAM_N]);
            if (rsp != 0) {
                break;
            }
        }
        if (rsp != 0) {
            continue;
        }

        // Copy message to signature package, and pack signature
        for (size_t i = 0; i < mlen; i++) {
            sm[PQCLEAN_QTESLAPIII_CLEAN_CRYPTO_BYTES + i] = m[i];
        }
        *smlen = PQCLEAN_QTESLAPIII_CLEAN_CRYPTO_BYTES + mlen;
        PQCLEAN_QTESLAPIII_CLEAN_encode_sig(sm, c, z);

        return 0;
    }
}


/************************************************************
* Name:        crypto_sign_open
* Description: verification of a signature sm
* Parameters:  inputs:
*              - const uint8_t *sm: signature
*              - size_t smlen: signature length
*              - const uint8_t* pk: public Key
*              outputs:
*              - uint8_t *m: original (signed) message
*              - size_t *mlen: message length*
* Returns:     0 for valid signature
*              <0 for invalid signature
************************************************************/
int PQCLEAN_QTESLAPIII_CLEAN_crypto_sign_open(uint8_t *m, size_t *mlen, const uint8_t *sm, size_t smlen, const uint8_t *pk) {
    uint8_t c[CRYPTO_C_BYTES], c_sig[CRYPTO_C_BYTES], seed[CRYPTO_SEEDBYTES], hm[HM_BYTES];
    uint32_t pos_list[PARAM_H];
    int16_t sign_list[PARAM_H];
    int32_t pk_t[PARAM_N * PARAM_K];
    poly_k w, a, Tc;
    poly z, z_ntt;
    size_t k;

    if (smlen < PQCLEAN_QTESLAPIII_CLEAN_CRYPTO_BYTES) {
        return -1;
    }

    PQCLEAN_QTESLAPIII_CLEAN_decode_sig(c, z, sm);
    if (test_z(z) != 0) {
        return -2;    // Check norm of z
    }
    PQCLEAN_QTESLAPIII_CLEAN_decode_pk(pk_t, seed, pk);
    PQCLEAN_QTESLAPIII_CLEAN_poly_uniform(a, seed);
    PQCLEAN_QTESLAPIII_CLEAN_encode_c(pos_list, sign_list, c);
    PQCLEAN_QTESLAPIII_CLEAN_poly_ntt(z_ntt, z);

    for (k = 0; k < PARAM_K; k++) {  // Compute w = az - tc
        PQCLEAN_QTESLAPIII_CLEAN_sparse_mul32(&Tc[k * PARAM_N], &pk_t[k * PARAM_N], pos_list, sign_list);
        PQCLEAN_QTESLAPIII_CLEAN_poly_mul(&w[k * PARAM_N], &a[k * PARAM_N], z_ntt);
        PQCLEAN_QTESLAPIII_CLEAN_poly_sub(&w[k * PARAM_N], &w[k * PARAM_N], &Tc[k * PARAM_N]);
    }
    SHAKE(hm, HM_BYTES, sm + PQCLEAN_QTESLAPIII_CLEAN_CRYPTO_BYTES, smlen - PQCLEAN_QTESLAPIII_CLEAN_CRYPTO_BYTES);
    hash_H(c_sig, w, hm);

    // Check if the calculated c matches c from the signature
    if (memcmp(c, c_sig, CRYPTO_C_BYTES) != 0) {
        return -3;
    }

    *mlen = smlen - PQCLEAN_QTESLAPIII_CLEAN_CRYPTO_BYTES;
    for (size_t i = 0; i < *mlen; i++) {
        m[i] = sm[PQCLEAN_QTESLAPIII_CLEAN_CRYPTO_BYTES + i];
    }

    return 0;
}

int PQCLEAN_QTESLAPIII_CLEAN_crypto_sign_signature(
    uint8_t *sig, size_t *siglen,
    const uint8_t *m, size_t mlen,
    const uint8_t *sk) {
    uint8_t c[CRYPTO_C_BYTES], randomness[CRYPTO_SEEDBYTES], randomness_input[CRYPTO_RANDOMBYTES + CRYPTO_SEEDBYTES + HM_BYTES];
    uint32_t pos_list[PARAM_H];
    int16_t sign_list[PARAM_H];
    poly y, y_ntt, Sc, z;
    poly_k v, Ec, a;
    size_t k;
    int rsp;
    uint16_t nonce = 0;  // Initialize domain separator for sampling y

    // Get H(seed_y, r, H(m)) to sample y
    randombytes(randomness_input + CRYPTO_RANDOMBYTES, CRYPTO_RANDOMBYTES);
    memcpy(randomness_input, &sk[PQCLEAN_QTESLAPIII_CLEAN_CRYPTO_SECRETKEYBYTES - CRYPTO_SEEDBYTES], CRYPTO_SEEDBYTES);
    SHAKE(randomness_input + CRYPTO_RANDOMBYTES + CRYPTO_SEEDBYTES, HM_BYTES, m, mlen);
    SHAKE(randomness, CRYPTO_SEEDBYTES, randomness_input, CRYPTO_RANDOMBYTES + CRYPTO_SEEDBYTES + HM_BYTES);

    PQCLEAN_QTESLAPIII_CLEAN_poly_uniform(a, &sk[PQCLEAN_QTESLAPIII_CLEAN_CRYPTO_SECRETKEYBYTES - 2 * CRYPTO_SEEDBYTES]);

    while (1) {
        PQCLEAN_QTESLAPIII_CLEAN_sample_y(y, randomness, ++nonce);           // Sample y uniformly at random from [-B,B]
        PQCLEAN_QTESLAPIII_CLEAN_poly_ntt (y_ntt, y);
        for (k = 0; k < PARAM_K; k++) {
            PQCLEAN_QTESLAPIII_CLEAN_poly_mul(&v[k * PARAM_N], &a[k * PARAM_N], y_ntt);
        }
        hash_H(c, v, randomness_input + CRYPTO_RANDOMBYTES + CRYPTO_SEEDBYTES);
        PQCLEAN_QTESLAPIII_CLEAN_encode_c(pos_list, sign_list, c);           // Generate c = Enc(c'), where c' is the hashing of v together with m
        PQCLEAN_QTESLAPIII_CLEAN_sparse_mul8(Sc, sk, pos_list, sign_list);
        PQCLEAN_QTESLAPIII_CLEAN_poly_add(z, y, Sc);                         // Compute z = y + sc

        if (test_rejection(z) != 0) {               // Rejection sampling
            continue;
        }

        for (k = 0; k < PARAM_K; k++) {
            PQCLEAN_QTESLAPIII_CLEAN_sparse_mul8(&Ec[k * PARAM_N], sk + (sizeof(int8_t)*PARAM_N * (k + 1)), pos_list, sign_list);
            PQCLEAN_QTESLAPIII_CLEAN_poly_sub(&v[k * PARAM_N], &v[k * PARAM_N], &Ec[k * PARAM_N]);
            rsp = test_correctness(&v[k * PARAM_N]);
            if (rsp != 0) {
                break;
            }
        }
        if (rsp != 0) {
            continue;
        }

        // pack signature
        *siglen = PQCLEAN_QTESLAPIII_CLEAN_CRYPTO_BYTES;
        PQCLEAN_QTESLAPIII_CLEAN_encode_sig(sig, c, z);

        return 0;
    }
}

int PQCLEAN_QTESLAPIII_CLEAN_crypto_sign_verify(
    const uint8_t *sig, size_t siglen,
    const uint8_t *m, size_t mlen,
    const uint8_t *pk) {
    uint8_t c[CRYPTO_C_BYTES], c_sig[CRYPTO_C_BYTES], seed[CRYPTO_SEEDBYTES], hm[HM_BYTES];
    uint32_t pos_list[PARAM_H];
    int16_t sign_list[PARAM_H];
    int32_t pk_t[PARAM_N * PARAM_K];
    poly_k w, a, Tc;
    poly z, z_ntt;
    size_t k;

    if (siglen < PQCLEAN_QTESLAPIII_CLEAN_CRYPTO_BYTES) {
        return -1;
    }

    PQCLEAN_QTESLAPIII_CLEAN_decode_sig(c, z, sig);
    if (test_z(z) != 0) {
        return -2;    // Check norm of z
    }
    PQCLEAN_QTESLAPIII_CLEAN_decode_pk(pk_t, seed, pk);
    PQCLEAN_QTESLAPIII_CLEAN_poly_uniform(a, seed);
    PQCLEAN_QTESLAPIII_CLEAN_encode_c(pos_list, sign_list, c);
    PQCLEAN_QTESLAPIII_CLEAN_poly_ntt(z_ntt, z);

    for (k = 0; k < PARAM_K; k++) {  // Compute w = az - tc
        PQCLEAN_QTESLAPIII_CLEAN_sparse_mul32(&Tc[k * PARAM_N], &pk_t[k * PARAM_N], pos_list, sign_list);
        PQCLEAN_QTESLAPIII_CLEAN_poly_mul(&w[k * PARAM_N], &a[k * PARAM_N], z_ntt);
        PQCLEAN_QTESLAPIII_CLEAN_poly_sub(&w[k * PARAM_N], &w[k * PARAM_N], &Tc[k * PARAM_N]);
    }
    SHAKE(hm, HM_BYTES, m, mlen);
    hash_H(c_sig, w, hm);

    // Check if the calculated c matches c from the signature
    if (memcmp(c, c_sig, CRYPTO_C_BYTES) != 0) {
        return -3;
    }

    return 0;
}
