#ifndef PQCLEAN_MCELIECE8192128_SSE_API_H
#define PQCLEAN_MCELIECE8192128_SSE_API_H

#define PQCLEAN_MCELIECE8192128_SSE_CRYPTO_ALGNAME "Classic McEliece 8192128"
#define PQCLEAN_MCELIECE8192128_SSE_CRYPTO_PUBLICKEYBYTES 1357824
#define PQCLEAN_MCELIECE8192128_SSE_CRYPTO_SECRETKEYBYTES 14080
#define PQCLEAN_MCELIECE8192128_SSE_CRYPTO_CIPHERTEXTBYTES 240
#define PQCLEAN_MCELIECE8192128_SSE_CRYPTO_BYTES 32


int PQCLEAN_MCELIECE8192128_SSE_crypto_kem_enc(
    unsigned char *c,
    unsigned char *key,
    const unsigned char *pk
);

int PQCLEAN_MCELIECE8192128_SSE_crypto_kem_dec(
    unsigned char *key,
    const unsigned char *c,
    const unsigned char *sk
);

int PQCLEAN_MCELIECE8192128_SSE_crypto_kem_keypair
(
    unsigned char *pk,
    unsigned char *sk
);

#endif
