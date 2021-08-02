#include "crypto_decode_953x2115.h"
#include <immintrin.h>
/* auto-generated; do not edit */

#define int16 int16_t
#define int32 int32_t

static inline int16 mullo(int16 x, int16 y) {
    return x * y;
}

static inline int16 mulhi(int16 x, int16 y) {
    return (int16) ((x * (int32)y) >> 16);
}

static inline __m256i add(__m256i x, __m256i y) {
    return _mm256_add_epi16(x, y);
}

static inline __m256i sub(__m256i x, __m256i y) {
    return _mm256_sub_epi16(x, y);
}

static inline __m256i shiftleftconst(__m256i x, int16 y) {
    return _mm256_slli_epi16(x, y);
}

static inline __m256i signedshiftrightconst(__m256i x, int16 y) {
    return _mm256_srai_epi16(x, y);
}

static inline __m256i subconst(__m256i x, int16 y) {
    return sub(x, _mm256_set1_epi16(y));
}

static inline __m256i mulloconst(__m256i x, int16 y) {
    return _mm256_mullo_epi16(x, _mm256_set1_epi16(y));
}

static inline __m256i mulhiconst(__m256i x, int16 y) {
    return _mm256_mulhi_epi16(x, _mm256_set1_epi16(y));
}

static inline __m256i ifgesubconst(__m256i x, int16 y) {
    __m256i y16 = _mm256_set1_epi16(y);
    __m256i top16 = _mm256_set1_epi16((int16)(y - 1));
    return sub(x, _mm256_cmpgt_epi16(x, top16) & y16);
}

static inline __m256i ifnegaddconst(__m256i x, int16 y) {
    return add(x, signedshiftrightconst(x, 15) & _mm256_set1_epi16(y));
}

