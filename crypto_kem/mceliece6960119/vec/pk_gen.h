#ifndef PK_GEN_H
#define PK_GEN_H
/*
  This file is for public-key generation
*/

#include <stdint.h>

int PQCLEAN_MCELIECE6960119_VEC_pk_gen(unsigned char * /*pk*/, uint32_t * /*perm*/, const unsigned char * /*sk*/);

#endif

