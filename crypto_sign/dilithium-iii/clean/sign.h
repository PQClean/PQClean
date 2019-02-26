#ifndef SIGN_H
#define SIGN_H

#include "params.h"
#include "poly.h"
#include "polyvec.h"

void PQCLEAN_DILITHIUMIII_CLEAN_expand_mat(polyvecl mat[K],
        const unsigned char rho[SEEDBYTES]);
void PQCLEAN_DILITHIUMIII_CLEAN_challenge(poly *c, const unsigned char mu[CRHBYTES],
        const polyveck *w1);

int PQCLEAN_DILITHIUMIII_CLEAN_crypto_sign_keypair(unsigned char *pk,
        unsigned char *sk);

int PQCLEAN_DILITHIUMIII_CLEAN_crypto_sign(unsigned char *sm,
        unsigned long long *smlen,
        const unsigned char *m,
        unsigned long long mlen,
        const unsigned char *sk);

int PQCLEAN_DILITHIUMIII_CLEAN_crypto_sign_open(unsigned char *m,
        unsigned long long *mlen,
        const unsigned char *sm,
        unsigned long long smlen,
        const unsigned char *pk);

#endif
