#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "api.h"
#include "fips202.h"
#include "gf31.h"
#include "mq.h"
#include "params.h"
#include "randombytes.h"

/* Takes an array of len bytes and computes a hash digest.
   This is used as a hash function in the Fiat-Shamir transform. */
static void H(unsigned char *out, const unsigned char *in, const size_t len)
{
    shake256(out, HASH_BYTES, in, len);
}

/* Takes two arrays of N packed elements and an array of M packed elements,
   and computes a HASH_BYTES commitment. */
static void com_0(unsigned char *c,
           const unsigned char *rho,
           const unsigned char *inn, const unsigned char *inn2,
           const unsigned char *inm)
{
    unsigned char buffer[HASH_BYTES + 2*NPACKED_BYTES + MPACKED_BYTES];
    memcpy(buffer, rho, HASH_BYTES);
    memcpy(buffer + HASH_BYTES, inn, NPACKED_BYTES);
    memcpy(buffer + HASH_BYTES + NPACKED_BYTES, inn2, NPACKED_BYTES);
    memcpy(buffer + HASH_BYTES + 2*NPACKED_BYTES, inm, MPACKED_BYTES);
    shake256(c, HASH_BYTES, buffer, HASH_BYTES + 2*NPACKED_BYTES + MPACKED_BYTES);
}

/* Takes an array of N packed elements and an array of M packed elements,
   and computes a HASH_BYTES commitment. */
static void com_1(unsigned char *c,
           const unsigned char *rho,
           const unsigned char *inn, const unsigned char *inm)
{
    unsigned char buffer[HASH_BYTES + NPACKED_BYTES + MPACKED_BYTES];
    memcpy(buffer, rho, HASH_BYTES);
    memcpy(buffer + HASH_BYTES, inn, NPACKED_BYTES);
    memcpy(buffer + HASH_BYTES + NPACKED_BYTES, inm, MPACKED_BYTES);
    shake256(c, HASH_BYTES, buffer, HASH_BYTES + NPACKED_BYTES + MPACKED_BYTES);
}

/*
 * Generates an MQDSS key pair.
 */
int PQCLEAN_MQDSS48_CLEAN_crypto_sign_keypair(uint8_t *pk, uint8_t *sk) {
    signed char F[F_LEN];
    unsigned char skbuf[SEED_BYTES * 2];
    gf31 sk_gf31[N];
    gf31 pk_gf31[M];

    // Expand sk to obtain a seed for F and the secret input s.
    // We also expand to obtain a value for sampling r0, t0 and e0 during
    //  signature generation, but that is not relevant here.
    randombytes(sk, SEED_BYTES);
    shake256(skbuf, SEED_BYTES * 2, sk, SEED_BYTES);

    memcpy(pk, skbuf, SEED_BYTES);
    PQCLEAN_MQDSS48_CLEAN_gf31_nrand_schar(F, F_LEN, pk, SEED_BYTES);
    PQCLEAN_MQDSS48_CLEAN_gf31_nrand(sk_gf31, N, skbuf + SEED_BYTES, SEED_BYTES);
    PQCLEAN_MQDSS48_CLEAN_MQ(pk_gf31, sk_gf31, F);
    PQCLEAN_MQDSS48_CLEAN_vgf31_unique(pk_gf31, pk_gf31);
    PQCLEAN_MQDSS48_CLEAN_gf31_npack(pk + SEED_BYTES, pk_gf31, M);

    return 0;
}


/**
 * Returns an array containing a detached signature.
 */
int PQCLEAN_MQDSS48_CLEAN_crypto_sign_signature(
    uint8_t *sig, size_t *siglen,
    const uint8_t *m, size_t mlen, const uint8_t *sk) {

    (void)sig;
    (void)siglen;
    (void)m;
    (void)mlen;
    (void)sk;

    return 0;
}

/**
 * Verifies a detached signature and message under a given public key.
 */
int PQCLEAN_MQDSS48_CLEAN_crypto_sign_verify(
    const uint8_t *sig, size_t siglen,
    const uint8_t *m, size_t mlen, const uint8_t *pk) {

    (void)sig;
    (void)siglen;
    (void)m;
    (void)mlen;
    (void)pk;

    return 0;
}


