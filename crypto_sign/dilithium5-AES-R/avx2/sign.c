#include "aes256ctr.h"
#include "alignment.h"
#include "fips202.h"
#include "packing.h"
#include "params.h"
#include "poly.h"
#include "polyvec.h"
#include "randombytes.h"
#include "sign.h"
#include "symmetric.h"
#include <stdint.h>


/*************************************************
* Name:        crypto_sign_keypair
*
* Description: Generates public and private key.
*
* Arguments:   - uint8_t *pk: pointer to output public key (allocated
*                             array of CRYPTO_PUBLICKEYBYTES bytes)
*              - uint8_t *sk: pointer to output private key (allocated
*                             array of CRYPTO_SECRETKEYBYTES bytes)
*
* Returns 0 (success)
**************************************************/
int crypto_sign_keypair(uint8_t *pk, uint8_t *sk) {
    unsigned int i;
    ALIGNED_UINT8(3 * SEEDBYTES) seedbuf_aligned;
    uint8_t *seedbuf = seedbuf_aligned.as_arr;

    ALIGNED_UINT8(CRHBYTES) tr_aligned;
    uint8_t *tr = tr_aligned.as_arr;

    const uint8_t *rho, *rhoprime, *key;
    polyvecl mat[K], s1;
    polyveck s2;
    poly t1, t0;

    /* Get randomness for rho, rhoprime and key */
    randombytes(seedbuf, SEEDBYTES);
    shake256(seedbuf, 3 * SEEDBYTES, seedbuf, SEEDBYTES);
    rho = seedbuf;
    rhoprime = seedbuf + SEEDBYTES;
    key = seedbuf + 2 * SEEDBYTES;

    /* Store rho, key */
    for (i = 0; i < SEEDBYTES; ++i) {
        pk[i] = rho[i];
    }
    for (i = 0; i < SEEDBYTES; ++i) {
        sk[i] = rho[i];
    }
    for (i = 0; i < SEEDBYTES; ++i) {
        sk[SEEDBYTES + i] = key[i];
    }

    /* Sample short vectors s1 and s2 */
    uint64_t nonce = 0;
    aes256ctr_ctx aesctx;
    aes256ctr_init(&aesctx, rhoprime, nonce++);
    for (i = 0; i < L; ++i) {
        poly_uniform_eta_preinit(&s1.vec[i], &aesctx);
        aesctx.n = _mm_loadl_epi64((__m128i *)&nonce);
        nonce++;
    }
    for (i = 0; i < K; ++i) {
        poly_uniform_eta_preinit(&s2.vec[i], &aesctx);
        aesctx.n = _mm_loadl_epi64((__m128i *)&nonce);
        nonce++;
    }

    /* Pack secret vectors */
    for (i = 0; i < L; i++) {
        polyeta_pack(sk + 2 * SEEDBYTES + CRHBYTES + i * POLYETA_PACKEDBYTES, &s1.vec[i]);
    }
    for (i = 0; i < K; i++) {
        polyeta_pack(sk + 2 * SEEDBYTES + CRHBYTES + (L + i)*POLYETA_PACKEDBYTES, &s2.vec[i]);
    }

    /* Transform s1 */
    polyvecl_ntt(&s1);

    for (i = 0; i < K; i++) {
        /* Expand matrix row */
        polyvec_matrix_expand_row(mat, rho, i);

        /* Compute inner-product */
        polyvecl_pointwise_acc_montgomery(&t1, &mat[i], &s1);
        poly_invntt_tomont(&t1);

        /* Add error polynomial */
        poly_add(&t1, &t1, &s2.vec[i]);

        /* Round t and pack t1, t0 */
        poly_caddq(&t1);
        poly_power2round(&t1, &t0, &t1);
        polyt1_pack(pk + SEEDBYTES + i * POLYT1_PACKEDBYTES, &t1);
        polyt0_pack(sk + 2 * SEEDBYTES + CRHBYTES + (L + K)*POLYETA_PACKEDBYTES + i * POLYT0_PACKEDBYTES, &t0);
    }

    /* Compute CRH(rho, t1) and store in secret key */
    crh(tr, pk, CRYPTO_PUBLICKEYBYTES);
    for (i = 0; i < CRHBYTES; ++i) {
        sk[2 * SEEDBYTES + i] = tr[i];
    }

    return 0;
}

