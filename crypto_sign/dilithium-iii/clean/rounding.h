#ifndef ROUNDING_H
#define ROUNDING_H

#include <stdint.h>

uint32_t PQCLEAN_DILITHIUMIII_power2round(uint32_t a, uint32_t *a0);
uint32_t PQCLEAN_DILITHIUMIII_decompose(uint32_t a, uint32_t *a0);
unsigned int PQCLEAN_DILITHIUMIII_make_hint(uint32_t a, uint32_t b);
uint32_t PQCLEAN_DILITHIUMIII_use_hint(uint32_t a, unsigned int hint);

#endif
