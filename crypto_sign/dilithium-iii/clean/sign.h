#ifndef SIGN_H
#define SIGN_H
#include <stddef.h>
#include <stdint.h>

#include "params.h"
#include "poly.h"
#include "polyvec.h"

void PQCLEAN_DILITHIUMIII_CLEAN_expand_mat(polyvecl mat[K],
        const uint8_t rho[SEEDBYTES]);
void PQCLEAN_DILITHIUMIII_CLEAN_challenge(poly *c, const uint8_t mu[CRHBYTES],
        const polyveck *w1);

int PQCLEAN_DILITHIUMIII_CLEAN_crypto_sign_keypair(uint8_t *pk,
        uint8_t *sk);

int PQCLEAN_DILITHIUMIII_CLEAN_crypto_sign(uint8_t *sm,
        size_t *smlen,
        const uint8_t *m,
        size_t mlen,
        const uint8_t *sk);

int PQCLEAN_DILITHIUMIII_CLEAN_crypto_sign_open(uint8_t *m,
        size_t *mlen,
        const uint8_t *sm,
        size_t smlen,
        const uint8_t *pk);

#endif
