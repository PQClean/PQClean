#ifndef PQCLEAN_DILITHIUM4_CLEAN_PACKING_H
#define PQCLEAN_DILITHIUM4_CLEAN_PACKING_H

#include "params.h"
#include "polyvec.h"

void PQCLEAN_DILITHIUM4_CLEAN_pack_pk(
    uint8_t *pk,
    const uint8_t *rho, const polyveck *t1);
void PQCLEAN_DILITHIUM4_CLEAN_pack_sk(
    uint8_t *sk,
    const uint8_t *rho,
    const uint8_t *key,
    const uint8_t *tr,
    const polyvecl *s1,
    const polyveck *s2,
    const polyveck *t0);
void PQCLEAN_DILITHIUM4_CLEAN_pack_sig(
    uint8_t *sig,
    const polyvecl *z, const polyveck *h, const poly *c);

void PQCLEAN_DILITHIUM4_CLEAN_unpack_pk(
    uint8_t *rho, polyveck *t1,
    const uint8_t *pk);
void PQCLEAN_DILITHIUM4_CLEAN_unpack_sk(
    uint8_t *rho,
    uint8_t *key,
    uint8_t *tr,
    polyvecl *s1,
    polyveck *s2,
    polyveck *t0,
    const uint8_t *sk);
int PQCLEAN_DILITHIUM4_CLEAN_unpack_sig(
    polyvecl *z, polyveck *h, poly *c, const uint8_t *sig);

#endif
