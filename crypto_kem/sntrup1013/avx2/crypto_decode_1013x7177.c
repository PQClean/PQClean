#include "crypto_decode_1013x7177.h"
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

void PQCLEAN_SNTRUP1013_AVX2_crypto_decode_1013x7177(void *v, const unsigned char *s) {
    int16 *R0 = v;
    int16 R1[507], R2[254], R3[127], R4[64], R5[32], R6[16], R7[8], R8[4], R9[2], R10[1];
    long long i;
    int16 a0, a1, a2;
    __m256i A0, A1, A2, S0, S1, B0, B1, C0, C1;

    s += PQCLEAN_SNTRUP1013_AVX2_crypto_decode_1013x7177_STRBYTES;
    a1 = 0;
    a1 += *--s; /* 0...255 */
    a1 = mulhi(a1, -78) - mulhi(mullo(a1, 4305), 274);
    a1 += *--s; /* -137...391 */
    a1 -= 274; /* -411...117 */
    a1 += (a1 >> 15) & 274; /* -137...273 */
    a1 += (a1 >> 15) & 274; /* 0...273 */
    R10[0] = a1;

    /* R10 ------> R9: reconstruct mod 1*[91]+[769] */

    i = 0;
    s -= 1;
    a2 = a0 = R10[0];
    a0 = mulhi(a0, 1) - mulhi(mullo(a0, 12243), 91); /* -46...45 */
    a0 += s[1 * i + 0]; /* -46...300 */
    a0 = mulhi(a0, 16) - mulhi(mullo(a0, -720), 91); /* -46...45 */
    a0 += (a0 >> 15) & 91; /* 0...90 */
    a1 = (int16) ((a2 << 8) + s[i] - a0);
    a1 = mullo(a1, 12243);

    /* invalid inputs might need reduction mod 769 */
    a1 -= 769;
    a1 += (a1 >> 15) & 769;

    R9[0] = a0;
    R9[1] = a1;
    s -= 0;

    /* R9 ------> R8: reconstruct mod 3*[152]+[1294] */

    i = 0;
    s -= 1;
    a2 = a0 = R9[1];
    a0 = mulhi(a0, 64) - mulhi(mullo(a0, 20696), 152); /* -76...92 */
    a0 += s[1 * i + 0]; /* -76...347 */
    a0 = mulhi(a0, 24) - mulhi(mullo(a0, -431), 152); /* -77...76 */
    a0 += (a0 >> 15) & 152; /* 0...151 */
    a1 = (int16) ((a2 << 5) + ((s[i] - a0) >> 3));
    a1 = mullo(a1, -13797);

    /* invalid inputs might need reduction mod 1294 */
    a1 -= 1294;
    a1 += (a1 >> 15) & 1294;

    R8[2] = a0;
    R8[3] = a1;
    s -= 1;
    for (i = 0; i >= 0; --i) {
        a2 = a0 = R9[i];
        a0 = mulhi(a0, 64) - mulhi(mullo(a0, 20696), 152); /* -76...92 */
        a0 += s[1 * i + 0]; /* -76...347 */
        a0 = mulhi(a0, 24) - mulhi(mullo(a0, -431), 152); /* -77...76 */
        a0 += (a0 >> 15) & 152; /* 0...151 */
        a1 = (int16) ((a2 << 5) + ((s[i] - a0) >> 3));
        a1 = mullo(a1, -13797);

        /* invalid inputs might need reduction mod 152 */
        a1 -= 152;
        a1 += (a1 >> 15) & 152;

        R8[2 * i] = a0;
        R8[2 * i + 1] = a1;
    }

    /* R8 ------> R7: reconstruct mod 7*[197]+[1681] */

    i = 0;
    s -= 1;
    a2 = a0 = R8[3];
    a0 = mulhi(a0, -92) - mulhi(mullo(a0, -19628), 197); /* -122...98 */
    a0 += s[1 * i + 0]; /* -122...353 */
    a0 -= 197; /* -319..>156 */
    a0 += (a0 >> 15) & 197; /* -122...196 */
    a0 += (a0 >> 15) & 197; /* 0...196 */
    a1 = (int16) ((a2 << 8) + s[i] - a0);
    a1 = mullo(a1, 32269);

    /* invalid inputs might need reduction mod 1681 */
    a1 -= 1681;
    a1 += (a1 >> 15) & 1681;

    R7[6] = a0;
    R7[7] = a1;
    s -= 3;
    for (i = 2; i >= 0; --i) {
        a2 = a0 = R8[i];
        a0 = mulhi(a0, -92) - mulhi(mullo(a0, -19628), 197); /* -122...98 */
        a0 += s[1 * i + 0]; /* -122...353 */
        a0 -= 197; /* -319..>156 */
        a0 += (a0 >> 15) & 197; /* -122...196 */
        a0 += (a0 >> 15) & 197; /* 0...196 */
        a1 = (int16) ((a2 << 8) + s[i] - a0);
        a1 = mullo(a1, 32269);

        /* invalid inputs might need reduction mod 197 */
        a1 -= 197;
        a1 += (a1 >> 15) & 197;

        R7[2 * i] = a0;
        R7[2 * i + 1] = a1;
    }

    /* R7 ------> R6: reconstruct mod 15*[3586]+[120] */

    i = 0;
    s -= 1;
    a2 = a0 = R7[7];
    a0 = mulhi(a0, -1678) - mulhi(mullo(a0, -4679), 3586); /* -2213...1793 */
    a0 += s[1 * i + 0]; /* -2213...2048 */
    a0 += (a0 >> 15) & 3586; /* 0...3585 */
    a1 = (int16) ((a2 << 7) + ((s[i] - a0) >> 1));
    a1 = mullo(a1, -1791);

    /* invalid inputs might need reduction mod 120 */
    a1 -= 120;
    a1 += (a1 >> 15) & 120;

    R6[14] = a0;
    R6[15] = a1;
    s -= 14;
    for (i = 6; i >= 0; --i) {
        a2 = a0 = R7[i];
        a0 = mulhi(a0, -1678) - mulhi(mullo(a0, -4679), 3586); /* -2213...1793 */
        a0 += s[2 * i + 1]; /* -2213...2048 */
        a0 = mulhi(a0, -1678) - mulhi(mullo(a0, -4679), 3586); /* -1846...1849 */
        a0 += s[2 * i + 0]; /* -1846...2104 */
        a0 += (a0 >> 15) & 3586; /* 0...3585 */
        a1 = (int16) ((a2 << 15) + (s[2 * i + 1] << 7) + ((s[2 * i] - a0) >> 1));
        a1 = mullo(a1, -1791);

        /* invalid inputs might need reduction mod 3586 */
        a1 -= 3586;
        a1 += (a1 >> 15) & 3586;

        R6[2 * i] = a0;
        R6[2 * i + 1] = a1;
    }

    /* R6 ------> R5: reconstruct mod 31*[958]+[8200] */

    i = 0;
    s -= 2;
    a2 = a0 = R6[15];
    a0 = mulhi(a0, -238) - mulhi(mullo(a0, -17513), 958); /* -539...479 */
    a0 += s[2 * i + 1]; /* -539...734 */
    a0 = mulhi(a0, -238) - mulhi(mullo(a0, -17513), 958); /* -482...480 */
    a0 += s[2 * i + 0]; /* -482...735 */
    a0 += (a0 >> 15) & 958; /* 0...957 */
    a1 = (int16) ((a2 << 15) + (s[2 * i + 1] << 7) + ((s[2 * i] - a0) >> 1));
    a1 = mullo(a1, -1505);

    /* invalid inputs might need reduction mod 8200 */
    a1 -= 8200;
    a1 += (a1 >> 15) & 8200;

    R5[30] = a0;
    R5[31] = a1;
    s -= 15;
    for (i = 14; i >= 0; --i) {
        a2 = a0 = R6[i];
        a0 = mulhi(a0, -238) - mulhi(mullo(a0, -17513), 958); /* -539...479 */
        a0 += s[1 * i + 0]; /* -539...734 */
        a0 += (a0 >> 15) & 958; /* 0...957 */
        a1 = (int16) ((a2 << 7) + ((s[i] - a0) >> 1));
        a1 = mullo(a1, -1505);

        /* invalid inputs might need reduction mod 958 */
        a1 -= 958;
        a1 += (a1 >> 15) & 958;

        R5[2 * i] = a0;
        R5[2 * i + 1] = a1;
    }

    /* R5 ------> R4: reconstruct mod 63*[7921]+[265] */

    i = 0;
    s -= 1;
    a2 = a0 = R5[31];
    a0 = mulhi(a0, 538) - mulhi(mullo(a0, -2118), 7921); /* -3961...4095 */
    a0 += s[1 * i + 0]; /* -3961...4350 */
    a0 += (a0 >> 15) & 7921; /* 0...7920 */
    a1 = (int16) ((a2 << 8) + s[i] - a0);
    a1 = mullo(a1, 4625);

    /* invalid inputs might need reduction mod 265 */
    a1 -= 265;
    a1 += (a1 >> 15) & 265;

    R4[62] = a0;
    R4[63] = a1;
    s -= 62;
    i = 15;
    for (;;) {
        A0 = _mm256_loadu_si256((__m256i *) &R5[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, 538), mulhiconst(mulloconst(A0, -2118), 7921)); /* -3961...4095 */
        A0 = add(A0, S1); /* -3961...4350 */
        A0 = sub(mulhiconst(A0, 538), mulhiconst(mulloconst(A0, -2118), 7921)); /* -3994...3996 */
        A0 = add(A0, S0); /* -3994...4251 */
        A0 = ifnegaddconst(A0, 7921); /* 0...7920 */
        A1 = add(shiftleftconst(S1, 8), sub(S0, A0));
        A1 = mulloconst(A1, 4625);

        /* invalid inputs might need reduction mod 7921 */
        A1 = ifgesubconst(A1, 7921);

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

    /* R4 ------> R3: reconstruct mod 126*[89]+[265] */

    R3[126] = R4[63];
    s -= 0;
    i = 47;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R4[i]);
        A0 = sub(mulhiconst(A0, 32), mulhiconst(mulloconst(A0, -736), 89)); /* -45...52 */
        A0 = ifnegaddconst(A0, 89); /* 0...88 */
        A1 = signedshiftrightconst(sub(A2, A0), 0);
        A1 = mulloconst(A1, 18409);

        /* invalid inputs might need reduction mod 89 */
        A1 = ifgesubconst(A1, 89);

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

    /* R3 ------> R2: reconstruct mod 253*[2414]+[7177] */

    i = 0;
    s -= 2;
    a2 = a0 = R3[126];
    a0 = mulhi(a0, -84) - mulhi(mullo(a0, -6950), 2414); /* -1228...1207 */
    a0 += s[2 * i + 1]; /* -1228...1462 */
    a0 = mulhi(a0, -84) - mulhi(mullo(a0, -6950), 2414); /* -1209...1208 */
    a0 += s[2 * i + 0]; /* -1209...1463 */
    a0 += (a0 >> 15) & 2414; /* 0...2413 */
    a1 = (int16) ((a2 << 15) + (s[2 * i + 1] << 7) + ((s[2 * i] - a0) >> 1));
    a1 = mullo(a1, 6407);

    /* invalid inputs might need reduction mod 7177 */
    a1 -= 7177;
    a1 += (a1 >> 15) & 7177;

    R2[252] = a0;
    R2[253] = a1;
    s -= 252;
    i = 110;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R3[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, -84), mulhiconst(mulloconst(A0, -6950), 2414)); /* -1228...1207 */
        A0 = add(A0, S1); /* -1228...1462 */
        A0 = sub(mulhiconst(A0, -84), mulhiconst(mulloconst(A0, -6950), 2414)); /* -1209...1208 */
        A0 = add(A0, S0); /* -1209...1463 */
        A0 = ifnegaddconst(A0, 2414); /* 0...2413 */
        A1 = add(add(shiftleftconst(A2, 15), shiftleftconst(S1, 7)), signedshiftrightconst(sub(S0, A0), 1));
        A1 = mulloconst(A1, 6407);

        /* invalid inputs might need reduction mod 2414 */
        A1 = ifgesubconst(A1, 2414);

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

    /* R2 ------> R1: reconstruct mod 506*[786]+[7177] */

    R1[506] = R2[253];
    s -= 253;
    i = 237;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R2[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, 46), mulhiconst(mulloconst(A0, -21345), 786)); /* -393...404 */
        A0 = add(A0, S0); /* -393...659 */
        A0 = ifnegaddconst(A0, 786); /* 0...785 */
        A1 = add(shiftleftconst(A2, 7), signedshiftrightconst(sub(S0, A0), 1));
        A1 = mulloconst(A1, -15175);

        /* invalid inputs might need reduction mod 786 */
        A1 = ifgesubconst(A1, 786);

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

    /* R1 ------> R0: reconstruct mod 1013*[7177] */

    R0[1012] = R1[506] - 3588;
    s -= 1012;
    i = 490;
    for (;;) {
        A0 = _mm256_loadu_si256((__m256i *) &R1[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, -2610), mulhiconst(mulloconst(A0, -2338), 7177)); /* -4241...3588 */
        A0 = add(A0, S1); /* -4241...3843 */
        A0 = sub(mulhiconst(A0, -2610), mulhiconst(mulloconst(A0, -2338), 7177)); /* -3742...3757 */
        A0 = add(A0, S0); /* -3742...4012 */
        A0 = ifnegaddconst(A0, 7177); /* 0...7176 */
        A1 = add(shiftleftconst(S1, 8), sub(S0, A0));
        A1 = mulloconst(A1, 12857);

        /* invalid inputs might need reduction mod 7177 */
        A1 = ifgesubconst(A1, 7177);

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
