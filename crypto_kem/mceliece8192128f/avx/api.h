#ifndef PQCLEAN_MCELIECE8192128F_AVX_API_H
#define PQCLEAN_MCELIECE8192128F_AVX_API_H

#define PQCLEAN_MCELIECE8192128F_AVX_CRYPTO_ALGNAME "Classic McEliece 8192128f"
#define PQCLEAN_MCELIECE8192128F_AVX_CRYPTO_PUBLICKEYBYTES 1357824
#define PQCLEAN_MCELIECE8192128F_AVX_CRYPTO_SECRETKEYBYTES 14080
#define PQCLEAN_MCELIECE8192128F_AVX_CRYPTO_CIPHERTEXTBYTES 240
#define PQCLEAN_MCELIECE8192128F_AVX_CRYPTO_BYTES 32


int PQCLEAN_MCELIECE8192128F_AVX_crypto_kem_enc(
    unsigned char *c,
    unsigned char *key,
    const unsigned char *pk
);

int PQCLEAN_MCELIECE8192128F_AVX_crypto_kem_dec(
    unsigned char *key,
    const unsigned char *c,
    const unsigned char *sk
);

int PQCLEAN_MCELIECE8192128F_AVX_crypto_kem_keypair
(
    unsigned char *pk,
    unsigned char *sk
);

#endif

