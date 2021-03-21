#ifndef PQCLEAN_LEDACRYPT23371_CLEAN_AES256_H
#define PQCLEAN_LEDACRYPT23371_CLEAN_AES256_H

#define NROUNDS 14
#define KEYLEN_b 256

#include <stdint.h>

int PQCLEAN_LEDACRYPT23371_CLEAN_rijndaelKeySetupEnc(uint32_t *rk, const uint8_t *cipherKey);
void PQCLEAN_LEDACRYPT23371_CLEAN_rijndaelEncrypt(const uint32_t *rk, int Nr,
                                                  const uint8_t *pt, uint8_t *ct);

#endif