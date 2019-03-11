#ifndef SHA2_H
#define SHA2_H

#include <stddef.h>
#include <stdint.h>

int sha256(uint8_t *out, const uint8_t *in, size_t inlen);

int sha384(uint8_t *out, const uint8_t *in, size_t inlen);

int sha512(uint8_t *out, const uint8_t *in, size_t inlen);

#endif
