#include "alpha_table.h"
#include "bch.h"
#include "fft.h"
#include "gf.h"
#include "parameters.h"
#include "vector.h"
#include <immintrin.h>
#include <stdint.h>
#include <string.h>
/**
 * @file bch.c
 * Constant time implementation of BCH codes
 */


static uint16_t mod(uint16_t i, uint16_t modulus);
static void compute_cyclotomic_cosets(uint16_t *cosets, uint16_t upper_bound);
static size_t compute_elp(uint16_t *sigma, const uint16_t *syndromes);
static void message_from_codeword(uint64_t *message, const uint64_t *codeword);
static void compute_syndromes(__m256i *syndromes, const uint64_t *rcv);
static void compute_roots(uint64_t *error, const uint16_t *sigma);

/**
 * @brief Returns i modulo the given modulus.
 *
 * i must be less than 2*modulus.
 * Therefore, the return value is either i or i-modulus.
 * @returns i mod (modulus)
 * @param[in] i The integer whose modulo is taken
 * @param[in] modulus The modulus
 */
static uint16_t mod(uint16_t i, uint16_t modulus) {
    uint16_t tmp = i - modulus;

    // mask = 0xffff if(i < PARAM_GF_MUL_ORDER)
    int16_t mask = -(tmp >> 15);

    return tmp + (mask & modulus);
}



/**
 * @brief Computes the odd binary cyclotomic cosets modulo 2^m-1 for integers less than upper_bound.
 *
 * The array cosets of size 2^m-1 is filled by placing at index i the coset representative of i.
 * @param[out] cosets Array receiving the coset representatives
 * @param[in] upper_bound The upper bound
 */
static void compute_cyclotomic_cosets(uint16_t *cosets, uint16_t upper_bound) {
    // Compute the odd cyclotomic classes
    for (uint16_t i = 1 ; i < upper_bound ; i += 2) {
        if (cosets[i] == 0) { // If i does not already belong to a class
            uint16_t tmp = i;
            size_t j = PARAM_M;
            cosets[i] = i;
            while (--j) { // Complete i's class
                tmp = mod(2 * tmp, PARAM_GF_MUL_ORDER);
                cosets[tmp] = i;
            }
        }
    }
}



/**
 * @brief Computes the generator polynomial of the primitive BCH code with given parameters.
 *
 * Code length is 2^m-1. <br>
 * Parameter t is the targeted correction capacity of the code
 * and receives the real correction capacity (which is at least equal to the target). <br>
 * exp and log are arrays giving antilog and log of GF(2^m) elements.
 * @returns the degree of the generator polynomial
 * @param[out] bch_poly Array of size (m*t + 1) receiving the coefficients of the generator polynomial
 * @param[in,out] t Targeted correction capacity; receives the real correction capacity
 * @param[in] exp Antilog table of GF(2^m)
 * @param[in] log Log table of GF(2^m)
 */
size_t PQCLEAN_HQC192_AVX2_compute_bch_poly(uint16_t *bch_poly, size_t *t, const uint16_t *exp, const uint16_t *log) {
    uint16_t cosets[PARAM_GF_MUL_ORDER];
    size_t deg_bch_poly = 0;

    memset(cosets, 0, 2 * PARAM_GF_MUL_ORDER);
    compute_cyclotomic_cosets(cosets, 2 * *t);

    // Start with bch_poly(X) = 1
    bch_poly[0] = 1;

    for (uint16_t i = 1 ; i < PARAM_GF_MUL_ORDER ; ++i) {
        if (cosets[i] == 0) {
            continue;
        }

        // Multiply bch_poly(X) by X-a^i
        for (size_t j = deg_bch_poly ; j ; --j) {
            int16_t mask = -((uint16_t) - bch_poly[j] >> 15);
            bch_poly[j] = (mask & exp[mod(log[bch_poly[j]] + i, PARAM_GF_MUL_ORDER)]) ^ bch_poly[j - 1];
        }
        bch_poly[0] = exp[mod(log[bch_poly[0]] + i, PARAM_GF_MUL_ORDER)];
        bch_poly[++deg_bch_poly] = 1;
    }

    // Determine the real correction capacity
    while (cosets[2 * *t + 1] != 0) {
        ++*t;
    }

    return deg_bch_poly;
}



