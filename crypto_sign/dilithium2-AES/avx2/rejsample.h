#ifndef REJSAMPLE_H
#define REJSAMPLE_H

#include "params.h"
#include <stdint.h>

#define rej_uniform_avx DILITHIUM_NAMESPACE(_rej_uniform_avx)
unsigned int rej_uniform_avx(int32_t *r, const uint8_t *buf);


#define rej_eta_avx DILITHIUM_NAMESPACE(_rej_eta_avx)
unsigned int rej_eta_avx(int32_t *r,
                         unsigned int len,
                         const uint8_t *buf,
                         unsigned int buflen);

#define rej_gamma1m1_avx DILITHIUM_NAMESPACE(_rej_uniform_gamma1m1_avx)
unsigned int rej_gamma1m1_avx(int32_t *r,
                              unsigned int len,
                              const uint8_t *buf,
                              unsigned int buflen);

#endif
