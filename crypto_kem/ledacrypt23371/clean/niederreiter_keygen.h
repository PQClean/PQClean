#ifndef NIEDERREITER_KEYGEN_H
#define NIEDERREITER_KEYGEN_H

#include "niederreiter.h"
#include "rng.h"

void key_gen_niederreiter(publicKeyNiederreiter_t   *const pk,
                          privateKeyNiederreiter_t *const sk);

void publicKey_deletion_niederreiter(publicKeyNiederreiter_t    *const pk);
void privateKey_deletion_niederreiter(privateKeyNiederreiter_t *const sk);

#endif