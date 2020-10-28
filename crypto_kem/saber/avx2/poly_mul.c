#include "SABER_params.h"
#include "poly.h"


#define L (SABER_N / 64)

static inline __m256i mul_add(__m256i a, __m256i b, __m256i c) {
    return _mm256_add_epi16(_mm256_mullo_epi16(a, b), c);
}

static void schoolbook_avx_acc(__m256i *c, const __m256i *a, const __m256i *b) {
    __m256i a0, a1, a2, a3, a4, a5, a6, a7, b0, b1, b2, b3, b4, b5, b6, b7;
    __m256i temp;

    a0 = a[0];
    a1 = a[1];
    a2 = a[2];
    a3 = a[3];
    a4 = a[4];
    a5 = a[5];
    a6 = a[6];
    a7 = a[7];

    b0 = b[0];
    b1 = b[1];
    b2 = b[2];
    b3 = b[3];
    b4 = b[4];
    b5 = b[5];
    b6 = b[6];
    b7 = b[7];

    c[0] = mul_add(a0, b0, c[0]);

    temp = _mm256_mullo_epi16(a0, b1);
    temp = mul_add(a1, b0, temp);
    c[1] = _mm256_add_epi16(temp, c[1]);

    temp = _mm256_mullo_epi16(a0, b2);
    temp = mul_add(a1, b1, temp);
    temp = mul_add(a2, b0, temp);
    c[2] = _mm256_add_epi16(temp, c[2]);

    temp = _mm256_mullo_epi16(a0, b3);
    temp = mul_add(a1, b2, temp);
    temp = mul_add(a2, b1, temp);
    temp = mul_add(a3, b0, temp);
    c[3] = _mm256_add_epi16(temp, c[3]);

    temp = _mm256_mullo_epi16(a0, b4);
    temp = mul_add(a1, b3, temp);
    temp = mul_add(a3, b1, temp);
    temp = mul_add(a4, b0, temp);
    temp = mul_add(a2, b2, temp);
    c[4] = _mm256_add_epi16(temp, c[4]);

    temp = _mm256_mullo_epi16(a0, b5);
    temp = mul_add(a1, b4, temp);
    temp = mul_add(a2, b3, temp);
    temp = mul_add(a3, b2, temp);
    temp = mul_add( a4, b1, temp);
    temp = mul_add(a5, b0, temp);
    c[5] = _mm256_add_epi16(temp, c[5]);

    temp = _mm256_mullo_epi16(a0, b6);
    temp = mul_add(a1, b5, temp);
    temp = mul_add(a5, b1, temp);
    temp = mul_add(a6, b0, temp);
    temp = mul_add(a2, b4, temp);
    temp = mul_add(a3, b3, temp);
    temp = mul_add(a4, b2, temp);
    c[6] = _mm256_add_epi16(temp, c[6]);

    temp = _mm256_mullo_epi16(a0, b7);
    temp = mul_add(a1, b6, temp);
    temp = mul_add(a6, b1, temp);
    temp = mul_add(a7, b0, temp);
    temp = mul_add(a2, b5, temp);
    temp = mul_add(a3, b4, temp);
    temp = mul_add(a4, b3, temp);
    temp = mul_add(a5, b2, temp);
    c[7] = _mm256_add_epi16(temp, c[7]);

    temp = _mm256_mullo_epi16(a0, b[8]);
    temp = mul_add(a1, b7, temp);
    temp = mul_add(a7, b1, temp);
    temp = mul_add(a[8], b0, temp);
    temp = mul_add(a2, b6, temp);
    temp = mul_add(a3, b5, temp);
    temp = mul_add(a4, b4, temp);
    temp = mul_add(a5, b3, temp);
    temp = mul_add(a6, b2, temp);
    c[8] = _mm256_add_epi16(temp, c[8]);

    temp = _mm256_mullo_epi16(a0, b[9]);
    temp = mul_add(a1, b[8], temp);
    temp = mul_add(a[8], b1, temp);
    temp = mul_add(a[9], b0, temp);
    temp = mul_add(a2, b7, temp);
    temp = mul_add(a3, b6, temp);
    temp = mul_add(a4, b5, temp);
    temp = mul_add(a5, b4, temp);
    temp = mul_add(a6, b3, temp);
    temp = mul_add(a7, b2, temp);
    c[9] = _mm256_add_epi16(temp, c[9]);

    temp = _mm256_mullo_epi16(a0, b[10]);
    temp = mul_add(a1, b[9], temp);
    temp = mul_add(a[9], b1, temp);
    temp = mul_add(a[10], b0, temp);
    temp = mul_add(a2, b[8], temp);
    temp = mul_add(a3, b7, temp);
    temp = mul_add(a4, b6, temp);
    temp = mul_add(a5, b5, temp);
    temp = mul_add(a6, b4, temp);
    temp = mul_add(a7, b3, temp);
    temp = mul_add(a[8], b2, temp);
    c[10] = _mm256_add_epi16(temp, c[10]);

    temp = _mm256_mullo_epi16(a0, b[11]);
    temp = mul_add(a1, b[10], temp);
    temp = mul_add(a[10], b1, temp);
    temp = mul_add(a[11], b0, temp);
    temp = mul_add(a2, b[9], temp);
    temp = mul_add(a3, b[8], temp);
    temp = mul_add(a4, b7, temp);
    temp = mul_add(a5, b6, temp);
    temp = mul_add(a6, b5, temp);
    temp = mul_add(a7, b4, temp);
    temp = mul_add(a[8], b3, temp);
    temp = mul_add(a[9], b2, temp);
    c[11] = _mm256_add_epi16(temp, c[11]);

    temp = _mm256_mullo_epi16(a0, b[12]);
    temp = mul_add(a1, b[11], temp);
    temp = mul_add(a[11], b1, temp);
    temp = mul_add(a[12], b0, temp);
    temp = mul_add(a2, b[10], temp);
    temp = mul_add(a3, b[9], temp);
    temp = mul_add(a4, b[8], temp);
    temp = mul_add(a5, b7, temp);
    temp = mul_add(a6, b6, temp);
    temp = mul_add(a7, b5, temp);
    temp = mul_add(a[8], b4, temp);
    temp = mul_add(a[9], b3, temp);
    temp = mul_add(a[10], b2, temp);
    c[12] = _mm256_add_epi16(temp, c[12]);

    temp = _mm256_mullo_epi16(a0, b[13]);
    temp = mul_add(a1, b[12], temp);
    temp = mul_add(a[12], b1, temp);
    temp = mul_add(a[13], b0, temp);
    temp = mul_add(a2, b[11], temp);
    temp = mul_add(a3, b[10], temp);
    temp = mul_add(a4, b[9], temp);
    temp = mul_add(a5, b[8], temp);
    temp = mul_add(a6, b7, temp);
    temp = mul_add(a7, b6, temp);
    temp = mul_add(a[8], b5, temp);
    temp = mul_add(a[9], b4, temp);
    temp = mul_add(a[10], b3, temp);
    temp = mul_add(a[11], b2, temp);
    c[13] = _mm256_add_epi16(temp, c[13]);

    temp = _mm256_mullo_epi16(a0, b[14]);
    temp = mul_add(a1, b[13], temp);
    temp = mul_add(a[13], b1, temp);
    temp = mul_add(a[14], b0, temp);
    temp = mul_add(a2, b[12], temp);
    temp = mul_add(a3, b[11], temp);
    temp = mul_add(a4, b[10], temp);
    temp = mul_add(a5, b[9], temp);
    temp = mul_add(a6, b[8], temp);
    temp = mul_add(a7, b7, temp);
    temp = mul_add(a[8], b6, temp);
    temp = mul_add(a[9], b5, temp);
    temp = mul_add(a[10], b4, temp);
    temp = mul_add(a[11], b3, temp);
    temp = mul_add(a[12], b2, temp);
    c[14] = _mm256_add_epi16(temp, c[14]);

    temp = _mm256_mullo_epi16(a0, b[15]);
    temp = mul_add(a1, b[14], temp);
    temp = mul_add(a[14], b1, temp);
    temp = mul_add(a[15], b0, temp);
    temp = mul_add(a2, b[13], temp);
    temp = mul_add(a3, b[12], temp);
    temp = mul_add(a4, b[11], temp);
    temp = mul_add(a5, b[10], temp);
    temp = mul_add(a6, b[9], temp);
    temp = mul_add(a7, b[8], temp);
    temp = mul_add(a[8], b7, temp);
    temp = mul_add(a[9], b6, temp);
    temp = mul_add(a[10], b5, temp);
    temp = mul_add(a[11], b4, temp);
    temp = mul_add(a[12], b3, temp);
    temp = mul_add(a[13], b2, temp);
    c[15] = _mm256_add_epi16(temp, c[15]);

    a0 = a[14];
    a1 = a[15];
    a2 = a[13];
    a3 = a[12];
    a4 = a[11];
    a5 = a[10];
    a6 = a[9];
    a7 = a[8];

    b0 = b[14];
    b1 = b[15];
    b2 = b[13];
    b3 = b[12];
    b4 = b[11];
    b5 = b[10];
    b6 = b[9];
    b7 = b[8];

    temp = _mm256_mullo_epi16(a[1], b1);
    temp = mul_add(a[2], b0, temp);
    temp = mul_add(a[3], b2, temp);
    temp = mul_add(a[4], b3, temp);
    temp = mul_add(a[5], b4, temp);
    temp = mul_add(a[6], b5, temp);
    temp = mul_add(a[7], b6, temp);
    temp = mul_add(a7, b7, temp);
    temp = mul_add(a6, b[7], temp);
    temp = mul_add(a5, b[6], temp);
    temp = mul_add(a4, b[5], temp);
    temp = mul_add(a3, b[4], temp);
    temp = mul_add(a2, b[3], temp);
    temp = mul_add(a0, b[2], temp);
    temp = mul_add(a1, b[1], temp);
    c[16] = _mm256_add_epi16(temp, c[16]);

    temp = _mm256_mullo_epi16(a[2], b1);
    temp = mul_add(a[3], b0, temp);
    temp = mul_add(a[4], b2, temp);
    temp = mul_add(a[5], b3, temp);
    temp = mul_add(a[6], b4, temp);
    temp = mul_add(a[7], b5, temp);
    temp = mul_add(a7, b6, temp);
    temp = mul_add(a6, b7, temp);
    temp = mul_add(a5, b[7], temp);
    temp = mul_add(a4, b[6], temp);
    temp = mul_add(a3, b[5], temp);
    temp = mul_add(a2, b[4], temp);
    temp = mul_add(a0, b[3], temp);
    temp = mul_add(a1, b[2], temp);
    c[17] = _mm256_add_epi16(temp, c[17]);

    temp = _mm256_mullo_epi16(a[3], b1);
    temp = mul_add(a[4], b0, temp);
    temp = mul_add(a[5], b2, temp);
    temp = mul_add(a[6], b3, temp);
    temp = mul_add(a[7], b4, temp);
    temp = mul_add(a7, b5, temp);
    temp = mul_add(a6, b6, temp);
    temp = mul_add(a5, b7, temp);
    temp = mul_add(a4, b[7], temp);
    temp = mul_add(a3, b[6], temp);
    temp = mul_add(a2, b[5], temp);
    temp = mul_add(a0, b[4], temp);
    temp = mul_add(a1, b[3], temp);
    c[18] = _mm256_add_epi16(temp, c[18]);

    temp = _mm256_mullo_epi16(a[4], b1);
    temp = mul_add(a[5], b0, temp);
    temp = mul_add(a[6], b2, temp);
    temp = mul_add(a[7], b3, temp);
    temp = mul_add(a7, b4, temp);
    temp = mul_add(a6, b5, temp);
    temp = mul_add(a5, b6, temp);
    temp = mul_add(a4, b7, temp);
    temp = mul_add(a3, b[7], temp);
    temp = mul_add(a2, b[6], temp);
    temp = mul_add(a0, b[5], temp);
    temp = mul_add(a1, b[4], temp);
    c[19] = _mm256_add_epi16(temp, c[19]);

    temp = _mm256_mullo_epi16(a[5], b1);
    temp = mul_add(a[6], b0, temp);
    temp = mul_add(a[7], b2, temp);
    temp = mul_add(a7, b3, temp);
    temp = mul_add(a6, b4, temp);
    temp = mul_add(a5, b5, temp);
    temp = mul_add(a4, b6, temp);
    temp = mul_add(a3, b7, temp);
    temp = mul_add(a2, b[7], temp);
    temp = mul_add(a0, b[6], temp);
    temp = mul_add(a1, b[5], temp);
    c[20] = _mm256_add_epi16(temp, c[20]);

    temp = _mm256_mullo_epi16(a[6], b1);
    temp = mul_add(a[7], b0, temp);
    temp = mul_add(a7, b2, temp);
    temp = mul_add(a6, b3, temp);
    temp = mul_add(a5, b4, temp);
    temp = mul_add(a4, b5, temp);
    temp = mul_add(a3, b6, temp);
    temp = mul_add(a2, b7, temp);
    temp = mul_add(a0, b[7], temp);
    temp = mul_add(a1, b[6], temp);
    c[21] = _mm256_add_epi16(temp, c[21]);

    temp = _mm256_mullo_epi16(a[7], b1);
    temp = mul_add(a7, b0, temp);
    temp = mul_add(a6, b2, temp);
    temp = mul_add(a5, b3, temp);
    temp = mul_add(a4, b4, temp);
    temp = mul_add(a3, b5, temp);
    temp = mul_add(a2, b6, temp);
    temp = mul_add(a0, b7, temp);
    temp = mul_add(a1, b[7], temp);
    c[22] = _mm256_add_epi16(temp, c[22]);

    temp = _mm256_mullo_epi16(a7, b1);
    temp = mul_add(a6, b0, temp);
    temp = mul_add(a5, b2, temp);
    temp = mul_add(a4, b3, temp);
    temp = mul_add(a3, b4, temp);
    temp = mul_add(a2, b5, temp);
    temp = mul_add(a0, b6, temp);
    temp = mul_add(a1, b7, temp);
    c[23] = _mm256_add_epi16(temp, c[23]);

    temp = _mm256_mullo_epi16(a6, b1);
    temp = mul_add(a5, b0, temp);
    temp = mul_add(a4, b2, temp);
    temp = mul_add(a3, b3, temp);
    temp = mul_add(a2, b4, temp);
    temp = mul_add(a0, b5, temp);
    temp = mul_add(a1, b6, temp);
    c[24] = _mm256_add_epi16(temp, c[24]);

    temp = _mm256_mullo_epi16(a5, b1);
    temp = mul_add(a4, b0, temp);
    temp = mul_add(a3, b2, temp);
    temp = mul_add(a2, b3, temp);
    temp = mul_add(a0, b4, temp);
    temp = mul_add(a1, b5, temp);
    c[25] = _mm256_add_epi16(temp, c[25]);

    temp = _mm256_mullo_epi16(a4, b1);
    temp = mul_add(a3, b0, temp);
    temp = mul_add(a2, b2, temp);
    temp = mul_add(a0, b3, temp);
    temp = mul_add(a1, b4, temp);
    c[26] = _mm256_add_epi16(temp, c[26]);

    temp = _mm256_mullo_epi16(a3, b1);
    temp = mul_add(a2, b0, temp);
    temp = mul_add(a0, b2, temp);
    temp = mul_add(a1, b3, temp);
    c[27] = _mm256_add_epi16(temp, c[27]);

    temp = _mm256_mullo_epi16(a2, b1);
    temp = mul_add(a0, b0, temp);
    temp = mul_add(a1, b2, temp);
    c[28] = _mm256_add_epi16(temp, c[28]);

    temp = _mm256_mullo_epi16(a0, b1);
    temp = mul_add(a1, b0, temp);
    c[29] = _mm256_add_epi16(temp, c[29]);

    c[30] = mul_add(a1, b1, c[30]);

    c[31] = _mm256_set_epi64x(0, 0, 0, 0);
}


