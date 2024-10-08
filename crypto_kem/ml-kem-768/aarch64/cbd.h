#ifndef PQCLEAN_MLKEM768_AARCH64_CBD_H
#define PQCLEAN_MLKEM768_AARCH64_CBD_H

/*
 * This file is licensed
 * under Apache 2.0 (https://www.apache.org/licenses/LICENSE-2.0.html) or
 * public domain at https://github.com/pq-crystals/kyber/tree/master/ref
 */

#include <stdint.h>
#include "params.h"
#include "poly.h"

#define poly_cbd_eta1 KYBER_NAMESPACE(poly_cbd_eta1)
void poly_cbd_eta1(int16_t *r, const uint8_t buf[KYBER_ETA1 * KYBER_N / 4]);

#define poly_cbd_eta2 KYBER_NAMESPACE(poly_cbd_eta2)
void poly_cbd_eta2(int16_t *r, const uint8_t buf[KYBER_ETA2 * KYBER_N / 4]);

#endif
