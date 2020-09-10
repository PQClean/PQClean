#include "nistseedexpander.h"
#include "parameters.h"
#include "parsing.h"
#include "randombytes.h"
#include "vector.h"
#include <immintrin.h>
#include <stdint.h>
#include <string.h>
/**
 * @file vector.c
 * @brief Implementation of vectors sampling and some utilities for the HQC scheme
 */



/**
 * @brief Generates a vector of a given Hamming weight
 *
 * This function generates uniformly at random a binary vector of a Hamming weight equal to the parameter <b>weight</b>.
 * To generate the vector we have to sample uniformly at random values in the interval [0, PARAM_N -1]. Suppose the PARAM_N is equal to \f$ 70853 \f$, to select a position \f$ r\f$ the function works as follow:
 *  1. It makes a call to the seedexpander function to obtain a random number \f$ x\f$ in \f$ [0, 2^{24}[ \f$.
 *  2. Let \f$ t = \lfloor {2^{24} \over 70853} \rfloor \times  70853\f$
 *  3. If \f$ x \geq t\f$, go to 1
 *  4. It return \f$ r = x \mod 70853\f$
 *
 * The parameter \f$ t \f$ is precomputed and it's denoted by UTILS_REJECTION_THRESHOLD (see the file parameters.h).
 *
 * @param[in] v Pointer to an array
 * @param[in] weight Integer that is the Hamming weight
 * @param[in] ctx Pointer to the context of the seed expander
 */
void PQCLEAN_HQCRMRS256_AVX2_vect_set_random_fixed_weight(AES_XOF_struct *ctx, uint64_t *v, uint16_t weight) {
    size_t random_bytes_size = 3 * weight;
    uint8_t rand_bytes[3 * PARAM_OMEGA_R] = {0};
    uint32_t random_data = 0;
    uint32_t tmp[PARAM_OMEGA_R] = {0};
    uint8_t exist = 0;
    size_t j = 0;
    __m256i bit256[PARAM_OMEGA_R];
    __m256i bloc256[PARAM_OMEGA_R];
    static __m256i posCmp256 = (__m256i) {
        0UL, 1UL, 2UL, 3UL
    };
#define LOOP_SIZE CEIL_DIVIDE(PARAM_N, 256)

    seedexpander(ctx, rand_bytes, random_bytes_size);

    for (uint32_t i = 0; i < weight; ++i) {
        exist = 0;
        do {
            if (j == random_bytes_size) {
                seedexpander(ctx, rand_bytes, random_bytes_size);
                j = 0;
            }

            random_data  = ((uint32_t) rand_bytes[j++]) << 16;
            random_data |= ((uint32_t) rand_bytes[j++]) << 8;
            random_data |= rand_bytes[j++];

        } while (random_data >= UTILS_REJECTION_THRESHOLD);

        random_data = random_data % PARAM_N;

        for (uint32_t k = 0; k < i; k++) {
            if (tmp[k] == random_data) {
                exist = 1;
            }
        }

        if (exist == 1) {
            i--;
        } else {
            tmp[i] = random_data;
        }
    }

    for (uint32_t i = 0; i < weight; i++) {
        // we store the bloc number and bit position of each vb[i]
        uint64_t bloc = tmp[i] >> 6;
        bloc256[i] = _mm256_set1_epi64x(bloc >> 2);
        uint64_t pos = (bloc & 0x3UL);
        __m256i pos256 = _mm256_set1_epi64x(pos);
        __m256i mask256 = _mm256_cmpeq_epi64(pos256, posCmp256);
        uint64_t bit64 = 1ULL << (tmp[i] & 0x3f);
        __m256i bloc256 = _mm256_set1_epi64x(bit64);
        bit256[i] = bloc256 & mask256;
    }

    for (uint32_t i = 0; i < LOOP_SIZE; i++) {
        __m256i aux = _mm256_loadu_si256(((__m256i *)v) + i);
        __m256i i256 = _mm256_set1_epi64x(i);

        for (uint32_t j = 0; j < weight; j++) {
            __m256i mask256 = _mm256_cmpeq_epi64(bloc256[j], i256);
            aux ^= bit256[j] & mask256;
        }
        _mm256_storeu_si256(((__m256i *)v) + i, aux);
    }

#undef LOOP_SIZE
}



