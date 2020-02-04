/*
  This file is for Niederreiter decryption
*/

#include "decrypt.h"

#include "benes.h"
#include "bm.h"
#include "fft.h"
#include "fft_tr.h"
#include "params.h"
#include "util.h"

#include <immintrin.h>
#include <stdio.h>

static void scaling(vec128 out[][GFBITS], vec128 inv[][GFBITS], const unsigned char *sk, vec128 *recv) {
    int i, j;

    vec128 irr_int[ GFBITS ];
    vec128 eval[64][ GFBITS ];
    vec128 tmp[ GFBITS ];

    //

    PQCLEAN_MCELIECE6960119_SSE_irr_load(irr_int, sk);

    PQCLEAN_MCELIECE6960119_SSE_fft(eval, irr_int);

    for (i = 0; i < 64; i++) {
        PQCLEAN_MCELIECE6960119_SSE_vec128_sq(eval[i], eval[i]);
    }

    PQCLEAN_MCELIECE6960119_SSE_vec128_copy(inv[0], eval[0]);

    for (i = 1; i < 64; i++) {
        PQCLEAN_MCELIECE6960119_SSE_vec128_mul(inv[i], inv[i - 1], eval[i]);
    }

    PQCLEAN_MCELIECE6960119_SSE_vec128_inv(tmp, inv[63]);

    for (i = 62; i >= 0; i--) {
        PQCLEAN_MCELIECE6960119_SSE_vec128_mul(inv[i + 1], tmp, inv[i]);
        PQCLEAN_MCELIECE6960119_SSE_vec128_mul(tmp, tmp, eval[i + 1]);
    }

    PQCLEAN_MCELIECE6960119_SSE_vec128_copy(inv[0], tmp);

    //

    for (i = 0; i < 64; i++) {
        for (j = 0; j < GFBITS; j++) {
            out[i][j] = PQCLEAN_MCELIECE6960119_SSE_vec128_and(inv[i][j], recv[i]);
        }
    }
}

static void preprocess(vec128 *recv, const unsigned char *s) {
    int i;
    unsigned char r[ 1024 ];

    for (i = 0; i < SYND_BYTES; i++) {
        r[i] = s[i];
    }

    r[i - 1] &= (1 << ((GFBITS * SYS_T) % 8)) - 1; // throwing away redundant bits

    for (i = SYND_BYTES; i < 1024; i++) {
        r[i] = 0;
    }

    for (i = 0; i < 64; i++) {
        recv[i] = PQCLEAN_MCELIECE6960119_SSE_load16(r + i * 16);
    }
}

static void postprocess(unsigned char *e, vec128 *err) {
    int i;
    unsigned char error8[ (1 << GFBITS) / 8 ];
    uint64_t v[2];

    for (i = 0; i < 64; i++) {
        v[0] = PQCLEAN_MCELIECE6960119_SSE_vec128_extract(err[i], 0);
        v[1] = PQCLEAN_MCELIECE6960119_SSE_vec128_extract(err[i], 1);

        PQCLEAN_MCELIECE6960119_SSE_store8(error8 + i * 16 + 0, v[0]);
        PQCLEAN_MCELIECE6960119_SSE_store8(error8 + i * 16 + 8, v[1]);
    }

    for (i = 0; i < SYS_N / 8; i++) {
        e[i] = error8[i];
    }
}

static void scaling_inv(vec128 out[][GFBITS], vec128 inv[][GFBITS], vec128 *recv) {
    int i, j;

    for (i = 0; i < 64; i++) {
        for (j = 0; j < GFBITS; j++) {
            out[i][j] = PQCLEAN_MCELIECE6960119_SSE_vec128_and(inv[i][j], recv[i]);
        }
    }
}

static uint16_t weight_check(unsigned char *e, vec128 *error) {
    int i;
    uint16_t w0 = 0;
    uint16_t w1 = 0;
    uint16_t check;

    for (i = 0; i < 64; i++) {
        w0 += _mm_popcnt_u64(PQCLEAN_MCELIECE6960119_SSE_vec128_extract(error[i], 0) );
        w0 += _mm_popcnt_u64( PQCLEAN_MCELIECE6960119_SSE_vec128_extract(error[i], 1) );
    }

    for (i = 0; i < SYS_N / 8; i++) {
        w1 += _mm_popcnt_u32( e[i] );
    }

    check = (w0 ^ SYS_T) | (w1 ^ SYS_T);
    check -= 1;
    check >>= 15;

    return check;
}

static uint16_t synd_cmp(vec128 s0[][ GFBITS ], vec128 s1[][ GFBITS ]) {
    int i, j;
    vec128 diff;

    diff = PQCLEAN_MCELIECE6960119_SSE_vec128_or(PQCLEAN_MCELIECE6960119_SSE_vec128_xor(s0[0][0], s1[0][0]),
            PQCLEAN_MCELIECE6960119_SSE_vec128_xor(s0[1][0], s1[1][0]));

    for (i = 0; i < 2; i++) {
        for (j = 1; j < GFBITS; j++) {
            diff = PQCLEAN_MCELIECE6960119_SSE_vec128_or(diff, PQCLEAN_MCELIECE6960119_SSE_vec128_xor(s0[i][j], s1[i][j]));
        }
    }

    return (uint16_t)PQCLEAN_MCELIECE6960119_SSE_vec128_testz(diff);
}

/* Niederreiter decryption with the Berlekamp decoder */
/* intput: sk, secret key */
/*         c, ciphertext (syndrome) */
/* output: e, error vector */
/* return: 0 for success; 1 for failure */
int PQCLEAN_MCELIECE6960119_SSE_decrypt(unsigned char *e, const unsigned char *sk, const unsigned char *c) {
    int i;

    uint16_t check_synd;
    uint16_t check_weight;

    vec128 inv[ 64 ][ GFBITS ];
    vec128 scaled[ 64 ][ GFBITS ];
    vec128 eval[ 64 ][ GFBITS ];

    vec128 error[ 64 ];

    vec128 s_priv[ 2 ][ GFBITS ];
    vec128 s_priv_cmp[ 2 ][ GFBITS ];

    vec128 locator[ GFBITS ];

    vec128 recv[ 64 ];
    vec128 allone;

    vec128 bits_int[25][32];

    // Berlekamp decoder

    preprocess(recv, c);

    PQCLEAN_MCELIECE6960119_SSE_load_bits(bits_int, sk + IRR_BYTES);
    PQCLEAN_MCELIECE6960119_SSE_benes(recv, bits_int, 1);

    scaling(scaled, inv, sk, recv);
    PQCLEAN_MCELIECE6960119_SSE_fft_tr(s_priv, scaled);
    PQCLEAN_MCELIECE6960119_SSE_bm(locator, s_priv);

    PQCLEAN_MCELIECE6960119_SSE_fft(eval, locator);

    // reencryption and weight check

    allone = PQCLEAN_MCELIECE6960119_SSE_vec128_setbits(1);

    for (i = 0; i < 64; i++) {
        error[i] = PQCLEAN_MCELIECE6960119_SSE_vec128_or_reduce(eval[i]);
        error[i] = PQCLEAN_MCELIECE6960119_SSE_vec128_xor(error[i], allone);
    }

    scaling_inv(scaled, inv, error);
    PQCLEAN_MCELIECE6960119_SSE_fft_tr(s_priv_cmp, scaled);

    check_synd = synd_cmp(s_priv, s_priv_cmp);

    //

    PQCLEAN_MCELIECE6960119_SSE_benes(error, bits_int, 0);

    postprocess(e, error);

    check_weight = weight_check(e, error);

    return 1 - (check_synd & check_weight);
}

