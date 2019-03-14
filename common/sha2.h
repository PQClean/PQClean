#ifndef SHA2_H
#define SHA2_H

#include <stddef.h>
#include <stdint.h>

/* The incremental API allows hashing of individual input blocks; these blocks
    must be exactly 64 bytes each.
    Use the 'finalize' functions for any remaining bytes (possibly over 64). */

void sha224_inc_init(uint8_t *state);
void sha224_inc_blocks(uint8_t *state, const uint8_t *in, size_t inblocks);
void sha224_inc_finalize(uint8_t *out, uint8_t *state, const uint8_t *in, size_t inlen);
void sha224(uint8_t *out, const uint8_t *in, size_t inlen);

void sha256_inc_init(uint8_t *state);
void sha256_inc_blocks(uint8_t *state, const uint8_t *in, size_t inblocks);
void sha256_inc_finalize(uint8_t *out, uint8_t *state, const uint8_t *in, size_t inlen);
void sha256(uint8_t *out, const uint8_t *in, size_t inlen);

void sha384_inc_init(uint8_t *state);
void sha384_inc_blocks(uint8_t *state, const uint8_t *in, size_t inblocks);
void sha384_inc_finalize(uint8_t *out, uint8_t *state, const uint8_t *in, size_t inlen);
void sha384(uint8_t *out, const uint8_t *in, size_t inlen);

void sha512_inc_init(uint8_t *state);
void sha512_inc_blocks(uint8_t *state, const uint8_t *in, size_t inblocks);
void sha512_inc_finalize(uint8_t *out, uint8_t *state, const uint8_t *in, size_t inlen);
void sha512(uint8_t *out, const uint8_t *in, size_t inlen);

#endif
