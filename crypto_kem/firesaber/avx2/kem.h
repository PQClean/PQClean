#ifndef INDCPA_H
#define INDCPA_H

#include <stdint.h>

void PQCLEAN_FIRESABER_AVX2_indcpa_keypair(uint8_t *pk, uint8_t *sk);


void PQCLEAN_FIRESABER_AVX2_indcpa_client(uint8_t *pk, uint8_t *b_prime, uint8_t *c, uint8_t *key);


void PQCLEAN_FIRESABER_AVX2_indcpa_server(uint8_t *pk, uint8_t *b_prime, uint8_t *c, uint8_t *key);


void PQCLEAN_FIRESABER_AVX2_indcpa_kem_keypair(uint8_t *pk, uint8_t *sk);

void PQCLEAN_FIRESABER_AVX2_indcpa_kem_enc(uint8_t *message, uint8_t *noiseseed, uint8_t *pk,  uint8_t *ciphertext);

void PQCLEAN_FIRESABER_AVX2_indcpa_kem_dec(uint8_t *sk, uint8_t *ciphertext, uint8_t message_dec[]);


int PQCLEAN_FIRESABER_AVX2_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);

int PQCLEAN_FIRESABER_AVX2_crypto_kem_enc(unsigned char *c, unsigned char *k, const unsigned char *pk);

int PQCLEAN_FIRESABER_AVX2_crypto_kem_dec(unsigned char *k, const unsigned char *c, const unsigned char *sk);



//uint64_t clock1,clock2;

//uint64_t clock_kp_kex, clock_enc_kex, clock_dec_kex;


#endif