/**
 * @brief Computes the values alpha^ij for decoding syndromes
 *
 * function to initialize a table which contains values alpha^ij for i in [0,N1[ and j in [1,2*PARAM_DELTA]
 * these values are used in order to compute the syndromes of the received word v(x)=v_0+v_1x+...+v_{n1-1}x^{n1-1}
 * value alpha^ij is stored in alpha_ij_table[2*PARAM_DELTA*i+j-1]
 * The syndromes are equal to v(alpha^k) for k in [1,2*PARAM_DELTA]
 * Size of the table is fixed to match 256 bit representation
 * Useless values are filled with 0.
 *
 * @param[in] exp Exp look-up-table of GF
 */
void PQCLEAN_HQC192_AVX2_table_alphaij_generation(const uint16_t *exp) {
    int32_t tmp_value;
    int16_t *alpha_tmp;

    // pre-computation of alpha^ij for i in [0, N1[ and j in [1, 2*PARAM_DELTA]
    // see comment of alpha_ij_table_init() function.
    for (uint16_t i = 0; i < PARAM_N1 ; ++i) {
        tmp_value = 0;
        alpha_tmp = table_alpha_ij + i * (PARAM_DELTA << 1);
        for (uint16_t j = 0 ; j < (PARAM_DELTA << 1) ; j++) {
            tmp_value = PQCLEAN_HQC192_AVX2_gf_mod(tmp_value + i);
            alpha_tmp[j] = exp[tmp_value];
        }
    }
}



/**
 * @brief Computes the error locator polynomial (ELP) sigma
 *
 * This is a constant time implementation of Berlekamp's simplified algorithm (see @cite joiner1995decoding). <br>
 * We use the letter p for rho which is initialized at -1/2. <br>
 * The array X_sigma_p represents the polynomial X^(2(mu-rho))*sigma_p(X). <br>
 * Instead of maintaining a list of sigmas, we update in place both sigma and X_sigma_p. <br>
 * sigma_copy serves as a temporary save of sigma in case X_sigma_p needs to be updated. <br>
 * We can properly correct only if the degree of sigma does not exceed PARAM_DELTA.
 * This means only the first PARAM_DELTA + 1 coefficients of sigma are of value
 * and we only need to save its first PARAM_DELTA - 1 coefficients.
 *
 * @returns the degree of the ELP sigma
 * @param[out] sigma Array of size (at least) PARAM_DELTA receiving the ELP
 * @param[in] syndromes Array of size (at least) 2*PARAM_DELTA storing the syndromes
 */
static size_t compute_elp(uint16_t *sigma, const uint16_t *syndromes) {
    sigma[0] = 1;
    size_t deg_sigma = 0;
    size_t deg_sigma_p = 0;
    uint16_t sigma_copy[PARAM_DELTA - 1] = {0};
    size_t deg_sigma_copy = 0;
    uint16_t X_sigma_p[PARAM_DELTA + 1] = {0, 1};
    int32_t pp = -1; // 2*rho
    uint16_t d_p = 1;
    uint16_t d = syndromes[0];

    for (size_t mu = 0 ; mu < PARAM_DELTA ; ++mu) {
        // Save sigma in case we need it to update X_sigma_p
        memcpy(sigma_copy, sigma, 2 * (PARAM_DELTA - 1));
        deg_sigma_copy = deg_sigma;

        uint16_t dd = PQCLEAN_HQC192_AVX2_gf_mul(d, PQCLEAN_HQC192_AVX2_gf_inverse(d_p)); // 0 if(d == 0)
        for (size_t i = 1 ; (i <= 2 * mu + 1) && (i <= PARAM_DELTA) ; ++i) {
            sigma[i] ^= PQCLEAN_HQC192_AVX2_gf_mul(dd, X_sigma_p[i]);
        }

        size_t deg_X = 2 * mu - pp; // 2*(mu-rho)
        size_t deg_X_sigma_p = deg_X + deg_sigma_p;

        // mask1 = 0xffff if(d != 0) and 0 otherwise
        int16_t mask1 = -((uint16_t) - d >> 15);

        // mask2 = 0xffff if(deg_X_sigma_p > deg_sigma) and 0 otherwise
        int16_t mask2 = -((uint16_t) (deg_sigma - deg_X_sigma_p) >> 15);

        // mask12 = 0xffff if the deg_sigma increased and 0 otherwise
        int16_t mask12 = mask1 & mask2;
        deg_sigma = (mask12 & deg_X_sigma_p) ^ (~mask12 & deg_sigma);

        if (mu == PARAM_DELTA - 1) {
            break;
        }

        // Update pp, d_p and X_sigma_p if needed
        pp = (mask12 & (2 * mu)) ^ (~mask12 & pp);
        d_p = (mask12 & d) ^ (~mask12 & d_p);
        for (size_t i = PARAM_DELTA - 1 ; i ; --i) {
            X_sigma_p[i + 1] = (mask12 & sigma_copy[i - 1]) ^ (~mask12 & X_sigma_p[i - 1]);
        }
        X_sigma_p[1] = 0;
        X_sigma_p[0] = 0;
        deg_sigma_p = (mask12 & deg_sigma_copy) ^ (~mask12 & deg_sigma_p);

        // Compute the next discrepancy
        d = syndromes[2 * mu + 2];
        for (size_t i = 1 ; (i <= 2 * mu + 1) && (i <= PARAM_DELTA) ; ++i) {
            d ^= PQCLEAN_HQC192_AVX2_gf_mul(sigma[i], syndromes[2 * mu + 2 - i]);
        }
    }

    return deg_sigma;
}



