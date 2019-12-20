/*
  This file is for public-key generation
*/

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "controlbits.h"
#include "benes.h"
#include "params.h"
#include "pk_gen.h"
#include "root.h"
#include "util.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))

static void transpose_64x64(uint64_t *out, const uint64_t *in) {
    int i, j, s, d;

    uint64_t x, y;
    uint64_t masks[6][2] = {
        {0x5555555555555555, 0xAAAAAAAAAAAAAAAA},
        {0x3333333333333333, 0xCCCCCCCCCCCCCCCC},
        {0x0F0F0F0F0F0F0F0F, 0xF0F0F0F0F0F0F0F0},
        {0x00FF00FF00FF00FF, 0xFF00FF00FF00FF00},
        {0x0000FFFF0000FFFF, 0xFFFF0000FFFF0000},
        {0x00000000FFFFFFFF, 0xFFFFFFFF00000000}
    };

    for (i = 0; i < 64; i++) {
        out[i] = in[i];
    }

    for (d = 5; d >= 0; d--) {
        s = 1 << d;

        for (i = 0; i < 64; i += s * 2) {
            for (j = i; j < i + s; j++) {
                x = (out[j] & masks[d][0]) | ((out[j + s] & masks[d][0]) << s);
                y = ((out[j] & masks[d][1]) >> s) | (out[j + s] & masks[d][1]);

                out[j + 0] = x;
                out[j + s] = y;
            }
        }
    }
}

/* return number of trailing zeros of the non-zero input in */
static inline int ctz(uint64_t in) {
    int i, b, m = 0, r = 0;

    for (i = 0; i < 64; i++) {
        b = (int)(in >> i) & 1;
        m |= b;
        r += (m ^ 1) & (b ^ 1);
    }

    return r;
}

static inline uint64_t same_mask(uint16_t x, uint16_t y) {
    uint64_t mask;

    mask = x ^ y;
    mask -= 1;
    mask >>= 63;
    mask = -mask;

    return mask;
}

static int mov_columns(uint8_t mat[][ SYS_N / 8 ], uint32_t *perm) {
    int i, j, k, s, block_idx, row;
    uint64_t buf[64], ctz_list[32], t, d, mask;

    row = GFBITS * SYS_T - 32;
    block_idx = row / 8;

    // extract the 32x64 matrix

    for (i = 0; i < 32; i++) {
        buf[i] = PQCLEAN_MCELIECE348864F_CLEAN_load8( &mat[ row + i ][ block_idx ] );
    }

    // compute the column indices of pivots by Gaussian elimination.
    // the indices are stored in ctz_list

    for (i = 0; i < 32; i++) {
        t = buf[i];
        for (j = i + 1; j < 32; j++) {
            t |= buf[j];
        }

        if (t == 0) {
            return -1;    // return if buf is not full rank
        }

        ctz_list[i] = s = ctz(t);

        for (j = i + 1; j < 32; j++) {
            mask = (buf[i] >> s) & 1;
            mask -= 1;
            buf[i] ^= buf[j] & mask;
        }
        for (j =   0; j <  i; j++) {
            mask = (buf[j] >> s) & 1;
            mask = -mask;
            buf[j] ^= buf[i] & mask;
        }
        for (j = i + 1; j < 32; j++) {
            mask = (buf[j] >> s) & 1;
            mask = -mask;
            buf[j] ^= buf[i] & mask;
        }
    }

    // updating permutation

    for (j = 0;   j < 32; j++) {
        for (k = j + 1; k < 64; k++) {
            d = perm[ row + j ] ^ perm[ row + k ];
            d &= same_mask((uint16_t)k, (uint16_t)ctz_list[j]);
            perm[ row + j ] ^= d;
            perm[ row + k ] ^= d;
        }
    }

    // moving columns of mat according to the column indices of pivots

    for (i = 0; i < GFBITS * SYS_T; i += 64) {
        for (j = 0; j < min(64, GFBITS * SYS_T - i); j++) {
            buf[j] = PQCLEAN_MCELIECE348864F_CLEAN_load8( &mat[ i + j ][ block_idx ] );
        }

        transpose_64x64(buf, buf);

        for (j = 0; j < 32; j++) {
            for (k = j + 1; k < 64; k++) {
                d = buf[ j ] ^ buf[ k ];
                d &= same_mask((uint16_t)k, (uint16_t)ctz_list[j]);
                buf[ j ] ^= d;
                buf[ k ] ^= d;
            }
        }

        transpose_64x64(buf, buf);

        for (j = 0; j < min(64, GFBITS * SYS_T - i); j++) {
            PQCLEAN_MCELIECE348864F_CLEAN_store8( &mat[ i + j ][ block_idx ], buf[j] );
        }
    }

    return 0;
}

