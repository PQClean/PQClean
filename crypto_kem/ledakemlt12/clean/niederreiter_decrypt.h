#pragma once

#include "niederreiter.h"
#include "gf2x_limbs.h"

/*----------------------------------------------------------------------------*/

int decrypt_niederreiter(DIGIT err[],       // return 1 if everything is ok
                         const privateKeyNiederreiter_t *const sk,
                         const DIGIT syndrome[]
                        );

/*----------------------------------------------------------------------------*/
