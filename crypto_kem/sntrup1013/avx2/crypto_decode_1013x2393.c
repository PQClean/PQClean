#include "crypto_decode_1013x2393.h"
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

void PQCLEAN_SNTRUP1013_AVX2_crypto_decode_1013x2393(void *v, const unsigned char *s) {
    int16 *R0 = v;
    int16 R1[507], R2[254], R3[127], R4[64], R5[32], R6[16], R7[8], R8[4], R9[2], R10[1];
    long long i;
    int16 a0, a1, a2;
    __m256i A0, A1, A2, S0, S1, B0, B1, C0, C1;

    s += PQCLEAN_SNTRUP1013_AVX2_crypto_decode_1013x2393_STRBYTES;
    a1 = 0;
    a1 += *--s; /* 0...255 */
    a1 = mulhi(a1, 4) - mulhi(mullo(a1, 4749), 276);
    a1 += *--s; /* -138...393 */
    a1 -= 276; /* -414...117 */
    a1 += (a1 >> 15) & 276; /* -138...275 */
    a1 += (a1 >> 15) & 276; /* 0...275 */
    R10[0] = a1;

    /* R10 ------> R9: reconstruct mod 1*[14506]+[1243] */

    i = 0;
    s -= 2;
    a2 = a0 = R10[0];
    a0 = mulhi(a0, -6226) - mulhi(mullo(a0, -1157), 14506); /* -8810...7253 */
    a0 += s[2 * i + 1]; /* -8810...7508 */
    a0 = mulhi(a0, -6226) - mulhi(mullo(a0, -1157), 14506); /* -7967...8089 */
    a0 += s[2 * i + 0]; /* -7967...8344 */
    a0 += (a0 >> 15) & 14506; /* 0...14505 */
    a1 = (int16) ((a2 << 15) + (s[2 * i + 1] << 7) + ((s[2 * i] - a0) >> 1));
    a1 = mullo(a1, 253);

    /* invalid inputs might need reduction mod 1243 */
    a1 -= 1243;
    a1 += (a1 >> 15) & 1243;

    R9[0] = a0;
    R9[1] = a1;
    s -= 0;

    /* R9 ------> R8: reconstruct mod 3*[1927]+[165] */

    i = 0;
    s -= 1;
    a2 = a0 = R9[1];
    a0 = mulhi(a0, 754) - mulhi(mullo(a0, -8706), 1927); /* -964...1152 */
    a0 += s[1 * i + 0]; /* -964...1407 */
    a0 += (a0 >> 15) & 1927; /* 0...1926 */
    a1 = (int16) ((a2 << 8) + s[i] - a0);
    a1 = mullo(a1, 3639);

    /* invalid inputs might need reduction mod 165 */
    a1 -= 165;
    a1 += (a1 >> 15) & 165;

    R8[2] = a0;
    R8[3] = a1;
    s -= 1;
    for (i = 0; i >= 0; --i) {
        a2 = a0 = R9[i];
        a0 = mulhi(a0, 754) - mulhi(mullo(a0, -8706), 1927); /* -964...1152 */
        a0 += s[1 * i + 0]; /* -964...1407 */
        a0 += (a0 >> 15) & 1927; /* 0...1926 */
        a1 = (int16) ((a2 << 8) + s[i] - a0);
        a1 = mullo(a1, 3639);

        /* invalid inputs might need reduction mod 1927 */
        a1 -= 1927;
        a1 += (a1 >> 15) & 1927;

        R8[2 * i] = a0;
        R8[2 * i + 1] = a1;
    }

    /* R8 ------> R7: reconstruct mod 7*[11236]+[962] */

    i = 0;
    s -= 2;
    a2 = a0 = R8[3];
    a0 = mulhi(a0, 1868) - mulhi(mullo(a0, -1493), 11236); /* -5618...6085 */
    a0 += s[2 * i + 1]; /* -5618...6340 */
    a0 = mulhi(a0, 1868) - mulhi(mullo(a0, -1493), 11236); /* -5779...5798 */
    a0 += s[2 * i + 0]; /* -5779...6053 */
    a0 += (a0 >> 15) & 11236; /* 0...11235 */
    a1 = (int16) ((a2 << 14) + (s[2 * i + 1] << 6) + ((s[2 * i] - a0) >> 2));
    a1 = mullo(a1, -26807);

    /* invalid inputs might need reduction mod 962 */
    a1 -= 962;
    a1 += (a1 >> 15) & 962;

    R7[6] = a0;
    R7[7] = a1;
    s -= 6;
    for (i = 2; i >= 0; --i) {
        a2 = a0 = R8[i];
        a0 = mulhi(a0, 1868) - mulhi(mullo(a0, -1493), 11236); /* -5618...6085 */
        a0 += s[2 * i + 1]; /* -5618...6340 */
        a0 = mulhi(a0, 1868) - mulhi(mullo(a0, -1493), 11236); /* -5779...5798 */
        a0 += s[2 * i + 0]; /* -5779...6053 */
        a0 += (a0 >> 15) & 11236; /* 0...11235 */
        a1 = (int16) ((a2 << 14) + (s[2 * i + 1] << 6) + ((s[2 * i] - a0) >> 2));
        a1 = mullo(a1, -26807);

        /* invalid inputs might need reduction mod 11236 */
        a1 -= 11236;
        a1 += (a1 >> 15) & 11236;

        R7[2 * i] = a0;
        R7[2 * i + 1] = a1;
    }

    /* R7 ------> R6: reconstruct mod 15*[106]+[2322] */

    i = 0;
    s -= 1;
    a2 = a0 = R7[7];
    a0 = mulhi(a0, -40) - mulhi(mullo(a0, -27204), 106); /* -63...53 */
    a0 += s[1 * i + 0]; /* -63...308 */
    a0 = mulhi(a0, 28) - mulhi(mullo(a0, -618), 106); /* -54...53 */
    a0 += (a0 >> 15) & 106; /* 0...105 */
    a1 = (int16) ((a2 << 7) + ((s[i] - a0) >> 1));
    a1 = mullo(a1, 21021);

    /* invalid inputs might need reduction mod 2322 */
    a1 -= 2322;
    a1 += (a1 >> 15) & 2322;

    R6[14] = a0;
    R6[15] = a1;
    s -= 0;
    for (i = 6; i >= 0; --i) {
        a2 = a0 = R7[i];
        a0 = mulhi(a0, 28) - mulhi(mullo(a0, -618), 106); /* -53...60 */
        a0 += (a0 >> 15) & 106; /* 0...105 */
        a1 = (int16) ((a2 - a0) >> 1);
        a1 = mullo(a1, 21021);

        /* invalid inputs might need reduction mod 106 */
        a1 -= 106;
        a1 += (a1 >> 15) & 106;

        R6[2 * i] = a0;
        R6[2 * i + 1] = a1;
    }

    /* R6 ------> R5: reconstruct mod 31*[164]+[3624] */

    i = 0;
    s -= 1;
    a2 = a0 = R6[15];
    a0 = mulhi(a0, 16) - mulhi(mullo(a0, 28772), 164); /* -82...86 */
    a0 += s[1 * i + 0]; /* -82...341 */
    a0 = mulhi(a0, -64) - mulhi(mullo(a0, -400), 164); /* -83...82 */
    a0 += (a0 >> 15) & 164; /* 0...163 */
    a1 = (int16) ((a2 << 6) + ((s[i] - a0) >> 2));
    a1 = mullo(a1, -25575);

    /* invalid inputs might need reduction mod 3624 */
    a1 -= 3624;
    a1 += (a1 >> 15) & 3624;

    R5[30] = a0;
    R5[31] = a1;
    s -= 15;
    for (i = 14; i >= 0; --i) {
        a2 = a0 = R6[i];
        a0 = mulhi(a0, 16) - mulhi(mullo(a0, 28772), 164); /* -82...86 */
        a0 += s[1 * i + 0]; /* -82...341 */
        a0 = mulhi(a0, -64) - mulhi(mullo(a0, -400), 164); /* -83...82 */
        a0 += (a0 >> 15) & 164; /* 0...163 */
        a1 = (int16) ((a2 << 6) + ((s[i] - a0) >> 2));
        a1 = mullo(a1, -25575);

        /* invalid inputs might need reduction mod 164 */
        a1 -= 164;
        a1 += (a1 >> 15) & 164;

        R5[2 * i] = a0;
        R5[2 * i + 1] = a1;
    }

    /* R5 ------> R4: reconstruct mod 63*[3278]+[283] */

    i = 0;
    s -= 1;
    a2 = a0 = R5[31];
    a0 = mulhi(a0, 412) - mulhi(mullo(a0, -5118), 3278); /* -1639...1742 */
    a0 += s[1 * i + 0]; /* -1639...1997 */
    a0 += (a0 >> 15) & 3278; /* 0...3277 */
    a1 = (int16) ((a2 << 7) + ((s[i] - a0) >> 1));
    a1 = mullo(a1, -19113);

    /* invalid inputs might need reduction mod 283 */
    a1 -= 283;
    a1 += (a1 >> 15) & 283;

    R4[62] = a0;
    R4[63] = a1;
    s -= 62;
    i = 15;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R5[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, 412), mulhiconst(mulloconst(A0, -5118), 3278)); /* -1639...1742 */
        A0 = add(A0, S1); /* -1639...1997 */
        A0 = sub(mulhiconst(A0, 412), mulhiconst(mulloconst(A0, -5118), 3278)); /* -1650...1651 */
        A0 = add(A0, S0); /* -1650...1906 */
        A0 = ifnegaddconst(A0, 3278); /* 0...3277 */
        A1 = add(add(shiftleftconst(A2, 15), shiftleftconst(S1, 7)), signedshiftrightconst(sub(S0, A0), 1));
        A1 = mulloconst(A1, -19113);

        /* invalid inputs might need reduction mod 3278 */
        A1 = ifgesubconst(A1, 3278);

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

    /* R4 ------> R3: reconstruct mod 126*[916]+[283] */

    R3[126] = R4[63];
    s -= 63;
    i = 47;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R4[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, -240), mulhiconst(mulloconst(A0, -18316), 916)); /* -518...458 */
        A0 = add(A0, S0); /* -518...713 */
        A0 = ifnegaddconst(A0, 916); /* 0...915 */
        A1 = add(shiftleftconst(A2, 6), signedshiftrightconst(sub(S0, A0), 2));
        A1 = mulloconst(A1, -17171);

        /* invalid inputs might need reduction mod 916 */
        A1 = ifgesubconst(A1, 916);

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

    /* R3 ------> R2: reconstruct mod 253*[7744]+[2393] */

    i = 0;
    s -= 2;
    a2 = a0 = R3[126];
    a0 = mulhi(a0, 3712) - mulhi(mullo(a0, -2166), 7744); /* -3872...4800 */
    a0 += s[2 * i + 1]; /* -3872...5055 */
    a0 = mulhi(a0, 3712) - mulhi(mullo(a0, -2166), 7744); /* -4092...4158 */
    a0 += s[2 * i + 0]; /* -4092...4413 */
    a0 += (a0 >> 15) & 7744; /* 0...7743 */
    a1 = (int16) ((a2 << 10) + (s[2 * i + 1] << 2) + ((s[2 * i] - a0) >> 6));
    a1 = mullo(a1, 27081);

    /* invalid inputs might need reduction mod 2393 */
    a1 -= 2393;
    a1 += (a1 >> 15) & 2393;

    R2[252] = a0;
    R2[253] = a1;
    s -= 252;
    i = 110;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R3[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, 3712), mulhiconst(mulloconst(A0, -2166), 7744)); /* -3872...4800 */
        A0 = add(A0, S1); /* -3872...5055 */
        A0 = sub(mulhiconst(A0, 3712), mulhiconst(mulloconst(A0, -2166), 7744)); /* -4092...4158 */
        A0 = add(A0, S0); /* -4092...4413 */
        A0 = ifnegaddconst(A0, 7744); /* 0...7743 */
        A1 = add(add(shiftleftconst(A2, 10), shiftleftconst(S1, 2)), signedshiftrightconst(sub(S0, A0), 6));
        A1 = mulloconst(A1, 27081);

        /* invalid inputs might need reduction mod 7744 */
        A1 = ifgesubconst(A1, 7744);

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

    /* R2 ------> R1: reconstruct mod 506*[88]+[2393] */

    R1[506] = R2[253];
    s -= 0;
    i = 237;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R2[i]);
        A0 = sub(mulhiconst(A0, -24), mulhiconst(mulloconst(A0, -745), 88)); /* -50...44 */
        A0 = ifnegaddconst(A0, 88); /* 0...87 */
        A1 = signedshiftrightconst(sub(A2, A0), 3);
        A1 = mulloconst(A1, -29789);

        /* invalid inputs might need reduction mod 88 */
        A1 = ifgesubconst(A1, 88);

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

    /* R1 ------> R0: reconstruct mod 1013*[2393] */

    R0[1012] = (int16) (3 * R1[506] - 3588);
    s -= 1012;
    i = 490;
    for (;;) {
        A0 = _mm256_loadu_si256((__m256i *) &R1[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, -107), mulhiconst(mulloconst(A0, -7011), 2393)); /* -1224...1196 */
        A0 = add(A0, S1); /* -1224...1451 */
        A0 = sub(mulhiconst(A0, -107), mulhiconst(mulloconst(A0, -7011), 2393)); /* -1199...1198 */
        A0 = add(A0, S0); /* -1199...1453 */
        A0 = ifnegaddconst(A0, 2393); /* 0...2392 */
        A1 = add(shiftleftconst(S1, 8), sub(S0, A0));
        A1 = mulloconst(A1, -20759);

        /* invalid inputs might need reduction mod 2393 */
        A1 = ifgesubconst(A1, 2393);

        A0 = mulloconst(A0, 3);
        A1 = mulloconst(A1, 3);
        A0 = subconst(A0, 3588);
        A1 = subconst(A1, 3588);
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
