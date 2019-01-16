#ifndef NTT_H
#define NTT_H

#include "params.h"
#include <stdint.h>

void ntt(uint32_t p[N]);
void invntt_frominvmont(uint32_t p[N]);

#endif