/**
 * Returns an array containing the signature followed by the message.
 */
int PQCLEAN_MQDSS48_CLEAN_crypto_sign(
    uint8_t *sm, size_t *smlen,
    const uint8_t *m, size_t mlen, const uint8_t *sk) {

    signed char F[F_LEN];
    unsigned char skbuf[SEED_BYTES * 4];
    gf31 pk_gf31[M];
    unsigned char pk[SEED_BYTES + MPACKED_BYTES];
    // Concatenated for convenient hashing.
    unsigned char D_sigma0_h0_sigma1[HASH_BYTES * 3 + ROUNDS * (NPACKED_BYTES + MPACKED_BYTES)];
    unsigned char *D = D_sigma0_h0_sigma1;
    unsigned char *sigma0 = D_sigma0_h0_sigma1 + HASH_BYTES;
    unsigned char *h0 = D_sigma0_h0_sigma1 + 2*HASH_BYTES;
    unsigned char *t1packed = D_sigma0_h0_sigma1 + 3*HASH_BYTES;
    unsigned char *e1packed = D_sigma0_h0_sigma1 + 3*HASH_BYTES + ROUNDS * NPACKED_BYTES;
    uint64_t shakestate[25] = {0};
    unsigned char shakeblock[SHAKE256_RATE];
    unsigned char h1[((ROUNDS + 7) & ~7) >> 3];
    unsigned char rnd_seed[HASH_BYTES + SEED_BYTES];
    unsigned char rho[2 * ROUNDS * HASH_BYTES];
    unsigned char *rho0 = rho;
    unsigned char *rho1 = rho + ROUNDS * HASH_BYTES;
    gf31 sk_gf31[N];
    gf31 rnd[(2 * N + M) * ROUNDS];  // Concatenated for easy RNG.
    gf31 *r0 = rnd;
    gf31 *t0 = rnd + N * ROUNDS;
    gf31 *e0 = rnd + 2 * N * ROUNDS;
    gf31 r1[N * ROUNDS];
    gf31 t1[N * ROUNDS];
    gf31 e1[M * ROUNDS];
    gf31 gx[M * ROUNDS];
    unsigned char packbuf0[NPACKED_BYTES];
    unsigned char packbuf1[NPACKED_BYTES];
    unsigned char packbuf2[MPACKED_BYTES];
    unsigned char c[HASH_BYTES * ROUNDS * 2];
    gf31 alpha;
    int alpha_count = 0;
    unsigned char b;
    int i, j;

    shake256(skbuf, SEED_BYTES * 4, sk, SEED_BYTES);

    PQCLEAN_MQDSS48_CLEAN_gf31_nrand_schar(F, F_LEN, skbuf, SEED_BYTES);

    assert(SIG_LEN > SEED_BYTES);
    memcpy(sm + SIG_LEN - SEED_BYTES, sk, SEED_BYTES);
    memcpy(sm + SIG_LEN, m, mlen);
    H(sm, sm + SIG_LEN - SEED_BYTES, mlen + SEED_BYTES);  // Compute R.

    memcpy(pk, skbuf, SEED_BYTES);
    PQCLEAN_MQDSS48_CLEAN_gf31_nrand(sk_gf31, N, skbuf + SEED_BYTES, SEED_BYTES);
    PQCLEAN_MQDSS48_CLEAN_MQ(pk_gf31, sk_gf31, F);
    PQCLEAN_MQDSS48_CLEAN_vgf31_unique(pk_gf31, pk_gf31);
    PQCLEAN_MQDSS48_CLEAN_gf31_npack(pk + SEED_BYTES, pk_gf31, M);

    memcpy(sm + SIG_LEN - HASH_BYTES - PK_BYTES, pk, PK_BYTES);
    memcpy(sm + SIG_LEN - HASH_BYTES, sm, HASH_BYTES);
    H(D, sm + SIG_LEN - HASH_BYTES - PK_BYTES, mlen + PK_BYTES + HASH_BYTES);

    sm += HASH_BYTES;  // Compensate for prefixed R.

    memcpy(rnd_seed, skbuf + 2*SEED_BYTES, SEED_BYTES);
    memcpy(rnd_seed + SEED_BYTES, D, HASH_BYTES);
    shake256(rho, 2 * ROUNDS * HASH_BYTES, rnd_seed, SEED_BYTES + HASH_BYTES);

    memcpy(rnd_seed, skbuf + 3*SEED_BYTES, SEED_BYTES);
    memcpy(rnd_seed + SEED_BYTES, D, HASH_BYTES);
    PQCLEAN_MQDSS48_CLEAN_gf31_nrand(rnd, (2 * N + M) * ROUNDS, rnd_seed, SEED_BYTES + HASH_BYTES);

    for (i = 0; i < ROUNDS; i++) {
        for (j = 0; j < N; j++) {
            r1[j + i*N] = (gf31)(31 + sk_gf31[j] - r0[j + i*N]);
        }
        PQCLEAN_MQDSS48_CLEAN_G(gx + i*M, t0 + i*N, r1 + i*N, F);
    }
    for (i = 0; i < ROUNDS * M; i++) {
        gx[i] = (gf31)(gx[i] + e0[i]);
    }
    for (i = 0; i < ROUNDS; i++) {
        PQCLEAN_MQDSS48_CLEAN_gf31_npack(packbuf0, r0 + i*N, N);
        PQCLEAN_MQDSS48_CLEAN_gf31_npack(packbuf1, t0 + i*N, N);
        PQCLEAN_MQDSS48_CLEAN_gf31_npack(packbuf2, e0 + i*M, M);
        com_0(c + HASH_BYTES * (2*i + 0), rho0 + i*HASH_BYTES, packbuf0, packbuf1, packbuf2);
        PQCLEAN_MQDSS48_CLEAN_vgf31_shorten_unique(r1 + i*N, r1 + i*N);
        PQCLEAN_MQDSS48_CLEAN_vgf31_shorten_unique(gx + i*M, gx + i*M);
        PQCLEAN_MQDSS48_CLEAN_gf31_npack(packbuf0, r1 + i*N, N);
        PQCLEAN_MQDSS48_CLEAN_gf31_npack(packbuf1, gx + i*M, M);
        com_1(c + HASH_BYTES * (2*i + 1), rho1 + i*HASH_BYTES, packbuf0, packbuf1);
    }

    H(sigma0, c, HASH_BYTES * ROUNDS * 2);  // Compute sigma_0.
    shake256_absorb(shakestate, D_sigma0_h0_sigma1, 2 * HASH_BYTES);
    shake256_squeezeblocks(shakeblock, 1, shakestate);

    memcpy(h0, shakeblock, HASH_BYTES);

    memcpy(sm, sigma0, HASH_BYTES);
    sm += HASH_BYTES;  // Compensate for sigma_0.

    for (i = 0; i < ROUNDS; i++) {
        do {
            alpha = shakeblock[alpha_count] & 31;
            alpha_count++;
            if (alpha_count == SHAKE256_RATE) {
                alpha_count = 0;
                shake256_squeezeblocks(shakeblock, 1, shakestate);
            }
        } while (alpha == 31);
        for (j = 0; j < N; j++) {
            t1[i*N + j] = (gf31)(alpha * r0[j + i*N] - t0[j + i*N] + 31);
        }
        PQCLEAN_MQDSS48_CLEAN_MQ(e1 + i*M, r0 + i*N, F);
        for (j = 0; j < N; j++) {
            e1[i*N + j] = (gf31)(alpha * e1[j + i*M] - e0[j + i*M] + 31);
        }
        PQCLEAN_MQDSS48_CLEAN_vgf31_shorten_unique(t1 + i*N, t1 + i*N);
        PQCLEAN_MQDSS48_CLEAN_vgf31_shorten_unique(e1 + i*N, e1 + i*N);
    }
    PQCLEAN_MQDSS48_CLEAN_gf31_npack(t1packed, t1, N * ROUNDS);
    PQCLEAN_MQDSS48_CLEAN_gf31_npack(e1packed, e1, M * ROUNDS);

    memcpy(sm, t1packed, NPACKED_BYTES * ROUNDS);
    sm += NPACKED_BYTES * ROUNDS;
    memcpy(sm, e1packed, MPACKED_BYTES * ROUNDS);
    sm += MPACKED_BYTES * ROUNDS;

    shake256(h1, ((ROUNDS + 7) & ~7) >> 3, D_sigma0_h0_sigma1, 3*HASH_BYTES + ROUNDS*(NPACKED_BYTES + MPACKED_BYTES));

    for (i = 0; i < ROUNDS; i++) {
        b = (h1[(i >> 3)] >> (i & 7)) & 1;
        if (b == 0) {
            PQCLEAN_MQDSS48_CLEAN_gf31_npack(sm, r0+i*N, N);
        } else if (b == 1) {
            PQCLEAN_MQDSS48_CLEAN_gf31_npack(sm, r1+i*N, N);
        }
        memcpy(sm + NPACKED_BYTES, c + HASH_BYTES * (2*i + (1 - b)), HASH_BYTES);
        memcpy(sm + NPACKED_BYTES + HASH_BYTES, rho + (i + b * ROUNDS) * HASH_BYTES, HASH_BYTES);
        sm += NPACKED_BYTES + 2*HASH_BYTES;
    }
    *smlen = SIG_LEN + mlen;
    return 0;
}