static void schoolbook_avx(__m256i *c, const __m256i *a, const __m256i *b) {
    __m256i a0, a1, a2, a3, a4, a5, a6, a7, b0, b1, b2, b3, b4, b5, b6, b7;
    __m256i temp;

    a0 = a[0];
    a1 = a[1];
    a2 = a[2];
    a3 = a[3];
    a4 = a[4];
    a5 = a[5];
    a6 = a[6];
    a7 = a[7];

    b0 = b[0];
    b1 = b[1];
    b2 = b[2];
    b3 = b[3];
    b4 = b[4];
    b5 = b[5];
    b6 = b[6];
    b7 = b[7];

    c[0] = _mm256_mullo_epi16(a0, b0);

    temp = _mm256_mullo_epi16(a0, b1);
    c[1] = mul_add(a1, b0, temp);

    temp = _mm256_mullo_epi16(a0, b2);
    temp = mul_add(a1, b1, temp);
    c[2] = mul_add(a2, b0, temp);

    temp = _mm256_mullo_epi16(a0, b3);
    temp = mul_add(a1, b2, temp);
    temp = mul_add(a2, b1, temp);
    c[3] = mul_add(a3, b0, temp);

    temp = _mm256_mullo_epi16(a0, b4);
    temp = mul_add(a1, b3, temp);
    temp = mul_add(a3, b1, temp);
    temp = mul_add(a4, b0, temp);
    c[4] = mul_add(a2, b2, temp);

    temp = _mm256_mullo_epi16(a0, b5);
    temp = mul_add(a1, b4, temp);
    temp = mul_add(a2, b3, temp);
    temp = mul_add(a3, b2, temp);
    temp = mul_add( a4, b1, temp);
    c[5] = mul_add(a5, b0, temp);

    temp = _mm256_mullo_epi16(a0, b6);
    temp = mul_add(a1, b5, temp);
    temp = mul_add(a5, b1, temp);
    temp = mul_add(a6, b0, temp);
    temp = mul_add(a2, b4, temp);
    temp = mul_add(a3, b3, temp);
    c[6] = mul_add(a4, b2, temp);

    temp = _mm256_mullo_epi16(a0, b7);
    temp = mul_add(a1, b6, temp);
    temp = mul_add(a6, b1, temp);
    temp = mul_add(a7, b0, temp);
    temp = mul_add(a2, b5, temp);
    temp = mul_add(a3, b4, temp);
    temp = mul_add(a4, b3, temp);
    c[7] = mul_add(a5, b2, temp);

    temp = _mm256_mullo_epi16(a0, b[8]);
    temp = mul_add(a1, b7, temp);
    temp = mul_add(a7, b1, temp);
    temp = mul_add(a[8], b0, temp);
    temp = mul_add(a2, b6, temp);
    temp = mul_add(a3, b5, temp);
    temp = mul_add(a4, b4, temp);
    temp = mul_add(a5, b3, temp);
    c[8] = mul_add(a6, b2, temp);

    temp = _mm256_mullo_epi16(a0, b[9]);
    temp = mul_add(a1, b[8], temp);
    temp = mul_add(a[8], b1, temp);
    temp = mul_add(a[9], b0, temp);
    temp = mul_add(a2, b7, temp);
    temp = mul_add(a3, b6, temp);
    temp = mul_add(a4, b5, temp);
    temp = mul_add(a5, b4, temp);
    temp = mul_add(a6, b3, temp);
    c[9] = mul_add(a7, b2, temp);

    temp = _mm256_mullo_epi16(a0, b[10]);
    temp = mul_add(a1, b[9], temp);
    temp = mul_add(a[9], b1, temp);
    temp = mul_add(a[10], b0, temp);
    temp = mul_add(a2, b[8], temp);
    temp = mul_add(a3, b7, temp);
    temp = mul_add(a4, b6, temp);
    temp = mul_add(a5, b5, temp);
    temp = mul_add(a6, b4, temp);
    temp = mul_add(a7, b3, temp);
    c[10] = mul_add(a[8], b2, temp);

    temp = _mm256_mullo_epi16(a0, b[11]);
    temp = mul_add(a1, b[10], temp);
    temp = mul_add(a[10], b1, temp);
    temp = mul_add(a[11], b0, temp);
    temp = mul_add(a2, b[9], temp);
    temp = mul_add(a3, b[8], temp);
    temp = mul_add(a4, b7, temp);
    temp = mul_add(a5, b6, temp);
    temp = mul_add(a6, b5, temp);
    temp = mul_add(a7, b4, temp);
    temp = mul_add(a[8], b3, temp);
    c[11] = mul_add(a[9], b2, temp);

    temp = _mm256_mullo_epi16(a0, b[12]);
    temp = mul_add(a1, b[11], temp);
    temp = mul_add(a[11], b1, temp);
    temp = mul_add(a[12], b0, temp);
    temp = mul_add(a2, b[10], temp);
    temp = mul_add(a3, b[9], temp);
    temp = mul_add(a4, b[8], temp);
    temp = mul_add(a5, b7, temp);
    temp = mul_add(a6, b6, temp);
    temp = mul_add(a7, b5, temp);
    temp = mul_add(a[8], b4, temp);
    temp = mul_add(a[9], b3, temp);
    c[12] = mul_add(a[10], b2, temp);

    temp = _mm256_mullo_epi16(a0, b[13]);
    temp = mul_add(a1, b[12], temp);
    temp = mul_add(a[12], b1, temp);
    temp = mul_add(a[13], b0, temp);
    temp = mul_add(a2, b[11], temp);
    temp = mul_add(a3, b[10], temp);
    temp = mul_add(a4, b[9], temp);
    temp = mul_add(a5, b[8], temp);
    temp = mul_add(a6, b7, temp);
    temp = mul_add(a7, b6, temp);
    temp = mul_add(a[8], b5, temp);
    temp = mul_add(a[9], b4, temp);
    temp = mul_add(a[10], b3, temp);
    c[13] = mul_add(a[11], b2, temp);

    temp = _mm256_mullo_epi16(a0, b[14]);
    temp = mul_add(a1, b[13], temp);
    temp = mul_add(a[13], b1, temp);
    temp = mul_add(a[14], b0, temp);
    temp = mul_add(a2, b[12], temp);
    temp = mul_add(a3, b[11], temp);
    temp = mul_add(a4, b[10], temp);
    temp = mul_add(a5, b[9], temp);
    temp = mul_add(a6, b[8], temp);
    temp = mul_add(a7, b7, temp);
    temp = mul_add(a[8], b6, temp);
    temp = mul_add(a[9], b5, temp);
    temp = mul_add(a[10], b4, temp);
    temp = mul_add(a[11], b3, temp);
    c[14] = mul_add(a[12], b2, temp);

    temp = _mm256_mullo_epi16(a0, b[15]);
    temp = mul_add(a1, b[14], temp);
    temp = mul_add(a[14], b1, temp);
    temp = mul_add(a[15], b0, temp);
    temp = mul_add(a2, b[13], temp);
    temp = mul_add(a3, b[12], temp);
    temp = mul_add(a4, b[11], temp);
    temp = mul_add(a5, b[10], temp);
    temp = mul_add(a6, b[9], temp);
    temp = mul_add(a7, b[8], temp);
    temp = mul_add(a[8], b7, temp);
    temp = mul_add(a[9], b6, temp);
    temp = mul_add(a[10], b5, temp);
    temp = mul_add(a[11], b4, temp);
    temp = mul_add(a[12], b3, temp);
    c[15] = mul_add(a[13], b2, temp);

    // unrolled second triangle
    a0 = a[14];
    a1 = a[15];
    a2 = a[13];
    a3 = a[12];
    a4 = a[11];
    a5 = a[10];
    a6 = a[9];
    a7 = a[8];

    b0 = b[14];
    b1 = b[15];
    b2 = b[13];
    b3 = b[12];
    b4 = b[11];
    b5 = b[10];
    b6 = b[9];
    b7 = b[8];

    temp = _mm256_mullo_epi16(a[1], b1);
    temp = mul_add(a[2], b0, temp);
    temp = mul_add(a[3], b2, temp);
    temp = mul_add(a[4], b3, temp);
    temp = mul_add(a[5], b4, temp);
    temp = mul_add(a[6], b5, temp);
    temp = mul_add(a[7], b6, temp);
    temp = mul_add(a7, b7, temp);
    temp = mul_add(a6, b[7], temp);
    temp = mul_add(a5, b[6], temp);
    temp = mul_add(a4, b[5], temp);
    temp = mul_add(a3, b[4], temp);
    temp = mul_add(a2, b[3], temp);
    temp = mul_add(a0, b[2], temp);
    c[16] = mul_add(a1, b[1], temp);

    temp = _mm256_mullo_epi16(a[2], b1);
    temp = mul_add(a[3], b0, temp);
    temp = mul_add(a[4], b2, temp);
    temp = mul_add(a[5], b3, temp);
    temp = mul_add(a[6], b4, temp);
    temp = mul_add(a[7], b5, temp);
    temp = mul_add(a7, b6, temp);
    temp = mul_add(a6, b7, temp);
    temp = mul_add(a5, b[7], temp);
    temp = mul_add(a4, b[6], temp);
    temp = mul_add(a3, b[5], temp);
    temp = mul_add(a2, b[4], temp);
    temp = mul_add(a0, b[3], temp);
    c[17] = mul_add(a1, b[2], temp);

    temp = _mm256_mullo_epi16(a[3], b1);
    temp = mul_add(a[4], b0, temp);
    temp = mul_add(a[5], b2, temp);
    temp = mul_add(a[6], b3, temp);
    temp = mul_add(a[7], b4, temp);
    temp = mul_add(a7, b5, temp);
    temp = mul_add(a6, b6, temp);
    temp = mul_add(a5, b7, temp);
    temp = mul_add(a4, b[7], temp);
    temp = mul_add(a3, b[6], temp);
    temp = mul_add(a2, b[5], temp);
    temp = mul_add(a0, b[4], temp);
    c[18] = mul_add(a1, b[3], temp);

    temp = _mm256_mullo_epi16(a[4], b1);
    temp = mul_add(a[5], b0, temp);
    temp = mul_add(a[6], b2, temp);
    temp = mul_add(a[7], b3, temp);
    temp = mul_add(a7, b4, temp);
    temp = mul_add(a6, b5, temp);
    temp = mul_add(a5, b6, temp);
    temp = mul_add(a4, b7, temp);
    temp = mul_add(a3, b[7], temp);
    temp = mul_add(a2, b[6], temp);
    temp = mul_add(a0, b[5], temp);
    c[19] = mul_add(a1, b[4], temp);

    temp = _mm256_mullo_epi16(a[5], b1);
    temp = mul_add(a[6], b0, temp);
    temp = mul_add(a[7], b2, temp);
    temp = mul_add(a7, b3, temp);
    temp = mul_add(a6, b4, temp);
    temp = mul_add(a5, b5, temp);
    temp = mul_add(a4, b6, temp);
    temp = mul_add(a3, b7, temp);
    temp = mul_add(a2, b[7], temp);
    temp = mul_add(a0, b[6], temp);
    c[20] = mul_add(a1, b[5], temp);

    temp = _mm256_mullo_epi16(a[6], b1);
    temp = mul_add(a[7], b0, temp);
    temp = mul_add(a7, b2, temp);
    temp = mul_add(a6, b3, temp);
    temp = mul_add(a5, b4, temp);
    temp = mul_add(a4, b5, temp);
    temp = mul_add(a3, b6, temp);
    temp = mul_add(a2, b7, temp);
    temp = mul_add(a0, b[7], temp);
    c[21] = mul_add(a1, b[6], temp);

    temp = _mm256_mullo_epi16(a[7], b1);
    temp = mul_add(a7, b0, temp);
    temp = mul_add(a6, b2, temp);
    temp = mul_add(a5, b3, temp);
    temp = mul_add(a4, b4, temp);
    temp = mul_add(a3, b5, temp);
    temp = mul_add(a2, b6, temp);
    temp = mul_add(a0, b7, temp);
    c[22] = mul_add(a1, b[7], temp);

    temp = _mm256_mullo_epi16(a7, b1);
    temp = mul_add(a6, b0, temp);
    temp = mul_add(a5, b2, temp);
    temp = mul_add(a4, b3, temp);
    temp = mul_add(a3, b4, temp);
    temp = mul_add(a2, b5, temp);
    temp = mul_add(a0, b6, temp);
    c[23] = mul_add(a1, b7, temp);

    temp = _mm256_mullo_epi16(a6, b1);
    temp = mul_add(a5, b0, temp);
    temp = mul_add(a4, b2, temp);
    temp = mul_add(a3, b3, temp);
    temp = mul_add(a2, b4, temp);
    temp = mul_add(a0, b5, temp);
    c[24] = mul_add(a1, b6, temp);

    temp = _mm256_mullo_epi16(a5, b1);
    temp = mul_add(a4, b0, temp);
    temp = mul_add(a3, b2, temp);
    temp = mul_add(a2, b3, temp);
    temp = mul_add(a0, b4, temp);
    c[25] = mul_add(a1, b5, temp);

    temp = _mm256_mullo_epi16(a4, b1);
    temp = mul_add(a3, b0, temp);
    temp = mul_add(a2, b2, temp);
    temp = mul_add(a0, b3, temp);
    c[26] = mul_add(a1, b4, temp);

    temp = _mm256_mullo_epi16(a3, b1);
    temp = mul_add(a2, b0, temp);
    temp = mul_add(a0, b2, temp);
    c[27] = mul_add(a1, b3, temp);

    temp = _mm256_mullo_epi16(a2, b1);
    temp = mul_add(a0, b0, temp);
    c[28] = mul_add(a1, b2, temp);

    temp = _mm256_mullo_epi16(a0, b1);
    c[29] = mul_add(a1, b0, temp);

    c[30] = _mm256_mullo_epi16(a1, b1);

    c[31] = _mm256_set_epi64x(0, 0, 0, 0);
}

