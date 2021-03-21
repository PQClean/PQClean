#ifndef NIEDERREITER_KEYGEN_H
#define NIEDERREITER_KEYGEN_H

#include "niederreiter.h"
#include "rng.h"

void PQCLEAN_LEDACRYPT23371_CLEAN_key_gen_niederreiter(publicKeyNiederreiter_t   *const pk,
                                                       privateKeyNiederreiter_t *const sk);

void PQCLEAN_LEDACRYPT23371_CLEAN_publicKey_deletion_niederreiter(publicKeyNiederreiter_t    *const pk);
void PQCLEAN_LEDACRYPT23371_CLEAN_privateKey_deletion_niederreiter(privateKeyNiederreiter_t *const sk);

#endif