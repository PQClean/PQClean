#include "api.h"
#include "fips202.h"
#include "hqc.h"
#include "parameters.h"
#include "parsing.h"
#include "shake_ds.h"
#include "vector.h"
#include <immintrin.h>
#include <stdint.h>
#include <string.h>
/**
 * @file kem.c
 * @brief Implementation of api.h
 */



/**
 * @brief Keygen of the HQC_KEM IND_CAA2 scheme
 *
 * The public key is composed of the syndrome <b>s</b> as well as the seed used to generate the vector <b>h</b>.
 *
 * The secret key is composed of the seed used to generate vectors <b>x</b> and <b>y</b>.
 * As a technicality, the public key is appended to the secret key in order to respect NIST API.
 *
 * @param[out] pk String containing the public key
 * @param[out] sk String containing the secret key
 * @returns 0 if keygen is successful
 */
int PQCLEAN_HQC192_AVX2_crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {

    PQCLEAN_HQC192_AVX2_hqc_pke_keygen(pk, sk);
    return 0;
}



/**
 * @brief Encapsulation of the HQC_KEM IND_CAA2 scheme
 *
 * @param[out] ct String containing the ciphertext
 * @param[out] ss String containing the shared secret
 * @param[in] pk String containing the public key
 * @returns 0 if encapsulation is successful
 */
int PQCLEAN_HQC192_AVX2_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk) {

    uint8_t theta[SHAKE256_512_BYTES] = {0};
    uint8_t m[VEC_K_SIZE_BYTES] = {0};
    static uint64_t u[VEC_N_256_SIZE_64] = {0};
    uint64_t v[VEC_N1N2_256_SIZE_64] = {0};
    uint8_t mc[VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES] = {0};
    uint64_t salt[SALT_SIZE_64] = {0};
    uint8_t tmp[VEC_K_SIZE_BYTES + PUBLIC_KEY_BYTES + SALT_SIZE_BYTES] = {0};
    shake256incctx shake256state;

    // Computing m
    PQCLEAN_HQC192_AVX2_vect_set_random_from_prng((uint64_t *)m, VEC_K_SIZE_64);

    // Computing theta
    PQCLEAN_HQC192_AVX2_vect_set_random_from_prng(salt, SALT_SIZE_64);
    memcpy(tmp, m, VEC_K_SIZE_BYTES);
    memcpy(tmp + VEC_K_SIZE_BYTES, pk, PUBLIC_KEY_BYTES);
    memcpy(tmp + VEC_K_SIZE_BYTES + PUBLIC_KEY_BYTES, salt, SALT_SIZE_BYTES);
    PQCLEAN_HQC192_AVX2_shake256_512_ds(&shake256state, theta, tmp, VEC_K_SIZE_BYTES + PUBLIC_KEY_BYTES + SALT_SIZE_BYTES, G_FCT_DOMAIN);

    // Encrypting m
    PQCLEAN_HQC192_AVX2_hqc_pke_encrypt(u, v, (uint64_t *)m, theta, pk);

    // Computing shared secret
    memcpy(mc, m, VEC_K_SIZE_BYTES);
    memcpy(mc + VEC_K_SIZE_BYTES, u, VEC_N_SIZE_BYTES);
    memcpy(mc + VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES, v, VEC_N1N2_SIZE_BYTES);
    PQCLEAN_HQC192_AVX2_shake256_512_ds(&shake256state, ss, mc, VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES, K_FCT_DOMAIN);

    // Computing ciphertext
    PQCLEAN_HQC192_AVX2_hqc_ciphertext_to_string(ct, u, v, salt);


    return 0;
}



/**
 * @brief Decapsulation of the HQC_KEM IND_CAA2 scheme
 *
 * @param[out] ss String containing the shared secret
 * @param[in] ct String containing the cipĥertext
 * @param[in] sk String containing the secret key
 * @returns 0 if decapsulation is successful, -1 otherwise
 */
int PQCLEAN_HQC192_AVX2_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk) {

    uint8_t result;
    __m256i u_256[VEC_N_256_SIZE_64 >> 2] = {0};
    uint64_t v[VEC_N1N2_256_SIZE_64] = {0};
    uint8_t pk[PUBLIC_KEY_BYTES] = {0};
    uint8_t m[VEC_K_SIZE_BYTES] = {0};
    uint8_t sigma[VEC_K_SIZE_BYTES] = {0};
    uint8_t theta[SHAKE256_512_BYTES] = {0};
    uint64_t u2[VEC_N_256_SIZE_64] = {0};
    uint64_t v2[VEC_N1N2_256_SIZE_64] = {0};
    uint8_t mc[VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES] = {0};
    uint64_t salt[SALT_SIZE_64] = {0};
    uint8_t tmp[VEC_K_SIZE_BYTES + PUBLIC_KEY_BYTES + SALT_SIZE_BYTES] = {0};
    shake256incctx shake256state;

    // Retrieving u, v and d from ciphertext
    PQCLEAN_HQC192_AVX2_hqc_ciphertext_from_string((uint64_t *) u_256, v, salt, ct);

    // Retrieving pk from sk
    memcpy(pk, sk + SEED_BYTES, PUBLIC_KEY_BYTES);

    // Decrypting
    result = PQCLEAN_HQC192_AVX2_hqc_pke_decrypt((uint64_t *)m, sigma, u_256, v, sk);

    // Computing theta
    memcpy(tmp, m, VEC_K_SIZE_BYTES);
    memcpy(tmp + VEC_K_SIZE_BYTES, pk, PUBLIC_KEY_BYTES);
    memcpy(tmp + VEC_K_SIZE_BYTES + PUBLIC_KEY_BYTES, salt, SALT_SIZE_BYTES);
    PQCLEAN_HQC192_AVX2_shake256_512_ds(&shake256state, theta, tmp, VEC_K_SIZE_BYTES + PUBLIC_KEY_BYTES + SALT_SIZE_BYTES, G_FCT_DOMAIN);

    // Encrypting m'
    PQCLEAN_HQC192_AVX2_hqc_pke_encrypt(u2, v2, (uint64_t *)m, theta, pk);

    // Check if c != c'
    result |= PQCLEAN_HQC192_AVX2_vect_compare((uint8_t *) u_256, (uint8_t *) u2, VEC_N_SIZE_BYTES);
    result |= PQCLEAN_HQC192_AVX2_vect_compare((uint8_t *) v, (uint8_t *) v2, VEC_N1N2_SIZE_BYTES);

    result = (uint8_t) (-((int16_t) result) >> 15);

    for (size_t i = 0; i < VEC_K_SIZE_BYTES; ++i) {
        mc[i] = (m[i] & result) ^ (sigma[i] & ~result);
    }

    // Computing shared secret
    memcpy(mc + VEC_K_SIZE_BYTES, u_256, VEC_N_SIZE_BYTES);
    memcpy(mc + VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES, v, VEC_N1N2_SIZE_BYTES);
    PQCLEAN_HQC192_AVX2_shake256_512_ds(&shake256state, ss, mc, VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES, K_FCT_DOMAIN);


    return -(~result & 1);
}