static void transpose(__m256i *M) {
    __m256i r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
    __m256i temp, temp0, temp1, temp2;

    r0 = _mm256_unpacklo_epi16(M[0], M[1]);
    r1 = _mm256_unpacklo_epi16(M[2], M[3]);
    r2 = _mm256_unpacklo_epi16(M[4], M[5]);
    r3 = _mm256_unpacklo_epi16(M[6], M[7]);
    r4 = _mm256_unpacklo_epi16(M[8], M[9]);
    r5 = _mm256_unpacklo_epi16(M[10], M[11]);
    r6 = _mm256_unpacklo_epi16(M[12], M[13]);
    r7 = _mm256_unpacklo_epi16(M[14], M[15]);

    temp = _mm256_unpacklo_epi32(r0, r1);
    temp0 = _mm256_unpacklo_epi32(r2, r3);
    temp1 = _mm256_unpacklo_epi32(r4, r5);
    temp2 = _mm256_unpacklo_epi32(r6, r7);

    r8 = _mm256_unpackhi_epi32(r0, r1);
    r9 = _mm256_unpackhi_epi32(r2, r3);
    r10 = _mm256_unpackhi_epi32(r4, r5);
    r11 = _mm256_unpackhi_epi32(r6, r7);

    r0 = _mm256_unpacklo_epi64(temp, temp0);
    r2 = _mm256_unpackhi_epi64(temp, temp0);
    r1 = _mm256_unpacklo_epi64(temp1, temp2);
    r3 = _mm256_unpackhi_epi64(temp1, temp2);

    temp = _mm256_unpackhi_epi16(M[0], M[1]);
    temp0 = _mm256_unpackhi_epi16(M[2], M[3]);
    temp1 = _mm256_unpackhi_epi16(M[4], M[5]);
    temp2 = _mm256_unpackhi_epi16(M[6], M[7]);

    r4 = _mm256_unpackhi_epi16(M[8], M[9]);
    M[0] = _mm256_permute2f128_si256(r0, r1, 0x20);
    M[8] = _mm256_permute2f128_si256(r0, r1, 0x31);
    M[1] = _mm256_permute2f128_si256(r2, r3, 0x20);
    M[9] = _mm256_permute2f128_si256(r2, r3, 0x31);
    r5 = _mm256_unpackhi_epi16(M[10], M[11]);
    r6 = _mm256_unpackhi_epi16(M[12], M[13]);
    r7 = _mm256_unpackhi_epi16(M[14], M[15]);

    r0 = _mm256_unpacklo_epi64(r8, r9);
    r1 = _mm256_unpacklo_epi64(r10, r11);
    r2 = _mm256_unpackhi_epi64(r8, r9);
    r3 = _mm256_unpackhi_epi64(r10, r11);

    M[3] = _mm256_permute2f128_si256(r2, r3, 0x20);
    M[11] = _mm256_permute2f128_si256(r2, r3, 0x31);
    M[2] = _mm256_permute2f128_si256(r0, r1, 0x20);
    M[10] = _mm256_permute2f128_si256(r0, r1, 0x31);

    r0 = _mm256_unpacklo_epi32(temp, temp0);
    r1 = _mm256_unpacklo_epi32(temp1, temp2);
    r2 = _mm256_unpacklo_epi32(r4, r5);
    r3 = _mm256_unpacklo_epi32(r6, r7);

    r8 = _mm256_unpacklo_epi64(r0, r1);
    r10 = _mm256_unpackhi_epi64(r0, r1);
    r9 = _mm256_unpacklo_epi64(r2, r3);
    r11 = _mm256_unpackhi_epi64(r2, r3);

    M[4] = _mm256_permute2f128_si256(r8, r9, 0x20);
    M[12] = _mm256_permute2f128_si256(r8, r9, 0x31);
    M[5] = _mm256_permute2f128_si256(r10, r11, 0x20);
    M[13] = _mm256_permute2f128_si256(r10, r11, 0x31);

    r0 = _mm256_unpackhi_epi32(temp, temp0);
    r1 = _mm256_unpackhi_epi32(temp1, temp2);
    r2 = _mm256_unpackhi_epi32(r4, r5);
    r3 = _mm256_unpackhi_epi32(r6, r7);

    r4 = _mm256_unpacklo_epi64(r0, r1);
    r6 = _mm256_unpackhi_epi64(r0, r1);
    r5 = _mm256_unpacklo_epi64(r2, r3);
    r7 = _mm256_unpackhi_epi64(r2, r3);

    M[6] = _mm256_permute2f128_si256(r4, r5, 0x20);
    M[14] = _mm256_permute2f128_si256(r4, r5, 0x31);
    M[7] = _mm256_permute2f128_si256(r6, r7, 0x20);
    M[15] = _mm256_permute2f128_si256(r6, r7, 0x31);
}