/**
 * @brief Retrieves the message message from the codeword codeword
 *
 * Since we performed a systematic encoding, the message is the last PARAM_K bits of the codeword.
 *
 * @param[out] message Array of size VEC_K_SIZE_BYTES receiving the message
 * @param[in] codeword Array of size VEC_N1_SIZE_BYTES storing the codeword
 */
static void message_from_codeword(uint64_t *message, const uint64_t *codeword) {
    int32_t val = PARAM_N1 - PARAM_K;

    uint64_t mask1 = (uint64_t) (0xffffffffffffffff << val % 64);
    uint64_t mask2 = (uint64_t) (0xffffffffffffffff >> (64 - val % 64));
    size_t index = val / 64;

    for (size_t i = 0 ; i < VEC_K_SIZE_64 - 1 ; ++i) {
        uint64_t message1 = (codeword[index] & mask1) >> val % 64;
        uint64_t message2 = (codeword[++index] & mask2) << (64 - val % 64);
        message[i] = message1 | message2;
    }

    // Last byte (8-val % 8 is the number of bits given by message1)
    if ((PARAM_K % 64 == 0) || (64 - val % 64 < PARAM_K % 64)) {
        uint64_t message1 = (codeword[index] & mask1) >> val % 64;
        uint64_t message2 = (codeword[++index] & mask2) << (64 - val % 64);
        message[VEC_K_SIZE_64 - 1] = message1 | message2;
    } else {
        uint64_t message1 = (codeword[index] & mask1) >> val % 64;
        message[VEC_K_SIZE_64 - 1] = message1;
    }
}



/**
 * @brief Computes the 2^PARAM_DELTA syndromes from the received vector vector
 *
 * Syndromes are the sum of powers of alpha weighted by vector's coefficients.
 * These powers have been pre-computed in table_alphaPARAM_DELTA.h
 * Syndromes are 16-bits long , hence we can simultaneously compute 16 syndromes
 * in a 256-bit register
 *
 * @param[out] syndromes Array of size 2^(PARAM_FFT_T) receiving the 2*PARAM_DELTA syndromes
 * @param[in] rcv Array of size VEC_N1_SIZE_BYTES storing the received word
 */
