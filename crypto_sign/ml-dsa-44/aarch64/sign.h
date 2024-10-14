#ifndef PQCLEAN_MLDSA44_AARCH64_SIGN_H
#define PQCLEAN_MLDSA44_AARCH64_SIGN_H

/*
 * This file is dual licensed
 * under Apache 2.0 (https://www.apache.org/licenses/LICENSE-2.0.html)
 * or public domain at https://github.com/pq-crystals/dilithium
 */

#include "params.h"
#include "poly.h"
#include "polyvec.h"
#include <stddef.h>
#include <stdint.h>

#define crypto_sign_keypair DILITHIUM_NAMESPACE(crypto_sign_keypair)
int crypto_sign_keypair(uint8_t *pk, uint8_t *sk);

#define crypto_sign_signature_ctx DILITHIUM_NAMESPACE(crypto_sign_signature_ctx)
int crypto_sign_signature_ctx(uint8_t *sig, size_t *siglen,
                              const uint8_t *m, size_t mlen,
                              const uint8_t *ctx, size_t ctxlen,
                              const uint8_t *sk);

#define crypto_sign_ctx DILITHIUM_NAMESPACE(crypto_sign_ctx)
int crypto_sign_ctx(uint8_t *sm, size_t *smlen,
                    const uint8_t *m, size_t mlen,
                    const uint8_t *ctx, size_t ctxlen,
                    const uint8_t *sk);

#define crypto_sign_verify_ctx DILITHIUM_NAMESPACE(crypto_sign_verify_ctx)
int crypto_sign_verify_ctx(const uint8_t *sig, size_t siglen,
                           const uint8_t *m, size_t mlen,
                           const uint8_t *ctx, size_t ctxlen,
                           const uint8_t *pk);

#define crypto_sign_open_ctx DILITHIUM_NAMESPACE(crypto_sign_open_ctx)
int crypto_sign_open_ctx(uint8_t *m, size_t *mlen,
                         const uint8_t *sm, size_t smlen,
                         const uint8_t *ctx, size_t ctxlen,
                         const uint8_t *pk);

#endif