static void batch_64coefficient_multiplications(toom4_points_product *c_eval, const __m256i *a, const toom4_points *b_eval, int accumulate) {
    toom4_points a_eval;// Holds evaluation (a & b) for 7 Karatsuba at a time
    __m256i r0_avx, r1_avx, r2_avx, r3_avx;
    __m256i *va = (__m256i *)a_eval.coeffs;
    __m256i *vb = (__m256i *)b_eval->coeffs;
    __m256i *vc = (__m256i *)c_eval->coeffs;

    //------------------AVX evaluation for 1st poly-----------------------
    r0_avx = a[0 * L + 0];
    r1_avx = a[0 * L + 1];
    r2_avx = a[0 * L + 2];
    r3_avx = a[0 * L + 3];

    va[0] = r0_avx;
    va[1] = r1_avx;
    va[2] = r2_avx;
    va[3] = r3_avx;
    va[4] = _mm256_add_epi16(r0_avx, r1_avx);
    va[5] = _mm256_add_epi16(r2_avx, r3_avx);
    va[6] = _mm256_add_epi16(r0_avx, r2_avx);
    va[7] = _mm256_add_epi16(r1_avx, r3_avx);
    va[8] = _mm256_add_epi16(va[6], va[7]);
    //------------------AVX evaluation for 1st poly ends------------------

    //------------------AVX evaluation for 2nd poly-----------------------
    r0_avx = a[1 * L + 0];
    r1_avx = a[1 * L + 1];
    r2_avx = a[1 * L + 2];
    r3_avx = a[1 * L + 3];

    va[0 + 9] = r0_avx;
    va[1 + 9] = r1_avx;
    va[2 + 9] = r2_avx;
    va[3 + 9] = r3_avx;
    va[4 + 9] = _mm256_add_epi16(r0_avx, r1_avx);
    va[5 + 9] = _mm256_add_epi16(r2_avx, r3_avx);
    va[6 + 9] = _mm256_add_epi16(r0_avx, r2_avx);
    va[7 + 9] = _mm256_add_epi16(r1_avx, r3_avx);
    va[8 + 9] = _mm256_add_epi16(va[6 + 9], va[7 + 9]);
    //------------------AVX evaluation for 2nd poly ends------------------

    //------------------AVX evaluation for 3rd poly-----------------------
    r0_avx = a[2 * L + 0];
    r1_avx = a[2 * L + 1];
    r2_avx = a[2 * L + 2];
    r3_avx = a[2 * L + 3];

    va[0 + 18] = r0_avx;
    va[1 + 18] = r1_avx;
    va[2 + 18] = r2_avx;
    va[3 + 18] = r3_avx;
    va[4 + 18] = _mm256_add_epi16(r0_avx, r1_avx);
    va[5 + 18] = _mm256_add_epi16(r2_avx, r3_avx);
    va[6 + 18] = _mm256_add_epi16(r0_avx, r2_avx);
    va[7 + 18] = _mm256_add_epi16(r1_avx, r3_avx);
    va[8 + 18] = _mm256_add_epi16(va[6 + 18], va[7 + 18]);
    //------------------AVX evaluation for 3rd poly ends------------------

    //------------------AVX evaluation for 4th poly-----------------------
    r0_avx = a[3 * L + 0];
    r1_avx = a[3 * L + 1];
    r2_avx = a[3 * L + 2];
    r3_avx = a[3 * L + 3];

    va[0 + 27] = r0_avx;
    va[1 + 27] = r1_avx;
    va[2 + 27] = r2_avx;
    va[3 + 27] = r3_avx;
    va[4 + 27] = _mm256_add_epi16(r0_avx, r1_avx);
    va[5 + 27] = _mm256_add_epi16(r2_avx, r3_avx);
    va[6 + 27] = _mm256_add_epi16(r0_avx, r2_avx);
    va[7 + 27] = _mm256_add_epi16(r1_avx, r3_avx);
    va[8 + 27] = _mm256_add_epi16(va[6 + 27], va[7 + 27]);
    //------------------AVX evaluation for 4th poly ends------------------

    //------------------AVX evaluation for 5th poly-----------------------
    r0_avx = a[4 * L + 0];
    r1_avx = a[4 * L + 1];
    r2_avx = a[4 * L + 2];
    r3_avx = a[4 * L + 3];

    va[0 + 36] = r0_avx;
    va[1 + 36] = r1_avx;
    va[2 + 36] = r2_avx;
    va[3 + 36] = r3_avx;
    va[4 + 36] = _mm256_add_epi16(r0_avx, r1_avx);
    va[5 + 36] = _mm256_add_epi16(r2_avx, r3_avx);
    va[6 + 36] = _mm256_add_epi16(r0_avx, r2_avx);
    va[7 + 36] = _mm256_add_epi16(r1_avx, r3_avx);
    va[8 + 36] = _mm256_add_epi16(va[6 + 36], va[7 + 36]);
    //------------------AVX evaluation for 5th poly ends------------------

    //------------------AVX evaluation for 6th poly-----------------------
    r0_avx = a[5 * L + 0];
    r1_avx = a[5 * L + 1];
    r2_avx = a[5 * L + 2];
    r3_avx = a[5 * L + 3];

    va[0 + 45] = r0_avx;
    va[1 + 45] = r1_avx;
    va[2 + 45] = r2_avx;
    va[3 + 45] = r3_avx;
    va[4 + 45] = _mm256_add_epi16(r0_avx, r1_avx);
    va[5 + 45] = _mm256_add_epi16(r2_avx, r3_avx);
    va[6 + 45] = _mm256_add_epi16(r0_avx, r2_avx);
    va[7 + 45] = _mm256_add_epi16(r1_avx, r3_avx);
    va[8 + 45] = _mm256_add_epi16(va[6 + 45], va[7 + 45]);
    //------------------AVX evaluation for 6th poly ends------------------

    //------------------AVX evaluation for 7th poly-----------------------
    r0_avx = a[6 * L + 0];
    r1_avx = a[6 * L + 1];
    r2_avx = a[6 * L + 2];
    r3_avx = a[6 * L + 3];

    va[0 + 54] = r0_avx;
    va[1 + 54] = r1_avx;
    va[2 + 54] = r2_avx;
    va[3 + 54] = r3_avx;
    va[4 + 54] = _mm256_add_epi16(r0_avx, r1_avx);
    va[5 + 54] = _mm256_add_epi16(r2_avx, r3_avx);
    va[6 + 54] = _mm256_add_epi16(r0_avx, r2_avx);
    va[7 + 54] = _mm256_add_epi16(r1_avx, r3_avx);
    va[8 + 54] = _mm256_add_epi16(va[6 + 54], va[7 + 54]);
    //------------------AVX evaluation for 7th poly ends------------------

    //-----------------Forward transposes--------------------------------------
    transpose(va);
    transpose(va + 16);
    transpose(va + 32);
    transpose(va + 48);
    //-----------------Forward transposes ends---------------------------------

    if (accumulate == 0) {
        schoolbook_avx(vc, va, vb);
        schoolbook_avx(vc + 32, va + 16, vb + 16);
        schoolbook_avx(vc + 64, va + 32, vb + 32);
        schoolbook_avx(vc + 96, va + 48, vb + 48);
    } else {
        schoolbook_avx_acc(vc, va, vb);
        schoolbook_avx_acc(vc + 32, va + 16, vb + 16);
        schoolbook_avx_acc(vc + 64, va + 32, vb + 32);
        schoolbook_avx_acc(vc + 96, va + 48, vb + 48);
    }
}

