#include "align.h"
#include "cbd.h"
#include "indcpa.h"
#include "ntt.h"
#include "params.h"
#include "poly.h"
#include "polyvec.h"
#include "randombytes.h"
#include "rejsample.h"
#include "symmetric.h"
#include <stddef.h>
#include <stdint.h>

/*************************************************
* Name:        pack_pk
*
* Description: Serialize the public key as concatenation of the
*              serialized vector of polynomials pk
*              and the public seed used to generate the matrix A.
*
* Arguments:   uint8_t *r:          pointer to the output serialized public key
*              polyvec *pk:         pointer to the input public-key polyvec
*              const uint8_t *seed: pointer to the input public seed
**************************************************/
static void pack_pk(uint8_t r[KYBER_INDCPA_PUBLICKEYBYTES],
                    polyvec *pk,
                    const uint8_t seed[KYBER_SYMBYTES]) {
    size_t i = 0;
    PQCLEAN_KYBER76890S_AVX2_polyvec_tobytes(r, pk);
    for (i = 0; i < KYBER_SYMBYTES; i++) {
        r[i + KYBER_POLYVECBYTES] = seed[i];
    }
}

/*************************************************
* Name:        unpack_pk
*
* Description: De-serialize public key from a byte array;
*              approximate inverse of pack_pk
*
* Arguments:   - polyvec *pk: pointer to output public-key polynomial vector
*              - uint8_t *seed: pointer to output seed to generate matrix A
*              - const uint8_t *packedpk: pointer to input serialized public key
**************************************************/
static void unpack_pk(polyvec *pk,
                      uint8_t seed[KYBER_SYMBYTES],
                      const uint8_t packedpk[KYBER_INDCPA_PUBLICKEYBYTES]) {
    size_t i = 0;
    PQCLEAN_KYBER76890S_AVX2_polyvec_frombytes(pk, packedpk);
    for (i = 0; i < KYBER_SYMBYTES; i++) {
        seed[i] = packedpk[i + KYBER_POLYVECBYTES];
    }
}

/*************************************************
* Name:        pack_sk
*
* Description: Serialize the secret key
*
* Arguments:   - uint8_t *r:  pointer to output serialized secret key
*              - polyvec *sk: pointer to input vector of polynomials (secret key)
**************************************************/
static void pack_sk(uint8_t r[KYBER_INDCPA_SECRETKEYBYTES], polyvec *sk) {
    PQCLEAN_KYBER76890S_AVX2_polyvec_tobytes(r, sk);
}

/*************************************************
* Name:        unpack_sk
*
* Description: De-serialize the secret key;
*              inverse of pack_sk
*
* Arguments:   - polyvec *sk: pointer to output vector of polynomials
*                (secret key)
*              - const uint8_t *packedsk: pointer to input serialized secret key
**************************************************/
static void unpack_sk(polyvec *sk,
                      const uint8_t packedsk[KYBER_INDCPA_SECRETKEYBYTES]) {
    PQCLEAN_KYBER76890S_AVX2_polyvec_frombytes(sk, packedsk);
}

/*************************************************
* Name:        pack_ciphertext
*
* Description: Serialize the ciphertext as concatenation of the
*              compressed and serialized vector of polynomials b
*              and the compressed and serialized polynomial v
*
* Arguments:   uint8_t *r: pointer to the output serialized ciphertext
*              poly *pk:   pointer to the input vector of polynomials b
*              poly *v:    pointer to the input polynomial v
**************************************************/
static void pack_ciphertext(uint8_t r[KYBER_INDCPA_BYTES],
                            polyvec *b,
                            poly *v) {
    PQCLEAN_KYBER76890S_AVX2_polyvec_compress(r, b);
    PQCLEAN_KYBER76890S_AVX2_poly_compress(r + KYBER_POLYVECCOMPRESSEDBYTES, v);
}

/*************************************************
* Name:        unpack_ciphertext
*
* Description: De-serialize and decompress ciphertext from a byte array;
*              approximate inverse of pack_ciphertext
*
* Arguments:   - polyvec *b:       pointer to the output vector of polynomials b
*              - poly *v:          pointer to the output polynomial v
*              - const uint8_t *c: pointer to the input serialized ciphertext
**************************************************/
static void unpack_ciphertext(polyvec *b,
                              poly *v,
                              const uint8_t c[KYBER_INDCPA_BYTES]) {
    PQCLEAN_KYBER76890S_AVX2_polyvec_decompress(b, c);
    PQCLEAN_KYBER76890S_AVX2_poly_decompress(v, c + KYBER_POLYVECCOMPRESSEDBYTES);
}

