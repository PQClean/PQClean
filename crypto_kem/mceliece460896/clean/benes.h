#ifndef PQCLEAN_MCELIECE460896_CLEAN_BENES_H
#define PQCLEAN_MCELIECE460896_CLEAN_BENES_H
/*
  This file is for Benes network related functions
*/

#include "gf.h"

void PQCLEAN_MCELIECE460896_CLEAN_apply_benes(uint8_t *r, const uint8_t *bits, int rev);
void PQCLEAN_MCELIECE460896_CLEAN_support_gen(gf *s, const uint8_t *c);

#endif