static void karatsuba_eval(__m256i *b_eval, const __m256i *b) {
    __m256i r0_avx, r1_avx, r2_avx, r3_avx;

    //-------1st poly----------------------------------------------------
    r0_avx = b[0 * L + 0];
    r1_avx = b[0 * L + 1];
    r2_avx = b[0 * L + 2];
    r3_avx = b[0 * L + 3];

    b_eval[0] = r0_avx;
    b_eval[1] = r1_avx;
    b_eval[2] = r2_avx;
    b_eval[3] = r3_avx;
    b_eval[4] = _mm256_add_epi16(r0_avx, r1_avx);
    b_eval[5] = _mm256_add_epi16(r2_avx, r3_avx);
    b_eval[6] = _mm256_add_epi16(r0_avx, r2_avx);
    b_eval[7] = _mm256_add_epi16(r1_avx, r3_avx);
    b_eval[8] = _mm256_add_epi16(b_eval[6], b_eval[7]);

    //-------2nd poly----------------------------------------------------
    r0_avx = b[1 * L + 0];
    r1_avx = b[1 * L + 1];
    r2_avx = b[1 * L + 2];
    r3_avx = b[1 * L + 3];

    b_eval[0 + 9] = r0_avx;
    b_eval[1 + 9] = r1_avx;
    b_eval[2 + 9] = r2_avx;
    b_eval[3 + 9] = r3_avx;
    b_eval[4 + 9] = _mm256_add_epi16(r0_avx, r1_avx);
    b_eval[5 + 9] = _mm256_add_epi16(r2_avx, r3_avx);
    b_eval[6 + 9] = _mm256_add_epi16(r0_avx, r2_avx);
    b_eval[7 + 9] = _mm256_add_epi16(r1_avx, r3_avx);
    b_eval[8 + 9] = _mm256_add_epi16(b_eval[6 + 9], b_eval[7 + 9]);

    //-------3rd poly----------------------------------------------------
    r0_avx = b[2 * L + 0];
    r1_avx = b[2 * L + 1];
    r2_avx = b[2 * L + 2];
    r3_avx = b[2 * L + 3];

    b_eval[0 + 18] = r0_avx;
    b_eval[1 + 18] = r1_avx;
    b_eval[2 + 18] = r2_avx;
    b_eval[3 + 18] = r3_avx;
    b_eval[4 + 18] = _mm256_add_epi16(r0_avx, r1_avx);
    b_eval[5 + 18] = _mm256_add_epi16(r2_avx, r3_avx);
    b_eval[6 + 18] = _mm256_add_epi16(r0_avx, r2_avx);
    b_eval[7 + 18] = _mm256_add_epi16(r1_avx, r3_avx);
    b_eval[8 + 18] = _mm256_add_epi16(b_eval[6 + 18], b_eval[7 + 18]);

    //-------4th poly----------------------------------------------------
    r0_avx = b[3 * L + 0];
    r1_avx = b[3 * L + 1];
    r2_avx = b[3 * L + 2];
    r3_avx = b[3 * L + 3];

    b_eval[0 + 27] = r0_avx;
    b_eval[1 + 27] = r1_avx;
    b_eval[2 + 27] = r2_avx;
    b_eval[3 + 27] = r3_avx;
    b_eval[4 + 27] = _mm256_add_epi16(r0_avx, r1_avx);
    b_eval[5 + 27] = _mm256_add_epi16(r2_avx, r3_avx);
    b_eval[6 + 27] = _mm256_add_epi16(r0_avx, r2_avx);
    b_eval[7 + 27] = _mm256_add_epi16(r1_avx, r3_avx);
    b_eval[8 + 27] = _mm256_add_epi16(b_eval[6 + 27], b_eval[7 + 27]);

    //-------5th poly----------------------------------------------------
    r0_avx = b[4 * L + 0];
    r1_avx = b[4 * L + 1];
    r2_avx = b[4 * L + 2];
    r3_avx = b[4 * L + 3];

    b_eval[0 + 36] = r0_avx;
    b_eval[1 + 36] = r1_avx;
    b_eval[2 + 36] = r2_avx;
    b_eval[3 + 36] = r3_avx;
    b_eval[4 + 36] = _mm256_add_epi16(r0_avx, r1_avx);
    b_eval[5 + 36] = _mm256_add_epi16(r2_avx, r3_avx);
    b_eval[6 + 36] = _mm256_add_epi16(r0_avx, r2_avx);
    b_eval[7 + 36] = _mm256_add_epi16(r1_avx, r3_avx);
    b_eval[8 + 36] = _mm256_add_epi16(b_eval[6 + 36], b_eval[7 + 36]);

    //-------6th poly----------------------------------------------------
    r0_avx = b[5 * L + 0];
    r1_avx = b[5 * L + 1];
    r2_avx = b[5 * L + 2];
    r3_avx = b[5 * L + 3];

    b_eval[0 + 45] = r0_avx;
    b_eval[1 + 45] = r1_avx;
    b_eval[2 + 45] = r2_avx;
    b_eval[3 + 45] = r3_avx;
    b_eval[4 + 45] = _mm256_add_epi16(r0_avx, r1_avx);
    b_eval[5 + 45] = _mm256_add_epi16(r2_avx, r3_avx);
    b_eval[6 + 45] = _mm256_add_epi16(r0_avx, r2_avx);
    b_eval[7 + 45] = _mm256_add_epi16(r1_avx, r3_avx);
    b_eval[8 + 45] = _mm256_add_epi16(b_eval[6 + 45], b_eval[7 + 45]);

    //-------7th poly----------------------------------------------------
    r0_avx = b[6 * L + 0];
    r1_avx = b[6 * L + 1];
    r2_avx = b[6 * L + 2];
    r3_avx = b[6 * L + 3];

    b_eval[0 + 54] = r0_avx;
    b_eval[1 + 54] = r1_avx;
    b_eval[2 + 54] = r2_avx;
    b_eval[3 + 54] = r3_avx;
    b_eval[4 + 54] = _mm256_add_epi16(r0_avx, r1_avx);
    b_eval[5 + 54] = _mm256_add_epi16(r2_avx, r3_avx);
    b_eval[6 + 54] = _mm256_add_epi16(r0_avx, r2_avx);
    b_eval[7 + 54] = _mm256_add_epi16(r1_avx, r3_avx);
    b_eval[8 + 54] = _mm256_add_epi16(b_eval[6 + 54], b_eval[7 + 54]);

    //--------------Evaluating B poly ends-------------------------------
    transpose(b_eval);
    transpose(b_eval + 16);
    transpose(b_eval + 32);
    transpose(b_eval + 48);
}

