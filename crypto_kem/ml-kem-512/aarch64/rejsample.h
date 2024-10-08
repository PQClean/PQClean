#ifndef PQCLEAN_MLKEM512_AARCH64_REJSAMPLE_H
#define PQCLEAN_MLKEM512_AARCH64_REJSAMPLE_H

/*
 * This file is licensed
 * under Apache 2.0 (https://www.apache.org/licenses/LICENSE-2.0.html)
 * at https://github.com/GMUCERG/PQC_NEON/blob/main/neon/kyber or
 * public domain at https://github.com/cothan/kyber/blob/master/neon
 */

#include <stdint.h>
#include "params.h"

#define neon_rej_uniform KYBER_NAMESPACE(_neon_rej_uniform)
unsigned int neon_rej_uniform(int16_t *r,
                              const uint8_t *buf);

#define rej_uniform KYBER_NAMESPACE(_ref_rej_uniform)
unsigned int rej_uniform(int16_t *r,
                         unsigned int len,
                         const uint8_t *buf,
                         unsigned int buflen);
#endif
