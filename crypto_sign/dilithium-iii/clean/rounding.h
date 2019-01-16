#ifndef ROUNDING_H
#define ROUNDING_H

#include <stdint.h>

uint32_t power2round(uint32_t a, uint32_t *a0);
uint32_t decompose(uint32_t a, uint32_t *a0);
unsigned int make_hint(uint32_t a, uint32_t b);
uint32_t use_hint(uint32_t a, unsigned int hint);

#endif
