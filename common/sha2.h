#ifndef SHA2_H
#define SHA2_H

#include <stddef.h>
#include <stdint.h>

/* The incremental API allows hashing of individual input blocks; these blocks
    must be exactly 64 bytes each.
    Use the 'finalize' functions for any remaining bytes (possibly over 64). */

typedef struct {
    uint8_t ctx[40];
} sha224ctx;

typedef struct {
    uint8_t ctx[40];
} sha256ctx;

typedef struct {
    uint8_t ctx[72];
} sha384ctx;

typedef struct {
    uint8_t ctx[72];
} sha512ctx;

void sha224_inc_init(sha224ctx *state);
void sha224_inc_clone_state(sha224ctx *stateout, const sha224ctx *statein);
void sha224_inc_blocks(sha224ctx *state, const uint8_t *in, size_t inblocks);
void sha224_inc_finalize(uint8_t *out, sha224ctx *state, const uint8_t *in, size_t inlen);
void sha224(uint8_t *out, const uint8_t *in, size_t inlen);

void sha256_inc_init(sha256ctx *state);
void sha256_inc_clone_state(sha256ctx *stateout, const sha256ctx *statein);
void sha256_inc_blocks(sha256ctx *state, const uint8_t *in, size_t inblocks);
void sha256_inc_finalize(uint8_t *out, sha256ctx *state, const uint8_t *in, size_t inlen);
void sha256(uint8_t *out, const uint8_t *in, size_t inlen);

void sha384_inc_init(sha384ctx *state);
void sha384_inc_clone_state(sha384ctx *stateout, const sha384ctx *statein);
void sha384_inc_blocks(sha384ctx *state, const uint8_t *in, size_t inblocks);
void sha384_inc_finalize(uint8_t *out, sha384ctx *state, const uint8_t *in, size_t inlen);
void sha384(uint8_t *out, const uint8_t *in, size_t inlen);

void sha512_inc_init(sha512ctx *state);
void sha512_inc_clone_state(sha512ctx *stateout, const sha512ctx *statein);
void sha512_inc_blocks(sha512ctx *state, const uint8_t *in, size_t inblocks);
void sha512_inc_finalize(uint8_t *out, sha512ctx *state, const uint8_t *in, size_t inlen);
void sha512(uint8_t *out, const uint8_t *in, size_t inlen);

#endif
