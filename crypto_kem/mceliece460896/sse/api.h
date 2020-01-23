#ifndef PQCLEAN_MCELIECE460896_SSE_API_H
#define PQCLEAN_MCELIECE460896_SSE_API_H

#define PQCLEAN_MCELIECE460896_SSE_CRYPTO_ALGNAME "Classic McEliece 460896"
#define PQCLEAN_MCELIECE460896_SSE_CRYPTO_PUBLICKEYBYTES 524160
#define PQCLEAN_MCELIECE460896_SSE_CRYPTO_SECRETKEYBYTES 13568
#define PQCLEAN_MCELIECE460896_SSE_CRYPTO_CIPHERTEXTBYTES 188
#define PQCLEAN_MCELIECE460896_SSE_CRYPTO_BYTES 32

int PQCLEAN_MCELIECE460896_SSE_crypto_kem_enc(
    unsigned char *c,
    unsigned char *key,
    const unsigned char *pk
);

int PQCLEAN_MCELIECE460896_SSE_crypto_kem_dec(
    unsigned char *key,
    const unsigned char *c,
    const unsigned char *sk
);

int PQCLEAN_MCELIECE460896_SSE_crypto_kem_keypair
(
    unsigned char *pk,
    unsigned char *sk
);

#endif
