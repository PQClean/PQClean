#include "api.h"
#include "niederreiter_keygen.h"
#include "niederreiter_encrypt.h"
#include "niederreiter_decrypt.h"
#include "rng.h"
#include "fips202.h"
#include <string.h>
/* Generates a keypair - pk is the public key and sk is the secret key. */
int PQCLEAN_LEDACRYPT23371_CLEAN_crypto_kem_keypair(uint8_t *pk,
                                                   uint8_t *sk )
{

    PQCLEAN_LEDACRYPT23371_CLEAN_key_gen_niederreiter((publicKeyNiederreiter_t *const) pk,
                                                      (privateKeyNiederreiter_t *const) sk);

   return 0; // NIST convention: 0 == zero errors
} // end PQCLEAN_LEDACRYPTXXXX_CLEAN_crypto_kem_keypair

/*----------------------------------------------------------------------------*/
/* Encrypt - pk is the public key,
 *           ct is a key encapsulation message (ciphertext),
 *           ss is the shared secret.
 */

int PQCLEAN_LEDACRYPT23371_CLEAN_crypto_kem_enc(uint8_t *ct,
                                               uint8_t *ss,
                                               const uint8_t *pk )
{

    PQCLEAN_LEDACRYPT23371_CLEAN_encrypt_niederreiter_indcca2((unsigned char *const)
                                                                      ct, /* ciphertext - output   */
                                                              (unsigned char *const) ss,  /* shared secret - output*/
                                                              (const publicKeyNiederreiter_t *const) pk);

   return 0; // NIST convention: 0 == zero errors
} //end PQCLEAN_LEDACRYPT23371_CLEAN_crypto_kem_enc

/*----------------------------------------------------------------------------*/
/* Decrypt - ct is a key encapsulation message (ciphertext),
 *           sk is the private key,
 *           ss is the shared secret
 */

int PQCLEAN_LEDACRYPT23371_CLEAN_crypto_kem_dec(uint8_t *ss,
                                               const uint8_t *ct,
                                               const uint8_t *sk)
{

    PQCLEAN_LEDACRYPT23371_CLEAN_decrypt_niederreiter_indcca2((unsigned char *const) ss,
                                                              (const unsigned char *const) ct,
                                                              (const privateKeyNiederreiter_t *const) sk);

   return 0; // NIST convention: 0 == zero errors
} // end PQCLEAN_LEDACRYPTXXXX_CLEAN_crypto_kem_dec

/*----------------------------------------------------------------------------*/
