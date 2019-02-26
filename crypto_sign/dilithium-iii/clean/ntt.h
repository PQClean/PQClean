#ifndef NTT_H
#define NTT_H

#include "params.h"
#include <stdint.h>

void PQCLEAN_DILITHIUMIII_CLEAN_ntt(uint32_t p[N]);
void PQCLEAN_DILITHIUMIII_CLEAN_invntt_frominvmont(uint32_t p[N]);

#endif
