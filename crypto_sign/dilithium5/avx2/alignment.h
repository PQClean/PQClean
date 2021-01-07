#ifndef PQCLEAN_DILITHIUM5_AVX2_ALIGNMENT_H
#define PQCLEAN_DILITHIUM5_AVX2_ALIGNMENT_H

#include <immintrin.h>
#include <stdint.h>

#define ALIGNED_UINT8(N)       \
    union {                    \
        uint8_t as_arr[N];     \
        __m256i as_vec[(N)/32];\
    }

#define ALIGNED_UINT32(N)      \
    union {                    \
        uint32_t as_arr[N];    \
        __m256i as_vec[(N)/8]; \
    }

#define ALIGNED_UINT64(N)      \
    union {                    \
        uint64_t as_arr[N];    \
        __m256i as_vec[(N)/8]; \
    }

#define ALIGNED_INT8(N)        \
    union {                    \
        int8_t as_arr[N];      \
        __m256i as_vec[(N)/32];\
    }

#define ALIGNED_INT32(N)       \
    union {                    \
        int32_t as_arr[N];     \
        __m256i as_vec[(N)/8]; \
    }

#define ALIGNED_INT64(N)       \
    union {                    \
        int64_t as_arr[N];     \
        __m256i as_vec[(N)/8]; \
    }

#endif //PQCLEAN_DILITHIUM5_AVX2_ALIGNMENT_H
