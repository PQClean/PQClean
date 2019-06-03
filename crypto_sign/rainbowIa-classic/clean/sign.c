///  @file  sign.c
///  @brief the implementations for functions in api.h
///
///

#include "api.h"
#include "rainbow.h"
#include "rainbow_config.h"
#include "rainbow_keypair.h"
#include "randombytes.h"
#include "utils_hash.h"
#include <stdlib.h>
#include <string.h>




int
PQCLEAN_RAINBOWIACLASSIC_CLEAN_crypto_sign_keypair(unsigned char *pk, unsigned char *sk) {
    unsigned char sk_seed[LEN_SKSEED] = {0};
    randombytes( sk_seed, LEN_SKSEED );

//    #if defined _RAINBOW_CLASSIC

    PQCLEAN_RAINBOWIACLASSIC_CLEAN_generate_keypair( (pk_t *) pk, (sk_t *) sk, sk_seed );

//   #elif defined _RAINBOW_CYCLIC

//   unsigned char pk_seed[LEN_PKSEED] = {0};
//   randombytes( pk_seed, LEN_PKSEED );
//   generate_keypair_cyclic( (cpk_t *) pk, (sk_t *) sk, pk_seed, sk_seed );

//   #elif defined _RAINBOW_CYCLIC_COMPRESSED

//   unsigned char pk_seed[LEN_PKSEED] = {0};
//   randombytes( pk_seed, LEN_PKSEED );
//   generate_compact_keypair_cyclic( (cpk_t *) pk, (csk_t *) sk, pk_seed, sk_seed );

//   #else
//   error here
//   #endif
    return 0;
}





int
PQCLEAN_RAINBOWIACLASSIC_CLEAN_crypto_sign(unsigned char *sm, size_t *smlen, const unsigned char *m, size_t mlen, const unsigned char *sk) {
    unsigned char digest[_HASH_LEN];

    PQCLEAN_RAINBOWIACLASSIC_CLEAN_hash_msg( digest, _HASH_LEN, m, mlen );

    memcpy( sm, m, mlen );
    smlen[0] = mlen + _SIGNATURE_BYTE;

//    #if defined _RAINBOW_CLASSIC

    return PQCLEAN_RAINBOWIACLASSIC_CLEAN_rainbow_sign( sm + mlen, (const sk_t *)sk, digest );

//   #elif defined _RAINBOW_CYCLIC

//   return rainbow_sign( sm + mlen, (const sk_t *)sk, digest );

//   #elif defined _RAINBOW_CYCLIC_COMPRESSED

//   return rainbow_sign_cyclic( sm + mlen, (const csk_t *)sk, digest );

//   #else
//   error here
//   #endif


}






int
PQCLEAN_RAINBOWIACLASSIC_CLEAN_crypto_sign_open(unsigned char *m, size_t *mlen, const unsigned char *sm, size_t smlen, const unsigned char *pk) {
//TODO: this should not copy out the message if verification fails
    if ( _SIGNATURE_BYTE > smlen ) {
        return -1;
    }
    memcpy( m, sm, smlen - _SIGNATURE_BYTE );
    mlen[0] = smlen - _SIGNATURE_BYTE;

    unsigned char digest[_HASH_LEN];
    PQCLEAN_RAINBOWIACLASSIC_CLEAN_hash_msg( digest, _HASH_LEN, m, *mlen );

//    #if defined _RAINBOW_CLASSIC

    return PQCLEAN_RAINBOWIACLASSIC_CLEAN_rainbow_verify( digest, sm + mlen[0], (const pk_t *)pk );

//   #elif defined _RAINBOW_CYCLIC

//   return PQCLEAN_RAINBOWIACLASSIC_CLEAN_rainbow_verify_cyclic( digest, sm + mlen[0], (const cpk_t *)pk );

//   #elif defined _RAINBOW_CYCLIC_COMPRESSED

//   return PQCLEAN_RAINBOWIACLASSIC_CLEAN_rainbow_verify_cyclic( digest, sm + mlen[0], (const cpk_t *)pk );

//   #else
//   error here
//   #endif


}

int PQCLEAN_RAINBOWIACLASSIC_CLEAN_crypto_sign_signature(
    uint8_t *sig, size_t *siglen,
    const uint8_t *m, size_t mlen, const uint8_t *sk) {
    unsigned char digest[_HASH_LEN];

    PQCLEAN_RAINBOWIACLASSIC_CLEAN_hash_msg( digest, _HASH_LEN, m, mlen );
    *siglen = _SIGNATURE_BYTE;
    // #if defined _RAINBOW_CLASSIC
    return PQCLEAN_RAINBOWIACLASSIC_CLEAN_rainbow_sign( sig, (const sk_t *)sk, digest );
    //  #elif defined _RAINBOW_CYCLIC
    //  return PQCLEAN_RAINBOWIACLASSIC_CLEAN_rainbow_sign( sig, (const sk_t *)sk, digest );
    //  #elif defined _RAINBOW_CYCLIC_COMPRESSED
    //  return PQCLEAN_RAINBOWIACLASSIC_CLEAN_rainbow_sign_cyclic( sig, (const csk_t *)sk, digest );
    //  #else
    //  error here
    //  #endif

}

int PQCLEAN_RAINBOWIACLASSIC_CLEAN_crypto_sign_verify(
    const uint8_t *sig, size_t siglen,
    const uint8_t *m, size_t mlen, const uint8_t *pk) {
    if (siglen != _SIGNATURE_BYTE) {
        return -1;
    }
    unsigned char digest[_HASH_LEN];
    PQCLEAN_RAINBOWIACLASSIC_CLEAN_hash_msg( digest, _HASH_LEN, m, mlen );
    // #if defined _RAINBOW_CLASSIC
    return PQCLEAN_RAINBOWIACLASSIC_CLEAN_rainbow_verify( digest, sig, (const pk_t *)pk );
    //  #elif defined _RAINBOW_CYCLIC
    //  return PQCLEAN_RAINBOWIACLASSIC_CLEAN_rainbow_verify_cyclic( digest, sig, (const cpk_t *)pk );
    //  #elif defined _RAINBOW_CYCLIC_COMPRESSED
    //  return PQCLEAN_RAINBOWIACLASSIC_CLEAN_rainbow_verify_cyclic( digest, sig, (const cpk_t *)pk );
    //  #else
    //  error here
    //  #endif

}