/*************************************************
* Name:        rej_uniform
*
* Description: Run rejection sampling on uniform random bytes to generate
*              uniform random integers mod q
*
* Arguments:   - int16_t *r: pointer to output buffer
*              - unsigned int len: requested number of 16-bit integers
*                (uniform mod q)
*              - const uint8_t *buf: pointer to input buffer
*                (assumed to be uniform random bytes)
*              - unsigned int buflen: length of input buffer in bytes
*
* Returns number of sampled 16-bit integers (at most len)
**************************************************/
static unsigned int rej_uniform(int16_t *r,
                                unsigned int len,
                                const uint8_t *buf,
                                unsigned int buflen) {
    unsigned int ctr = 0, pos = 0;
    uint16_t val = 0;

    ctr = pos = 0;
    while (ctr < len && pos + 2 <= buflen) {
        val = buf[pos] | ((uint16_t)buf[pos + 1] << 8);
        pos += 2;

        if (val < 19 * KYBER_Q) {
            val -= ((uint32_t)val * 20159 >> 26) * KYBER_Q; // Barrett reduction
            r[ctr++] = (int16_t)val;
        }
    }

    return ctr;
}

#define gen_a(A,B)  PQCLEAN_KYBER76890S_AVX2_gen_matrix(A,B,0)
#define gen_at(A,B) PQCLEAN_KYBER76890S_AVX2_gen_matrix(A,B,1)

/*************************************************
* Name:        PQCLEAN_KYBER76890S_AVX2_gen_matrix
*
* Description: Deterministically generate matrix A (or the transpose of A)
*              from a seed. Entries of the matrix are polynomials that look
*              uniformly random. Performs rejection sampling on output of
*              a XOF
*
* Arguments:   - polyvec *a: pointer to ouptput matrix A
*              - const uint8_t *seed: pointer to input seed
*              - int transposed: boolean deciding whether A or A^T is generated
**************************************************/
#define GEN_MATRIX_NBLOCKS ((2*KYBER_N*(1U << 16)/(19*KYBER_Q) \
                             + XOF_BLOCKBYTES)/XOF_BLOCKBYTES)
void PQCLEAN_KYBER76890S_AVX2_gen_matrix(polyvec *a, const uint8_t seed[KYBER_SYMBYTES], int transposed) {
    unsigned int ctr = 0, i = 0, j = 0;
    ALIGN16_TYPE(uint64_t) nonce = {.orig = 0};
    ALIGN32_ARRAY(uint8_t, GEN_MATRIX_NBLOCKS * XOF_BLOCKBYTES) buf;
    aes256ctr_ctx state;

    PQCLEAN_KYBER76890S_AVX2_aes256ctr_init(&state, seed, 0);

    for (i = 0; i < KYBER_K; i++) {
        for (j = 0; j < KYBER_K; j++) {
            if (transposed) {
                nonce.orig = (j << 8) | i;
            } else {
                nonce.orig = (i << 8) | j;
            }

            state.n = _mm_loadl_epi64(&nonce.vec);
            PQCLEAN_KYBER76890S_AVX2_aes256ctr_squeezeblocks(buf.arr, GEN_MATRIX_NBLOCKS, &state);
            ctr = PQCLEAN_KYBER76890S_AVX2_rej_uniform_avx(a[i].vec[j].coeffs, buf.arr);

            while (ctr < KYBER_N) {
                PQCLEAN_KYBER76890S_AVX2_aes256ctr_squeezeblocks(buf.arr, 1, &state);
                ctr += rej_uniform(a[i].vec[j].coeffs + ctr, KYBER_N - ctr, buf.arr,
                                   XOF_BLOCKBYTES);
            }

            PQCLEAN_KYBER76890S_AVX2_poly_nttunpack(&a[i].vec[j]);
        }
    }
}

