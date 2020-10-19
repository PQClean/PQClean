#include "SABER_indcpa.h"
#include "SABER_params.h"
#include "fips202.h"
#include "pack_unpack.h"
#include "poly.h"
#include "randombytes.h"
#include <stdint.h>
#include <string.h>

#define h1 (1 << (SABER_EQ - SABER_EP - 1))
#define h2 ((1 << (SABER_EP - 2)) - (1 << (SABER_EP - SABER_ET - 1)) + (1 << (SABER_EQ - SABER_EP - 1)))

void PQCLEAN_SABER_CLEAN_indcpa_kem_keypair(uint8_t pk[SABER_INDCPA_PUBLICKEYBYTES], uint8_t sk[SABER_INDCPA_SECRETKEYBYTES]) {
    uint16_t A[SABER_L][SABER_L][SABER_N];
    uint16_t s[SABER_L][SABER_N];
    uint16_t b[SABER_L][SABER_N] = {{0}};

    uint8_t seed_A[SABER_SEEDBYTES];
    uint8_t seed_s[SABER_NOISE_SEEDBYTES];
    size_t i, j;

    randombytes(seed_A, SABER_SEEDBYTES);
    shake128(seed_A, SABER_SEEDBYTES, seed_A, SABER_SEEDBYTES); // for not revealing system RNG state
    randombytes(seed_s, SABER_NOISE_SEEDBYTES);

    PQCLEAN_SABER_CLEAN_GenMatrix(A, seed_A);
    PQCLEAN_SABER_CLEAN_GenSecret(s, seed_s);
    PQCLEAN_SABER_CLEAN_MatrixVectorMul(b, (const uint16_t (*)[SABER_L][SABER_N])A, (const uint16_t (*)[SABER_N])s, 1);

    for (i = 0; i < SABER_L; i++) {
        for (j = 0; j < SABER_N; j++) {
            b[i][j] = (b[i][j] + h1) >> (SABER_EQ - SABER_EP);
        }
    }

    PQCLEAN_SABER_CLEAN_POLVECq2BS(sk, (const uint16_t (*)[SABER_N])s);
    PQCLEAN_SABER_CLEAN_POLVECp2BS(pk, (const uint16_t (*)[SABER_N])b);
    memcpy(pk + SABER_POLYVECCOMPRESSEDBYTES, seed_A, sizeof(seed_A));
}

void PQCLEAN_SABER_CLEAN_indcpa_kem_enc(uint8_t ciphertext[SABER_BYTES_CCA_DEC], const uint8_t m[SABER_KEYBYTES], const uint8_t seed_sp[SABER_NOISE_SEEDBYTES], const uint8_t pk[SABER_INDCPA_PUBLICKEYBYTES]) {
    uint16_t A[SABER_L][SABER_L][SABER_N];
    uint16_t sp[SABER_L][SABER_N];
    uint16_t bp[SABER_L][SABER_N] = {{0}};
    uint16_t vp[SABER_N] = {0};
    uint16_t mp[SABER_N];
    uint16_t b[SABER_L][SABER_N];
    size_t i, j;
    const uint8_t *seed_A = pk + SABER_POLYVECCOMPRESSEDBYTES;

    PQCLEAN_SABER_CLEAN_GenMatrix(A, seed_A);
    PQCLEAN_SABER_CLEAN_GenSecret(sp, seed_sp);
    PQCLEAN_SABER_CLEAN_MatrixVectorMul(bp, (const uint16_t (*)[SABER_L][SABER_N])A, (const uint16_t (*)[SABER_N])sp, 0);

    for (i = 0; i < SABER_L; i++) {
        for (j = 0; j < SABER_N; j++) {
            bp[i][j] = (bp[i][j] + h1) >> (SABER_EQ - SABER_EP);
        }
    }

    PQCLEAN_SABER_CLEAN_POLVECp2BS(ciphertext, (const uint16_t (*)[SABER_N])bp);
    PQCLEAN_SABER_CLEAN_BS2POLVECp(b, pk);
    PQCLEAN_SABER_CLEAN_InnerProd(vp, (const uint16_t (*)[SABER_N])b, (const uint16_t (*)[SABER_N])sp);

    PQCLEAN_SABER_CLEAN_BS2POLmsg(mp, m);

    for (j = 0; j < SABER_N; j++) {
        vp[j] = (vp[j] - (mp[j] << (SABER_EP - 1)) + h1) >> (SABER_EP - SABER_ET);
    }

    PQCLEAN_SABER_CLEAN_POLT2BS(ciphertext + SABER_POLYVECCOMPRESSEDBYTES, vp);
}

void PQCLEAN_SABER_CLEAN_indcpa_kem_dec(uint8_t m[SABER_KEYBYTES], const uint8_t sk[SABER_INDCPA_SECRETKEYBYTES], const uint8_t ciphertext[SABER_BYTES_CCA_DEC]) {

    uint16_t s[SABER_L][SABER_N];
    uint16_t b[SABER_L][SABER_N];
    uint16_t v[SABER_N] = {0};
    uint16_t cm[SABER_N];
    size_t i;

    PQCLEAN_SABER_CLEAN_BS2POLVECq(s, sk);
    PQCLEAN_SABER_CLEAN_BS2POLVECp(b, ciphertext);
    PQCLEAN_SABER_CLEAN_InnerProd(v, (const uint16_t (*)[SABER_N])b, (const uint16_t (*)[SABER_N])s);
    PQCLEAN_SABER_CLEAN_BS2POLT(cm, ciphertext + SABER_POLYVECCOMPRESSEDBYTES);

    for (i = 0; i < SABER_N; i++) {
        v[i] = (v[i] + h2 - (cm[i] << (SABER_EP - SABER_ET))) >> (SABER_EP - 1);
    }

    PQCLEAN_SABER_CLEAN_POLmsg2BS(m, v);
}
