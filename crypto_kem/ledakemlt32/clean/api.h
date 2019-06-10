#ifndef PQCLEAN_LEDAKEMLT32_CLEAN_API_H
#define PQCLEAN_LEDAKEMLT32_CLEAN_API_H

#define PQCLEAN_LEDAKEMLT32_CLEAN_CRYPTO_SECRETKEYBYTES  33
#define PQCLEAN_LEDAKEMLT32_CLEAN_CRYPTO_PUBLICKEYBYTES  12032
#define PQCLEAN_LEDAKEMLT32_CLEAN_CRYPTO_CIPHERTEXTBYTES 12032
#define PQCLEAN_LEDAKEMLT32_CLEAN_CRYPTO_BYTES           48

#define PQCLEAN_LEDAKEMLT32_CLEAN_CRYPTO_ALGNAME "LEDAKEMLT32"

int PQCLEAN_LEDAKEMLT32_CLEAN_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);

int PQCLEAN_LEDAKEMLT32_CLEAN_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);

int PQCLEAN_LEDAKEMLT32_CLEAN_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);


#endif