/*************************************************
* Name:        PQCLEAN_KYBER76890S_AVX2_indcpa_keypair
*
* Description: Generates public and private key for the CPA-secure
*              public-key encryption scheme underlying Kyber
*
* Arguments:   - uint8_t *pk: pointer to output public key
*                             (of length KYBER_INDCPA_PUBLICKEYBYTES bytes)
*              - uint8_t *sk: pointer to output private key
                              (of length KYBER_INDCPA_SECRETKEYBYTES bytes)
**************************************************/
void PQCLEAN_KYBER76890S_AVX2_indcpa_keypair(uint8_t pk[KYBER_INDCPA_PUBLICKEYBYTES],
        uint8_t sk[KYBER_INDCPA_SECRETKEYBYTES]) {
    unsigned int i = 0;
    ALIGN32_ARRAY(uint8_t, 2 * KYBER_SYMBYTES) buf;
    const uint8_t *publicseed = buf.arr;
    const uint8_t *noiseseed = buf.arr + KYBER_SYMBYTES;
    polyvec a[KYBER_K], e, pkpv, skpv;

    randombytes(buf.arr, KYBER_SYMBYTES);
    hash_g(buf.arr, buf.arr, KYBER_SYMBYTES);

    gen_a(a, publicseed);

    ALIGN16_TYPE(uint64_t) nonce = {.orig = 0};
    aes256ctr_ctx state;
    ALIGN32_ARRAY(uint8_t, 128) coins;
    PQCLEAN_KYBER76890S_AVX2_aes256ctr_init(&state, noiseseed, nonce.orig++);
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER76890S_AVX2_aes256ctr_squeezeblocks(coins.arr, 2, &state);
        state.n = _mm_loadl_epi64(&nonce.vec);
        nonce.orig++;
        PQCLEAN_KYBER76890S_AVX2_cbd(&skpv.vec[i], coins.arr);
    }
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER76890S_AVX2_aes256ctr_squeezeblocks(coins.arr, 2, &state);
        state.n = _mm_loadl_epi64(&nonce.vec);
        nonce.orig++;
        PQCLEAN_KYBER76890S_AVX2_cbd(&e.vec[i], coins.arr);
    }

    PQCLEAN_KYBER76890S_AVX2_polyvec_ntt(&skpv);
    PQCLEAN_KYBER76890S_AVX2_polyvec_ntt(&e);

    // matrix-vector multiplication
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER76890S_AVX2_polyvec_pointwise_acc_montgomery(&pkpv.vec[i], &a[i], &skpv);
        PQCLEAN_KYBER76890S_AVX2_poly_tomont(&pkpv.vec[i]);
    }

    PQCLEAN_KYBER76890S_AVX2_polyvec_add(&pkpv, &pkpv, &e);
    PQCLEAN_KYBER76890S_AVX2_polyvec_reduce(&pkpv);

    pack_sk(sk, &skpv);
    pack_pk(pk, &pkpv, publicseed);
}