static void karatsuba_interp(__m256i *result_final0, __m256i *result_final1, __m256i *result_final2, __m256i *result_final3, __m256i *result_final4, __m256i *result_final5, __m256i *result_final6, const __m256i *c_eval) {
    __m256i res_avx0, res_avx1, res_avx2, res_avx3, res_avx4, res_avx5, res_avx6, res_avx7; // to hold each 64X64 poly mul results
    __m256i temp, c6_avx, c7_avx, c8_avx, c20_avx, c21_avx, c22_avx, c23_avx, c24_avx;

    //------------------------AVX interpolation for 1st poly external-------------------
    res_avx0 = c_eval[0];
    res_avx2 = c_eval[1];
    res_avx4 = c_eval[2];
    res_avx6 = c_eval[3];
    c6_avx = c_eval[6];
    c7_avx = c_eval[7];

    c8_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[8], c6_avx), c7_avx);

    res_avx1 = c_eval[16];
    res_avx3 = c_eval[17];
    res_avx5 = c_eval[18];
    res_avx7 = c_eval[19];
    c22_avx = c_eval[22];
    c23_avx = c_eval[23];

    c21_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[21], res_avx5), res_avx7);
    c24_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[24], c22_avx), c23_avx);
    c20_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[20], res_avx1), res_avx3);
    temp = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[5], res_avx4), res_avx6);
    res_avx5 = _mm256_add_epi16(res_avx5, temp);
    temp = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[4], res_avx0), res_avx2);
    res_avx1 = _mm256_add_epi16(res_avx1, temp);
    c22_avx = _mm256_add_epi16(c22_avx, c8_avx);
    res_avx6 = _mm256_add_epi16(res_avx6, c21_avx);
    res_avx2 = _mm256_add_epi16(res_avx2, c20_avx);
    c7_avx = _mm256_add_epi16(c7_avx, c24_avx);
    c6_avx = _mm256_sub_epi16(_mm256_sub_epi16(c6_avx, res_avx0), res_avx4);
    c22_avx = _mm256_sub_epi16(_mm256_sub_epi16(c22_avx, res_avx1), res_avx5);
    c7_avx = _mm256_sub_epi16(_mm256_sub_epi16(c7_avx, res_avx2), res_avx6);
    c23_avx = _mm256_sub_epi16(_mm256_sub_epi16(c23_avx, res_avx3), res_avx7);

    result_final0[0] = res_avx0;
    result_final0[1] = res_avx1;
    result_final0[2] = _mm256_add_epi16(res_avx2, c6_avx);
    result_final0[3] = _mm256_add_epi16(res_avx3, c22_avx);
    result_final0[4] = _mm256_add_epi16(res_avx4, c7_avx);
    result_final0[5] = _mm256_add_epi16(res_avx5, c23_avx);
    result_final0[6] = res_avx6;
    result_final0[7] = res_avx7;
    //------------------------AVX interpolation for 1st poly ends--------------


    //------------------------AVX interpolation for 2nd poly external-------------------
    res_avx0 = c_eval[9]; //c_eval0
    res_avx2 = c_eval[10]; //c_eval1
    res_avx4 = c_eval[11]; //c_eval2
    res_avx6 = c_eval[12]; //c_eval3
    c6_avx = c_eval[15]; //c_eval6
    c7_avx = c_eval[32]; //c_eval7

    c8_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[33], c6_avx), c7_avx);

    res_avx1 = c_eval[25]; //c_eval0
    res_avx3 = c_eval[26]; //c_eval1
    res_avx5 = c_eval[27]; //c_eval2
    res_avx7 = c_eval[28]; //c_eval3
    c22_avx = c_eval[31];
    c23_avx = c_eval[48];

    c21_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[30], res_avx5), res_avx7);
    c24_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[49], c22_avx), c23_avx);
    c20_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[29], res_avx1), res_avx3);
    temp = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[14], res_avx4), res_avx6);
    res_avx5 = _mm256_add_epi16(res_avx5, temp);
    temp = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[13], res_avx0), res_avx2);
    res_avx1 = _mm256_add_epi16(res_avx1, temp);
    c22_avx = _mm256_add_epi16(c22_avx, c8_avx);
    res_avx6 = _mm256_add_epi16(res_avx6, c21_avx);
    res_avx2 = _mm256_add_epi16(res_avx2, c20_avx);
    c7_avx = _mm256_add_epi16(c7_avx, c24_avx);
    c6_avx = _mm256_sub_epi16(_mm256_sub_epi16(c6_avx, res_avx0), res_avx4);
    c22_avx = _mm256_sub_epi16(_mm256_sub_epi16(c22_avx, res_avx1), res_avx5);
    c7_avx = _mm256_sub_epi16(_mm256_sub_epi16(c7_avx, res_avx2), res_avx6);
    c23_avx = _mm256_sub_epi16(_mm256_sub_epi16(c23_avx, res_avx3), res_avx7);

    result_final1[0] = res_avx0;
    result_final1[1] = res_avx1;
    result_final1[2] = _mm256_add_epi16(res_avx2, c6_avx);
    result_final1[3] = _mm256_add_epi16(res_avx3, c22_avx);
    result_final1[4] = _mm256_add_epi16(res_avx4, c7_avx);
    result_final1[5] = _mm256_add_epi16(res_avx5, c23_avx);
    result_final1[6] = res_avx6;
    result_final1[7] = res_avx7;
    //------------------------AVX interpolation for 2nd poly ends--------------

    //------------------------AVX interpolation for 3rd poly external-------------------
    res_avx0 = c_eval[34]; //c_eval0
    res_avx2 = c_eval[35]; //c_eval1
    res_avx4 = c_eval[36];
    res_avx6 = c_eval[37];
    c6_avx = c_eval[40];
    c7_avx = c_eval[41];

    c8_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[42], c6_avx), c7_avx);

    res_avx1 = c_eval[50]; //c_eval0
    res_avx3 = c_eval[51]; //c_eval1
    res_avx5 = c_eval[52];
    res_avx7 = c_eval[53];
    c22_avx = c_eval[56];
    c23_avx = c_eval[57];

    c21_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[55], res_avx5), res_avx7);
    c24_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[58], c22_avx), c23_avx);
    c20_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[54], res_avx1), res_avx3);
    temp = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[39], res_avx4), res_avx6);
    res_avx5 = _mm256_add_epi16(res_avx5, temp);
    temp = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[38], res_avx0), res_avx2);
    res_avx1 = _mm256_add_epi16(res_avx1, temp);
    c22_avx = _mm256_add_epi16(c22_avx, c8_avx);
    res_avx6 = _mm256_add_epi16(res_avx6, c21_avx);
    res_avx2 = _mm256_add_epi16(res_avx2, c20_avx);
    c7_avx = _mm256_add_epi16(c7_avx, c24_avx);
    c6_avx = _mm256_sub_epi16(_mm256_sub_epi16(c6_avx, res_avx0), res_avx4);
    c22_avx = _mm256_sub_epi16(_mm256_sub_epi16(c22_avx, res_avx1), res_avx5);
    c7_avx = _mm256_sub_epi16(_mm256_sub_epi16(c7_avx, res_avx2), res_avx6);
    c23_avx = _mm256_sub_epi16(_mm256_sub_epi16(c23_avx, res_avx3), res_avx7);

    result_final2[0] = res_avx0;
    result_final2[1] = res_avx1;
    result_final2[2] = _mm256_add_epi16(res_avx2, c6_avx);
    result_final2[3] = _mm256_add_epi16(res_avx3, c22_avx);
    result_final2[4] = _mm256_add_epi16(res_avx4, c7_avx);
    result_final2[5] = _mm256_add_epi16(res_avx5, c23_avx);
    result_final2[6] = res_avx6;
    result_final2[7] = res_avx7;
    //------------------------AVX interpolation for 3rd poly ends--------------

    //------------------------AVX interpolation for 4th poly external-------------------
    res_avx0 = c_eval[43];
    res_avx2 = c_eval[44];
    res_avx4 = c_eval[45];
    res_avx6 = c_eval[46];
    c6_avx = c_eval[65];
    c7_avx = c_eval[66];

    c8_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[67], c6_avx), c7_avx);

    res_avx1 = c_eval[59];
    res_avx3 = c_eval[60];
    res_avx5 = c_eval[61];
    res_avx7 = c_eval[62];
    c22_avx = c_eval[81];
    c23_avx = c_eval[82];

    c21_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[80], res_avx5), res_avx7);
    c24_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[83], c22_avx), c23_avx);
    c20_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[63], res_avx1), res_avx3);
    temp = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[64], res_avx4), res_avx6);
    res_avx5 = _mm256_add_epi16(res_avx5, temp);
    temp = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[47], res_avx0), res_avx2);
    res_avx1 = _mm256_add_epi16(res_avx1, temp);
    c22_avx = _mm256_add_epi16(c22_avx, c8_avx);
    res_avx6 = _mm256_add_epi16(res_avx6, c21_avx);
    res_avx2 = _mm256_add_epi16(res_avx2, c20_avx);
    c7_avx = _mm256_add_epi16(c7_avx, c24_avx);
    c6_avx = _mm256_sub_epi16(_mm256_sub_epi16(c6_avx, res_avx0), res_avx4);
    c22_avx = _mm256_sub_epi16(_mm256_sub_epi16(c22_avx, res_avx1), res_avx5);
    c7_avx = _mm256_sub_epi16(_mm256_sub_epi16(c7_avx, res_avx2), res_avx6);
    c23_avx = _mm256_sub_epi16(_mm256_sub_epi16(c23_avx, res_avx3), res_avx7);

    result_final3[0] = res_avx0;
    result_final3[1] = res_avx1;
    result_final3[2] = _mm256_add_epi16(res_avx2, c6_avx);
    result_final3[3] = _mm256_add_epi16(res_avx3, c22_avx);
    result_final3[4] = _mm256_add_epi16(res_avx4, c7_avx);
    result_final3[5] = _mm256_add_epi16(res_avx5, c23_avx);
    result_final3[6] = res_avx6;
    result_final3[7] = res_avx7;
    //------------------------AVX interpolation for 4th poly ends--------------

    //------------------------AVX interpolation for 5th poly external-------------------
    res_avx0 = c_eval[68];
    res_avx2 = c_eval[69];
    res_avx4 = c_eval[70];
    res_avx6 = c_eval[71];
    c6_avx = c_eval[74];
    c7_avx = c_eval[75];

    c8_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[76], c6_avx), c7_avx);

    res_avx1 = c_eval[84];
    res_avx3 = c_eval[85];
    res_avx5 = c_eval[86];
    res_avx7 = c_eval[87];
    c22_avx = c_eval[90];
    c23_avx = c_eval[91];

    c21_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[89], res_avx5), res_avx7);
    c24_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[92], c22_avx), c23_avx);
    c20_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[88], res_avx1), res_avx3);
    temp = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[73], res_avx4), res_avx6);
    res_avx5 = _mm256_add_epi16(res_avx5, temp);
    temp = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[72], res_avx0), res_avx2);
    res_avx1 = _mm256_add_epi16(res_avx1, temp);
    c22_avx = _mm256_add_epi16(c22_avx, c8_avx);
    res_avx6 = _mm256_add_epi16(res_avx6, c21_avx);
    res_avx2 = _mm256_add_epi16(res_avx2, c20_avx);
    c7_avx = _mm256_add_epi16(c7_avx, c24_avx);
    c6_avx = _mm256_sub_epi16(_mm256_sub_epi16(c6_avx, res_avx0), res_avx4);
    c22_avx = _mm256_sub_epi16(_mm256_sub_epi16(c22_avx, res_avx1), res_avx5);
    c7_avx = _mm256_sub_epi16(_mm256_sub_epi16(c7_avx, res_avx2), res_avx6);
    c23_avx = _mm256_sub_epi16(_mm256_sub_epi16(c23_avx, res_avx3), res_avx7);

    result_final4[0] = res_avx0;
    result_final4[1] = res_avx1;
    result_final4[2] = _mm256_add_epi16(res_avx2, c6_avx);
    result_final4[3] = _mm256_add_epi16(res_avx3, c22_avx);
    result_final4[4] = _mm256_add_epi16(res_avx4, c7_avx);
    result_final4[5] = _mm256_add_epi16(res_avx5, c23_avx);
    result_final4[6] = res_avx6;
    result_final4[7] = res_avx7;
    //------------------------AVX interpolation for 5th poly ends--------------

    //------------------------AVX interpolation for 6th poly external-------------------
    res_avx0 = c_eval[77];
    res_avx2 = c_eval[78];
    res_avx4 = c_eval[79];
    res_avx6 = c_eval[96];
    c6_avx = c_eval[99];
    c7_avx = c_eval[100];

    c8_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[101], c6_avx), c7_avx);

    res_avx1 = c_eval[93];
    res_avx3 = c_eval[94];
    res_avx5 = c_eval[95];
    res_avx7 = c_eval[112];
    c22_avx = c_eval[115];
    c23_avx = c_eval[116];

    c21_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[114], res_avx5), res_avx7);
    c24_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[117], c22_avx), c23_avx);
    c20_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[113], res_avx1), res_avx3);
    temp = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[98], res_avx4), res_avx6);
    res_avx5 = _mm256_add_epi16(res_avx5, temp);
    temp = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[97], res_avx0), res_avx2);
    res_avx1 = _mm256_add_epi16(res_avx1, temp);
    c22_avx = _mm256_add_epi16(c22_avx, c8_avx);
    res_avx6 = _mm256_add_epi16(res_avx6, c21_avx);
    res_avx2 = _mm256_add_epi16(res_avx2, c20_avx);
    c7_avx = _mm256_add_epi16(c7_avx, c24_avx);
    c6_avx = _mm256_sub_epi16(_mm256_sub_epi16(c6_avx, res_avx0), res_avx4);
    c22_avx = _mm256_sub_epi16(_mm256_sub_epi16(c22_avx, res_avx1), res_avx5);
    c7_avx = _mm256_sub_epi16(_mm256_sub_epi16(c7_avx, res_avx2), res_avx6);
    c23_avx = _mm256_sub_epi16(_mm256_sub_epi16(c23_avx, res_avx3), res_avx7);

    result_final5[0] = res_avx0;
    result_final5[1] = res_avx1;
    result_final5[2] = _mm256_add_epi16(res_avx2, c6_avx);
    result_final5[3] = _mm256_add_epi16(res_avx3, c22_avx);
    result_final5[4] = _mm256_add_epi16(res_avx4, c7_avx);
    result_final5[5] = _mm256_add_epi16(res_avx5, c23_avx);
    result_final5[6] = res_avx6;
    result_final5[7] = res_avx7;
    //------------------------AVX interpolation for 6th poly ends--------------

    //------------------------AVX interpolation for 7th poly external-------------------
    res_avx0 = c_eval[102];
    res_avx2 = c_eval[103];
    res_avx4 = c_eval[104];
    res_avx6 = c_eval[105];
    c6_avx = c_eval[108];
    c7_avx = c_eval[109];

    c8_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[110], c6_avx), c7_avx);

    res_avx1 = c_eval[118];
    res_avx3 = c_eval[119];
    res_avx5 = c_eval[120];
    res_avx7 = c_eval[121];
    c22_avx = c_eval[124];
    c23_avx = c_eval[125];

    c21_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[123], res_avx5), res_avx7);
    c24_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[126], c22_avx), c23_avx);
    c20_avx = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[122], res_avx1), res_avx3);
    temp = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[107], res_avx4), res_avx6);
    res_avx5 = _mm256_add_epi16(res_avx5, temp);
    temp = _mm256_sub_epi16(_mm256_sub_epi16(c_eval[106], res_avx0), res_avx2);
    res_avx1 = _mm256_add_epi16(res_avx1, temp);
    c22_avx = _mm256_add_epi16(c22_avx, c8_avx);
    res_avx6 = _mm256_add_epi16(res_avx6, c21_avx);
    res_avx2 = _mm256_add_epi16(res_avx2, c20_avx);
    c7_avx = _mm256_add_epi16(c7_avx, c24_avx);
    c6_avx = _mm256_sub_epi16(_mm256_sub_epi16(c6_avx, res_avx0), res_avx4);
    c22_avx = _mm256_sub_epi16(_mm256_sub_epi16(c22_avx, res_avx1), res_avx5);
    c7_avx = _mm256_sub_epi16(_mm256_sub_epi16(c7_avx, res_avx2), res_avx6);
    c23_avx = _mm256_sub_epi16(_mm256_sub_epi16(c23_avx, res_avx3), res_avx7);

    result_final6[0] = res_avx0;
    result_final6[1] = res_avx1;
    result_final6[2] = _mm256_add_epi16(res_avx2, c6_avx);
    result_final6[3] = _mm256_add_epi16(res_avx3, c22_avx);
    result_final6[4] = _mm256_add_epi16(res_avx4, c7_avx);
    result_final6[5] = _mm256_add_epi16(res_avx5, c23_avx);
    result_final6[6] = res_avx6;
    result_final6[7] = res_avx7;
    //------------------------AVX interpolation for 7th poly ends--------------
}

