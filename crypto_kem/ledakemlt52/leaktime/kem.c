#include "api.h"
#include "niederreiter.h"
#include "randombytes.h"
#include "rng.h"

#include <string.h>

/*
static void pack_pk(uint8_t *pk_bytes, publicKeyNiederreiter_t *pk) {
    size_t i;
    for (i = 0; i < N0 - 1; i++) {
        PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_tobytes(pk_bytes + i * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B,
                pk->Mtr + i * NUM_DIGITS_GF2X_ELEMENT);
    }
}

static void unpack_pk(publicKeyNiederreiter_t *pk, const uint8_t *pk_bytes) {
    size_t i;
    for (i = 0; i < N0 - 1; i++) {
        PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_frombytes(pk->Mtr + i * NUM_DIGITS_GF2X_ELEMENT,
                pk_bytes + i * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);
    }
}

static void pack_ct(uint8_t *sk_bytes, DIGIT *ct) {
    PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_tobytes(sk_bytes, ct);
}

static void unpack_ct(DIGIT *ct, const uint8_t *ct_bytes) {
    PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_frombytes(ct, ct_bytes);
}

static void pack_error(uint8_t *error_bytes, DIGIT *error_digits) {
    size_t i;
    for (i = 0; i < N0; i++) {
        PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_tobytes(error_bytes + i * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B,
                error_digits + i * NUM_DIGITS_GF2X_ELEMENT);
    }
}
*/

/* IND-CCA2 Keygen */
int PQCLEAN_LEDAKEMLT52_LEAKTIME_crypto_kem_keypair(uint8_t *pk, uint8_t *sk) {

    PQCLEAN_LEDAKEMLT52_LEAKTIME_niederreiter_keygen((publicKeyNiederreiter_t *) pk,
            (privateKeyNiederreiter_t *) sk);

    return 0;
}

/* IND-CCA2 Encapsulation */
int PQCLEAN_LEDAKEMLT52_LEAKTIME_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk) {
    AES_XOF_struct hashedAndTruncatedSeed_expander;
    POSITION_T errorPos[NUM_ERRORS_T];
    DIGIT error_vector[N0 * NUM_DIGITS_GF2X_ELEMENT];
    uint8_t seed[TRNG_BYTE_LENGTH];
    uint8_t ss_input[2 * TRNG_BYTE_LENGTH] = {0};
    uint8_t hashedSeed[HASH_BYTE_LENGTH];
    uint8_t hashedAndTruncatedSeed[TRNG_BYTE_LENGTH] = {0};
    uint8_t hashedErrorVector[HASH_BYTE_LENGTH];
    uint8_t hashedAndTruncatedErrorVector[TRNG_BYTE_LENGTH] = {0};
    uint8_t maskedSeed[TRNG_BYTE_LENGTH];

    randombytes(seed, TRNG_BYTE_LENGTH);
    memcpy(ss_input, seed, TRNG_BYTE_LENGTH);
    HASH_FUNCTION(ss, ss_input, 2 * TRNG_BYTE_LENGTH);
    HASH_FUNCTION(hashedSeed, seed, TRNG_BYTE_LENGTH);

    memcpy(hashedAndTruncatedSeed, hashedSeed, TRNG_BYTE_LENGTH);

    memset(&hashedAndTruncatedSeed_expander, 0x00, sizeof(AES_XOF_struct));
    PQCLEAN_LEDAKEMLT52_LEAKTIME_seedexpander_from_trng(&hashedAndTruncatedSeed_expander, hashedAndTruncatedSeed);
    PQCLEAN_LEDAKEMLT52_LEAKTIME_rand_error_pos(errorPos, &hashedAndTruncatedSeed_expander);
    PQCLEAN_LEDAKEMLT52_LEAKTIME_expand_error(error_vector, errorPos);

    HASH_FUNCTION(hashedErrorVector, (const uint8_t *) error_vector, (N0 * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B));

    memcpy(hashedAndTruncatedErrorVector, hashedErrorVector, TRNG_BYTE_LENGTH);

    for (int i = 0; i < TRNG_BYTE_LENGTH; ++i) {
        maskedSeed[i] = seed[i] ^ hashedAndTruncatedErrorVector[i];
    }

    PQCLEAN_LEDAKEMLT52_LEAKTIME_niederreiter_encrypt((DIGIT *) ct, (const publicKeyNiederreiter_t *)pk, error_vector);

    memcpy(ct + (NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B), maskedSeed, TRNG_BYTE_LENGTH);
    return 0;
}