/*************************************************
* Name:        PQCLEAN_KYBER76890S_AVX2_indcpa_enc
*
* Description: Encryption function of the CPA-secure
*              public-key encryption scheme underlying Kyber.
*
* Arguments:   - uint8_t *c:           pointer to output ciphertext
*                                      (of length KYBER_INDCPA_BYTES bytes)
*              - const uint8_t *m:     pointer to input message
*                                      (of length KYBER_INDCPA_MSGBYTES bytes)
*              - const uint8_t *pk:    pointer to input public key
*                                      (of length KYBER_INDCPA_PUBLICKEYBYTES)
*              - const uint8_t *coins: pointer to input random coins
*                                      used as seed (of length KYBER_SYMBYTES)
*                                      to deterministically generate all
*                                      randomness
**************************************************/
void PQCLEAN_KYBER76890S_AVX2_indcpa_enc(uint8_t c[KYBER_INDCPA_BYTES],
        const uint8_t m[KYBER_INDCPA_MSGBYTES],
        const uint8_t pk[KYBER_INDCPA_PUBLICKEYBYTES],
        const uint8_t coins[KYBER_SYMBYTES]) {
    unsigned int i = 0;
    ALIGN32_ARRAY(uint8_t, KYBER_SYMBYTES) seed;
    polyvec sp, pkpv, ep, at[KYBER_K], bp;
    poly v, k, epp;

    unpack_pk(&pkpv, seed.arr, pk);
    PQCLEAN_KYBER76890S_AVX2_poly_frommsg(&k, m);
    gen_at(at, seed.arr);

    ALIGN16_TYPE(uint64_t) nonce = {.orig = 0};
    aes256ctr_ctx state;
    ALIGN32_ARRAY(uint8_t, 128) buf;
    PQCLEAN_KYBER76890S_AVX2_aes256ctr_init(&state, coins, nonce.orig++);
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER76890S_AVX2_aes256ctr_squeezeblocks(buf.arr, 2, &state);
        state.n = _mm_loadl_epi64(&nonce.vec);
        nonce.orig++;
        PQCLEAN_KYBER76890S_AVX2_cbd(&sp.vec[i], buf.arr);
    }
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER76890S_AVX2_aes256ctr_squeezeblocks(buf.arr, 2, &state);
        state.n = _mm_loadl_epi64(&nonce.vec);
        nonce.orig++;
        PQCLEAN_KYBER76890S_AVX2_cbd(&ep.vec[i], buf.arr);
    }
    PQCLEAN_KYBER76890S_AVX2_aes256ctr_squeezeblocks(buf.arr, 2, &state);
    state.n = _mm_loadl_epi64(&nonce.vec);
    nonce.orig++;
    PQCLEAN_KYBER76890S_AVX2_cbd(&epp, buf.arr);

    PQCLEAN_KYBER76890S_AVX2_polyvec_ntt(&sp);

    // matrix-vector multiplication
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_KYBER76890S_AVX2_polyvec_pointwise_acc_montgomery(&bp.vec[i], &at[i], &sp);
    }

    PQCLEAN_KYBER76890S_AVX2_polyvec_pointwise_acc_montgomery(&v, &pkpv, &sp);

    PQCLEAN_KYBER76890S_AVX2_polyvec_invntt_tomont(&bp);
    PQCLEAN_KYBER76890S_AVX2_poly_invntt_tomont(&v);

    PQCLEAN_KYBER76890S_AVX2_polyvec_add(&bp, &bp, &ep);
    PQCLEAN_KYBER76890S_AVX2_poly_add(&v, &v, &epp);
    PQCLEAN_KYBER76890S_AVX2_poly_add(&v, &v, &k);
    PQCLEAN_KYBER76890S_AVX2_polyvec_reduce(&bp);
    PQCLEAN_KYBER76890S_AVX2_poly_reduce(&v);

    pack_ciphertext(c, &bp, &v);
}

/*************************************************
* Name:        PQCLEAN_KYBER76890S_AVX2_indcpa_dec
*
* Description: Decryption function of the CPA-secure
*              public-key encryption scheme underlying Kyber.
*
* Arguments:   - uint8_t *m:        pointer to output decrypted message
*                                   (of length KYBER_INDCPA_MSGBYTES)
*              - const uint8_t *c:  pointer to input ciphertext
*                                   (of length KYBER_INDCPA_BYTES)
*              - const uint8_t *sk: pointer to input secret key
*                                   (of length KYBER_INDCPA_SECRETKEYBYTES)
**************************************************/
void PQCLEAN_KYBER76890S_AVX2_indcpa_dec(uint8_t m[KYBER_INDCPA_MSGBYTES],
        const uint8_t c[KYBER_INDCPA_BYTES],
        const uint8_t sk[KYBER_INDCPA_SECRETKEYBYTES]) {
    polyvec bp, skpv;
    poly v, mp;

    unpack_ciphertext(&bp, &v, c);
    unpack_sk(&skpv, sk);

    PQCLEAN_KYBER76890S_AVX2_polyvec_ntt(&bp);
    PQCLEAN_KYBER76890S_AVX2_polyvec_pointwise_acc_montgomery(&mp, &skpv, &bp);
    PQCLEAN_KYBER76890S_AVX2_poly_invntt_tomont(&mp);

    PQCLEAN_KYBER76890S_AVX2_poly_sub(&mp, &v, &mp);
    PQCLEAN_KYBER76890S_AVX2_poly_reduce(&mp);

    PQCLEAN_KYBER76890S_AVX2_poly_tomsg(m, &mp);
}