/**
 * Verifies a given signature-message pair under a given public key.
 */
int PQCLEAN_MQDSS48_CLEAN_crypto_sign_open(
    uint8_t *m, size_t *mlen,
    const uint8_t *sm, size_t smlen, const uint8_t *pk)
{
    gf31 r[N];
    gf31 t[N];
    gf31 e[M];
    signed char F[F_LEN];
    gf31 pk_gf31[M];
    unsigned char sig[SIG_LEN];
    unsigned char *sigptr = sig;
    // Concatenated for convenient hashing.
    unsigned char D_sigma0_h0_sigma1[HASH_BYTES * 3 + ROUNDS * (NPACKED_BYTES + MPACKED_BYTES)];
    unsigned char *D = D_sigma0_h0_sigma1;
    unsigned char *sigma0 = D_sigma0_h0_sigma1 + HASH_BYTES;
    unsigned char *h0 = D_sigma0_h0_sigma1 + 2*HASH_BYTES;
    unsigned char *t1packed = D_sigma0_h0_sigma1 + 3*HASH_BYTES;
    unsigned char *e1packed = D_sigma0_h0_sigma1 + 3*HASH_BYTES + ROUNDS * NPACKED_BYTES;
    unsigned char h1[((ROUNDS + 7) & ~7) >> 3];
    unsigned char c[HASH_BYTES * ROUNDS * 2];
    memset(c, 0, HASH_BYTES*2);
    gf31 x[N];
    gf31 y[M];
    gf31 z[M];
    unsigned char packbuf0[NPACKED_BYTES];
    unsigned char packbuf1[MPACKED_BYTES];
    uint64_t shakestate[25] = {0};
    unsigned char shakeblock[SHAKE256_RATE];
    int i, j;
    gf31 alpha;
    int alpha_count = 0;
    unsigned char b;

    /* The API caller does not necessarily know what size a signature should be
       but MQDSS signatures are always exactly SIG_LEN. */
    if (smlen < SIG_LEN) {
        memset(m, 0, smlen);
        *mlen = 0;
        return 1;
    }

    *mlen = smlen - SIG_LEN;

    /* Create a copy of the signature so that m = sm is not an issue */
    memcpy(sig, sm, SIG_LEN);

    /* Put the message all the way at the end of the m buffer, so that we can
     * prepend the required other inputs for the hash function. */
    memcpy(m + SIG_LEN, sm + SIG_LEN, *mlen);

    memcpy(m + SIG_LEN - PK_BYTES - HASH_BYTES, pk, PK_BYTES);  // Copy pk to m.
    memcpy(m + SIG_LEN - HASH_BYTES, sigptr, HASH_BYTES);  // Copy R to m.
    H(D, m + SIG_LEN - PK_BYTES - HASH_BYTES, *mlen + PK_BYTES + HASH_BYTES);

    sigptr += HASH_BYTES;

    PQCLEAN_MQDSS48_CLEAN_gf31_nrand_schar(F, F_LEN, pk, SEED_BYTES);
    pk += SEED_BYTES;
    PQCLEAN_MQDSS48_CLEAN_gf31_nunpack(pk_gf31, pk, M);

    memcpy(sigma0, sigptr, HASH_BYTES);

    shake256_absorb(shakestate, D_sigma0_h0_sigma1, 2 * HASH_BYTES);
    shake256_squeezeblocks(shakeblock, 1, shakestate);

    memcpy(h0, shakeblock, HASH_BYTES);

    sigptr += HASH_BYTES;

    memcpy(t1packed, sigptr, ROUNDS * NPACKED_BYTES);
    sigptr += ROUNDS*NPACKED_BYTES;
    memcpy(e1packed, sigptr, ROUNDS * MPACKED_BYTES);
    sigptr += ROUNDS*MPACKED_BYTES;

    shake256(h1, ((ROUNDS + 7) & ~7) >> 3, D_sigma0_h0_sigma1, 3*HASH_BYTES + ROUNDS*(NPACKED_BYTES + MPACKED_BYTES));

    for (i = 0; i < ROUNDS; i++) {
        do {
            alpha = shakeblock[alpha_count] & 31;
            alpha_count++;
            if (alpha_count == SHAKE256_RATE) {
                alpha_count = 0;
                shake256_squeezeblocks(shakeblock, 1, shakestate);
            }
        } while (alpha == 31);
        b = (h1[(i >> 3)] >> (i & 7)) & 1;

        PQCLEAN_MQDSS48_CLEAN_gf31_nunpack(r, sigptr, N);
        PQCLEAN_MQDSS48_CLEAN_gf31_nunpack(t, t1packed + NPACKED_BYTES*i, N);
        PQCLEAN_MQDSS48_CLEAN_gf31_nunpack(e, e1packed + MPACKED_BYTES*i, M);

        if (b == 0) {
            PQCLEAN_MQDSS48_CLEAN_MQ(y, r, F);
            for (j = 0; j < N; j++) {
                x[j] = (gf31)(alpha * r[j] - t[j] + 31);
            }
            for (j = 0; j < N; j++) {
                y[j] = (gf31)(alpha * y[j] - e[j] + 31);
            }
            PQCLEAN_MQDSS48_CLEAN_vgf31_shorten_unique(x, x);
            PQCLEAN_MQDSS48_CLEAN_vgf31_shorten_unique(y, y);
            PQCLEAN_MQDSS48_CLEAN_gf31_npack(packbuf0, x, N);
            PQCLEAN_MQDSS48_CLEAN_gf31_npack(packbuf1, y, M);
            com_0(c + HASH_BYTES*(2*i + 0), sigptr + HASH_BYTES + NPACKED_BYTES, sigptr, packbuf0, packbuf1);
        } else {
            PQCLEAN_MQDSS48_CLEAN_MQ(y, r, F);
            PQCLEAN_MQDSS48_CLEAN_G(z, t, r, F);
            for (j = 0; j < N; j++) {
                y[j] = (gf31)(alpha * (31 + pk_gf31[j] - y[j]) - z[j] - e[j] + 62);
            }
            PQCLEAN_MQDSS48_CLEAN_vgf31_shorten_unique(y, y);
            PQCLEAN_MQDSS48_CLEAN_gf31_npack(packbuf0, y, M);
            com_1(c + HASH_BYTES*(2*i + 1), sigptr + HASH_BYTES + NPACKED_BYTES, sigptr, packbuf0);
        }
        memcpy(c + HASH_BYTES*(2*i + (1 - b)), sigptr + NPACKED_BYTES, HASH_BYTES);
        sigptr += NPACKED_BYTES + 2*HASH_BYTES;
    }

    H(c, c, HASH_BYTES * ROUNDS * 2);
    if (memcmp(c, sigma0, HASH_BYTES)) {
        memset(m, 0, smlen);
        *mlen = 0;
        return 1;
    }

    /* If verification was successful, move the message to the right place. */
    memmove(m, m + SIG_LEN, *mlen);

    return 0;
}
