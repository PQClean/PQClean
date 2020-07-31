#ifndef PQCLEAN_KYBER1024_AVX2_ALIGN_H
#define PQCLEAN_KYBER1024_AVX2_ALIGN_H
#include <immintrin.h>

#define ALIGN16_TYPE(t) \
    union {                       \
        __m128i vec;              \
        t  orig;     \
    }

#define ALIGN32_ARRAY(t, s) \
    union {                       \
        __m256i vec;              \
        t  arr[(s)];     \
    }

#define ALIGN32_ARRAY_2D(t, n, m) \
    union {                       \
        __m256i vec;              \
        t  arr[(n)][(m)];         \
    }
#endif
