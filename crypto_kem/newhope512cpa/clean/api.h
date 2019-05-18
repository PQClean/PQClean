#ifndef PQCLEAN_NEWHOPE512CPA_CLEAN_API_H
#define PQCLEAN_NEWHOPE512CPA_CLEAN_API_H


#define PQCLEAN_NEWHOPE512CPA_CLEAN_CRYPTO_SECRETKEYBYTES  896
#define PQCLEAN_NEWHOPE512CPA_CLEAN_CRYPTO_PUBLICKEYBYTES  928
#define PQCLEAN_NEWHOPE512CPA_CLEAN_CRYPTO_CIPHERTEXTBYTES 1088
#define PQCLEAN_NEWHOPE512CPA_CLEAN_CRYPTO_BYTES           32
#define PQCLEAN_NEWHOPE512CPA_CLEAN_CRYPTO_ALGNAME "NewHope512-CPAKEM"

int PQCLEAN_NEWHOPE512CPA_CLEAN_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
int PQCLEAN_NEWHOPE512CPA_CLEAN_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);
int PQCLEAN_NEWHOPE512CPA_CLEAN_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

#endif
