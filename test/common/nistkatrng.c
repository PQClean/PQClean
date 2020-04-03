//
//  rng.c
//
//  Created by Bassham, Lawrence E (Fed) on 8/29/17.
//  Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
//  Modified for liboqs by Douglas Stebila
//

#include <assert.h>
#include <string.h>

#include "aes.h"
#include "randombytes.h"

typedef struct {
    uint8_t Key[32];
    uint8_t V[16];
    int reseed_counter;
} AES256_CTR_DRBG_struct;

static AES256_CTR_DRBG_struct DRBG_ctx;
static void AES256_CTR_DRBG_Update(const uint8_t *provided_data, uint8_t *Key, uint8_t *V);

// Use whatever AES implementation you have. This uses AES from openSSL library
//    key - 256-bit AES key
//    ctr - a 128-bit plaintext value
//    buffer - a 128-bit ciphertext value
static void AES256_ECB(uint8_t *key, uint8_t *ctr, uint8_t *buffer) {
    aes256ctx ctx;
    aes256_ecb_keyexp(&ctx, key);
    aes256_ecb(buffer, ctr, 1, &ctx);
    aes256_ctx_release(&ctx);
}

void nist_kat_init(uint8_t *entropy_input, const uint8_t *personalization_string, int security_strength);
void nist_kat_init(uint8_t *entropy_input, const uint8_t *personalization_string, int security_strength) {
    uint8_t seed_material[48];

    assert(security_strength == 256);
    memcpy(seed_material, entropy_input, 48);
    if (personalization_string) {
        for (int i = 0; i < 48; i++) {
            seed_material[i] ^= personalization_string[i];
        }
    }
    memset(DRBG_ctx.Key, 0x00, 32);
    memset(DRBG_ctx.V, 0x00, 16);
    AES256_CTR_DRBG_Update(seed_material, DRBG_ctx.Key, DRBG_ctx.V);
    DRBG_ctx.reseed_counter = 1;
}

int randombytes(uint8_t *buf, size_t n) {
    uint8_t block[16];
    int i = 0;

    while (n > 0) {
        //increment V
        for (int j = 15; j >= 0; j--) {
            if (DRBG_ctx.V[j] == 0xff) {
                DRBG_ctx.V[j] = 0x00;
            } else {
                DRBG_ctx.V[j]++;
                break;
            }
        }
        AES256_ECB(DRBG_ctx.Key, DRBG_ctx.V, block);
        if (n > 15) {
            memcpy(buf + i, block, 16);
            i += 16;
            n -= 16;
        } else {
            memcpy(buf + i, block, n);
            n = 0;
        }
    }
    AES256_CTR_DRBG_Update(NULL, DRBG_ctx.Key, DRBG_ctx.V);
    DRBG_ctx.reseed_counter++;
    return 0;
}

static void AES256_CTR_DRBG_Update(const uint8_t *provided_data, uint8_t *Key, uint8_t *V) {
    uint8_t temp[48];

    for (int i = 0; i < 3; i++) {
        //increment V
        for (int j = 15; j >= 0; j--) {
            if (V[j] == 0xff) {
                V[j] = 0x00;
            } else {
                V[j]++;
                break;
            }
        }

        AES256_ECB(Key, V, temp + 16 * i);
    }
    if (provided_data != NULL) {
        for (int i = 0; i < 48; i++) {
            temp[i] ^= provided_data[i];
        }
    }
    memcpy(Key, temp, 32);
    memcpy(V, temp + 32, 16);
}
