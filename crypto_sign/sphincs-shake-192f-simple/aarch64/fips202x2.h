#ifndef SPX_FIPS202X2_H
#define SPX_FIPS202X2_H

#include "context.h"
#include "params.h"
#include <stdint.h>
#define load64 SPX_NAMESPACE(load64)
uint64_t load64(const unsigned char *x);
#define store64 SPX_NAMESPACE(store64)
void store64(uint8_t *x, uint64_t u);

#define shake128x2 SPX_NAMESPACE(shake128x2)
void shake128x2(unsigned char *out0,
                unsigned char *out1,
                unsigned long long outlen,
                unsigned char *in0,
                unsigned char *in1,
                unsigned long long inlen);

#define shake256x2 SPX_NAMESPACE(shake256x2)
void shake256x2(unsigned char *out0,
                unsigned char *out1,
                unsigned long long outlen,
                unsigned char *in0,
                unsigned char *in1,
                unsigned long long inlen);

#endif
