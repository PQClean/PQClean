#ifndef PQCLEAN_LEDAKEMLT52_CLEAN_API_H
#define PQCLEAN_LEDAKEMLT52_CLEAN_API_H

#define PQCLEAN_LEDAKEMLT52_CLEAN_CRYPTO_SECRETKEYBYTES  41
#define PQCLEAN_LEDAKEMLT52_CLEAN_CRYPTO_PUBLICKEYBYTES  19040
#define PQCLEAN_LEDAKEMLT52_CLEAN_CRYPTO_CIPHERTEXTBYTES 19040
#define PQCLEAN_LEDAKEMLT52_CLEAN_CRYPTO_BYTES           64

#define PQCLEAN_LEDAKEMLT52_CLEAN_CRYPTO_ALGNAME "LEDAKEMLT52"

int PQCLEAN_LEDAKEMLT52_CLEAN_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);

int PQCLEAN_LEDAKEMLT52_CLEAN_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);

int PQCLEAN_LEDAKEMLT52_CLEAN_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);


#endif
