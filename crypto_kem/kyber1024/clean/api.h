#ifndef PQCLEAN_KYBER1024_CLEAN_API_H
#define PQCLEAN_KYBER1024_CLEAN_API_H

#include "params.h"

#define PQCLEAN_KYBER1024_CLEAN_CRYPTO_SECRETKEYBYTES  KYBER_SECRETKEYBYTES
#define PQCLEAN_KYBER1024_CLEAN_CRYPTO_PUBLICKEYBYTES  KYBER_PUBLICKEYBYTES
#define PQCLEAN_KYBER1024_CLEAN_CRYPTO_CIPHERTEXTBYTES KYBER_CIPHERTEXTBYTES
#define PQCLEAN_KYBER1024_CLEAN_CRYPTO_BYTES           KYBER_SSBYTES

#if   (KYBER_K == 2)
#define PQCLEAN_KYBER1024_CLEAN_CRYPTO_ALGNAME "Kyber512"
#elif (KYBER_K == 3)
#define PQCLEAN_KYBER1024_CLEAN_CRYPTO_ALGNAME "Kyber768"
#elif (KYBER_K == 4)
#define PQCLEAN_KYBER1024_CLEAN_CRYPTO_ALGNAME "Kyber1024"
#else
#error "KYBER_K must be in {2,3,4}"
#endif

int PQCLEAN_KYBER1024_CLEAN_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);

int PQCLEAN_KYBER1024_CLEAN_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);

int PQCLEAN_KYBER1024_CLEAN_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);


#endif