/* INDCCA2 Decapsulation  */
int PQCLEAN_LEDAKEMLT52_LEAKTIME_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk) {
    AES_XOF_struct hashedAndTruncatedSeed_expander;
    POSITION_T reconstructed_errorPos[NUM_ERRORS_T];
    DIGIT reconstructed_error_vector[N0 * NUM_DIGITS_GF2X_ELEMENT];
    DIGIT decoded_error_vector[N0 * NUM_DIGITS_GF2X_ELEMENT];
    uint8_t hashedErrorVector[HASH_BYTE_LENGTH];
    uint8_t hashedAndTruncatedErrorVector[TRNG_BYTE_LENGTH] = {0};
    uint8_t decoded_seed[TRNG_BYTE_LENGTH];
    uint8_t hashed_decoded_seed[HASH_BYTE_LENGTH];
    uint8_t hashedAndTruncated_decoded_seed[TRNG_BYTE_LENGTH] = {0};
    uint8_t ss_input[2 * TRNG_BYTE_LENGTH], tail[TRNG_BYTE_LENGTH] = {0};

    int decode_ok = PQCLEAN_LEDAKEMLT52_LEAKTIME_niederreiter_decrypt(decoded_error_vector,
                    (const privateKeyNiederreiter_t *)sk,
                    (DIGIT *)ct);

    HASH_FUNCTION(hashedErrorVector,
                  (const uint8_t *) decoded_error_vector,
                  (N0 * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B));

    memcpy(hashedAndTruncatedErrorVector, hashedErrorVector, TRNG_BYTE_LENGTH);

    for (int i = 0; i < TRNG_BYTE_LENGTH; ++i) {
        decoded_seed[i] = ct[(NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B) + i] ^
                          hashedAndTruncatedErrorVector[i];
    }

    HASH_FUNCTION(hashed_decoded_seed, decoded_seed, TRNG_BYTE_LENGTH);

    memcpy(hashedAndTruncated_decoded_seed, hashed_decoded_seed, TRNG_BYTE_LENGTH);

    memset(&hashedAndTruncatedSeed_expander, 0x00, sizeof(AES_XOF_struct));
    PQCLEAN_LEDAKEMLT52_LEAKTIME_seedexpander_from_trng(&hashedAndTruncatedSeed_expander,
            hashed_decoded_seed);

    PQCLEAN_LEDAKEMLT52_LEAKTIME_rand_error_pos(reconstructed_errorPos, &hashedAndTruncatedSeed_expander);

    PQCLEAN_LEDAKEMLT52_LEAKTIME_expand_error(reconstructed_error_vector, reconstructed_errorPos);

    int equal = (0 == memcmp((const uint8_t *) decoded_error_vector,
                             (const uint8_t *) reconstructed_error_vector,
                             N0 * NUM_DIGITS_GF2X_ELEMENT));
    // equal == 1, if the reconstructed error vector match !!!

    int decryptOk = (decode_ok == 1 && equal == 1);

    memcpy(ss_input, decoded_seed, TRNG_BYTE_LENGTH);

    if (decryptOk == 1) {
        memcpy(ss_input + sizeof(decoded_seed), tail, TRNG_BYTE_LENGTH);
    } else { // decryption failure
        memcpy(ss_input + sizeof(decoded_seed), ((const privateKeyNiederreiter_t *)sk)->decryption_failure_secret, TRNG_BYTE_LENGTH);
    }

    HASH_FUNCTION(ss, ss_input, 2 * TRNG_BYTE_LENGTH);

    return 0;
}