void PQCLEAN_NTRULPR953_AVX2_crypto_decode_953x2115(void *v, const unsigned char *s) {
    int16 *R0 = v;
    int16 R1[477], R2[239], R3[120], R4[60], R5[30], R6[15], R7[8], R8[4], R9[2], R10[1];
    long long i;
    int16 a0, a1, a2;
    __m256i A0, A1, A2, S0, S1, B0, B1, C0, C1;

    s += PQCLEAN_NTRULPR953_AVX2_crypto_decode_953x2115_STRBYTES;
    a1 = 0;
    a1 += *--s; /* 0...255 */
    a1 -= 124; /* -124...131 */
    a1 -= 124; /* -248...7 */
    a1 += (a1 >> 15) & 124; /* -124...123 */
    a1 += (a1 >> 15) & 124; /* 0...123 */
    R10[0] = a1;

    /* R10 ------> R9: reconstruct mod 1*[3846]+[2107] */

    i = 0;
    s -= 2;
    a2 = a0 = R10[0];
    a0 = mulhi(a0, 964) - mulhi(mullo(a0, -4362), 3846); /* -1923...2164 */
    a0 += s[2 * i + 1]; /* -1923...2419 */
    a0 = mulhi(a0, 964) - mulhi(mullo(a0, -4362), 3846); /* -1952...1958 */
    a0 += s[2 * i + 0]; /* -1952...2213 */
    a0 += (a0 >> 15) & 3846; /* 0...3845 */
    a1 = (int16) ((a2 << 15) + (s[2 * i + 1] << 7) + ((s[2 * i] - a0) >> 1));
    a1 = mullo(a1, -16597);

    /* invalid inputs might need reduction mod 2107 */
    a1 -= 2107;
    a1 += (a1 >> 15) & 2107;

    R9[0] = a0;
    R9[1] = a1;
    s -= 0;

    /* R9 ------> R8: reconstruct mod 3*[15876]+[8694] */

    i = 0;
    s -= 2;
    a2 = a0 = R9[1];
    a0 = mulhi(a0, -3716) - mulhi(mullo(a0, -1057), 15876); /* -8867...7938 */
    a0 += s[2 * i + 1]; /* -8867...8193 */
    a0 = mulhi(a0, -3716) - mulhi(mullo(a0, -1057), 15876); /* -8403...8440 */
    a0 += s[2 * i + 0]; /* -8403...8695 */
    a0 += (a0 >> 15) & 15876; /* 0...15875 */
    a1 = (int16) ((a2 << 14) + (s[2 * i + 1] << 6) + ((s[2 * i] - a0) >> 2));
    a1 = mullo(a1, 12417);

    /* invalid inputs might need reduction mod 8694 */
    a1 -= 8694;
    a1 += (a1 >> 15) & 8694;

    R8[2] = a0;
    R8[3] = a1;
    s -= 2;
    for (i = 0; i >= 0; --i) {
        a2 = a0 = R9[i];
        a0 = mulhi(a0, -3716) - mulhi(mullo(a0, -1057), 15876); /* -8867...7938 */
        a0 += s[2 * i + 1]; /* -8867...8193 */
        a0 = mulhi(a0, -3716) - mulhi(mullo(a0, -1057), 15876); /* -8403...8440 */
        a0 += s[2 * i + 0]; /* -8403...8695 */
        a0 += (a0 >> 15) & 15876; /* 0...15875 */
        a1 = (int16) ((a2 << 14) + (s[2 * i + 1] << 6) + ((s[2 * i] - a0) >> 2));
        a1 = mullo(a1, 12417);

        /* invalid inputs might need reduction mod 15876 */
        a1 -= 15876;
        a1 += (a1 >> 15) & 15876;

        R8[2 * i] = a0;
        R8[2 * i + 1] = a1;
    }

    /* R8 ------> R7: reconstruct mod 7*[126]+[69] */

    s -= 0;
    a2 = a0 = R8[3];
    a0 = mulhi(a0, 16) - mulhi(mullo(a0, -520), 126); /* -63...67 */
    a0 += (a0 >> 15) & 126; /* 0...125 */
    a1 = (int16) ((a2 - a0) >> 1);
    a1 = mullo(a1, -4161);

    /* invalid inputs might need reduction mod 69 */
    a1 -= 69;
    a1 += (a1 >> 15) & 69;

    R7[6] = a0;
    R7[7] = a1;
    s -= 0;
    for (i = 2; i >= 0; --i) {
        a2 = a0 = R8[i];
        a0 = mulhi(a0, 16) - mulhi(mullo(a0, -520), 126); /* -63...67 */
        a0 += (a0 >> 15) & 126; /* 0...125 */
        a1 = (int16) ((a2 - a0) >> 1);
        a1 = mullo(a1, -4161);

        /* invalid inputs might need reduction mod 126 */
        a1 -= 126;
        a1 += (a1 >> 15) & 126;

        R7[2 * i] = a0;
        R7[2 * i + 1] = a1;
    }

    /* R7 ------> R6: reconstruct mod 14*[2863]+[69] */

    R6[14] = R7[7];
    s -= 14;
    for (i = 6; i >= 0; --i) {
        a0 = R7[i];
        a0 = mulhi(a0, 36) - mulhi(mullo(a0, -5860), 2863); /* -1432...1440 */
        a0 += s[2 * i + 1]; /* -1432...1695 */
        a0 = mulhi(a0, 36) - mulhi(mullo(a0, -5860), 2863); /* -1433...1432 */
        a0 += s[2 * i + 0]; /* -1433...1687 */
        a0 += (a0 >> 15) & 2863; /* 0...2862 */
        a1 = (int16) ((s[2 * i + 1] << 8) + s[2 * i] - a0);
        a1 = mullo(a1, 7119);

        /* invalid inputs might need reduction mod 2863 */
        a1 -= 2863;
        a1 += (a1 >> 15) & 2863;

        R6[2 * i] = a0;
        R6[2 * i + 1] = a1;
    }

    /* R6 ------> R5: reconstruct mod 29*[856]+[5227] */

    i = 0;
    s -= 2;
    a2 = a0 = R6[14];
    a0 = mulhi(a0, -384) - mulhi(mullo(a0, -19600), 856); /* -524...428 */
    a0 += s[2 * i + 1]; /* -524...683 */
    a0 = mulhi(a0, -384) - mulhi(mullo(a0, -19600), 856); /* -433...431 */
    a0 += s[2 * i + 0]; /* -433...686 */
    a0 += (a0 >> 15) & 856; /* 0...855 */
    a1 = (int16) ((a2 << 13) + (s[2 * i + 1] << 5) + ((s[2 * i] - a0) >> 3));
    a1 = mullo(a1, -21437);

    /* invalid inputs might need reduction mod 5227 */
    a1 -= 5227;
    a1 += (a1 >> 15) & 5227;

    R5[28] = a0;
    R5[29] = a1;
    s -= 14;
    for (i = 13; i >= 0; --i) {
        a2 = a0 = R6[i];
        a0 = mulhi(a0, -384) - mulhi(mullo(a0, -19600), 856); /* -524...428 */
        a0 += s[1 * i + 0]; /* -524...683 */
        a0 += (a0 >> 15) & 856; /* 0...855 */
        a1 = (int16) ((a2 << 5) + ((s[i] - a0) >> 3));
        a1 = mullo(a1, -21437);

        /* invalid inputs might need reduction mod 856 */
        a1 -= 856;
        a1 += (a1 >> 15) & 856;

        R5[2 * i] = a0;
        R5[2 * i + 1] = a1;
    }

    /* R5 ------> R4: reconstruct mod 59*[468]+[2859] */

    i = 0;
    s -= 1;
    a2 = a0 = R5[29];
    a0 = mulhi(a0, -116) - mulhi(mullo(a0, 29687), 468); /* -263...234 */
    a0 += s[1 * i + 0]; /* -263...489 */
    a0 -= 468; /* -731..>21 */
    a0 += (a0 >> 15) & 468; /* -263...467 */
    a0 += (a0 >> 15) & 468; /* 0...467 */
    a1 = (int16) ((a2 << 6) + ((s[i] - a0) >> 2));
    a1 = mullo(a1, -12323);

    /* invalid inputs might need reduction mod 2859 */
    a1 -= 2859;
    a1 += (a1 >> 15) & 2859;

    R4[58] = a0;
    R4[59] = a1;
    s -= 29;
    i = 13;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R5[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, -116), mulhiconst(mulloconst(A0, 29687), 468)); /* -263...234 */
        A0 = add(A0, S0); /* -263...489 */
        A0 = subconst(A0, 468); /* -731...21 */
        A0 = ifnegaddconst(A0, 468); /* -263...467 */
        A0 = ifnegaddconst(A0, 468); /* 0...467 */
        A1 = add(shiftleftconst(A2, 6), signedshiftrightconst(sub(S0, A0), 2));
        A1 = mulloconst(A1, -12323);

        /* invalid inputs might need reduction mod 468 */
        A1 = ifgesubconst(A1, 468);

        /* A0: r0r2r4r6r8r10r12r14 r16r18r20r22r24r26r28r30 */
        /* A1: r1r3r5r7r9r11r13r15 r17r19r21r23r25r27r29r31 */
        B0 = _mm256_unpacklo_epi16(A0, A1);
        B1 = _mm256_unpackhi_epi16(A0, A1);
        /* B0: r0r1r2r3r4r5r6r7 r16r17r18r19r20r21r22r23 */
        /* B1: r8r9r10r11r12r13r14r15 r24r25r26r27r28r29r30r31 */
        C0 = _mm256_permute2x128_si256(B0, B1, 0x20);
        C1 = _mm256_permute2x128_si256(B0, B1, 0x31);
        /* C0: r0r1r2r3r4r5r6r7 r8r9r10r11r12r13r14r15 */
        /* C1: r16r17r18r19r20r21r22r23 r24r25r26r27r28r29r30r31 */
        _mm256_storeu_si256((__m256i *) (&R4[2 * i]), C0);
        _mm256_storeu_si256((__m256i *) (16 + &R4[2 * i]), C1);
        if (!i) {
            break;
        }
        i = -16 - ((~15) & -i);
    }

    /* R4 ------> R3: reconstruct mod 119*[346]+[2115] */

    i = 0;
    s -= 1;
    a2 = a0 = R4[59];
    a0 = mulhi(a0, 22) - mulhi(mullo(a0, 17047), 346); /* -173...178 */
    a0 += s[1 * i + 0]; /* -173...433 */
    a0 -= 346; /* -519..>87 */
    a0 += (a0 >> 15) & 346; /* -173...345 */
    a0 += (a0 >> 15) & 346; /* 0...345 */
    a1 = (int16) ((a2 << 7) + ((s[i] - a0) >> 1));
    a1 = mullo(a1, 25381);

    /* invalid inputs might need reduction mod 2115 */
    a1 -= 2115;
    a1 += (a1 >> 15) & 2115;

    R3[118] = a0;
    R3[119] = a1;
    s -= 59;
    i = 43;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R4[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, 22), mulhiconst(mulloconst(A0, 17047), 346)); /* -173...178 */
        A0 = add(A0, S0); /* -173...433 */
        A0 = subconst(A0, 346); /* -519...87 */
        A0 = ifnegaddconst(A0, 346); /* -173...345 */
        A0 = ifnegaddconst(A0, 346); /* 0...345 */
        A1 = add(shiftleftconst(A2, 7), signedshiftrightconst(sub(S0, A0), 1));
        A1 = mulloconst(A1, 25381);

        /* invalid inputs might need reduction mod 346 */
        A1 = ifgesubconst(A1, 346);

        /* A0: r0r2r4r6r8r10r12r14 r16r18r20r22r24r26r28r30 */
        /* A1: r1r3r5r7r9r11r13r15 r17r19r21r23r25r27r29r31 */
        B0 = _mm256_unpacklo_epi16(A0, A1);
        B1 = _mm256_unpackhi_epi16(A0, A1);
        /* B0: r0r1r2r3r4r5r6r7 r16r17r18r19r20r21r22r23 */
        /* B1: r8r9r10r11r12r13r14r15 r24r25r26r27r28r29r30r31 */
        C0 = _mm256_permute2x128_si256(B0, B1, 0x20);
        C1 = _mm256_permute2x128_si256(B0, B1, 0x31);
        /* C0: r0r1r2r3r4r5r6r7 r8r9r10r11r12r13r14r15 */
        /* C1: r16r17r18r19r20r21r22r23 r24r25r26r27r28r29r30r31 */
        _mm256_storeu_si256((__m256i *) (&R3[2 * i]), C0);
        _mm256_storeu_si256((__m256i *) (16 + &R3[2 * i]), C1);
        if (!i) {
            break;
        }
        i = -16 - ((~15) & -i);
    }

    /* R3 ------> R2: reconstruct mod 238*[4761]+[2115] */

    R2[238] = R3[119];
    s -= 238;
    i = 103;
    for (;;) {
        A0 = _mm256_loadu_si256((__m256i *) &R3[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, -548), mulhiconst(mulloconst(A0, -3524), 4761)); /* -2518...2380 */
        A0 = add(A0, S1); /* -2518...2635 */
        A0 = sub(mulhiconst(A0, -548), mulhiconst(mulloconst(A0, -3524), 4761)); /* -2403...2401 */
        A0 = add(A0, S0); /* -2403...2656 */
        A0 = ifnegaddconst(A0, 4761); /* 0...4760 */
        A1 = add(shiftleftconst(S1, 8), sub(S0, A0));
        A1 = mulloconst(A1, 8617);

        /* invalid inputs might need reduction mod 4761 */
        A1 = ifgesubconst(A1, 4761);

        /* A0: r0r2r4r6r8r10r12r14 r16r18r20r22r24r26r28r30 */
        /* A1: r1r3r5r7r9r11r13r15 r17r19r21r23r25r27r29r31 */
        B0 = _mm256_unpacklo_epi16(A0, A1);
        B1 = _mm256_unpackhi_epi16(A0, A1);
        /* B0: r0r1r2r3r4r5r6r7 r16r17r18r19r20r21r22r23 */
        /* B1: r8r9r10r11r12r13r14r15 r24r25r26r27r28r29r30r31 */
        C0 = _mm256_permute2x128_si256(B0, B1, 0x20);
        C1 = _mm256_permute2x128_si256(B0, B1, 0x31);
        /* C0: r0r1r2r3r4r5r6r7 r8r9r10r11r12r13r14r15 */
        /* C1: r16r17r18r19r20r21r22r23 r24r25r26r27r28r29r30r31 */
        _mm256_storeu_si256((__m256i *) (&R2[2 * i]), C0);
        _mm256_storeu_si256((__m256i *) (16 + &R2[2 * i]), C1);
        if (!i) {
            break;
        }
        i = -16 - ((~15) & -i);
    }

    /* R2 ------> R1: reconstruct mod 476*[69]+[2115] */

    R1[476] = R2[238];
    s -= 0;
    i = 222;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R2[i]);
        A0 = sub(mulhiconst(A0, -14), mulhiconst(mulloconst(A0, -950), 69)); /* -38...34 */
        A0 = ifnegaddconst(A0, 69); /* 0...68 */
        A1 = signedshiftrightconst(sub(A2, A0), 0);
        A1 = mulloconst(A1, 4749);

        /* invalid inputs might need reduction mod 69 */
        A1 = ifgesubconst(A1, 69);

        /* A0: r0r2r4r6r8r10r12r14 r16r18r20r22r24r26r28r30 */
        /* A1: r1r3r5r7r9r11r13r15 r17r19r21r23r25r27r29r31 */
        B0 = _mm256_unpacklo_epi16(A0, A1);
        B1 = _mm256_unpackhi_epi16(A0, A1);
        /* B0: r0r1r2r3r4r5r6r7 r16r17r18r19r20r21r22r23 */
        /* B1: r8r9r10r11r12r13r14r15 r24r25r26r27r28r29r30r31 */
        C0 = _mm256_permute2x128_si256(B0, B1, 0x20);
        C1 = _mm256_permute2x128_si256(B0, B1, 0x31);
        /* C0: r0r1r2r3r4r5r6r7 r8r9r10r11r12r13r14r15 */
        /* C1: r16r17r18r19r20r21r22r23 r24r25r26r27r28r29r30r31 */
        _mm256_storeu_si256((__m256i *) (&R1[2 * i]), C0);
        _mm256_storeu_si256((__m256i *) (16 + &R1[2 * i]), C1);
        if (!i) {
            break;
        }
        i = -16 - ((~15) & -i);
    }

    /* R1 ------> R0: reconstruct mod 953*[2115] */

    R0[952] = (int16) (3 * R1[476] - 3171);
    s -= 952;
    i = 460;
    for (;;) {
        A0 = _mm256_loadu_si256((__m256i *) &R1[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, 1036), mulhiconst(mulloconst(A0, -7932), 2115)); /* -1058...1316 */
        A0 = add(A0, S1); /* -1058...1571 */
        A0 = sub(mulhiconst(A0, 1036), mulhiconst(mulloconst(A0, -7932), 2115)); /* -1075...1082 */
        A0 = add(A0, S0); /* -1075...1337 */
        A0 = ifnegaddconst(A0, 2115); /* 0...2114 */
        A1 = add(shiftleftconst(S1, 8), sub(S0, A0));
        A1 = mulloconst(A1, -31637);

        /* invalid inputs might need reduction mod 2115 */
        A1 = ifgesubconst(A1, 2115);

        A0 = mulloconst(A0, 3);
        A1 = mulloconst(A1, 3);
        A0 = subconst(A0, 3171);
        A1 = subconst(A1, 3171);
        /* A0: r0r2r4r6r8r10r12r14 r16r18r20r22r24r26r28r30 */
        /* A1: r1r3r5r7r9r11r13r15 r17r19r21r23r25r27r29r31 */
        B0 = _mm256_unpacklo_epi16(A0, A1);
        B1 = _mm256_unpackhi_epi16(A0, A1);
        /* B0: r0r1r2r3r4r5r6r7 r16r17r18r19r20r21r22r23 */
        /* B1: r8r9r10r11r12r13r14r15 r24r25r26r27r28r29r30r31 */
        C0 = _mm256_permute2x128_si256(B0, B1, 0x20);
        C1 = _mm256_permute2x128_si256(B0, B1, 0x31);
        /* C0: r0r1r2r3r4r5r6r7 r8r9r10r11r12r13r14r15 */
        /* C1: r16r17r18r19r20r21r22r23 r24r25r26r27r28r29r30r31 */
        _mm256_storeu_si256((__m256i *) (&R0[2 * i]), C0);
        _mm256_storeu_si256((__m256i *) (16 + &R0[2 * i]), C1);
        if (!i) {
            break;
        }
        i = -16 - ((~15) & -i);
    }
}
