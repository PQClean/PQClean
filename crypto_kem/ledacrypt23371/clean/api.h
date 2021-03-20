#ifndef PQCLEAN_LEDACRYPT23371_CLEAN_API_H
#define PQCLEAN_LEDACRYPT23371_CLEAN_API_H

#include <stddef.h>
#include <stdint.h>


#define PQCLEAN_LEDACRYPT23371_CLEAN_CRYPTO_ALGNAME "LEDA"

/* required bytes of input randomness */
#define  PQCLEAN_LEDACRYPT23371_CLEAN_CRYPTO_RANDOMBYTES 24      //TRNG_BYTE_LENGTH

/* size in bytes of the secret key */
#define  PQCLEAN_LEDACRYPT23371_CLEAN_CRYPTO_SECRETKEYBYTES 50     //(TRNG_BYTE_LENGTH*2)+2

/* size in bytes of the public key */
#define  PQCLEAN_LEDACRYPT23371_CLEAN_CRYPTO_PUBLICKEYBYTES 2928    // DIGIT Mtr[(N0-1)*[(P+DIGIT_b-1)/DIGIT_b]]

/* size in bytes of the shared secret */
#define  PQCLEAN_LEDACRYPT23371_CLEAN_CRYPTO_BYTES 32        //HASH_BYTE_LENGTH

/*size in bytes of the ciphertext*/
#define  PQCLEAN_LEDACRYPT23371_CLEAN_CRYPTO_CIPHERTEXTBYTES 2952     // [(P+DIGIT_b-1)/DIGIT_b]]*DIGIT_SIZE_B+PQCLEAN_LEDACRYPTXXXX_CLEAN_CRYPTO_RANDOMBYTES)


/* Generates a keypair - pk is the public key and sk is the secret key. */
int PQCLEAN_LEDACRYPT23371_CLEAN_crypto_kem_keypair(uint8_t *pk,
                                                   uint8_t *sk );

/* Encrypt - pk is the public key, ct is a key encapsulation message
  (ciphertext), ss is the shared secret.*/
int PQCLEAN_LEDACRYPT23371_CLEAN_crypto_kem_enc(uint8_t *ct,
                                               uint8_t *ss,
                                               const uint8_t *pk );


/* Decrypt - ct is a key encapsulation message (ciphertext), sk is the private
   key, ss is the shared secret */

int PQCLEAN_LEDACRYPT23371_CLEAN_crypto_kem_dec(uint8_t *ss,
                                               const uint8_t *ct,
                                               const uint8_t *sk );

#endif