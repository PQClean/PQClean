#include "params.h"

#include <stdint.h>

extern void PQCLEAN_MCELIECE348864F_SSE_vec_mul_asm(uint64_t *, const uint64_t *, const uint64_t *, int);

static inline void vec_mul(uint64_t *h, const uint64_t *f, const uint64_t *g) {
    PQCLEAN_MCELIECE348864F_SSE_vec_mul_asm(h, f, g, 8);
}

static inline void vec_add(uint64_t *h, const uint64_t *f, const uint64_t *g) {
    int b;

    for (b = 0; b < GFBITS; b++) {
        h[b] = f[b] ^ g[b];
    }
}

