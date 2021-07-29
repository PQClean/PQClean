#include "crypto_encode_953x2115.h"
#include <immintrin.h>
/* auto-generated; do not edit */

#define int16 int16_t
#define uint16 uint16_t
#define uint32 uint32_t

void PQCLEAN_NTRULPR953_AVX2_crypto_encode_953x2115(unsigned char *out, const void *v) {
    const int16 *R0 = v;
    /* XXX: caller could overlap R with input */
    uint16 R[477];
    long i;
    const uint16 *reading;
    uint16 *writing;
    uint16 r0, r1;
    uint32 r2;
    uint32 s0;

    reading = (uint16 *) R0;
    writing = R;
    i = 30;
    while (i > 0) {
        __m256i x, x2, y, y2;
        --i;
        if (!i) {
            reading -= 8;
            writing -= 4;
            out -= 8;
        }
        x = _mm256_loadu_si256((__m256i *) (reading + 0));
        x2 = _mm256_loadu_si256((__m256i *) (reading + 16));
        x = _mm256_add_epi16(x, _mm256_set1_epi16(3171));
        x2 = _mm256_add_epi16(x2, _mm256_set1_epi16(3171));
        x &= _mm256_set1_epi16(16383);
        x2 &= _mm256_set1_epi16(16383);
        x = _mm256_mulhi_epi16(x, _mm256_set1_epi16(21846));
        x2 = _mm256_mulhi_epi16(x2, _mm256_set1_epi16(21846));
        y = x & _mm256_set1_epi32(65535);
        y2 = x2 & _mm256_set1_epi32(65535);
        x = _mm256_srli_epi32(x, 16);
        x2 = _mm256_srli_epi32(x2, 16);
        x = _mm256_mullo_epi32(x, _mm256_set1_epi32(2115));
        x2 = _mm256_mullo_epi32(x2, _mm256_set1_epi32(2115));
        x = _mm256_add_epi32(y, x);
        x2 = _mm256_add_epi32(y2, x2);
        x = _mm256_shuffle_epi8(x, _mm256_set_epi8(
                                    15, 14, 11, 10, 7, 6, 3, 2, 13, 12, 9, 8, 5, 4, 1, 0,
                                    15, 14, 11, 10, 7, 6, 3, 2, 13, 12, 9, 8, 5, 4, 1, 0
                                ));
        x2 = _mm256_shuffle_epi8(x2, _mm256_set_epi8(
                                     15, 14, 11, 10, 7, 6, 3, 2, 13, 12, 9, 8, 5, 4, 1, 0,
                                     15, 14, 11, 10, 7, 6, 3, 2, 13, 12, 9, 8, 5, 4, 1, 0
                                 ));
        x = _mm256_permute4x64_epi64(x, 0xd8);
        x2 = _mm256_permute4x64_epi64(x2, 0xd8);
        _mm256_storeu_si256((__m256i *) writing, _mm256_permute2f128_si256(x, x2, 0x31));
        _mm256_storeu_si256((__m256i *) out, _mm256_permute2f128_si256(x, x2, 0x20));
        reading += 32;
        writing += 16;
        out += 32;
    }
    R[476] = (uint16) ((((R0[952] + 3171) & 16383) * 10923) >> 15);

    for (i = 0; i < 238; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)69;
        R[i] = (uint16) r2;
    }
    R[238] = R[476];

    reading = (uint16 *) R;
    writing = R;
    i = 8;
    while (i > 0) {
        __m256i x, x2, y, y2;
        --i;
        if (!i) {
            reading -= 18;
            writing -= 9;
            out -= 18;
        }
        x = _mm256_loadu_si256((__m256i *) (reading + 0));
        x2 = _mm256_loadu_si256((__m256i *) (reading + 16));
        y = x & _mm256_set1_epi32(65535);
        y2 = x2 & _mm256_set1_epi32(65535);
        x = _mm256_srli_epi32(x, 16);
        x2 = _mm256_srli_epi32(x2, 16);
        x = _mm256_mullo_epi32(x, _mm256_set1_epi32(4761));
        x2 = _mm256_mullo_epi32(x2, _mm256_set1_epi32(4761));
        x = _mm256_add_epi32(y, x);
        x2 = _mm256_add_epi32(y2, x2);
        x = _mm256_shuffle_epi8(x, _mm256_set_epi8(
                                    15, 14, 11, 10, 7, 6, 3, 2, 13, 12, 9, 8, 5, 4, 1, 0,
                                    15, 14, 11, 10, 7, 6, 3, 2, 13, 12, 9, 8, 5, 4, 1, 0
                                ));
        x2 = _mm256_shuffle_epi8(x2, _mm256_set_epi8(
                                     15, 14, 11, 10, 7, 6, 3, 2, 13, 12, 9, 8, 5, 4, 1, 0,
                                     15, 14, 11, 10, 7, 6, 3, 2, 13, 12, 9, 8, 5, 4, 1, 0
                                 ));
        x = _mm256_permute4x64_epi64(x, 0xd8);
        x2 = _mm256_permute4x64_epi64(x2, 0xd8);
        _mm256_storeu_si256((__m256i *) writing, _mm256_permute2f128_si256(x, x2, 0x31));
        _mm256_storeu_si256((__m256i *) out, _mm256_permute2f128_si256(x, x2, 0x20));
        reading += 32;
        writing += 16;
        out += 32;
    }
    R[119] = R[238];

    reading = (uint16 *) R;
    writing = R;
    i = 8;
    while (i > 0) {
        __m256i x, y;
        --i;
        if (!i) {
            reading -= 8;
            writing -= 4;
            out -= 4;
        }
        x = _mm256_loadu_si256((__m256i *) reading);
        y = x & _mm256_set1_epi32(65535);
        x = _mm256_srli_epi32(x, 16);
        x = _mm256_mullo_epi32(x, _mm256_set1_epi32(346));
        x = _mm256_add_epi32(y, x);
        x = _mm256_shuffle_epi8(x, _mm256_set_epi8(
                                    12, 8, 4, 0, 12, 8, 4, 0, 14, 13, 10, 9, 6, 5, 2, 1,
                                    12, 8, 4, 0, 12, 8, 4, 0, 14, 13, 10, 9, 6, 5, 2, 1
                                ));
        x = _mm256_permute4x64_epi64(x, 0xd8);
        _mm_storeu_si128((__m128i *) writing, _mm256_extractf128_si256(x, 0));
        s0 = (uint32) _mm256_extract_epi32(x, 4);
        *out++ = (unsigned char) s0;
        s0 >>= 8;
        *out++ = (unsigned char) s0;
        s0 >>= 8;
        *out++ = (unsigned char) s0;
        s0 >>= 8;
        *out++ = (unsigned char) s0;
        s0 = (uint32) _mm256_extract_epi32(x, 6);
        *out++ = (unsigned char) s0;
        s0 >>= 8;
        *out++ = (unsigned char) s0;
        s0 >>= 8;
        *out++ = (unsigned char) s0;
        s0 >>= 8;
        *out++ = (unsigned char) s0;
        reading += 16;
        writing += 8;
    }

    reading = (uint16 *) R;
    writing = R;
    i = 4;
    while (i > 0) {
        __m256i x, y;
        --i;
        if (!i) {
            reading -= 4;
            writing -= 2;
            out -= 2;
        }
        x = _mm256_loadu_si256((__m256i *) reading);
        y = x & _mm256_set1_epi32(65535);
        x = _mm256_srli_epi32(x, 16);
        x = _mm256_mullo_epi32(x, _mm256_set1_epi32(468));
        x = _mm256_add_epi32(y, x);
        x = _mm256_shuffle_epi8(x, _mm256_set_epi8(
                                    12, 8, 4, 0, 12, 8, 4, 0, 14, 13, 10, 9, 6, 5, 2, 1,
                                    12, 8, 4, 0, 12, 8, 4, 0, 14, 13, 10, 9, 6, 5, 2, 1
                                ));
        x = _mm256_permute4x64_epi64(x, 0xd8);
        _mm_storeu_si128((__m128i *) writing, _mm256_extractf128_si256(x, 0));
        s0 = (uint32) _mm256_extract_epi32(x, 4);
        *out++ = (unsigned char) s0;
        s0 >>= 8;
        *out++ = (unsigned char) s0;
        s0 >>= 8;
        *out++ = (unsigned char) s0;
        s0 >>= 8;
        *out++ = (unsigned char) s0;
        s0 = (uint32) _mm256_extract_epi32(x, 6);
        *out++ = (unsigned char) s0;
        s0 >>= 8;
        *out++ = (unsigned char) s0;
        s0 >>= 8;
        *out++ = (unsigned char) s0;
        s0 >>= 8;
        *out++ = (unsigned char) s0;
        reading += 16;
        writing += 8;
    }

    reading = (uint16 *) R;
    writing = R;
    i = 2;
    while (i > 0) {
        __m256i x, y;
        --i;
        if (!i) {
            reading -= 4;
            writing -= 2;
            out -= 2;
        }
        x = _mm256_loadu_si256((__m256i *) reading);
        y = x & _mm256_set1_epi32(65535);
        x = _mm256_srli_epi32(x, 16);
        x = _mm256_mullo_epi32(x, _mm256_set1_epi32(856));
        x = _mm256_add_epi32(y, x);
        x = _mm256_shuffle_epi8(x, _mm256_set_epi8(
                                    12, 8, 4, 0, 12, 8, 4, 0, 14, 13, 10, 9, 6, 5, 2, 1,
                                    12, 8, 4, 0, 12, 8, 4, 0, 14, 13, 10, 9, 6, 5, 2, 1
                                ));
        x = _mm256_permute4x64_epi64(x, 0xd8);
        _mm_storeu_si128((__m128i *) writing, _mm256_extractf128_si256(x, 0));
        s0 = (uint32) _mm256_extract_epi32(x, 4);
        *out++ = (unsigned char) s0;
        s0 >>= 8;
        *out++ = (unsigned char) s0;
        s0 >>= 8;
        *out++ = (unsigned char) s0;
        s0 >>= 8;
        *out++ = (unsigned char) s0;
        s0 = (uint32) _mm256_extract_epi32(x, 6);
        *out++ = (unsigned char) s0;
        s0 >>= 8;
        *out++ = (unsigned char) s0;
        s0 >>= 8;
        *out++ = (unsigned char) s0;
        s0 >>= 8;
        *out++ = (unsigned char) s0;
        reading += 16;
        writing += 8;
    }
    r0 = R[28];
    r1 = R[29];
    r2 = r0 + r1 * (uint32)856;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    R[14] = (uint16) r2;

    for (i = 0; i < 7; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)2863;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    R[7] = R[14];

    for (i = 0; i < 4; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)126;
        R[i] = (uint16) r2;
    }

    for (i = 0; i < 2; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)15876;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }

    r0 = R[0];
    r1 = R[1];
    r2 = r0 + r1 * (uint32)3846;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    R[0] = (uint16) r2;

    r0 = R[0];
    *out++ = (unsigned char) r0;
}