void PQCLEAN_SABER_AVX2_toom4_mul_A_by_B_eval(toom4_points_product *c_eval, const poly *a, const toom4_points *b_eval, int accumulate) {
    size_t i;
    __m256i r0_avx, r1_avx, r2_avx, r3_avx, r4_avx, r5_avx, r6_avx;
    __m256i aw_avx[7 * L];
    __m256i *va = (__m256i *)a->coeffs;

    for (i = 0; i < L; i++) {
        r0_avx = va[0 * L + i];
        r1_avx = va[1 * L + i];
        r2_avx = va[2 * L + i];
        r3_avx = va[3 * L + i];
        r4_avx = _mm256_add_epi16(r0_avx, r2_avx);
        r5_avx = _mm256_add_epi16(r1_avx, r3_avx);
        aw_avx[2 * L + i] = _mm256_add_epi16(r4_avx, r5_avx);
        aw_avx[3 * L + i] = _mm256_sub_epi16(r4_avx, r5_avx);
        r4_avx = _mm256_slli_epi16(r0_avx, 2);
        r4_avx = _mm256_add_epi16(r4_avx, r2_avx);
        r4_avx = _mm256_slli_epi16(r4_avx, 1);
        r5_avx = _mm256_slli_epi16(r1_avx, 2);
        r5_avx = _mm256_add_epi16(r5_avx, r3_avx);
        aw_avx[4 * L + i] = _mm256_add_epi16(r4_avx, r5_avx);
        aw_avx[5 * L + i] = _mm256_sub_epi16(r4_avx, r5_avx);
        r4_avx = _mm256_slli_epi16(r3_avx, 3);
        r6_avx = _mm256_slli_epi16(r2_avx, 2);
        r4_avx = _mm256_add_epi16(r4_avx, r6_avx);
        r6_avx = _mm256_slli_epi16(r1_avx, 1);
        r4_avx = _mm256_add_epi16(r4_avx, r6_avx);
        aw_avx[1 * L + i] = _mm256_add_epi16(r4_avx, r0_avx);
        aw_avx[6 * L + i] = r0_avx;
        aw_avx[0 * L + i] = r3_avx;
    }

    batch_64coefficient_multiplications(c_eval, aw_avx, b_eval, accumulate);
}

