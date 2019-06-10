#include "api.h"
#include "niederreiter.h"
#include "randombytes.h"
#include "rng.h"

#include <string.h>

/* Generates a keypair - pk is the public key and sk is the secret key. */
int PQCLEAN_LEDAKEMLT32_CLEAN_crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
    AES_XOF_struct niederreiter_keys_expander;

    randombytes(((privateKeyNiederreiter_t *)sk)->prng_seed, TRNG_BYTE_LENGTH);
    PQCLEAN_LEDAKEMLT32_CLEAN_seedexpander_from_trng(&niederreiter_keys_expander, ((privateKeyNiederreiter_t *)sk)->prng_seed);
    PQCLEAN_LEDAKEMLT32_CLEAN_niederreiter_keygen((publicKeyNiederreiter_t *) pk, (privateKeyNiederreiter_t *) sk, &niederreiter_keys_expander);

    return 0;
}

static void pack_error(uint8_t *error_bytes, const DIGIT *error_digits) {
    size_t i;
    for (i = 0; i < N0; i++) {
        PQCLEAN_LEDAKEMLT32_CLEAN_gf2x_tobytes(error_bytes + i * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B, error_digits + i * NUM_DIGITS_GF2X_ELEMENT);
    }
}

/* Encrypt - pk is the public key, ct is a key encapsulation message
  (ciphertext), ss is the shared secret.*/
int PQCLEAN_LEDAKEMLT32_CLEAN_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk) {
    AES_XOF_struct niederreiter_encap_key_expander;
    unsigned char encapsulated_key_seed[TRNG_BYTE_LENGTH];
    DIGIT error_vector[N0 * NUM_DIGITS_GF2X_ELEMENT];
    uint8_t error_bytes[N0 * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B];

    randombytes(encapsulated_key_seed, TRNG_BYTE_LENGTH);
    PQCLEAN_LEDAKEMLT32_CLEAN_seedexpander_from_trng(&niederreiter_encap_key_expander, encapsulated_key_seed);
    PQCLEAN_LEDAKEMLT32_CLEAN_rand_circulant_blocks_sequence(error_vector, &niederreiter_encap_key_expander);
    pack_error(error_bytes, error_vector);
    HASH_FUNCTION(ss, error_bytes, (N0 * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B));
    PQCLEAN_LEDAKEMLT32_CLEAN_niederreiter_encrypt((DIGIT *) ct, (publicKeyNiederreiter_t *) pk, error_vector);

    return 0;
}


/* Decrypt - ct is a key encapsulation message (ciphertext), sk is the private
   key, ss is the shared secret */
int PQCLEAN_LEDAKEMLT32_CLEAN_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk) {
    DIGIT decoded_error_vector[N0 * NUM_DIGITS_GF2X_ELEMENT];
    uint8_t decoded_error_bytes[N0 * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B];

    PQCLEAN_LEDAKEMLT32_CLEAN_niederreiter_decrypt(decoded_error_vector, (privateKeyNiederreiter_t *)sk, (DIGIT *)ct);
    pack_error(decoded_error_bytes, decoded_error_vector);
    HASH_FUNCTION(ss, decoded_error_bytes, (N0 * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B));

    return 0;
}
