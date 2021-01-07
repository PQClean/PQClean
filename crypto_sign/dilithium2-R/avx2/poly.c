#include "alignment.h"
#include "consts.h"
#include "fips202x4.h"
#include "ntt.h"
#include "params.h"
#include "poly.h"
#include "rejsample.h"
#include "rounding.h"
#include "symmetric.h"
#include <immintrin.h>
#include <stdint.h>


#define DBENCH_START()
#define DBENCH_STOP(t)

#define _mm256_blendv_epi32(a,b,mask) \
    _mm256_castps_si256(_mm256_blendv_ps(_mm256_castsi256_ps(a), \
                                         _mm256_castsi256_ps(b), \
                                         _mm256_castsi256_ps(mask)))

/*************************************************
* Name:        poly_reduce
*
* Description: Inplace reduction of all coefficients of polynomial to
*              representative in [-6283009,6283007].
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void poly_reduce(poly *a) {
    unsigned int i;
    __m256i f, g;
    const __m256i q = _mm256_load_si256((__m256i *)&qdata.as_arr[_8XQ]);
    const __m256i off = _mm256_set1_epi32(1 << 22);
    DBENCH_START();

    for (i = 0; i < N / 8; ++i) {
        f = _mm256_load_si256((__m256i *)&a->coeffs[8 * i]);
        g = _mm256_add_epi32(f, off);
        g = _mm256_srai_epi32(g, 23);
        g = _mm256_mullo_epi32(g, q);
        f = _mm256_sub_epi32(f, g);
        _mm256_store_si256((__m256i *)&a->coeffs[8 * i], f);
    }

    DBENCH_STOP(*tred);
}

/*************************************************
* Name:        poly_addq
*
* Description: For all coefficients of in/out polynomial add Q if
*              coefficient is negative.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void poly_caddq(poly *a) {
    unsigned int i;
    __m256i f, g;
    const __m256i q = _mm256_load_si256((__m256i *)&qdata.as_arr[_8XQ]);
    const __m256i zero = _mm256_setzero_si256();
    DBENCH_START();

    for (i = 0; i < N / 8; ++i) {
        f = _mm256_load_si256((__m256i *)&a->coeffs[8 * i]);
        g = _mm256_blendv_epi32(zero, q, f);
        f = _mm256_add_epi32(f, g);
        _mm256_store_si256((__m256i *)&a->coeffs[8 * i], f);
    }

    DBENCH_STOP(*tred);
}

/*************************************************
* Name:        poly_freeze
*
* Description: Inplace reduction of all coefficients of polynomial to
*              standard representatives.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void poly_freeze(poly *a) {
    DBENCH_START();

    poly_reduce(a);
    poly_caddq(a);

    DBENCH_STOP(*tred);
}

/*************************************************
* Name:        poly_add
*
* Description: Add polynomials. No modular reduction is performed.
*
* Arguments:   - poly *c: pointer to output polynomial
*              - const poly *a: pointer to first summand
*              - const poly *b: pointer to second summand
**************************************************/
void poly_add(poly *c, const poly *a, const poly *b)  {
    unsigned int i;
    __m256i vec0, vec1;
    DBENCH_START();

    for (i = 0; i < N; i += 8) {
        vec0 = _mm256_load_si256((__m256i *)&a->coeffs[i]);
        vec1 = _mm256_load_si256((__m256i *)&b->coeffs[i]);
        vec0 = _mm256_add_epi32(vec0, vec1);
        _mm256_store_si256((__m256i *)&c->coeffs[i], vec0);
    }

    DBENCH_STOP(*tadd);
}

/*************************************************
* Name:        poly_sub
*
* Description: Subtract polynomials. No modular reduction is
*              performed.
*
* Arguments:   - poly *c: pointer to output polynomial
*              - const poly *a: pointer to first input polynomial
*              - const poly *b: pointer to second input polynomial to be
*                               subtraced from first input polynomial
**************************************************/
void poly_sub(poly *c, const poly *a, const poly *b) {
    unsigned int i;
    __m256i vec0, vec1;
    DBENCH_START();

    for (i = 0; i < N; i += 8) {
        vec0 = _mm256_load_si256((__m256i *)&a->coeffs[i]);
        vec1 = _mm256_load_si256((__m256i *)&b->coeffs[i]);
        vec0 = _mm256_sub_epi32(vec0, vec1);
        _mm256_store_si256((__m256i *)&c->coeffs[i], vec0);
    }

    DBENCH_STOP(*tadd);
}

/*************************************************
* Name:        poly_shiftl
*
* Description: Multiply polynomial by 2^D without modular reduction. Assumes
*              input coefficients to be less than 2^{31-D} in absolute value.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void poly_shiftl(poly *a) {
    unsigned int i;
    __m256i vec;
    DBENCH_START();

    for (i = 0; i < N; i += 8) {
        vec = _mm256_load_si256((__m256i *)&a->coeffs[i]);
        vec = _mm256_slli_epi32(vec, D);
        _mm256_store_si256((__m256i *)&a->coeffs[i], vec);
    }

    DBENCH_STOP(*tmul);
}

/*************************************************
* Name:        poly_ntt
*
* Description: Inplace forward NTT. Coefficients can grow by up to
*              8*Q in absolute value.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void poly_ntt(poly *a) {
    DBENCH_START();

    ntt_avx(a->coeffs, qdata.as_arr);

    DBENCH_STOP(*tmul);
}

/*************************************************
* Name:        poly_invntt_tomont
*
* Description: Inplace inverse NTT and multiplication by 2^{32}.
*              Input coefficients need to be less than Q in absolute
*              value and output coefficients are again bounded by Q.
*
* Arguments:   - poly *a: pointer to input/output polynomial
**************************************************/
void poly_invntt_tomont(poly *a) {
    DBENCH_START();

    invntt_avx(a->coeffs, qdata.as_arr);

    DBENCH_STOP(*tmul);
}

