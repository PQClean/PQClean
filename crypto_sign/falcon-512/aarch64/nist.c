/*
 * Wrapper for implementing the NIST API for the PQC standardization
 * process.
 */

#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "api.h"
#include "inner.h"
#include "config.h"

#define NONCELEN   40

/*
 * If stack usage is an issue, define TEMPALLOC to static in order to
 * allocate temporaries in the data section instead of the stack. This
 * would make the crypto_sign_keypair(), crypto_sign(), and
 * crypto_sign_open() functions not reentrant and not thread-safe, so
 * this should be done only for testing purposes.
 */
#define TEMPALLOC

void randombytes_init(unsigned char *entropy_input,
                      unsigned char *personalization_string,
                      int security_strength);
int randombytes(unsigned char *x, unsigned long long xlen);


int
crypto_sign_keypair(unsigned char *pk, unsigned char *sk) {
    TEMPALLOC union {
        uint8_t b[28 * FALCON_N];
        uint64_t dummy_u64;
        fpr dummy_fpr;
    } tmp;
    TEMPALLOC int8_t f[FALCON_N] = {0}, g[FALCON_N] = {0}, F[FALCON_N] = {0};
    TEMPALLOC uint16_t h[FALCON_N] = {0};
    TEMPALLOC unsigned char seed[48];
    TEMPALLOC inner_shake256_context rng;
    size_t u, v;



    /*
     * Generate key pair.
     */
    randombytes(seed, sizeof seed);
    inner_shake256_init(&rng);
    inner_shake256_inject(&rng, seed, sizeof seed);
    inner_shake256_flip(&rng);
    Zf(keygen)(&rng, f, g, F, NULL, h, FALCON_LOGN, tmp.b);


    /*
     * Encode private key.
     */
    sk[0] = 0x50 + FALCON_LOGN;
    u = 1;
    v = Zf(trim_i8_encode)(sk + u, CRYPTO_SECRETKEYBYTES - u, f, Zf(max_fg_bits)[FALCON_LOGN]);
    if (v == 0) {
        return -1;
    }
    u += v;
    v = Zf(trim_i8_encode)(sk + u, CRYPTO_SECRETKEYBYTES - u, g, Zf(max_fg_bits)[FALCON_LOGN]);
    if (v == 0) {
        return -1;
    }
    u += v;
    v = Zf(trim_i8_encode)(sk + u, CRYPTO_SECRETKEYBYTES - u, F, Zf(max_FG_bits)[FALCON_LOGN]);
    if (v == 0) {
        return -1;
    }
    u += v;
    if (u != CRYPTO_SECRETKEYBYTES) {
        return -1;
    }

    /*
     * Encode public key.
     */
    pk[0] = 0x00 + FALCON_LOGN;
    v = Zf(modq_encode)(pk + 1, CRYPTO_PUBLICKEYBYTES - 1, h, FALCON_LOGN);
    if (v != CRYPTO_PUBLICKEYBYTES - 1) {
        return -1;
    }

    return 0;
}

