#ifndef PQCLEAN_MCELIECE460896F_CLEAN_API_H
#define PQCLEAN_MCELIECE460896F_CLEAN_API_H

#define PQCLEAN_MCELIECE460896F_CLEAN_CRYPTO_ALGNAME "Classic McEliece 460896f"
#define PQCLEAN_MCELIECE460896F_CLEAN_CRYPTO_PUBLICKEYBYTES 524160
#define PQCLEAN_MCELIECE460896F_CLEAN_CRYPTO_SECRETKEYBYTES 13568
#define PQCLEAN_MCELIECE460896F_CLEAN_CRYPTO_CIPHERTEXTBYTES 188
#define PQCLEAN_MCELIECE460896F_CLEAN_CRYPTO_BYTES 32

int PQCLEAN_MCELIECE460896F_CLEAN_crypto_kem_enc(
    unsigned char *c,
    unsigned char *key,
    const unsigned char *pk
);

int PQCLEAN_MCELIECE460896F_CLEAN_crypto_kem_dec(
    unsigned char *key,
    const unsigned char *c,
    const unsigned char *sk
);

int PQCLEAN_MCELIECE460896F_CLEAN_crypto_kem_keypair
(
    unsigned char *pk,
    unsigned char *sk
);

#endif

