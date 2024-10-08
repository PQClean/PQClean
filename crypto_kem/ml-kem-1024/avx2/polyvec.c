#include "consts.h"
#include "ntt.h"
#include "params.h"
#include "poly.h"
#include "polyvec.h"
#include <immintrin.h>
#include <stdint.h>
#include <string.h>

static void poly_compress11(uint8_t r[352 + 2], const poly *restrict a) {
    unsigned int i;
    __m256i f0, f1, f2;
    __m128i t0, t1;
    const __m256i v = _mm256_load_si256(&PQCLEAN_MLKEM1024_AVX2_qdata.vec[_16XV / 16]);
    const __m256i v8 = _mm256_slli_epi16(v, 3);
    const __m256i off = _mm256_set1_epi16(36);
    const __m256i shift1 = _mm256_set1_epi16(1 << 13);
    const __m256i mask = _mm256_set1_epi16(2047);
    const __m256i shift2 = _mm256_set1_epi64x((2048LL << 48) + (1LL << 32) + (2048 << 16) + 1);
    const __m256i sllvdidx = _mm256_set1_epi64x(10);
    const __m256i srlvqidx = _mm256_set_epi64x(30, 10, 30, 10);
    const __m256i shufbidx = _mm256_set_epi8( 4, 3, 2, 1, 0, 0, -1, -1, -1, -1, 10, 9, 8, 7, 6, 5,
                             -1, -1, -1, -1, -1, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);

    for (i = 0; i < KYBER_N / 16; i++) {
        f0 = _mm256_load_si256(&a->vec[i]);
        f1 = _mm256_mullo_epi16(f0, v8);
        f2 = _mm256_add_epi16(f0, off);
        f0 = _mm256_slli_epi16(f0, 3);
        f0 = _mm256_mulhi_epi16(f0, v);
        f2 = _mm256_sub_epi16(f1, f2);
        f1 = _mm256_andnot_si256(f1, f2);
        f1 = _mm256_srli_epi16(f1, 15);
        f0 = _mm256_sub_epi16(f0, f1);
        f0 = _mm256_mulhrs_epi16(f0, shift1);
        f0 = _mm256_and_si256(f0, mask);
        f0 = _mm256_madd_epi16(f0, shift2);
        f0 = _mm256_sllv_epi32(f0, sllvdidx);
        f1 = _mm256_bsrli_epi128(f0, 8);
        f0 = _mm256_srlv_epi64(f0, srlvqidx);
        f1 = _mm256_slli_epi64(f1, 34);
        f0 = _mm256_add_epi64(f0, f1);
        f0 = _mm256_shuffle_epi8(f0, shufbidx);
        t0 = _mm256_castsi256_si128(f0);
        t1 = _mm256_extracti128_si256(f0, 1);
        t0 = _mm_blendv_epi8(t0, t1, _mm256_castsi256_si128(shufbidx));
        _mm_storeu_si128((__m128i *)&r[22 * i + 0], t0);
        _mm_storel_epi64((__m128i *)&r[22 * i + 16], t1);
    }
}

static void poly_decompress11(poly *restrict r, const uint8_t a[352 + 10]) {
    unsigned int i;
    __m256i f;
    const __m256i q = _mm256_load_si256(&PQCLEAN_MLKEM1024_AVX2_qdata.vec[_16XQ / 16]);
    const __m256i shufbidx = _mm256_set_epi8(13, 12, 12, 11, 10, 9, 9, 8,
                             8, 7, 6, 5, 5, 4, 4, 3,
                             10, 9, 9, 8, 7, 6, 6, 5,
                             5, 4, 3, 2, 2, 1, 1, 0);
    const __m256i srlvdidx = _mm256_set_epi32(0, 0, 1, 0, 0, 0, 1, 0);
    const __m256i srlvqidx = _mm256_set_epi64x(2, 0, 2, 0);
    const __m256i shift = _mm256_set_epi16(4, 32, 1, 8, 32, 1, 4, 32, 4, 32, 1, 8, 32, 1, 4, 32);
    const __m256i mask = _mm256_set1_epi16(32752);

    for (i = 0; i < KYBER_N / 16; i++) {
        f = _mm256_loadu_si256((__m256i *)&a[22 * i]);
        f = _mm256_permute4x64_epi64(f, 0x94);
        f = _mm256_shuffle_epi8(f, shufbidx);
        f = _mm256_srlv_epi32(f, srlvdidx);
        f = _mm256_srlv_epi64(f, srlvqidx);
        f = _mm256_mullo_epi16(f, shift);
        f = _mm256_srli_epi16(f, 1);
        f = _mm256_and_si256(f, mask);
        f = _mm256_mulhrs_epi16(f, q);
        _mm256_store_si256(&r->vec[i], f);
    }
}