/*************************************************
* Name:        poly_pointwise_montgomery
*
* Description: Pointwise multiplication of polynomials in NTT domain
*              representation and multiplication of resulting polynomial
*              by 2^{-32}.
*
* Arguments:   - poly *c: pointer to output polynomial
*              - const poly *a: pointer to first input polynomial
*              - const poly *b: pointer to second input polynomial
**************************************************/
void poly_pointwise_montgomery(poly *c, const poly *a, const poly *b) {
    DBENCH_START();

    pointwise_avx(c->coeffs, a->coeffs, b->coeffs, qdata.as_arr);

    DBENCH_STOP(*tmul);
}

/*************************************************
* Name:        poly_power2round
*
* Description: For all coefficients c of the input polynomial,
*              compute c0, c1 such that c mod^+ Q = c1*2^D + c0
*              with -2^{D-1} < c0 <= 2^{D-1}. Assumes coefficients to be
*              standard representatives.
*
* Arguments:   - poly *a1: pointer to output polynomial with coefficients c1
*              - poly *a0: pointer to output polynomial with coefficients c0
*              - const poly *a: pointer to input polynomial
**************************************************/
void poly_power2round(poly *a1, poly *a0, const poly *a) {
    DBENCH_START();

    power2round_avx(a1->coeffs, a0->coeffs, a->coeffs);

    DBENCH_STOP(*tround);
}

/*************************************************
* Name:        poly_decompose
*
* Description: For all coefficients c of the input polynomial,
*              compute high and low bits c0, c1 such c mod Q = c1*ALPHA + c0
*              with -ALPHA/2 < c0 <= ALPHA/2 except c1 = (Q-1)/ALPHA where we
*              set c1 = 0 and -ALPHA/2 <= c0 = c mod Q - Q < 0.
*              Assumes coefficients to be standard representatives.
*
* Arguments:   - poly *a1: pointer to output polynomial with coefficients c1
*              - poly *a0: pointer to output polynomial with coefficients c0
*              - const poly *a: pointer to input polynomial
**************************************************/
void poly_decompose(poly *a1, poly *a0, const poly *a) {
    DBENCH_START();

    decompose_avx(a1->coeffs, a0->coeffs, a->coeffs);

    DBENCH_STOP(*tround);
}

/*************************************************
* Name:        poly_make_hint
*
* Description: Compute hint polynomial. The coefficients of which indicate
*              whether the low bits of the corresponding coefficient of
*              the input polynomial overflow into the high bits.
*
* Arguments:   - poly *h: pointer to output hint polynomial
*              - const poly *a0: pointer to low part of input polynomial
*              - const poly *a1: pointer to high part of input polynomial
*
* Returns number of 1 bits.
**************************************************/
unsigned int poly_make_hint(poly *h, const poly *a0, const poly *a1) {
    unsigned int r;
    DBENCH_START();

    r = make_hint_avx(h->coeffs, a0->coeffs, a1->coeffs);

    DBENCH_STOP(*tround);
    return r;
}

/*************************************************
* Name:        poly_use_hint
*
* Description: Use hint polynomial to correct the high bits of a polynomial.
*
* Arguments:   - poly *b: pointer to output polynomial with corrected high bits
*              - const poly *a: pointer to input polynomial
*              - const poly *h: pointer to input hint polynomial
**************************************************/
void poly_use_hint(poly *b, const poly *a, const poly *h) {
    DBENCH_START();

    use_hint_avx(b->coeffs, a->coeffs, h->coeffs);

    DBENCH_STOP(*tround);
}

/*************************************************
* Name:        poly_chknorm
*
* Description: Check infinity norm of polynomial against given bound.
*              Assumes input polynomial to be reduced by poly_reduce().
*
* Arguments:   - const poly *a: pointer to polynomial
*              - int32_t B: norm bound
*
* Returns 0 if norm is strictly smaller than B <= (Q-1)/8 and 1 otherwise.
**************************************************/
int poly_chknorm(const poly *a, int32_t B) {
    unsigned int i;
    int r;
    __m256i f, t;
    const __m256i bound = _mm256_set1_epi32(B - 1);
    DBENCH_START();

    if (B > (Q - 1) / 8) {
        return 1;
    }

    t = _mm256_setzero_si256();
    for (i = 0; i < N / 8; ++i) {
        f = _mm256_load_si256((__m256i *)&a->coeffs[8 * i]);
        f = _mm256_abs_epi32(f);
        f = _mm256_cmpgt_epi32(f, bound);
        t = _mm256_or_si256(t, f);
    }

    r = _mm256_testz_si256(t, t);
    DBENCH_STOP(*tsample);

    if (r) {
        return 0;
    }
    return 1;
}