/* input: secret key sk */
/* output: public key pk */
int PQCLEAN_MCELIECE348864F_CLEAN_pk_gen(uint8_t *pk,  uint32_t *perm, const uint8_t *sk) {
    int i, j, k;
    int row, c;

    uint64_t buf[ 1 << GFBITS ];

    unsigned char mat[ GFBITS * SYS_T ][ SYS_N / 8 ];
    unsigned char mask;
    unsigned char b;

    gf g[ SYS_T + 1 ]; // Goppa polynomial
    gf L[ SYS_N ]; // support
    gf inv[ SYS_N ];

    //

    g[ SYS_T ] = 1;

    for (i = 0; i < SYS_T; i++) {
        g[i] = PQCLEAN_MCELIECE348864F_CLEAN_load2(sk);
        g[i] &= GFMASK;
        sk += 2;
    }

    for (i = 0; i < (1 << GFBITS); i++) {
        buf[i] = perm[i];
        buf[i] <<= 31;
        buf[i] |= i;
    }

    PQCLEAN_MCELIECE348864F_CLEAN_sort_63b(1 << GFBITS, buf);

    for (i = 0; i < (1 << GFBITS); i++) {
        perm[i] = buf[i] & GFMASK;
    }
    for (i = 0; i < SYS_N;         i++) {
        L[i] = PQCLEAN_MCELIECE348864F_CLEAN_bitrev((gf)perm[i]);
    }

    // filling the matrix

    PQCLEAN_MCELIECE348864F_CLEAN_root(inv, g, L);

    for (i = 0; i < SYS_N; i++) {
        inv[i] = PQCLEAN_MCELIECE348864F_CLEAN_gf_inv(inv[i]);
    }

    for (i = 0; i < PK_NROWS; i++) {
        for (j = 0; j < SYS_N / 8; j++) {
            mat[i][j] = 0;
        }
    }

    for (i = 0; i < SYS_T; i++) {
        for (j = 0; j < SYS_N; j += 8) {
            for (k = 0; k < GFBITS;  k++) {
                b  = (inv[j + 7] >> k) & 1;
                b <<= 1;
                b |= (inv[j + 6] >> k) & 1;
                b <<= 1;
                b |= (inv[j + 5] >> k) & 1;
                b <<= 1;
                b |= (inv[j + 4] >> k) & 1;
                b <<= 1;
                b |= (inv[j + 3] >> k) & 1;
                b <<= 1;
                b |= (inv[j + 2] >> k) & 1;
                b <<= 1;
                b |= (inv[j + 1] >> k) & 1;
                b <<= 1;
                b |= (inv[j + 0] >> k) & 1;

                mat[ i * GFBITS + k ][ j / 8 ] = b;
            }
        }

        for (j = 0; j < SYS_N; j++) {
            inv[j] = PQCLEAN_MCELIECE348864F_CLEAN_gf_mul(inv[j], L[j]);
        }

    }

    // gaussian elimination

    for (i = 0; i < (GFBITS * SYS_T + 7) / 8; i++) {
        for (j = 0; j < 8; j++) {
            row = i * 8 + j;

            if (row >= GFBITS * SYS_T) {
                break;
            }

            if (row == GFBITS * SYS_T - 32) {
                if (mov_columns(mat, perm)) {
                    return -1;
                }
            }

            for (k = row + 1; k < GFBITS * SYS_T; k++) {
                mask = mat[ row ][ i ] ^ mat[ k ][ i ];
                mask >>= j;
                mask &= 1;
                mask = -mask;

                for (c = 0; c < SYS_N / 8; c++) {
                    mat[ row ][ c ] ^= mat[ k ][ c ] & mask;
                }
            }

            if ( ((mat[ row ][ i ] >> j) & 1) == 0 ) { // return if not systematic
                return -1;
            }

            for (k = 0; k < GFBITS * SYS_T; k++) {
                if (k != row) {
                    mask = mat[ k ][ i ] >> j;
                    mask &= 1;
                    mask = -mask;

                    for (c = 0; c < SYS_N / 8; c++) {
                        mat[ k ][ c ] ^= mat[ row ][ c ] & mask;
                    }
                }
            }
        }
    }

    for (i = 0; i < PK_NROWS; i++) {
        memcpy(pk + i * PK_ROW_BYTES, mat[i] + PK_NROWS / 8, PK_ROW_BYTES);
    }

    return 0;
}

