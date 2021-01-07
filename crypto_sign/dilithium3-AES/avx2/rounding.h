#ifndef ROUNDING_H
#define ROUNDING_H

#include "params.h"
#include <stdint.h>

#define power2round_avx DILITHIUM_NAMESPACE(_power2round_avx)
void power2round_avx(int32_t a1[N], int32_t a0[N], const int32_t a[N]);
#define decompose_avx DILITHIUM_NAMESPACE(_decompose_avx)
void decompose_avx(int32_t a1[N], int32_t a0[N], const int32_t a[N]);
#define make_hint_avx DILITHIUM_NAMESPACE(_make_hint_avx)
unsigned int make_hint_avx(int32_t h[N], const int32_t a0[N], const int32_t a1[N]);
#define use_hint_avx DILITHIUM_NAMESPACE(_use_hint_avx)
void use_hint_avx(int32_t b[N], const int32_t a[N], const int32_t hint[N]);

#endif