/*************************************************
* Name:        rej_uniform
*
* Description: Sample uniformly random coefficients in [0, Q-1] by
*              performing rejection sampling on array of random bytes.
*
* Arguments:   - int32_t *a: pointer to output array (allocated)
*              - unsigned int len: number of coefficients to be sampled
*              - const uint8_t *buf: array of random bytes
*              - unsigned int buflen: length of array of random bytes
*
* Returns number of sampled coefficients. Can be smaller than len if not enough
* random bytes were given.
**************************************************/
static unsigned int rej_uniform(int32_t *a,
                                unsigned int len,
                                const uint8_t *buf,
                                unsigned int buflen) {
    unsigned int ctr, pos;
    uint32_t t;
    DBENCH_START();

    ctr = pos = 0;
    while (ctr < len && pos + 3 <= buflen) {
        t  = buf[pos++];
        t |= (uint32_t)buf[pos++] << 8;
        t |= (uint32_t)buf[pos++] << 16;
        t &= 0x7FFFFF;

        if (t < Q) {
            a[ctr++] = t;
        }
    }

    DBENCH_STOP(*tsample);
    return ctr;
}

/*************************************************
* Name:        poly_uniform
*
* Description: Sample polynomial with uniformly random coefficients
*              in [0,Q-1] by performing rejection sampling on the
*              output stream of SHAKE256(seed|nonce) or AES256CTR(seed,nonce).
*
* Arguments:   - poly *a: pointer to output polynomial
*              - const uint8_t seed[]: byte array with seed of length SEEDBYTES
*              - uint16_t nonce: 2-byte nonce
**************************************************/
#define POLY_UNIFORM_NBLOCKS ((768+STREAM128_BLOCKBYTES-1)/STREAM128_BLOCKBYTES)
void poly_uniform_preinit(poly *a, stream128_state *state) {
    unsigned int ctr;

    ALIGNED_UINT8(POLY_UNIFORM_NBLOCKS * STREAM128_BLOCKBYTES + 8) buf_aligned;
    uint8_t *buf = buf_aligned.as_arr;

    stream128_squeezeblocks(buf, POLY_UNIFORM_NBLOCKS, state);
    ctr = rej_uniform_avx(a->coeffs, buf);

    while (ctr < N) {
        /* length of buf is always divisible by 3; hence, no bytes left */
        stream128_squeezeblocks(buf, 1, state);
        ctr += rej_uniform(a->coeffs + ctr, N - ctr, buf, STREAM128_BLOCKBYTES);
    }
}

void poly_uniform(poly *a,
                  const uint8_t seed[SEEDBYTES],
                  uint16_t nonce) {
    stream128_state state;
    stream128_init(&state, seed, nonce);
    poly_uniform_preinit(a, &state);
    stream128_ctx_release(&state);
}

void poly_uniform_4x(poly *a0,
                     poly *a1,
                     poly *a2,
                     poly *a3,
                     const uint8_t seed[32],
                     uint16_t nonce0,
                     uint16_t nonce1,
                     uint16_t nonce2,
                     uint16_t nonce3) {
    unsigned int ctr0, ctr1, ctr2, ctr3;
    ALIGNED_UINT8(864) buf[4];

    keccakx4_state state;
    __m256i f;

    f = _mm256_loadu_si256((__m256i *)seed);
    _mm256_store_si256(buf[0].as_vec, f);
    _mm256_store_si256(buf[1].as_vec, f);
    _mm256_store_si256(buf[2].as_vec, f);
    _mm256_store_si256(buf[3].as_vec, f);

    buf[0].as_arr[SEEDBYTES + 0] = nonce0;
    buf[0].as_arr[SEEDBYTES + 1] = nonce0 >> 8;
    buf[1].as_arr[SEEDBYTES + 0] = nonce1;
    buf[1].as_arr[SEEDBYTES + 1] = nonce1 >> 8;
    buf[2].as_arr[SEEDBYTES + 0] = nonce2;
    buf[2].as_arr[SEEDBYTES + 1] = nonce2 >> 8;
    buf[3].as_arr[SEEDBYTES + 0] = nonce3;
    buf[3].as_arr[SEEDBYTES + 1] = nonce3 >> 8;

    shake128x4_absorb(&state, buf[0].as_arr, buf[1].as_arr, buf[2].as_arr, buf[3].as_arr, SEEDBYTES + 2);
    shake128x4_squeezeblocks(buf[0].as_arr, buf[1].as_arr, buf[2].as_arr, buf[3].as_arr, 5, &state);

    ctr0 = rej_uniform_avx(a0->coeffs, buf[0].as_arr);
    ctr1 = rej_uniform_avx(a1->coeffs, buf[1].as_arr);
    ctr2 = rej_uniform_avx(a2->coeffs, buf[2].as_arr);
    ctr3 = rej_uniform_avx(a3->coeffs, buf[3].as_arr);

    while (ctr0 < N || ctr1 < N || ctr2 < N || ctr3 < N) {
        shake128x4_squeezeblocks(buf[0].as_arr, buf[1].as_arr, buf[2].as_arr, buf[3].as_arr, 1, &state);

        ctr0 += rej_uniform(a0->coeffs + ctr0, N - ctr0, buf[0].as_arr, SHAKE128_RATE);
        ctr1 += rej_uniform(a1->coeffs + ctr1, N - ctr1, buf[1].as_arr, SHAKE128_RATE);
        ctr2 += rej_uniform(a2->coeffs + ctr2, N - ctr2, buf[2].as_arr, SHAKE128_RATE);
        ctr3 += rej_uniform(a3->coeffs + ctr3, N - ctr3, buf[3].as_arr, SHAKE128_RATE);
    }
}

