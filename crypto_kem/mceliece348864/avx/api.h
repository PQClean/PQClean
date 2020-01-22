#ifndef PQCLEAN_MCELIECE348864_AVX_API_H
#define PQCLEAN_MCELIECE348864_AVX_API_H

#define PQCLEAN_MCELIECE348864_AVX_CRYPTO_ALGNAME "Classic McEliece 348864"
#define PQCLEAN_MCELIECE348864_AVX_CRYPTO_PUBLICKEYBYTES 261120
#define PQCLEAN_MCELIECE348864_AVX_CRYPTO_SECRETKEYBYTES 6452
#define PQCLEAN_MCELIECE348864_AVX_CRYPTO_CIPHERTEXTBYTES 128
#define PQCLEAN_MCELIECE348864_AVX_CRYPTO_BYTES 32


int PQCLEAN_MCELIECE348864_AVX_crypto_kem_enc(
    unsigned char *c,
    unsigned char *key,
    const unsigned char *pk
);

int PQCLEAN_MCELIECE348864_AVX_crypto_kem_dec(
    unsigned char *key,
    const unsigned char *c,
    const unsigned char *sk
);

int PQCLEAN_MCELIECE348864_AVX_crypto_kem_keypair
(
    unsigned char *pk,
    unsigned char *sk
);

#endif