/*************************************************
* Name:        crypto_sign_signature
*
* Description: Computes signature.
*
* Arguments:   - uint8_t *sig: pointer to output signature (of length CRYPTO_BYTES)
*              - size_t *siglen: pointer to output length of signature
*              - uint8_t *m: pointer to message to be signed
*              - size_t mlen: length of message
*              - uint8_t *sk: pointer to bit-packed secret key
*
* Returns 0 (success)
**************************************************/
int crypto_sign_signature(uint8_t *sig,
                          size_t *siglen,
                          const uint8_t *m,
                          size_t mlen,
                          const uint8_t *sk) {
    unsigned int i, j, n, pos;

    ALIGNED_UINT8(2 * SEEDBYTES + 3 * CRHBYTES) seedbuf_aligned;
    uint8_t *seedbuf = seedbuf_aligned.as_arr;

    uint8_t *rho, *tr, *key, *mu, *rhoprime;
    uint8_t *hint = sig + SEEDBYTES + L * POLYZ_PACKEDBYTES;
    uint64_t nonce = 0;
    polyvecl mat[K], s1, y, z;
    polyveck t0, s2, w1, w0;
    poly cp, h;
    shake256incctx state;

    rho = seedbuf;
    tr = rho + SEEDBYTES;
    key = tr + CRHBYTES;
    mu = key + SEEDBYTES;
    rhoprime = mu + CRHBYTES;
    unpack_sk(rho, tr, key, &t0, &s1, &s2, sk);

    /* Compute CRH(tr, msg) */
    shake256_inc_init(&state);
    shake256_inc_absorb(&state, tr, CRHBYTES);
    shake256_inc_absorb(&state, m, mlen);
    shake256_inc_finalize(&state);
    shake256_inc_squeeze(mu, CRHBYTES, &state);
    shake256_inc_ctx_release(&state);

    randombytes(rhoprime, CRHBYTES);

    /* Expand matrix and transform vectors */
    polyvec_matrix_expand(mat, rho);
    polyvecl_ntt(&s1);
    polyveck_ntt(&s2);
    polyveck_ntt(&t0);

    aes256ctr_ctx aesctx;
    aes256ctr_init(&aesctx, rhoprime, nonce++);

rej:
    /* Sample intermediate vector y */
    for (i = 0; i < L; ++i) {
        poly_uniform_gamma1_preinit(&y.vec[i], &aesctx);
        aesctx.n = _mm_loadl_epi64((__m128i *)&nonce);
        nonce++;
    }

    /* Save y and transform it */
    z = y;
    polyvecl_ntt(&y);

    for (i = 0; i < K; i++) {
        /* Compute inner-product */
        polyvecl_pointwise_acc_montgomery(&w1.vec[i], &mat[i], &y);
        poly_invntt_tomont(&w1.vec[i]);

        /* Decompose w and use sig as temporary buffer for packed w1 */
        poly_caddq(&w1.vec[i]);
        poly_decompose(&w1.vec[i], &w0.vec[i], &w1.vec[i]);
        polyw1_pack(sig + i * POLYW1_PACKEDBYTES, &w1.vec[i]);
    }

    /* Call the random oracle */
    shake256_inc_init(&state);
    shake256_inc_absorb(&state, mu, CRHBYTES);
    shake256_inc_absorb(&state, sig, K * POLYW1_PACKEDBYTES);
    shake256_inc_finalize(&state);
    shake256_inc_squeeze(sig, SEEDBYTES, &state);
    shake256_inc_ctx_release(&state);
    poly_challenge(&cp, sig);
    poly_ntt(&cp);

    /* Compute z, reject if it reveals secret */
    for (i = 0; i < L; i++) {
        poly_pointwise_montgomery(&h, &cp, &s1.vec[i]);
        poly_invntt_tomont(&h);
        poly_add(&z.vec[i], &z.vec[i], &h);
        poly_reduce(&z.vec[i]);
        if (poly_chknorm(&z.vec[i], GAMMA1 - BETA)) {
            goto rej;
        }
    }

    /* Zero hint in signature */
    n = pos = 0;
    for (i = 0; i < OMEGA + K; i++) {
        hint[i] = 0;
    }

    for (i = 0; i < K; i++) {
        /* Check that subtracting cs2 does not change high bits of w and low bits
         * do not reveal secret information */
        poly_pointwise_montgomery(&h, &cp, &s2.vec[i]);
        poly_invntt_tomont(&h);
        poly_sub(&w0.vec[i], &w0.vec[i], &h);
        poly_reduce(&w0.vec[i]);
        if (poly_chknorm(&w0.vec[i], GAMMA2 - BETA)) {
            goto rej;
        }

        /* Compute hints */
        poly_pointwise_montgomery(&h, &cp, &t0.vec[i]);
        poly_invntt_tomont(&h);
        poly_reduce(&h);
        if (poly_chknorm(&h, GAMMA2)) {
            goto rej;
        }

        poly_add(&w0.vec[i], &w0.vec[i], &h);
        poly_caddq(&w0.vec[i]);
        n += poly_make_hint(&h, &w0.vec[i], &w1.vec[i]);
        if (n > OMEGA) {
            goto rej;
        }

        /* Store hints in signature */
        for (j = 0; j < N; ++j) {
            if (h.coeffs[j] != 0) {
                hint[pos++] = j;
            }
        }
        hint[OMEGA + i] = pos;
    }

    /* Pack z into signature */
    for (i = 0; i < L; i++) {
        polyz_pack(sig + SEEDBYTES + i * POLYZ_PACKEDBYTES, &z.vec[i]);
    }

    *siglen = CRYPTO_BYTES;
    return 0;
}