/*************************************************
* Name:        rej_eta
*
* Description: Sample uniformly random coefficients in [-ETA, ETA] by
*              performing rejection sampling on array of random bytes.
*
* Arguments:   - int32_t *a: pointer to output array (allocated)
*              - unsigned int len: number of coefficients to be sampled
*              - const uint8_t *buf: array of random bytes
*              - unsigned int buflen: length of array of random bytes
*
* Returns number of sampled coefficients. Can be smaller than len if not enough
* random bytes were given.
**************************************************/
static unsigned int rej_eta(int32_t *a,
                            unsigned int len,
                            const uint8_t *buf,
                            unsigned int buflen) {
    unsigned int ctr, pos;
    uint32_t t0, t1;
    DBENCH_START();

    ctr = pos = 0;
    while (ctr < len && pos < buflen) {
        t0 = buf[pos] & 0x0F;
        t1 = buf[pos++] >> 4;

        if (t0 < 15) {
            t0 = t0 - (205 * t0 >> 10) * 5;
            a[ctr++] = 2 - t0;
        }
        if (t1 < 15 && ctr < len) {
            t1 = t1 - (205 * t1 >> 10) * 5;
            a[ctr++] = 2 - t1;
        }
    }

    DBENCH_STOP(*tsample);
    return ctr;
}

/*************************************************
* Name:        poly_uniform_eta
*
* Description: Sample polynomial with uniformly random coefficients
*              in [-ETA,ETA] by performing rejection sampling using the
*              output stream of SHAKE256(seed|nonce)
*              or AES256CTR(seed,nonce).
*
* Arguments:   - poly *a: pointer to output polynomial
*              - const uint8_t seed[]: byte array with seed of length
*                                      SEEDBYTES
*              - uint16_t nonce: 2-byte nonce
**************************************************/
#define POLY_UNIFORM_ETA_NBLOCKS ((136 + STREAM128_BLOCKBYTES - 1)/STREAM128_BLOCKBYTES)
void poly_uniform_eta_preinit(poly *a, stream128_state *state) {
    unsigned int ctr;
    ALIGNED_UINT8(POLY_UNIFORM_ETA_NBLOCKS * STREAM128_BLOCKBYTES + 8) buf_aligned;
    uint8_t *buf = buf_aligned.as_arr;

    stream128_squeezeblocks(buf, POLY_UNIFORM_ETA_NBLOCKS, state);
    ctr = rej_eta(a->coeffs, N, buf, POLY_UNIFORM_ETA_NBLOCKS * STREAM128_BLOCKBYTES);

    while (ctr < N) {
        stream128_squeezeblocks(buf, 1, state);
        ctr += rej_eta(a->coeffs + ctr, N - ctr, buf, STREAM128_BLOCKBYTES);
    }
}

void poly_uniform_eta(poly *a,
                      const uint8_t seed[SEEDBYTES],
                      uint16_t nonce) {
    stream128_state state;
    stream128_init(&state, seed, nonce);
    poly_uniform_eta_preinit(a, &state);
    stream128_ctx_release(&state);
}

void poly_uniform_eta_4x(poly *a0,
                         poly *a1,
                         poly *a2,
                         poly *a3,
                         const uint8_t seed[32],
                         uint16_t nonce0,
                         uint16_t nonce1,
                         uint16_t nonce2,
                         uint16_t nonce3) {
    unsigned int ctr0, ctr1, ctr2, ctr3;
    ALIGNED_UINT8(192) buf[4];
    __m256i f;
    keccakx4_state state;

    f = _mm256_load_si256((__m256i *)seed);
    _mm256_store_si256(buf[0].as_vec, f);
    _mm256_store_si256(buf[1].as_vec, f);
    _mm256_store_si256(buf[2].as_vec, f);
    _mm256_store_si256(buf[3].as_vec, f);

    buf[0].as_arr[SEEDBYTES + 0] = nonce0;
    buf[0].as_arr[SEEDBYTES + 1] = nonce0 >> 8;
    buf[1].as_arr[SEEDBYTES + 0] = nonce1;
    buf[1].as_arr[SEEDBYTES + 1] = nonce1 >> 8;
    buf[2].as_arr[SEEDBYTES + 0] = nonce2;
    buf[2].as_arr[SEEDBYTES + 1] = nonce2 >> 8;
    buf[3].as_arr[SEEDBYTES + 0] = nonce3;
    buf[3].as_arr[SEEDBYTES + 1] = nonce3 >> 8;

    shake128x4_absorb(&state, buf[0].as_arr, buf[1].as_arr, buf[2].as_arr, buf[3].as_arr, SEEDBYTES + 2);
    shake128x4_squeezeblocks(buf[0].as_arr, buf[1].as_arr, buf[2].as_arr, buf[3].as_arr, POLY_UNIFORM_ETA_NBLOCKS, &state);

    ctr0 = rej_eta(a0->coeffs, N, buf[0].as_arr, POLY_UNIFORM_ETA_NBLOCKS * SHAKE128_RATE);
    ctr1 = rej_eta(a1->coeffs, N, buf[1].as_arr, POLY_UNIFORM_ETA_NBLOCKS * SHAKE128_RATE);
    ctr2 = rej_eta(a2->coeffs, N, buf[2].as_arr, POLY_UNIFORM_ETA_NBLOCKS * SHAKE128_RATE);
    ctr3 = rej_eta(a3->coeffs, N, buf[3].as_arr, POLY_UNIFORM_ETA_NBLOCKS * SHAKE128_RATE);

    while (ctr0 < N || ctr1 < N || ctr2 < N || ctr3 < N) {
        shake128x4_squeezeblocks(buf[0].as_arr, buf[1].as_arr, buf[2].as_arr, buf[3].as_arr, 1, &state);

        ctr0 += rej_eta(a0->coeffs + ctr0, N - ctr0, buf[0].as_arr, SHAKE128_RATE);
        ctr1 += rej_eta(a1->coeffs + ctr1, N - ctr1, buf[1].as_arr, SHAKE128_RATE);
        ctr2 += rej_eta(a2->coeffs + ctr2, N - ctr2, buf[2].as_arr, SHAKE128_RATE);
        ctr3 += rej_eta(a3->coeffs + ctr3, N - ctr3, buf[3].as_arr, SHAKE128_RATE);
    }
}

