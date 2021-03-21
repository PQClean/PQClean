#ifndef NIEDERREITER_ENCRYPT_H
#define NIEDERREITER_ENCRYPT_H

#include "niederreiter.h"
#include "gf2x_limbs.h"


void PQCLEAN_LEDACRYPT23371_CLEAN_encrypt_niederreiter_indcca2(unsigned char *const
                                  ct,  /* ciphertext - output    */
                                  unsigned char *const ss,  /* shared secret - output */
                                  const publicKeyNiederreiter_t *const pk);

#endif