/*************************************************
* Name:        crypto_sign
*
* Description: Compute signed message.
*
* Arguments:   - uint8_t *sm: pointer to output signed message (allocated
*                             array with CRYPTO_BYTES + mlen bytes),
*                             can be equal to m
*              - size_t *smlen: pointer to output length of signed
*                               message
*              - const uint8_t *m: pointer to message to be signed
*              - size_t mlen: length of message
*              - const uint8_t *sk: pointer to bit-packed secret key
*
* Returns 0 (success)
**************************************************/
int crypto_sign(uint8_t *sm,
                size_t *smlen,
                const uint8_t *m,
                size_t mlen,
                const uint8_t *sk) {
    size_t i;

    for (i = 0; i < mlen; ++i) {
        sm[CRYPTO_BYTES + mlen - 1 - i] = m[mlen - 1 - i];
    }
    crypto_sign_signature(sm, smlen, sm + CRYPTO_BYTES, mlen, sk);
    *smlen += mlen;
    return 0;
}

/*************************************************
* Name:        crypto_sign_verify
*
* Description: Verifies signature.
*
* Arguments:   - uint8_t *m: pointer to input signature
*              - size_t siglen: length of signature
*              - const uint8_t *m: pointer to message
*              - size_t mlen: length of message
*              - const uint8_t *pk: pointer to bit-packed public key
*
* Returns 0 if signature could be verified correctly and -1 otherwise
**************************************************/
int crypto_sign_verify(const uint8_t *sig,
                       size_t siglen,
                       const uint8_t *m,
                       size_t mlen,
                       const uint8_t *pk) {
    unsigned int i, j, pos = 0;

    ALIGNED_UINT8(K * POLYW1_PACKEDBYTES) buf_aligned;
    uint8_t *buf = buf_aligned.as_arr;
    uint8_t mu[CRHBYTES];
    uint8_t c[SEEDBYTES];
    const uint8_t *hint = sig + SEEDBYTES + L * POLYZ_PACKEDBYTES;
    polyvecl mat[K], z;
    poly cp, w1, t1, h;
    shake256incctx state;

    if (siglen != CRYPTO_BYTES) {
        return -1;
    }

    /* Compute CRH(CRH(rho, t1), msg) */
    crh(mu, pk, CRYPTO_PUBLICKEYBYTES);
    shake256_inc_init(&state);
    shake256_inc_absorb(&state, mu, CRHBYTES);
    shake256_inc_absorb(&state, m, mlen);
    shake256_inc_finalize(&state);
    shake256_inc_squeeze(mu, CRHBYTES, &state);
    shake256_inc_ctx_release(&state);

    /* Expand challenge */
    poly_challenge(&cp, sig);
    poly_ntt(&cp);

    /* Unpack z; shortness follows from unpacking */
    for (i = 0; i < L; i++) {
        polyz_unpack(&z.vec[i], sig + SEEDBYTES + i * POLYZ_PACKEDBYTES);
        poly_ntt(&z.vec[i]);
    }

    for (i = 0; i < K; i++) {
        /* Expand matrix row */
        polyvec_matrix_expand_row(mat, pk, i);

        /* Compute i-th row of Az - c2^Dt1 */
        polyvecl_pointwise_acc_montgomery(&w1, &mat[i], &z);

        polyt1_unpack(&t1, pk + SEEDBYTES + i * POLYT1_PACKEDBYTES);
        poly_shiftl(&t1);
        poly_ntt(&t1);
        poly_pointwise_montgomery(&t1, &cp, &t1);

        poly_sub(&w1, &w1, &t1);
        poly_reduce(&w1);
        poly_invntt_tomont(&w1);

        /* Get hint polynomial and reconstruct w1 */
        for (j = 0; j < N; ++j) {
            h.coeffs[j] = 0;
        }

        if (hint[OMEGA + i] < pos || hint[OMEGA + i] > OMEGA) {
            return -1;
        }

        for (j = pos; j < hint[OMEGA + i]; ++j) {
            /* Coefficients are ordered for strong unforgeability */
            if (j > pos && hint[j] <= hint[j - 1]) {
                return -1;
            }
            h.coeffs[hint[j]] = 1;
        }
        pos = hint[OMEGA + i];

        poly_caddq(&w1);
        poly_use_hint(&w1, &w1, &h);
        polyw1_pack(buf + i * POLYW1_PACKEDBYTES, &w1);
    }

    /* Extra indices are zero for strong unforgeability */
    for (j = pos; j < OMEGA; ++j) {
        if (hint[j]) {
            return -1;
        }
    }

    shake256_inc_init(&state);
    shake256_inc_absorb(&state, mu, CRHBYTES);
    shake256_inc_absorb(&state, buf, K * POLYW1_PACKEDBYTES);
    shake256_inc_finalize(&state);
    shake256_inc_squeeze(c, SEEDBYTES, &state);
    shake256_inc_ctx_release(&state);
    for (i = 0; i < SEEDBYTES; ++i) {
        if (c[i] != sig[i]) {
            return -1;
        }
    }

    return 0;
}

