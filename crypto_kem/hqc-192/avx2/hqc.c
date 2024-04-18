#include "code.h"
#include "gf2x.h"
#include "hqc.h"
#include "parameters.h"
#include "parsing.h"
#include "shake_prng.h"
#include "vector.h"
#include <stdint.h>
#include <string.h>
/**
 * @file hqc.c
 * @brief Implementation of hqc.h
 */



/**
 * @brief Keygen of the HQC_PKE IND_CPA scheme
 *
 * The public key is composed of the syndrome <b>s</b> as well as the <b>seed</b> used to generate the vector <b>h</b>.
 *
 * The secret key is composed of the <b>seed</b> used to generate vectors <b>x</b> and  <b>y</b>.
 * As a technicality, the public key is appended to the secret key in order to respect NIST API.
 *
 * @param[out] pk String containing the public key
 * @param[out] sk String containing the secret key
 */
void PQCLEAN_HQC192_AVX2_hqc_pke_keygen(unsigned char *pk, unsigned char *sk) {
    seedexpander_state sk_seedexpander;
    seedexpander_state pk_seedexpander;
    uint8_t sk_seed[SEED_BYTES] = {0};
    uint8_t sigma[VEC_K_SIZE_BYTES] = {0};
    uint8_t pk_seed[SEED_BYTES] = {0};
    static __m256i h_256[VEC_N_256_SIZE_64 >> 2];
    static __m256i y_256[VEC_N_256_SIZE_64 >> 2];
    static __m256i x_256[VEC_N_256_SIZE_64 >> 2];
    static uint64_t s[VEC_N_256_SIZE_64];
    static __m256i tmp_256[VEC_N_256_SIZE_64 >> 2];

    memset(x_256, 0, (VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
    memset(y_256, 0, (VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
    memset(h_256, 0, (VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));


    // Create seed_expanders for public key and secret key
    PQCLEAN_HQC192_AVX2_shake_prng(sk_seed, SEED_BYTES);
    PQCLEAN_HQC192_AVX2_shake_prng(sigma, VEC_K_SIZE_BYTES);
    PQCLEAN_HQC192_AVX2_seedexpander_init(&sk_seedexpander, sk_seed, SEED_BYTES);

    PQCLEAN_HQC192_AVX2_shake_prng(pk_seed, SEED_BYTES);
    PQCLEAN_HQC192_AVX2_seedexpander_init(&pk_seedexpander, pk_seed, SEED_BYTES);

    // Compute secret key
    PQCLEAN_HQC192_AVX2_vect_set_random_fixed_weight(&sk_seedexpander, x_256, PARAM_OMEGA);
    PQCLEAN_HQC192_AVX2_vect_set_random_fixed_weight(&sk_seedexpander, y_256, PARAM_OMEGA);

    // Compute public key
    PQCLEAN_HQC192_AVX2_vect_set_random(&pk_seedexpander, (uint64_t *) h_256);
    PQCLEAN_HQC192_AVX2_vect_mul(tmp_256, y_256, h_256);
    PQCLEAN_HQC192_AVX2_vect_add(s, (uint64_t *) x_256, (uint64_t *) tmp_256, VEC_N_256_SIZE_64);

    // Parse keys to string
    PQCLEAN_HQC192_AVX2_hqc_public_key_to_string(pk, pk_seed, s);
    PQCLEAN_HQC192_AVX2_hqc_secret_key_to_string(sk, sk_seed, sigma, pk);

}



/**
 * @brief Encryption of the HQC_PKE IND_CPA scheme
 *
 * The cihertext is composed of vectors <b>u</b> and <b>v</b>.
 *
 * @param[out] u Vector u (first part of the ciphertext)
 * @param[out] v Vector v (second part of the ciphertext)
 * @param[in] m Vector representing the message to encrypt
 * @param[in] theta Seed used to derive randomness required for encryption
 * @param[in] pk String containing the public key
 */
void PQCLEAN_HQC192_AVX2_hqc_pke_encrypt(uint64_t *u, uint64_t *v, uint64_t *m, unsigned char *theta, const unsigned char *pk) {
    seedexpander_state PQCLEAN_HQC192_AVX2_seedexpander;
    static __m256i h_256[VEC_N_256_SIZE_64 >> 2];
    static __m256i s_256[VEC_N_256_SIZE_64 >> 2];
    static __m256i r2_256[VEC_N_256_SIZE_64 >> 2];

    static __m256i r1_256[VEC_N_256_SIZE_64 >> 2];
    static __m256i e_256[VEC_N_256_SIZE_64 >> 2];

    static __m256i tmp1_256[VEC_N_256_SIZE_64 >> 2];
    static __m256i tmp2_256[VEC_N_256_SIZE_64 >> 2];
    static __m256i tmp3_256[VEC_N_256_SIZE_64 >> 2];
    static uint64_t tmp4[VEC_N_256_SIZE_64];

    memset(r2_256, 0, (VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
    memset(h_256, 0, (VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
    memset(s_256, 0, (VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
    memset(r1_256, 0, (VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));
    memset(e_256, 0, (VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));

    // Create seed_expander from theta
    PQCLEAN_HQC192_AVX2_seedexpander_init(&PQCLEAN_HQC192_AVX2_seedexpander, theta, SEED_BYTES);

    // Retrieve h and s from public key
    PQCLEAN_HQC192_AVX2_hqc_public_key_from_string((uint64_t *) h_256, (uint64_t *) s_256, pk);

    // Generate r1, r2 and e
    PQCLEAN_HQC192_AVX2_vect_set_random_fixed_weight(&PQCLEAN_HQC192_AVX2_seedexpander, r1_256, PARAM_OMEGA_R);
    PQCLEAN_HQC192_AVX2_vect_set_random_fixed_weight(&PQCLEAN_HQC192_AVX2_seedexpander, r2_256, PARAM_OMEGA_R);
    PQCLEAN_HQC192_AVX2_vect_set_random_fixed_weight(&PQCLEAN_HQC192_AVX2_seedexpander, e_256, PARAM_OMEGA_E);

    // Compute u = r1 + r2.h
    PQCLEAN_HQC192_AVX2_vect_mul(tmp1_256, r2_256, h_256);
    PQCLEAN_HQC192_AVX2_vect_add(u, (uint64_t *) r1_256, (uint64_t *) tmp1_256, VEC_N_256_SIZE_64);

    // Compute v = m.G by encoding the message
    PQCLEAN_HQC192_AVX2_code_encode(v, m);
    PQCLEAN_HQC192_AVX2_vect_resize((uint64_t *) tmp2_256, PARAM_N, v, PARAM_N1N2);

    // Compute v = m.G + s.r2 + e
    PQCLEAN_HQC192_AVX2_vect_mul(tmp3_256, r2_256, s_256);
    PQCLEAN_HQC192_AVX2_vect_add(tmp4, (uint64_t *) e_256, (uint64_t *) tmp3_256, VEC_N_256_SIZE_64);
    PQCLEAN_HQC192_AVX2_vect_add((uint64_t *) tmp3_256, (uint64_t *) tmp2_256, tmp4, VEC_N_256_SIZE_64);
    PQCLEAN_HQC192_AVX2_vect_resize(v, PARAM_N1N2, (uint64_t *) tmp3_256, PARAM_N);

}



/**
 * @brief Decryption of the HQC_PKE IND_CPA scheme
 *
 * @param[out] m Vector representing the decrypted message
 * @param[in] u Vector u (first part of the ciphertext)
 * @param[in] v Vector v (second part of the ciphertext)
 * @param[in] sk String containing the secret key
 * @returns 0
 */
uint8_t PQCLEAN_HQC192_AVX2_hqc_pke_decrypt(uint64_t *m, uint8_t *sigma, const __m256i *u_256, const uint64_t *v, const uint8_t *sk) {
    static __m256i x_256[VEC_N_256_SIZE_64 >> 2] = {0};
    static __m256i y_256[VEC_N_256_SIZE_64 >> 2] = {0};
    uint8_t pk[PUBLIC_KEY_BYTES] = {0};
    static uint64_t tmp1[VEC_N_256_SIZE_64] = {0};
    static uint64_t tmp2[VEC_N_256_SIZE_64] = {0};
    static __m256i tmp3_256[VEC_N_256_SIZE_64 >> 2];

    memset(y_256, 0, (VEC_N_256_SIZE_64 >> 2) * sizeof(__m256i));

    // Retrieve x, y, pk from secret key
    PQCLEAN_HQC192_AVX2_hqc_secret_key_from_string(x_256, y_256, sigma, pk, sk);

    // Compute v - u.y
    PQCLEAN_HQC192_AVX2_vect_resize(tmp1, PARAM_N, v, PARAM_N1N2);
    PQCLEAN_HQC192_AVX2_vect_mul(tmp3_256, y_256, u_256);
    PQCLEAN_HQC192_AVX2_vect_add(tmp2, tmp1, (uint64_t *) tmp3_256, VEC_N_256_SIZE_64);


    // Compute m by decoding v - u.y
    PQCLEAN_HQC192_AVX2_code_decode(m, tmp2);

    return 0;
}