/*************************************************
* Name:        poly_uniform_gamma1
*
* Description: Sample polynomial with uniformly random coefficients
*              in [-(GAMMA1 - 1), GAMMA1] by unpacking output stream
*              of SHAKE256(seed|nonce) or AES256CTR(seed,nonce).
*
* Arguments:   - poly *a: pointer to output polynomial
*              - const uint8_t seed[]: byte array with seed of length CRHBYTES
*              - uint16_t nonce: 16-bit nonce
**************************************************/
#define POLY_UNIFORM_GAMMA1_NBLOCKS ((576 + STREAM256_BLOCKBYTES - 1)/STREAM256_BLOCKBYTES)
void poly_uniform_gamma1_preinit(poly *a, stream256_state *state) {
    ALIGNED_UINT8(POLY_UNIFORM_GAMMA1_NBLOCKS * STREAM256_BLOCKBYTES) buf_aligned;
    uint8_t *buf = buf_aligned.as_arr;

    stream256_squeezeblocks(buf, POLY_UNIFORM_GAMMA1_NBLOCKS, state);
    polyz_unpack(a, buf);
}

void poly_uniform_gamma1(poly *a,
                         const uint8_t seed[CRHBYTES],
                         uint16_t nonce) {
    stream256_state state;
    stream256_init(&state, seed, nonce);
    poly_uniform_gamma1_preinit(a, &state);
    stream256_ctx_release(&state);
}

void poly_uniform_gamma1_4x(poly *a0,
                            poly *a1,
                            poly *a2,
                            poly *a3,
                            const uint8_t seed[48],
                            uint16_t nonce0,
                            uint16_t nonce1,
                            uint16_t nonce2,
                            uint16_t nonce3) {
    ALIGNED_UINT8(704) buf[4];

    keccakx4_state state;
    __m256i f;
    __m128i g;

    f = _mm256_load_si256((__m256i *)seed);
    _mm256_store_si256(buf[0].as_vec, f);
    _mm256_store_si256(buf[1].as_vec, f);
    _mm256_store_si256(buf[2].as_vec, f);
    _mm256_store_si256(buf[3].as_vec, f);
    g = _mm_load_si128((__m128i *)&seed[32]);
    _mm_store_si128((__m128i *)&buf[0].as_arr[32], g);
    _mm_store_si128((__m128i *)&buf[1].as_arr[32], g);
    _mm_store_si128((__m128i *)&buf[2].as_arr[32], g);
    _mm_store_si128((__m128i *)&buf[3].as_arr[32], g);

    buf[0].as_arr[CRHBYTES + 0] = nonce0;
    buf[0].as_arr[CRHBYTES + 1] = nonce0 >> 8;
    buf[1].as_arr[CRHBYTES + 0] = nonce1;
    buf[1].as_arr[CRHBYTES + 1] = nonce1 >> 8;
    buf[2].as_arr[CRHBYTES + 0] = nonce2;
    buf[2].as_arr[CRHBYTES + 1] = nonce2 >> 8;
    buf[3].as_arr[CRHBYTES + 0] = nonce3;
    buf[3].as_arr[CRHBYTES + 1] = nonce3 >> 8;

    shake256x4_absorb(&state, buf[0].as_arr, buf[1].as_arr, buf[2].as_arr, buf[3].as_arr, CRHBYTES + 2);
    shake256x4_squeezeblocks(buf[0].as_arr, buf[1].as_arr, buf[2].as_arr, buf[3].as_arr, 5, &state);

    polyz_unpack(a0, buf[0].as_arr);
    polyz_unpack(a1, buf[1].as_arr);
    polyz_unpack(a2, buf[2].as_arr);
    polyz_unpack(a3, buf[3].as_arr);
}

/*************************************************
* Name:        challenge
*
* Description: Implementation of H. Samples polynomial with TAU nonzero
*              coefficients in {-1,1} using the output stream of
*              SHAKE256(seed).
*
* Arguments:   - poly *c: pointer to output polynomial
*              - const uint8_t mu[]: byte array containing seed of length SEEDBYTES
**************************************************/
void poly_challenge(poly *c, const uint8_t seed[SEEDBYTES]) {
    unsigned int i, b, pos;
    uint64_t signs;
    uint8_t buf[SHAKE256_RATE];
    shake256ctx state;

    shake256_absorb(&state, seed, SEEDBYTES);
    shake256_squeezeblocks(buf, 1, &state);

    signs = 0;
    for (i = 0; i < 8; ++i) {
        signs |= (uint64_t)buf[i] << 8 * i;
    }
    pos = 8;

    for (i = 0; i < N; ++i) {
        c->coeffs[i] = 0;
    }
    for (i = N - TAU; i < N; ++i) {
        do {
            if (pos >= SHAKE256_RATE) {
                shake256_squeezeblocks(buf, 1, &state);
                pos = 0;
            }

            b = buf[pos++];
        } while (b > i);

        c->coeffs[i] = c->coeffs[b];
        c->coeffs[b] = 1 - 2 * (signs & 1);
        signs >>= 1;
    }
    shake256_ctx_release(&state);
}