/**
 * @brief Generates a random vector of dimension <b>PARAM_N</b>
 *
 * This function generates a random binary vector of dimension <b>PARAM_N</b>. It generates a random
 * array of bytes using the seedexpander function, and drop the extra bits using a mask.
 *
 * @param[in] v Pointer to an array
 * @param[in] ctx Pointer to the context of the seed expander
 */
void PQCLEAN_HQCRMRS256_AVX2_vect_set_random(AES_XOF_struct *ctx, uint64_t *v) {
    uint8_t rand_bytes[VEC_N_SIZE_BYTES] = {0};

    seedexpander(ctx, rand_bytes, VEC_N_SIZE_BYTES);

    PQCLEAN_HQCRMRS256_AVX2_load8_arr(v, VEC_N_SIZE_64, rand_bytes, VEC_N1N2_SIZE_BYTES);
    v[VEC_N_SIZE_64 - 1] &= RED_MASK;
}



/**
 * @brief Generates a random vector
 *
 * This function generates a random binary vector. It uses the the randombytes function.
 *
 * @param[in] v Pointer to an array
 */
void PQCLEAN_HQCRMRS256_AVX2_vect_set_random_from_randombytes(uint64_t *v) {
    uint8_t rand_bytes [VEC_K_SIZE_BYTES] = {0};

    randombytes(rand_bytes, VEC_K_SIZE_BYTES);
    PQCLEAN_HQCRMRS256_AVX2_load8_arr(v, VEC_K_SIZE_64, rand_bytes, VEC_K_SIZE_BYTES);
}



/**
 * @brief Adds two vectors
 *
 * @param[out] o Pointer to an array that is the result
 * @param[in] v1 Pointer to an array that is the first vector
 * @param[in] v2 Pointer to an array that is the second vector
 * @param[in] size Integer that is the size of the vectors
 */
void PQCLEAN_HQCRMRS256_AVX2_vect_add(uint64_t *o, const uint64_t *v1, const uint64_t *v2, uint32_t size) {
    for (uint32_t i = 0; i < size; ++i) {
        o[i] = v1[i] ^ v2[i];
    }
}



/**
 * @brief Compares two vectors
 *
 * @param[in] v1 Pointer to an array that is first vector
 * @param[in] v2 Pointer to an array that is second vector
 * @param[in] size Integer that is the size of the vectors
 * @returns 0 if the vectors are equals and a negative/psotive value otherwise
 */
int PQCLEAN_HQCRMRS256_AVX2_vect_compare(const uint64_t *v1, const uint64_t *v2, uint32_t size) {
    unsigned char diff = 0;

    for (uint32_t i = 0; i < size; i++) {
        diff |= ((uint8_t *) v1)[i] ^ ((uint8_t *) v2)[i];
    }
    return diff != 0;
}



/**
 * @brief Resize a vector so that it contains <b>size_o</b> bits
 *
 * @param[out] o Pointer to the output vector
 * @param[in] size_o Integer that is the size of the output vector in bits
 * @param[in] v Pointer to the input vector
 * @param[in] size_v Integer that is the size of the input vector in bits
 */
void PQCLEAN_HQCRMRS256_AVX2_vect_resize(uint64_t *o, uint32_t size_o, const uint64_t *v, uint32_t size_v) {
    if (size_o < size_v) {
        uint64_t mask = 0x7FFFFFFFFFFFFFFF;
        int8_t val = 0;

        if (size_o % 64) {
            val = 64 - (size_o % 64);
        }

        memcpy(o, v, VEC_N1N2_SIZE_BYTES);

        for (int8_t i = 0; i < val; ++i) {
            o[VEC_N1N2_SIZE_64 - 1] &= (mask >> i);
        }
    } else {
        memcpy(o, v, CEIL_DIVIDE(size_v, 8));
    }
}
