#ifndef AES256_H
#define AES256_H

#define NROUNDS 14
#define KEYLEN_b 256

#include <stdint.h>

int rijndaelKeySetupEnc(uint32_t rk[], const uint8_t cipherKey[]);
void rijndaelEncrypt(const uint32_t rk[/*4*(Nr + 1)*/], int Nr,
                     const uint8_t pt[16], uint8_t ct[16]);

#endif