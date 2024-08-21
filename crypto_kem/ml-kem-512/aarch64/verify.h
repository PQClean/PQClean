#ifndef PQCLEAN_MLKEM512_AARCH64_VERIFY_H
#define PQCLEAN_MLKEM512_AARCH64_VERIFY_H

/*
 * This file is licensed
 * under Apache 2.0 (https://www.apache.org/licenses/LICENSE-2.0.html) or
 * public domain at https://github.com/pq-crystals/kyber/tree/master/ref
 */

#include <stddef.h>
#include <stdint.h>
#include "params.h"

#define verify KYBER_NAMESPACE(verify)
int verify(const uint8_t *a, const uint8_t *b, size_t len);

#define cmov KYBER_NAMESPACE(cmov)
void cmov(uint8_t *r, const uint8_t *x, size_t len, uint8_t b);

#define cmov_int16 KYBER_NAMESPACE(cmov_int16)
void cmov_int16(int16_t *r, int16_t v, uint16_t b);
#endif
