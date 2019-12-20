#ifndef PQCLEAN_MCELIECE6688128F_VEC_API_H
#define PQCLEAN_MCELIECE6688128F_VEC_API_H

#define PQCLEAN_MCELIECE6688128F_VEC_CRYPTO_ALGNAME "Classic McEliece 6688128"
#define PQCLEAN_MCELIECE6688128F_VEC_CRYPTO_PUBLICKEYBYTES 1044992
#define PQCLEAN_MCELIECE6688128F_VEC_CRYPTO_SECRETKEYBYTES 13892
#define PQCLEAN_MCELIECE6688128F_VEC_CRYPTO_CIPHERTEXTBYTES 240
#define PQCLEAN_MCELIECE6688128F_VEC_CRYPTO_BYTES 32


int PQCLEAN_MCELIECE6688128F_VEC_crypto_kem_enc(
    unsigned char *c,
    unsigned char *key,
    const unsigned char *pk
);

int PQCLEAN_MCELIECE6688128F_VEC_crypto_kem_dec(
    unsigned char *key,
    const unsigned char *c,
    const unsigned char *sk
);

int PQCLEAN_MCELIECE6688128F_VEC_crypto_kem_keypair
(
    unsigned char *pk,
    unsigned char *sk
);

#endif