void compute_syndromes(__m256i *syndromes, const uint64_t *rcv) {
    const __m256i zero_256 = _mm256_set1_epi64x(0);
    const __m256i mask_one = _mm256_set_epi64x(0x0303030303030303, 0x0202020202020202, 0x0101010101010101, 0x0);
    const __m256i mask_two  = _mm256_set1_epi64x(-0x7FBFDFEFF7FBFDFF);
    const __m256i un_256 = _mm256_set1_epi64x(1);

    __m256i y;
    __m256i S;
    __m256i L;
    __m256i tmp_repeat;
    uint32_t *aux;
    int16_t *alpha_tmp;
    uint32_t i;
    // static variable so that it is stored in the DATA segment
    // not in the STACK segment
    static uint8_t tmp_array[PARAM_N1 + 4]; // +4 to control overflow due to management of 256 bits
    __m256i *z = (__m256i *) tmp_array;
    // vectorized version of the separation of the coordinates of the vector v in order to put each coordinate in an unsigned char
    // aux is used to consider 4 elements in v at each step of the loop
    aux = (uint32_t *) rcv;
    for (i = 0 ; i < ((VEC_N1_SIZE_BYTES >> 2) << 2) ; i += 4) {
        // duplicate aux 8 times in y , i.e y= (aux aux aux .... aux)
        y = _mm256_set1_epi32(*aux);
        // shuffle the bytes of y so that if aux=(a0 a1 a2 a3)
        // then y = (a0 a0 a0 a0 a0 a0 a0 a0 a1 a1 a1 a1 a1 a1 a1 a1 .... a3)
        y = _mm256_shuffle_epi8(y, mask_one);
        // apply a mask on each byte of y to determine if jth bit of a_k is 0 or 1
        z[i >> 2] = _mm256_and_si256(y, mask_two);
        aux ++;
    }

    // Evaluation of the polynomial corresponding to the vector v in alpha^i for i in {1, ..., 2 * PARAM_DELTA}
    for (size_t j = 0 ; j < SYND_SIZE_256 ; ++j) {
        S = zero_256;
        alpha_tmp = table_alpha_ij + (j << 4);

        for (size_t i = 0 ; i < PARAM_N1 ; ++i) {
            tmp_repeat = _mm256_set1_epi64x((long long)(tmp_array[i] != 0));
            L = _mm256_cmpeq_epi64(tmp_repeat, un_256);
            tmp_repeat = _mm256_lddqu_si256((__m256i *)(alpha_tmp + i * (PARAM_DELTA << 1)));
            L = _mm256_and_si256(L, tmp_repeat);
            S = _mm256_xor_si256(L, S);
        }
        _mm256_storeu_si256(syndromes + j, S);
    }
}


/**
 * @brief Computes the error polynomial error from the error locator polynomial sigma
 *
 * See function PQCLEAN_HQC192_AVX2_fft for more details.
 *
 * @param[out] error Array of VEC_N1_SIZE_BYTES elements receiving the error polynomial
 * @param[in] sigma Array of 2^PARAM_FFT elements storing the error locator polynomial
 */
static void compute_roots(uint64_t *error, const uint16_t *sigma) {
    uint16_t w[1 << PARAM_M] = {0}; // w will receive the evaluation of sigma in all field elements

    PQCLEAN_HQC192_AVX2_fft(w, sigma, PARAM_DELTA + 1);
    PQCLEAN_HQC192_AVX2_fft_retrieve_bch_error_poly(error, w);
}



/**
 * @brief Decodes the received word
 *
 * This function relies on four steps:
 *    <ol>
 *    <li> The first step, done by additive FFT transpose, is the computation of the 2*PARAM_DELTA syndromes.
 *    <li> The second step is the computation of the error-locator polynomial sigma.
 *    <li> The third step, done by additive FFT, is finding the error-locator numbers by calculating the roots of the polynomial sigma and takings their inverses.
 *    <li> The fourth step is the correction of the errors in the received polynomial.
 *    </ol>
 * For a more complete picture on BCH decoding, see Shu. Lin and Daniel J. Costello in Error Control Coding: Fundamentals and Applications @cite lin1983error
 *
 * @param[out] message Array of size VEC_K_SIZE_BYTES receiving the decoded message
 * @param[in] vector Array of size VEC_N1_SIZE_BYTES storing the received word
 */

void PQCLEAN_HQC192_AVX2_bch_code_decode(uint64_t *message, uint64_t *vector) {
    uint16_t sigma[1 << PARAM_FFT] = {0};
    uint64_t error[(1 << PARAM_M) / 8] = {0};
    static __m256i syndromes_256[SYND_SIZE_256];

    // Calculate the 2*PARAM_DELTA syndromes
    compute_syndromes(syndromes_256, vector);

    // Compute the error locator polynomial sigma
    // Sigma's degree is at most PARAM_DELTA but the FFT requires the extra room
    compute_elp(sigma, (uint16_t *)syndromes_256);

    // Compute the error polynomial error
    compute_roots(error, sigma);

    // Add the error polynomial to the received polynomial
    PQCLEAN_HQC192_AVX2_vect_add(vector, vector, error, VEC_N1_SIZE_64);

    // Retrieve the message from the decoded codeword
    message_from_codeword(message, vector);

}