/*************************************************
* Name:        crypto_sign_open
*
* Description: Verify signed message.
*
* Arguments:   - uint8_t *m: pointer to output message (allocated
*                            array with smlen bytes), can be equal to sm
*              - size_t *mlen: pointer to output length of message
*              - const uint8_t *sm: pointer to signed message
*              - size_t smlen: length of signed message
*              - const uint8_t *pk: pointer to bit-packed public key
*
* Returns 0 if signed message could be verified correctly and -1 otherwise
**************************************************/
int crypto_sign_open(uint8_t *m,
                     size_t *mlen,
                     const uint8_t *sm,
                     size_t smlen,
                     const uint8_t *pk) {
    size_t i;

    if (smlen < CRYPTO_BYTES) {
        goto badsig;
    }

    *mlen = smlen - CRYPTO_BYTES;
    if (crypto_sign_verify(sm, CRYPTO_BYTES, sm + CRYPTO_BYTES, *mlen, pk)) {
        goto badsig;
    } else {
        /* All good, copy msg, return 0 */
        for (i = 0; i < *mlen; ++i) {
            m[i] = sm[CRYPTO_BYTES + i];
        }
        return 0;
    }

badsig:
    /* Signature verification failed */
    *mlen = -1;
    for (i = 0; i < smlen; ++i) {
        m[i] = 0;
    }

    return -1;
}
