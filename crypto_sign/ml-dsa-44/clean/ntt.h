#ifndef PQCLEAN_MLDSA44_CLEAN_NTT_H
#define PQCLEAN_MLDSA44_CLEAN_NTT_H
#include "params.h"
#include <stdint.h>

void PQCLEAN_MLDSA44_CLEAN_ntt(int32_t a[N]);

void PQCLEAN_MLDSA44_CLEAN_invntt_tomont(int32_t a[N]);

#endif