/*************************************************
* Name:        polyeta_pack
*
* Description: Bit-pack polynomial with coefficients in [-ETA,ETA].
*
* Arguments:   - uint8_t *r: pointer to output byte array with at least
*                            POLYETA_PACKED_BYTES bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void polyeta_pack(uint8_t *r, const poly *a) {
    unsigned int i;
    uint8_t t[8];
    DBENCH_START();

    for (i = 0; i < N / 8; ++i) {
        t[0] = ETA - a->coeffs[8 * i + 0];
        t[1] = ETA - a->coeffs[8 * i + 1];
        t[2] = ETA - a->coeffs[8 * i + 2];
        t[3] = ETA - a->coeffs[8 * i + 3];
        t[4] = ETA - a->coeffs[8 * i + 4];
        t[5] = ETA - a->coeffs[8 * i + 5];
        t[6] = ETA - a->coeffs[8 * i + 6];
        t[7] = ETA - a->coeffs[8 * i + 7];

        r[3 * i + 0]  = (t[0] >> 0) | (t[1] << 3) | (t[2] << 6);
        r[3 * i + 1]  = (t[2] >> 2) | (t[3] << 1) | (t[4] << 4) | (t[5] << 7);
        r[3 * i + 2]  = (t[5] >> 1) | (t[6] << 2) | (t[7] << 5);
    }

    DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyeta_unpack
*
* Description: Unpack polynomial with coefficients in [-ETA,ETA].
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *a: byte array with bit-packed polynomial
**************************************************/
void polyeta_unpack(poly *r, const uint8_t *a) {
    unsigned int i;
    DBENCH_START();

    for (i = 0; i < N / 8; ++i) {
        r->coeffs[8 * i + 0] =  (a[3 * i + 0] >> 0) & 7;
        r->coeffs[8 * i + 1] =  (a[3 * i + 0] >> 3) & 7;
        r->coeffs[8 * i + 2] = ((a[3 * i + 0] >> 6) | (a[3 * i + 1] << 2)) & 7;
        r->coeffs[8 * i + 3] =  (a[3 * i + 1] >> 1) & 7;
        r->coeffs[8 * i + 4] =  (a[3 * i + 1] >> 4) & 7;
        r->coeffs[8 * i + 5] = ((a[3 * i + 1] >> 7) | (a[3 * i + 2] << 1)) & 7;
        r->coeffs[8 * i + 6] =  (a[3 * i + 2] >> 2) & 7;
        r->coeffs[8 * i + 7] =  (a[3 * i + 2] >> 5) & 7;

        r->coeffs[8 * i + 0] = ETA - r->coeffs[8 * i + 0];
        r->coeffs[8 * i + 1] = ETA - r->coeffs[8 * i + 1];
        r->coeffs[8 * i + 2] = ETA - r->coeffs[8 * i + 2];
        r->coeffs[8 * i + 3] = ETA - r->coeffs[8 * i + 3];
        r->coeffs[8 * i + 4] = ETA - r->coeffs[8 * i + 4];
        r->coeffs[8 * i + 5] = ETA - r->coeffs[8 * i + 5];
        r->coeffs[8 * i + 6] = ETA - r->coeffs[8 * i + 6];
        r->coeffs[8 * i + 7] = ETA - r->coeffs[8 * i + 7];
    }

    DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyt1_pack
*
* Description: Bit-pack polynomial t1 with coefficients fitting in 10 bits.
*              Input coefficients are assumed to be standard representatives.
*
* Arguments:   - uint8_t *r: pointer to output byte array with at least
*                            POLYT1_PACKED_BYTES bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void polyt1_pack(uint8_t *r, const poly *a) {
    unsigned int i;
    DBENCH_START();

    for (i = 0; i < N / 4; ++i) {
        r[5 * i + 0] = (a->coeffs[4 * i + 0] >> 0);
        r[5 * i + 1] = (a->coeffs[4 * i + 0] >> 8) | (a->coeffs[4 * i + 1] << 2);
        r[5 * i + 2] = (a->coeffs[4 * i + 1] >> 6) | (a->coeffs[4 * i + 2] << 4);
        r[5 * i + 3] = (a->coeffs[4 * i + 2] >> 4) | (a->coeffs[4 * i + 3] << 6);
        r[5 * i + 4] = (a->coeffs[4 * i + 3] >> 2);
    }

    DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyt1_unpack
*
* Description: Unpack polynomial t1 with 10-bit coefficients.
*              Output coefficients are standard representatives.
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *a: byte array with bit-packed polynomial
**************************************************/
void polyt1_unpack(poly *r, const uint8_t *a) {
    unsigned int i;
    DBENCH_START();

    for (i = 0; i < N / 4; ++i) {
        r->coeffs[4 * i + 0] = ((a[5 * i + 0] >> 0) | ((uint32_t)a[5 * i + 1] << 8)) & 0x3FF;
        r->coeffs[4 * i + 1] = ((a[5 * i + 1] >> 2) | ((uint32_t)a[5 * i + 2] << 6)) & 0x3FF;
        r->coeffs[4 * i + 2] = ((a[5 * i + 2] >> 4) | ((uint32_t)a[5 * i + 3] << 4)) & 0x3FF;
        r->coeffs[4 * i + 3] = ((a[5 * i + 3] >> 6) | ((uint32_t)a[5 * i + 4] << 2)) & 0x3FF;
    }

    DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyt0_pack
*
* Description: Bit-pack polynomial t0 with coefficients in ]-2^{D-1}, 2^{D-1}].
*
* Arguments:   - uint8_t *r: pointer to output byte array with at least
*                            POLYT0_PACKED_BYTES bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void polyt0_pack(uint8_t *r, const poly *a) {
    unsigned int i;
    uint32_t t[8];
    DBENCH_START();

    for (i = 0; i < N / 8; ++i) {
        t[0] = (1 << (D - 1)) - a->coeffs[8 * i + 0];
        t[1] = (1 << (D - 1)) - a->coeffs[8 * i + 1];
        t[2] = (1 << (D - 1)) - a->coeffs[8 * i + 2];
        t[3] = (1 << (D - 1)) - a->coeffs[8 * i + 3];
        t[4] = (1 << (D - 1)) - a->coeffs[8 * i + 4];
        t[5] = (1 << (D - 1)) - a->coeffs[8 * i + 5];
        t[6] = (1 << (D - 1)) - a->coeffs[8 * i + 6];
        t[7] = (1 << (D - 1)) - a->coeffs[8 * i + 7];

        r[13 * i + 0]  =  t[0];
        r[13 * i + 1]  =  t[0] >>  8;
        r[13 * i + 1] |=  t[1] <<  5;
        r[13 * i + 2]  =  t[1] >>  3;
        r[13 * i + 3]  =  t[1] >> 11;
        r[13 * i + 3] |=  t[2] <<  2;
        r[13 * i + 4]  =  t[2] >>  6;
        r[13 * i + 4] |=  t[3] <<  7;
        r[13 * i + 5]  =  t[3] >>  1;
        r[13 * i + 6]  =  t[3] >>  9;
        r[13 * i + 6] |=  t[4] <<  4;
        r[13 * i + 7]  =  t[4] >>  4;
        r[13 * i + 8]  =  t[4] >> 12;
        r[13 * i + 8] |=  t[5] <<  1;
        r[13 * i + 9]  =  t[5] >>  7;
        r[13 * i + 9] |=  t[6] <<  6;
        r[13 * i + 10]  =  t[6] >>  2;
        r[13 * i + 11]  =  t[6] >> 10;
        r[13 * i + 11] |=  t[7] <<  3;
        r[13 * i + 12]  =  t[7] >>  5;
    }

    DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyt0_unpack
*
* Description: Unpack polynomial t0 with coefficients in ]-2^{D-1}, 2^{D-1}].
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *a: byte array with bit-packed polynomial
**************************************************/
void polyt0_unpack(poly *r, const uint8_t *a) {
    unsigned int i;
    DBENCH_START();

    for (i = 0; i < N / 8; ++i) {
        r->coeffs[8 * i + 0]  = a[13 * i + 0];
        r->coeffs[8 * i + 0] |= (uint32_t)a[13 * i + 1] << 8;
        r->coeffs[8 * i + 0] &= 0x1FFF;

        r->coeffs[8 * i + 1]  = a[13 * i + 1] >> 5;
        r->coeffs[8 * i + 1] |= (uint32_t)a[13 * i + 2] << 3;
        r->coeffs[8 * i + 1] |= (uint32_t)a[13 * i + 3] << 11;
        r->coeffs[8 * i + 1] &= 0x1FFF;

        r->coeffs[8 * i + 2]  = a[13 * i + 3] >> 2;
        r->coeffs[8 * i + 2] |= (uint32_t)a[13 * i + 4] << 6;
        r->coeffs[8 * i + 2] &= 0x1FFF;

        r->coeffs[8 * i + 3]  = a[13 * i + 4] >> 7;
        r->coeffs[8 * i + 3] |= (uint32_t)a[13 * i + 5] << 1;
        r->coeffs[8 * i + 3] |= (uint32_t)a[13 * i + 6] << 9;
        r->coeffs[8 * i + 3] &= 0x1FFF;

        r->coeffs[8 * i + 4]  = a[13 * i + 6] >> 4;
        r->coeffs[8 * i + 4] |= (uint32_t)a[13 * i + 7] << 4;
        r->coeffs[8 * i + 4] |= (uint32_t)a[13 * i + 8] << 12;
        r->coeffs[8 * i + 4] &= 0x1FFF;

        r->coeffs[8 * i + 5]  = a[13 * i + 8] >> 1;
        r->coeffs[8 * i + 5] |= (uint32_t)a[13 * i + 9] << 7;
        r->coeffs[8 * i + 5] &= 0x1FFF;

        r->coeffs[8 * i + 6]  = a[13 * i + 9] >> 6;
        r->coeffs[8 * i + 6] |= (uint32_t)a[13 * i + 10] << 2;
        r->coeffs[8 * i + 6] |= (uint32_t)a[13 * i + 11] << 10;
        r->coeffs[8 * i + 6] &= 0x1FFF;

        r->coeffs[8 * i + 7]  = a[13 * i + 11] >> 3;
        r->coeffs[8 * i + 7] |= (uint32_t)a[13 * i + 12] << 5;
        r->coeffs[8 * i + 7] &= 0x1FFF;

        r->coeffs[8 * i + 0] = (1 << (D - 1)) - r->coeffs[8 * i + 0];
        r->coeffs[8 * i + 1] = (1 << (D - 1)) - r->coeffs[8 * i + 1];
        r->coeffs[8 * i + 2] = (1 << (D - 1)) - r->coeffs[8 * i + 2];
        r->coeffs[8 * i + 3] = (1 << (D - 1)) - r->coeffs[8 * i + 3];
        r->coeffs[8 * i + 4] = (1 << (D - 1)) - r->coeffs[8 * i + 4];
        r->coeffs[8 * i + 5] = (1 << (D - 1)) - r->coeffs[8 * i + 5];
        r->coeffs[8 * i + 6] = (1 << (D - 1)) - r->coeffs[8 * i + 6];
        r->coeffs[8 * i + 7] = (1 << (D - 1)) - r->coeffs[8 * i + 7];
    }

    DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyz_pack
*
* Description: Bit-pack polynomial with coefficients
*              in [-(GAMMA1 - 1), GAMMA1].
*
* Arguments:   - uint8_t *r: pointer to output byte array with at least
*                            POLYZ_PACKED_BYTES bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void polyz_pack(uint8_t *r, const poly *a) {
    unsigned int i;
    uint32_t t[4];
    DBENCH_START();

    for (i = 0; i < N / 4; ++i) {
        t[0] = GAMMA1 - a->coeffs[4 * i + 0];
        t[1] = GAMMA1 - a->coeffs[4 * i + 1];
        t[2] = GAMMA1 - a->coeffs[4 * i + 2];
        t[3] = GAMMA1 - a->coeffs[4 * i + 3];

        r[9 * i + 0]  = t[0];
        r[9 * i + 1]  = t[0] >> 8;
        r[9 * i + 2]  = t[0] >> 16;
        r[9 * i + 2] |= t[1] << 2;
        r[9 * i + 3]  = t[1] >> 6;
        r[9 * i + 4]  = t[1] >> 14;
        r[9 * i + 4] |= t[2] << 4;
        r[9 * i + 5]  = t[2] >> 4;
        r[9 * i + 6]  = t[2] >> 12;
        r[9 * i + 6] |= t[3] << 6;
        r[9 * i + 7]  = t[3] >> 2;
        r[9 * i + 8]  = t[3] >> 10;
    }

    DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyz_unpack
*
* Description: Unpack polynomial z with coefficients
*              in [-(GAMMA1 - 1), GAMMA1].
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *a: byte array with bit-packed polynomial
**************************************************/
void polyz_unpack(poly *r, const uint8_t *a) {
    unsigned int i;
    DBENCH_START();

    for (i = 0; i < N / 4; ++i) {
        r->coeffs[4 * i + 0]  = a[9 * i + 0];
        r->coeffs[4 * i + 0] |= (uint32_t)a[9 * i + 1] << 8;
        r->coeffs[4 * i + 0] |= (uint32_t)a[9 * i + 2] << 16;
        r->coeffs[4 * i + 0] &= 0x3FFFF;

        r->coeffs[4 * i + 1]  = a[9 * i + 2] >> 2;
        r->coeffs[4 * i + 1] |= (uint32_t)a[9 * i + 3] << 6;
        r->coeffs[4 * i + 1] |= (uint32_t)a[9 * i + 4] << 14;
        r->coeffs[4 * i + 1] &= 0x3FFFF;

        r->coeffs[4 * i + 2]  = a[9 * i + 4] >> 4;
        r->coeffs[4 * i + 2] |= (uint32_t)a[9 * i + 5] << 4;
        r->coeffs[4 * i + 2] |= (uint32_t)a[9 * i + 6] << 12;
        r->coeffs[4 * i + 2] &= 0x3FFFF;

        r->coeffs[4 * i + 3]  = a[9 * i + 6] >> 6;
        r->coeffs[4 * i + 3] |= (uint32_t)a[9 * i + 7] << 2;
        r->coeffs[4 * i + 3] |= (uint32_t)a[9 * i + 8] << 10;
        r->coeffs[4 * i + 3] &= 0x3FFFF;

        r->coeffs[4 * i + 0] = GAMMA1 - r->coeffs[4 * i + 0];
        r->coeffs[4 * i + 1] = GAMMA1 - r->coeffs[4 * i + 1];
        r->coeffs[4 * i + 2] = GAMMA1 - r->coeffs[4 * i + 2];
        r->coeffs[4 * i + 3] = GAMMA1 - r->coeffs[4 * i + 3];
    }

    DBENCH_STOP(*tpack);
}

/*************************************************
* Name:        polyw1_pack
*
* Description: Bit-pack polynomial w1 with coefficients in [0,15] or [0,43].
*              Input coefficients are assumed to be standard representatives.
*
* Arguments:   - uint8_t *r: pointer to output byte array with at least
*                            POLYW1_PACKED_BYTES bytes
*              - const poly *a: pointer to input polynomial
**************************************************/
void polyw1_pack(uint8_t *r, const poly *a) {
    unsigned int i;
    DBENCH_START();

    for (i = 0; i < N / 4; ++i) {
        r[3 * i + 0]  = a->coeffs[4 * i + 0];
        r[3 * i + 0] |= a->coeffs[4 * i + 1] << 6;
        r[3 * i + 1]  = a->coeffs[4 * i + 1] >> 2;
        r[3 * i + 1] |= a->coeffs[4 * i + 2] << 4;
        r[3 * i + 2]  = a->coeffs[4 * i + 2] >> 4;
        r[3 * i + 2] |= a->coeffs[4 * i + 3] << 2;
    }

    DBENCH_STOP(*tpack);
}

