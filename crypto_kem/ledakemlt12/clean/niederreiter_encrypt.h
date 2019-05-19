#pragma once

#include "niederreiter.h"
#include "gf2x_limbs.h"

void encrypt_niederreiter(DIGIT syndrome[],
                          const publicKeyNiederreiter_t *const pk,
                          const DIGIT err[]
                         );