/*************************************************
* Name:        PQCLEAN_MLKEM1024_AVX2_polyvec_compress
*
* Description: Compress and serialize vector of polynomials
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (needs space for KYBER_POLYVECCOMPRESSEDBYTES)
*              - polyvec *a: pointer to input vector of polynomials
**************************************************/
void PQCLEAN_MLKEM1024_AVX2_polyvec_compress(uint8_t r[KYBER_POLYVECCOMPRESSEDBYTES + 2], const polyvec *a) {
    size_t i;

    for (i = 0; i < KYBER_K; i++) {
        poly_compress11(&r[352 * i], &a->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_MLKEM1024_AVX2_polyvec_decompress
*
* Description: De-serialize and decompress vector of polynomials;
*              approximate inverse of PQCLEAN_MLKEM1024_AVX2_polyvec_compress
*
* Arguments:   - polyvec *r: pointer to output vector of polynomials
*              - const uint8_t *a: pointer to input byte array
*                                  (of length KYBER_POLYVECCOMPRESSEDBYTES)
**************************************************/
void PQCLEAN_MLKEM1024_AVX2_polyvec_decompress(polyvec *r, const uint8_t a[KYBER_POLYVECCOMPRESSEDBYTES + 12]) {
    size_t i;

    for (i = 0; i < KYBER_K; i++) {
        poly_decompress11(&r->vec[i], &a[352 * i]);
    }
}

/*************************************************
* Name:        PQCLEAN_MLKEM1024_AVX2_polyvec_tobytes
*
* Description: Serialize vector of polynomials
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (needs space for KYBER_POLYVECBYTES)
*              - polyvec *a: pointer to input vector of polynomials
**************************************************/
void PQCLEAN_MLKEM1024_AVX2_polyvec_tobytes(uint8_t r[KYBER_POLYVECBYTES], const polyvec *a) {
    size_t i;
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_MLKEM1024_AVX2_poly_tobytes(r + i * KYBER_POLYBYTES, &a->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_MLKEM1024_AVX2_polyvec_frombytes
*
* Description: De-serialize vector of polynomials;
*              inverse of PQCLEAN_MLKEM1024_AVX2_polyvec_tobytes
*
* Arguments:   - uint8_t *r: pointer to output byte array
*              - const polyvec *a: pointer to input vector of polynomials
*                                  (of length KYBER_POLYVECBYTES)
**************************************************/
void PQCLEAN_MLKEM1024_AVX2_polyvec_frombytes(polyvec *r, const uint8_t a[KYBER_POLYVECBYTES]) {
    size_t i;
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_MLKEM1024_AVX2_poly_frombytes(&r->vec[i], a + i * KYBER_POLYBYTES);
    }
}

/*************************************************
* Name:        PQCLEAN_MLKEM1024_AVX2_polyvec_ntt
*
* Description: Apply forward NTT to all elements of a vector of polynomials
*
* Arguments:   - polyvec *r: pointer to in/output vector of polynomials
**************************************************/
void PQCLEAN_MLKEM1024_AVX2_polyvec_ntt(polyvec *r) {
    size_t i;
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_MLKEM1024_AVX2_poly_ntt(&r->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_MLKEM1024_AVX2_polyvec_invntt_tomont
*
* Description: Apply inverse NTT to all elements of a vector of polynomials
*              and multiply by Montgomery factor 2^16
*
* Arguments:   - polyvec *r: pointer to in/output vector of polynomials
**************************************************/
void PQCLEAN_MLKEM1024_AVX2_polyvec_invntt_tomont(polyvec *r) {
    size_t i;
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_MLKEM1024_AVX2_poly_invntt_tomont(&r->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_MLKEM1024_AVX2_polyvec_basemul_acc_montgomery
*
* Description: Multiply elements in a and b in NTT domain, accumulate into r,
*              and multiply by 2^-16.
*
* Arguments: - poly *r: pointer to output polynomial
*            - const polyvec *a: pointer to first input vector of polynomials
*            - const polyvec *b: pointer to second input vector of polynomials
**************************************************/
void PQCLEAN_MLKEM1024_AVX2_polyvec_basemul_acc_montgomery(poly *r, const polyvec *a, const polyvec *b) {
    size_t i;
    poly tmp;

    PQCLEAN_MLKEM1024_AVX2_poly_basemul_montgomery(r, &a->vec[0], &b->vec[0]);
    for (i = 1; i < KYBER_K; i++) {
        PQCLEAN_MLKEM1024_AVX2_poly_basemul_montgomery(&tmp, &a->vec[i], &b->vec[i]);
        PQCLEAN_MLKEM1024_AVX2_poly_add(r, r, &tmp);
    }
}

/*************************************************
* Name:        PQCLEAN_MLKEM1024_AVX2_polyvec_reduce
*
* Description: Applies Barrett reduction to each coefficient
*              of each element of a vector of polynomials;
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments:   - polyvec *r: pointer to input/output polynomial
**************************************************/
void PQCLEAN_MLKEM1024_AVX2_polyvec_reduce(polyvec *r) {
    size_t i;
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_MLKEM1024_AVX2_poly_reduce(&r->vec[i]);
    }
}

/*************************************************
* Name:        PQCLEAN_MLKEM1024_AVX2_polyvec_add
*
* Description: Add vectors of polynomials
*
* Arguments: - polyvec *r:       pointer to output vector of polynomials
*            - const polyvec *a: pointer to first input vector of polynomials
*            - const polyvec *b: pointer to second input vector of polynomials
**************************************************/
void PQCLEAN_MLKEM1024_AVX2_polyvec_add(polyvec *r, const polyvec *a, const polyvec *b) {
    size_t i;
    for (i = 0; i < KYBER_K; i++) {
        PQCLEAN_MLKEM1024_AVX2_poly_add(&r->vec[i], &a->vec[i], &b->vec[i]);
    }
}
