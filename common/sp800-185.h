#ifndef SP800_185_H
#define SP800_185_H

#include <stddef.h>
#include <stdint.h>

#include "fips202.h"

void cshake128_inc_init(shake128incctx *state, const uint8_t *name, size_t namelen, const uint8_t *cstm, size_t cstmlen);
void cshake128_inc_absorb(shake128incctx *state, const uint8_t *input, size_t inlen);
void cshake128_inc_finalize(shake128incctx *state);
void cshake128_inc_squeeze(uint8_t *output, size_t outlen, shake128incctx *state);
void cshake128_inc_ctx_release(shake128incctx *state);
void cshake128_inc_ctx_clone(shake128incctx *dest, const shake128incctx *src);

void cshake128(uint8_t *output, size_t outlen, const uint8_t *name, size_t namelen, const uint8_t *cstm, size_t cstmlen, const uint8_t *input, size_t inlen);

void cshake256_inc_init(shake256incctx *state, const uint8_t *name, size_t namelen, const uint8_t *cstm, size_t cstmlen);
void cshake256_inc_absorb(shake256incctx *state, const uint8_t *input, size_t inlen);
void cshake256_inc_finalize(shake256incctx *state);
void cshake256_inc_squeeze(uint8_t *output, size_t outlen, shake256incctx *state);
void cshake256_inc_ctx_release(shake256incctx *state);
void cshake256_inc_ctx_clone(shake256incctx *dest, const shake256incctx *src);

void cshake256(uint8_t *output, size_t outlen, const uint8_t *name, size_t namelen, const uint8_t* cstm, size_t cstmlen, const uint8_t *input, size_t inlen);

#endif
