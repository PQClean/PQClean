#pragma once

#include "niederreiter.h"
#include "rng.h"

void key_gen_niederreiter(publicKeyNiederreiter_t   *const pk,
                          privateKeyNiederreiter_t *const sk,
                          AES_XOF_struct *keys_expander);

void publicKey_deletion_niederreiter(publicKeyNiederreiter_t    *const pk);
void privateKey_deletion_niederreiter(privateKeyNiederreiter_t *const sk);
