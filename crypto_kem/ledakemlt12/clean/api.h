#ifndef PQCLEAN_LEDAKEMLT12_CLEAN_API_H
#define PQCLEAN_LEDAKEMLT12_CLEAN_API_H

#define PQCLEAN_LEDAKEMLT12_CLEAN_CRYPTO_SECRETKEYBYTES  25
#define PQCLEAN_LEDAKEMLT12_CLEAN_CRYPTO_PUBLICKEYBYTES  6520
#define PQCLEAN_LEDAKEMLT12_CLEAN_CRYPTO_CIPHERTEXTBYTES 6520
#define PQCLEAN_LEDAKEMLT12_CLEAN_CRYPTO_BYTES           32

#define PQCLEAN_LEDAKEMLT12_CLEAN_CRYPTO_ALGNAME "LEDAKEMLT12"

int PQCLEAN_LEDAKEMLT12_CLEAN_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);

int PQCLEAN_LEDAKEMLT12_CLEAN_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);

int PQCLEAN_LEDAKEMLT12_CLEAN_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);


#endif