int
crypto_sign(unsigned char *sm, unsigned long long *smlen,
            const unsigned char *m, unsigned long long mlen,
            const unsigned char *sk) {
    TEMPALLOC union {
        uint8_t b[72 * FALCON_N];
        uint64_t dummy_u64;
        fpr dummy_fpr;
    } tmp;
    TEMPALLOC int8_t f[FALCON_N], g[FALCON_N], F[FALCON_N], G[FALCON_N];
    TEMPALLOC union {
        int16_t sig[FALCON_N];
        uint16_t hm[FALCON_N];
    } r;
    TEMPALLOC unsigned char seed[48], nonce[NONCELEN];
    TEMPALLOC unsigned char esig[CRYPTO_BYTES - 2 - sizeof nonce];
    TEMPALLOC inner_shake256_context sc;
    size_t u, v, sig_len;

    /*
     * Decode the private key.
     */
    if (sk[0] != 0x50 + FALCON_LOGN) {
        return -1;
    }
    u = 1;
    v = Zf(trim_i8_decode)(f, Zf(max_fg_bits)[FALCON_LOGN], sk + u, CRYPTO_SECRETKEYBYTES - u);
    if (v == 0) {
        return -1;
    }
    u += v;
    v = Zf(trim_i8_decode)(g, Zf(max_fg_bits)[FALCON_LOGN], sk + u, CRYPTO_SECRETKEYBYTES - u);
    if (v == 0) {
        return -1;
    }
    u += v;
    v = Zf(trim_i8_decode)(F, Zf(max_FG_bits)[FALCON_LOGN], sk + u, CRYPTO_SECRETKEYBYTES - u);
    if (v == 0) {
        return -1;
    }
    u += v;
    if (u != CRYPTO_SECRETKEYBYTES) {
        return -1;
    }
    if (!Zf(complete_private)(G, f, g, F, tmp.b)) {
        return -1;
    }

    /*
     * Create a random nonce (40 bytes).
     */
    randombytes(nonce, sizeof nonce);

    /*
     * Hash message nonce + message into a vector.
     */
    inner_shake256_init(&sc);
    inner_shake256_inject(&sc, nonce, sizeof nonce);
    inner_shake256_inject(&sc, m, mlen);
    inner_shake256_flip(&sc);
    Zf(hash_to_point_vartime)(&sc, r.hm, FALCON_LOGN);

    /*
     * Initialize a RNG.
     */
    randombytes(seed, sizeof seed);
    inner_shake256_init(&sc);
    inner_shake256_inject(&sc, seed, sizeof seed);
    inner_shake256_flip(&sc);


    /*
     * Compute the signature.
     */
    Zf(sign_dyn)(r.sig, &sc, f, g, F, G, r.hm, tmp.b);


    /*
     * Encode the signature and bundle it with the message. Format is:
     *   signature length     2 bytes, big-endian
     *   nonce                40 bytes
     *   message              mlen bytes
     *   signature            slen bytes
     */
    esig[0] = 0x20 + FALCON_LOGN;
    sig_len = Zf(comp_encode)(esig + 1, (sizeof esig) - 1, r.sig);
    if (sig_len == 0) {
        return -1;
    }
    sig_len ++;
    memmove(sm + 2 + sizeof nonce, m, mlen);
    sm[0] = (unsigned char)(sig_len >> 8);
    sm[1] = (unsigned char)sig_len;
    memcpy(sm + 2, nonce, sizeof nonce);
    memcpy(sm + 2 + (sizeof nonce) + mlen, esig, sig_len);
    *smlen = 2 + (sizeof nonce) + mlen + sig_len;
    return 0;
}

int
crypto_sign_open(unsigned char *m, unsigned long long *mlen,
                 const unsigned char *sm, unsigned long long smlen,
                 const unsigned char *pk) {

    const unsigned char *esig;
    TEMPALLOC int16_t h[FALCON_N], hm[FALCON_N],
              sig[FALCON_N], tmp[FALCON_N];
    TEMPALLOC inner_shake256_context sc;
    size_t sig_len, msg_len;

    /*
     * Decode public key.
     */
    if (pk[0] != 0x00 + FALCON_LOGN) {
        return -1;
    }
    if (Zf(modq_decode)((uint16_t *)h, pk + 1, CRYPTO_PUBLICKEYBYTES - 1, FALCON_LOGN) != CRYPTO_PUBLICKEYBYTES - 1) {
        return -1;
    }

    /*
     * Find nonce, signature, message length.
     */
    if (smlen < 2 + NONCELEN) {
        return -1;
    }
    sig_len = ((size_t)sm[0] << 8) | (size_t)sm[1];
    if (sig_len > (smlen - 2 - NONCELEN)) {
        return -1;
    }
    msg_len = smlen - 2 - NONCELEN - sig_len;

    /*
     * Decode signature.
     */
    esig = sm + 2 + NONCELEN + msg_len;
    if (sig_len < 1 || esig[0] != 0x20 + FALCON_LOGN) {
        return -1;
    }
    if (Zf(comp_decode)(sig, esig + 1, sig_len - 1) != sig_len - 1) {
        return -1;
    }

    /*
     * Hash nonce + message into a vector.
     */
    inner_shake256_init(&sc);
    inner_shake256_inject(&sc, sm + 2, NONCELEN + msg_len);
    inner_shake256_flip(&sc);
    Zf(hash_to_point_vartime)(&sc, (uint16_t *) hm, FALCON_LOGN);

    /*
     * Verify signature.
     */
    if (!Zf(verify_raw)(hm, sig, h, tmp)) {
        return -1;
    }

    /*
     * Return plaintext.
     */
    memmove(m, sm + 2 + NONCELEN, msg_len);
    *mlen = msg_len;
    return 0;
}
