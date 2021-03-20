#ifndef NIEDERREITER_DECRYPT_H
#define NIEDERREITER_DECRYPT_H

#include "niederreiter.h"
#include "gf2x_limbs.h"

/*----------------------------------------------------------------------------*/

void decrypt_niederreiter_indcca2(unsigned char *const ss,
                                  const unsigned char *const ct,
                                  const privateKeyNiederreiter_t *const sk);

/*----------------------------------------------------------------------------*/

#endif