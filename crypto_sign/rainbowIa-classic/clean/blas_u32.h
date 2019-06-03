#ifndef _BLAS_U32_H_
#define _BLAS_U32_H_
/// @file blas_u32.h
/// @brief Inlined functions for implementing basic linear algebra functions for uint32 arch.
///


#include "gf16.h"

#include <stdint.h>

static inline void _gf256v_add_u32(uint8_t *accu_b, const uint8_t *a, unsigned _num_byte) {
    unsigned n_u32 = _num_byte >> 2;
    uint32_t *b_u32 = (uint32_t *) accu_b;
    const uint32_t *a_u32 = (const uint32_t *) a;
    for (unsigned i = 0; i < n_u32; i++) {
        b_u32[i] ^= a_u32[i];
    }

    a += (n_u32 << 2);
    accu_b += (n_u32 << 2);
    unsigned rem = _num_byte & 3;
    for (unsigned i = 0; i < rem; i++) {
        accu_b[i] ^= a[i];
    }
}

static inline void _gf256v_predicated_add_u32(uint8_t *accu_b, uint8_t predicate, const uint8_t *a, unsigned _num_byte) {
    uint32_t pr_u32 = ((uint32_t) 0) - ((uint32_t) predicate);
    uint8_t pr_u8 = pr_u32 & 0xff;

    unsigned n_u32 = _num_byte >> 2;
    uint32_t *b_u32 = (uint32_t *) accu_b;
    const uint32_t *a_u32 = (const uint32_t *) a;
    for (unsigned i = 0; i < n_u32; i++) {
        b_u32[i] ^= (a_u32[i] & pr_u32);
    }

    a += (n_u32 << 2);
    accu_b += (n_u32 << 2);
    unsigned rem = _num_byte & 3;
    for (unsigned i = 0; i < rem; i++) {
        accu_b[i] ^= (a[i] & pr_u8);
    }
}


static inline void _gf16v_mul_scalar_u32(uint8_t *a, uint8_t gf16_b, unsigned _num_byte) {
    unsigned n_u32 = _num_byte >> 2;
    uint32_t *a_u32 = (uint32_t *) a;
    for (unsigned i = 0; i < n_u32; i++) {
        a_u32[i] = gf16v_mul_u32(a_u32[i], gf16_b);
    }

    union tmp_32 {
        uint8_t u8[4];
        uint32_t u32;
    } t;
    a += (n_u32 << 2);
    unsigned rem = _num_byte & 3;
    for (unsigned i = 0; i < rem; i++) {
        t.u8[i] = a[i];
    }
    t.u32 = gf16v_mul_u32(t.u32, gf16_b);
    for (unsigned i = 0; i < rem; i++) {
        a[i] = t.u8[i];
    }
}

static inline void _gf256v_mul_scalar_u32(uint8_t *a, uint8_t b, unsigned _num_byte) {
    unsigned n_u32 = _num_byte >> 2;
    uint32_t *a_u32 = (uint32_t *) a;
    for (unsigned i = 0; i < n_u32; i++) {
        a_u32[i] = gf256v_mul_u32(a_u32[i], b);
    }

    union tmp_32 {
        uint8_t u8[4];
        uint32_t u32;
    } t;
    a += (n_u32 << 2);
    unsigned rem = _num_byte & 3;
    for (unsigned i = 0; i < rem; i++) {
        t.u8[i] = a[i];
    }
    t.u32 = gf256v_mul_u32(t.u32, b);
    for (unsigned i = 0; i < rem; i++) {
        a[i] = t.u8[i];
    }
}


static inline void _gf16v_madd_u32(uint8_t *accu_c, const uint8_t *a, uint8_t gf16_b, unsigned _num_byte) {
    unsigned n_u32 = _num_byte >> 2;
    uint32_t *c_u32 = (uint32_t *) accu_c;
    const uint32_t *a_u32 = (const uint32_t *) a;
    for (unsigned i = 0; i < n_u32; i++) {
        c_u32[i] ^= gf16v_mul_u32(a_u32[i], gf16_b);
    }

    // TODO: this will certainly not work on Big Endian
    union tmp_32 {
        uint8_t u8[4];
        uint32_t u32;
    } t;
    t.u32 = 0;
    accu_c += (n_u32 << 2);
    a += (n_u32 << 2);
    unsigned rem = _num_byte & 3;
    for (unsigned i = 0; i < rem; i++) {
        t.u8[i] = a[i];
    }
    t.u32 = gf16v_mul_u32(t.u32, gf16_b);
    for (unsigned i = 0; i < rem; i++) {
        accu_c[i] ^= t.u8[i];
    }
}

static inline void _gf256v_madd_u32(uint8_t *accu_c, const uint8_t *a, uint8_t gf256_b, unsigned _num_byte) {
    unsigned n_u32 = _num_byte >> 2;
    uint32_t *c_u32 = (uint32_t *) accu_c;
    const uint32_t *a_u32 = (const uint32_t *) a;
    for (unsigned i = 0; i < n_u32; i++) {
        c_u32[i] ^= gf256v_mul_u32(a_u32[i], gf256_b);
    }

    // TODO: this will certainly not work on Big Endian
    union tmp_32 {
        uint8_t u8[4];
        uint32_t u32;
    } t;
    t.u32 = 0;
    accu_c += (n_u32 << 2);
    a += (n_u32 << 2);
    unsigned rem = _num_byte & 3;
    for (unsigned i = 0; i < rem; i++) {
        t.u8[i] = a[i];
    }
    t.u32 = gf256v_mul_u32(t.u32, gf256_b);
    for (unsigned i = 0; i < rem; i++) {
        accu_c[i] ^= t.u8[i];
    }
}


static inline uint8_t _gf16v_dot_u32(const uint8_t *a, const uint8_t *b, unsigned _num_byte) {
    unsigned n_u32 = _num_byte >> 2;
    const uint32_t *a_u32 = (const uint32_t *) a;
    const uint32_t *b_u32 = (const uint32_t *) b;
    uint32_t r = 0;
    for (unsigned i = 0; i < n_u32; i++) {
        r ^= gf16v_mul_u32_u32(a_u32[i], b_u32[i]);
    }

    unsigned rem = _num_byte & 3;
    if (rem) {
        union tmp_32 {
            uint8_t u8[4];
            uint32_t u32;
        } ta, tb;
        ta.u32 = 0;
        tb.u32 = 0;
        for (unsigned i = 0; i < rem; i++) {
            ta.u8[i] = a[(n_u32 << 2) + i];
        }
        for (unsigned i = 0; i < rem; i++) {
            tb.u8[i] = b[(n_u32 << 2) + i];
        }
        r ^= gf16v_mul_u32_u32(ta.u32, tb.u32);
    }
    return gf16v_reduce_u32(r);
}

#endif // _BLAS_U32_H_

