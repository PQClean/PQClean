#include "poly.h"
#include "verify.h"

static uint16_t mod3(uint16_t a) {
    uint16_t r;
    int16_t t, c;

    r = (a >> 8) + (a & 0xff); // r mod 255 == a mod 255
    r = (r >> 4) + (r & 0xf); // r' mod 15 == r mod 15
    r = (r >> 2) + (r & 0x3); // r' mod 3 == r mod 3
    r = (r >> 2) + (r & 0x3); // r' mod 3 == r mod 3

    t = r - 3;
    c = t >> 15;

    return (c & r) ^ (~c & t);
}

#define POLY_S3_FMADD(I,A,B,S) \
    for ((I)=0; (I)<NTRU_N; (I)++) { \
        (A).coeffs[(I)] = mod3((A).coeffs[(I)] + (S) * (B).coeffs[(I)]); \
    }

static void cswappoly(poly *a, poly *b, int swap) {
    int i;
    uint16_t t;
    swap = -swap;
    for (i = 0; i < NTRU_N; i++) {
        t = (a->coeffs[i] ^ b->coeffs[i]) & swap;
        a->coeffs[i] ^= t;
        b->coeffs[i] ^= t;
    }
}

static inline void poly_divx(poly *a, int s) {
    int i;

    for (i = 1; i < NTRU_N; i++) {
        a->coeffs[i - 1] = (unsigned char) ((s * a->coeffs[i]) | (!s * a->coeffs[i - 1]));
    }
    a->coeffs[NTRU_N - 1] = (!s * a->coeffs[NTRU_N - 1]);
}

static inline void poly_mulx(poly *a, int s) {
    int i;

    for (i = 1; i < NTRU_N; i++) {
        a->coeffs[NTRU_N - i] = (unsigned char) ((s * a->coeffs[NTRU_N - i - 1]) | (!s * a->coeffs[NTRU_N - i]));
    }
    a->coeffs[0] = (!s * a->coeffs[0]);
}

void PQCLEAN_NTRUHPS4096821_CLEAN_poly_S3_inv(poly *r, const poly *a) {
    /* Schroeppel--Orman--O'Malley--Spatscheck
     * "Almost Inverse" algorithm as described
     * by Silverman in NTRU Tech Report #14 */
    // with several modifications to make it run in constant-time
    int i, j;
    uint16_t k = 0;
    uint16_t degf = NTRU_N - 1;
    uint16_t degg = NTRU_N - 1;
    int sign, fsign = 0, t, swap;
    int16_t done = 0;
    poly b, c, f, g;
    poly *temp_r = &f;

    /* b(X) := 1 */
    for (i = 1; i < NTRU_N; i++) {
        b.coeffs[i] = 0;
    }
    b.coeffs[0] = 1;

    /* c(X) := 0 */
    for (i = 0; i < NTRU_N; i++) {
        c.coeffs[i] = 0;
    }

    /* f(X) := a(X) */
    for (i = 0; i < NTRU_N; i++) {
        f.coeffs[i] = a->coeffs[i];
    }

    /* g(X) := 1 + X + X^2 + ... + X^{N-1} */
    for (i = 0; i < NTRU_N; i++) {
        g.coeffs[i] = 1;
    }

    for (j = 0; j < 2 * (NTRU_N - 1) - 1; j++) {
        sign = mod3(2 * g.coeffs[0] * f.coeffs[0]);
        swap = (((sign & 2) >> 1) | sign) & !done & ((degf - degg) >> 15);

        cswappoly(&f, &g, swap);
        cswappoly(&b, &c, swap);
        t = (degf ^ degg) & (-swap);
        degf ^= t;
        degg ^= t;

        for (i = 0; i < NTRU_N; i++) {
            f.coeffs[i] = mod3(f.coeffs[i] + ((uint16_t) (sign * (!done))) * g.coeffs[i]);
        }
        for (i = 0; i < NTRU_N; i++) {
            b.coeffs[i] = mod3(b.coeffs[i] + ((uint16_t) (sign * (!done))) * c.coeffs[i]);
        }

        poly_divx(&f, !done);
        poly_mulx(&c, !done);
        degf -= !done;
        k += !done;

        done = 1 - (((uint16_t) - degf) >> 15);
    }

    fsign = f.coeffs[0];
    k = k - NTRU_N * ((uint16_t)(NTRU_N - k - 1) >> 15);

    /* Return X^{N-k} * b(X) */
    /* This is a k-coefficient rotation. We do this by looking at the binary
       representation of k, rotating for every power of 2, and performing a cmov
       if the respective bit is set. */
    for (i = 0; i < NTRU_N; i++) {
        r->coeffs[i] = mod3((uint16_t) fsign * b.coeffs[i]);
    }

    for (i = 0; i < 10; i++) {
        for (j = 0; j < NTRU_N; j++) {
            temp_r->coeffs[j] = r->coeffs[(j + (1 << i)) % NTRU_N];
        }
        PQCLEAN_NTRUHPS4096821_CLEAN_cmov((unsigned char *) & (r->coeffs),
                                          (unsigned char *) & (temp_r->coeffs), sizeof(uint16_t) * NTRU_N, k & 1);
        k >>= 1;
    }

    /* Reduce modulo Phi_n */
    for (i = 0; i < NTRU_N; i++) {
        r->coeffs[i] = mod3(r->coeffs[i] + 2 * r->coeffs[NTRU_N - 1]);
    }
}