void PQCLEAN_SABER_AVX2_toom4_eval(toom4_points *b_eval, const poly *b) {
    size_t i;
    __m256i bw_avx[7 * L];
    __m256i r0_avx, r1_avx, r2_avx, r3_avx, r4_avx, r5_avx, r6_avx;
    __m256i *vb = (__m256i *)b->coeffs;
    __m256i *vb_eval = (__m256i *)b_eval->coeffs;

    for (i = 0; i < L; i++) {
        r0_avx = vb[0 * L + i];
        r1_avx = vb[1 * L + i];
        r2_avx = vb[2 * L + i];
        r3_avx = vb[3 * L + i];
        r4_avx = _mm256_add_epi16(r0_avx, r2_avx);
        r5_avx = _mm256_add_epi16(r1_avx, r3_avx);
        bw_avx[2 * L + i] = _mm256_add_epi16(r4_avx, r5_avx);
        bw_avx[3 * L + i] = _mm256_sub_epi16(r4_avx, r5_avx);
        r4_avx = _mm256_slli_epi16(r0_avx, 2);
        r4_avx = _mm256_add_epi16(r4_avx, r2_avx);
        r4_avx = _mm256_slli_epi16(r4_avx, 1);
        r5_avx = _mm256_slli_epi16(r1_avx, 2);
        r5_avx = _mm256_add_epi16(r5_avx, r3_avx);
        bw_avx[4 * L + i] = _mm256_add_epi16(r4_avx, r5_avx);
        bw_avx[5 * L + i] = _mm256_sub_epi16(r4_avx, r5_avx);
        r4_avx = _mm256_slli_epi16(r3_avx, 3);
        r6_avx = _mm256_slli_epi16(r2_avx, 2);
        r4_avx = _mm256_add_epi16(r4_avx, r6_avx);
        r6_avx = _mm256_slli_epi16(r1_avx, 1);
        r4_avx = _mm256_add_epi16(r4_avx, r6_avx);
        bw_avx[1 * L + i] = _mm256_add_epi16(r4_avx, r0_avx);
        bw_avx[6 * L + i] = r0_avx;
        bw_avx[0 * L + i] = r3_avx;
    }

    karatsuba_eval(vb_eval, bw_avx);
}


void PQCLEAN_SABER_AVX2_toom4_interp(poly *res, const toom4_points_product *c_eval) {
    size_t i;
    __m256i r0_avx, r1_avx, r2_avx, r3_avx, r4_avx, r5_avx, r6_avx, temp_avx;
    __m256i w1_avx[2 * L], w2_avx[2 * L], w3_avx[2 * L], w4_avx[2 * L], w5_avx[2 * L], w6_avx[2 * L], w7_avx[2 * L];
    __m256i res_full[32];
    __m256i *vc = (__m256i *)c_eval->coeffs;
    __m256i *vres = (__m256i *)res->coeffs;

    transpose(vc);
    transpose(vc + 16);
    transpose(vc + 32);
    transpose(vc + 48);
    transpose(vc + 64);
    transpose(vc + 80);
    transpose(vc + 96);
    transpose(vc + 112);

    karatsuba_interp(w1_avx, w2_avx, w3_avx, w4_avx, w5_avx, w6_avx, w7_avx, vc);

    for (i = 0; i < 2 * L; i++) {
        r0_avx = w1_avx[i];
        r1_avx = w2_avx[i];
        r2_avx = w3_avx[i];
        r3_avx = w4_avx[i];
        r4_avx = w5_avx[i];
        r5_avx = w6_avx[i];
        r6_avx = w7_avx[i];

        r1_avx = _mm256_add_epi16(r1_avx, r4_avx);
        r5_avx = _mm256_sub_epi16(r5_avx, r4_avx);
        r3_avx = _mm256_sub_epi16(r3_avx, r2_avx);
        r3_avx = _mm256_srli_epi16(r3_avx, 1);
        r4_avx = _mm256_sub_epi16(r4_avx, r0_avx);
        temp_avx = _mm256_slli_epi16(r6_avx, 6);

        r4_avx = _mm256_sub_epi16(r4_avx, temp_avx);
        r4_avx = _mm256_slli_epi16(r4_avx, 1);
        r4_avx = _mm256_add_epi16(r4_avx, r5_avx);
        r2_avx = _mm256_add_epi16(r2_avx, r3_avx);
        temp_avx = _mm256_slli_epi16(r2_avx, 6);

        r1_avx = _mm256_sub_epi16(r1_avx, temp_avx);
        r1_avx = _mm256_sub_epi16(r1_avx, r2_avx);
        r2_avx = _mm256_sub_epi16(r2_avx, r6_avx);
        r2_avx = _mm256_sub_epi16(r2_avx, r0_avx);
        temp_avx = _mm256_mullo_epi16(r2_avx, _mm256_set1_epi16(45));

        r1_avx = _mm256_add_epi16(r1_avx, temp_avx);
        temp_avx = _mm256_slli_epi16(r2_avx, 3);

        r4_avx = _mm256_sub_epi16(r4_avx, temp_avx);
        r4_avx = _mm256_mullo_epi16(r4_avx, _mm256_set1_epi16(-21845)); // -21845 = 1/3 (mod 2^16)
        r4_avx = _mm256_srli_epi16(r4_avx, 3);
        r5_avx = _mm256_add_epi16(r5_avx, r1_avx);
        temp_avx = _mm256_slli_epi16(r3_avx, 4);

        r1_avx = _mm256_add_epi16(r1_avx, temp_avx);
        r1_avx = _mm256_mullo_epi16(r1_avx, _mm256_set1_epi16(-29127)); // -29127 = 1/9 (mod 2^16)
        r1_avx = _mm256_srli_epi16(r1_avx, 1);
        r3_avx = _mm256_add_epi16(r1_avx, r3_avx);
        r3_avx = _mm256_sub_epi16(_mm256_set1_epi16(0), r3_avx);
        temp_avx = _mm256_mullo_epi16(r1_avx, _mm256_set1_epi16(30));
        temp_avx = _mm256_sub_epi16(temp_avx, r5_avx);
        temp_avx = _mm256_mullo_epi16(temp_avx, _mm256_set1_epi16(-4369)); // -4369 = 1/15 (mod 2^16)

        r5_avx = _mm256_srli_epi16(temp_avx, 2);
        r2_avx = _mm256_sub_epi16(r2_avx, r4_avx);
        r1_avx = _mm256_sub_epi16(r1_avx, r5_avx);

        if (i < L) {
            res_full[0 * L + i] = r6_avx;
            res_full[1 * L + i] = r5_avx;
            res_full[2 * L + i] = r4_avx;
            res_full[3 * L + i] = r3_avx;
            res_full[4 * L + i] = r2_avx;
            res_full[5 * L + i] = r1_avx;
            res_full[6 * L + i] = r0_avx;
        } else {
            res_full[0 * L + i] = _mm256_add_epi16(res_full[0 * L + i], r6_avx);
            res_full[1 * L + i] = _mm256_add_epi16(res_full[1 * L + i], r5_avx);
            res_full[2 * L + i] = _mm256_add_epi16(res_full[2 * L + i], r4_avx);
            res_full[3 * L + i] = _mm256_add_epi16(res_full[3 * L + i], r3_avx);
            res_full[4 * L + i] = _mm256_add_epi16(res_full[4 * L + i], r2_avx);
            res_full[5 * L + i] = _mm256_add_epi16(res_full[5 * L + i], r1_avx);
            res_full[6 * L + i] = r0_avx;
        }
    }

    // Reduction by X^256 + 1
    for (i = 0; i < 16; i++) {
        vres[i] = _mm256_sub_epi16(res_full[i], res_full[i + 16]);
    }
}
