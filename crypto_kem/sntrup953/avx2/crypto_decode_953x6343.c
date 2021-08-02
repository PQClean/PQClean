#include "crypto_decode_953x6343.h"
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

void PQCLEAN_SNTRUP953_AVX2_crypto_decode_953x6343(void *v, const unsigned char *s) {
    int16 *R0 = v;
    int16 R1[477], R2[239], R3[120], R4[60], R5[30], R6[15], R7[8], R8[4], R9[2], R10[1];
    long long i;
    int16 a0, a1, a2;
    __m256i A0, A1, A2, S0, S1, B0, B1, C0, C1;

    s += PQCLEAN_SNTRUP953_AVX2_crypto_decode_953x6343_STRBYTES;
    a1 = 0;
    a1 += *--s; /* 0...255 */
    a1 = mulhi(a1, -272) - mulhi(mullo(a1, -1336), 12558);
    a1 += *--s; /* -6279...6532 */
    a1 += (a1 >> 15) & 12558; /* 0...12557 */
    R10[0] = a1;

    /* R10 ------> R9: reconstruct mod 1*[2383]+[1349] */

    i = 0;
    s -= 1;
    a2 = a0 = R10[0];
    a0 = mulhi(a0, 896) - mulhi(mullo(a0, -7040), 2383); /* -1192...1415 */
    a0 += s[1 * i + 0]; /* -1192...1670 */
    a0 += (a0 >> 15) & 2383; /* 0...2382 */
    a1 = (int16) ((a2 << 8) + s[i] - a0);
    a1 = mullo(a1, 28079);

    /* invalid inputs might need reduction mod 1349 */
    a1 -= 1349;
    a1 += (a1 >> 15) & 1349;

    R9[0] = a0;
    R9[1] = a1;
    s -= 0;

    /* R9 ------> R8: reconstruct mod 3*[781]+[442] */

    i = 0;
    s -= 1;
    a2 = a0 = R9[1];
    a0 = mulhi(a0, -226) - mulhi(mullo(a0, -21482), 781); /* -447...390 */
    a0 += s[1 * i + 0]; /* -447...645 */
    a0 += (a0 >> 15) & 781; /* 0...780 */
    a1 = (int16) ((a2 << 8) + s[i] - a0);
    a1 = mullo(a1, -31803);

    /* invalid inputs might need reduction mod 442 */
    a1 -= 442;
    a1 += (a1 >> 15) & 442;

    R8[2] = a0;
    R8[3] = a1;
    s -= 1;
    for (i = 0; i >= 0; --i) {
        a2 = a0 = R9[i];
        a0 = mulhi(a0, -226) - mulhi(mullo(a0, -21482), 781); /* -447...390 */
        a0 += s[1 * i + 0]; /* -447...645 */
        a0 += (a0 >> 15) & 781; /* 0...780 */
        a1 = (int16) ((a2 << 8) + s[i] - a0);
        a1 = mullo(a1, -31803);

        /* invalid inputs might need reduction mod 781 */
        a1 -= 781;
        a1 += (a1 >> 15) & 781;

        R8[2 * i] = a0;
        R8[2 * i + 1] = a1;
    }

    /* R8 ------> R7: reconstruct mod 7*[447]+[253] */

    i = 0;
    s -= 1;
    a2 = a0 = R8[3];
    a0 = mulhi(a0, -35) - mulhi(mullo(a0, 28003), 447); /* -233...223 */
    a0 += s[1 * i + 0]; /* -233...478 */
    a0 -= 447; /* -680..>31 */
    a0 += (a0 >> 15) & 447; /* -233...446 */
    a0 += (a0 >> 15) & 447; /* 0...446 */
    a1 = (int16) ((a2 << 8) + s[i] - a0);
    a1 = mullo(a1, -4545);

    /* invalid inputs might need reduction mod 253 */
    a1 -= 253;
    a1 += (a1 >> 15) & 253;

    R7[6] = a0;
    R7[7] = a1;
    s -= 3;
    for (i = 2; i >= 0; --i) {
        a2 = a0 = R8[i];
        a0 = mulhi(a0, -35) - mulhi(mullo(a0, 28003), 447); /* -233...223 */
        a0 += s[1 * i + 0]; /* -233...478 */
        a0 -= 447; /* -680..>31 */
        a0 += (a0 >> 15) & 447; /* -233...446 */
        a0 += (a0 >> 15) & 447; /* 0...446 */
        a1 = (int16) ((a2 << 8) + s[i] - a0);
        a1 = mullo(a1, -4545);

        /* invalid inputs might need reduction mod 447 */
        a1 -= 447;
        a1 += (a1 >> 15) & 447;

        R7[2 * i] = a0;
        R7[2 * i + 1] = a1;
    }

    /* R7 ------> R6: reconstruct mod 14*[338]+[253] */

    R6[14] = R7[7];
    s -= 7;
    for (i = 6; i >= 0; --i) {
        a2 = a0 = R7[i];
        a0 = mulhi(a0, -90) - mulhi(mullo(a0, 15899), 338); /* -192...169 */
        a0 += s[1 * i + 0]; /* -192...424 */
        a0 -= 338; /* -530..>86 */
        a0 += (a0 >> 15) & 338; /* -192...337 */
        a0 += (a0 >> 15) & 338; /* 0...337 */
        a1 = (int16) ((a2 << 7) + ((s[i] - a0) >> 1));
        a1 = mullo(a1, -23655);

        /* invalid inputs might need reduction mod 338 */
        a1 -= 338;
        a1 += (a1 >> 15) & 338;

        R6[2 * i] = a0;
        R6[2 * i + 1] = a1;
    }

    /* R6 ------> R5: reconstruct mod 29*[4701]+[3519] */

    i = 0;
    s -= 2;
    a0 = R6[14];
    a0 = mulhi(a0, -653) - mulhi(mullo(a0, -3569), 4701); /* -2514...2350 */
    a0 += s[2 * i + 1]; /* -2514...2605 */
    a0 = mulhi(a0, -653) - mulhi(mullo(a0, -3569), 4701); /* -2377...2375 */
    a0 += s[2 * i + 0]; /* -2377...2630 */
    a0 += (a0 >> 15) & 4701; /* 0...4700 */
    a1 = (int16) ((s[2 * i + 1] << 8) + s[2 * i] - a0);
    a1 = mullo(a1, 20981);

    /* invalid inputs might need reduction mod 3519 */
    a1 -= 3519;
    a1 += (a1 >> 15) & 3519;

    R5[28] = a0;
    R5[29] = a1;
    s -= 28;
    for (i = 13; i >= 0; --i) {
        a0 = R6[i];
        a0 = mulhi(a0, -653) - mulhi(mullo(a0, -3569), 4701); /* -2514...2350 */
        a0 += s[2 * i + 1]; /* -2514...2605 */
        a0 = mulhi(a0, -653) - mulhi(mullo(a0, -3569), 4701); /* -2377...2375 */
        a0 += s[2 * i + 0]; /* -2377...2630 */
        a0 += (a0 >> 15) & 4701; /* 0...4700 */
        a1 = (int16) ((s[2 * i + 1] << 8) + s[2 * i] - a0);
        a1 = mullo(a1, 20981);

        /* invalid inputs might need reduction mod 4701 */
        a1 -= 4701;
        a1 += (a1 >> 15) & 4701;

        R5[2 * i] = a0;
        R5[2 * i + 1] = a1;
    }

    /* R5 ------> R4: reconstruct mod 59*[1097]+[821] */

    i = 0;
    s -= 1;
    a2 = a0 = R5[29];
    a0 = mulhi(a0, -302) - mulhi(mullo(a0, -15294), 1097); /* -624...548 */
    a0 += s[1 * i + 0]; /* -624...803 */
    a0 += (a0 >> 15) & 1097; /* 0...1096 */
    a1 = (int16) ((a2 << 8) + s[i] - a0);
    a1 = mullo(a1, 11769);

    /* invalid inputs might need reduction mod 821 */
    a1 -= 821;
    a1 += (a1 >> 15) & 821;

    R4[58] = a0;
    R4[59] = a1;
    s -= 29;
    i = 13;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R5[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, -302), mulhiconst(mulloconst(A0, -15294), 1097)); /* -624...548 */
        A0 = add(A0, S0); /* -624...803 */
        A0 = ifnegaddconst(A0, 1097); /* 0...1096 */
        A1 = add(shiftleftconst(A2, 8), sub(S0, A0));
        A1 = mulloconst(A1, 11769);

        /* invalid inputs might need reduction mod 1097 */
        A1 = ifgesubconst(A1, 1097);

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

    /* R4 ------> R3: reconstruct mod 119*[8476]+[6343] */

    i = 0;
    s -= 2;
    a2 = a0 = R4[59];
    a0 = mulhi(a0, 3212) - mulhi(mullo(a0, -1979), 8476); /* -4238...5041 */
    a0 += s[2 * i + 1]; /* -4238...5296 */
    a0 = mulhi(a0, 3212) - mulhi(mullo(a0, -1979), 8476); /* -4446...4497 */
    a0 += s[2 * i + 0]; /* -4446...4752 */
    a0 += (a0 >> 15) & 8476; /* 0...8475 */
    a1 = (int16) ((a2 << 14) + (s[2 * i + 1] << 6) + ((s[2 * i] - a0) >> 2));
    a1 = mullo(a1, 8567);

    /* invalid inputs might need reduction mod 6343 */
    a1 -= 6343;
    a1 += (a1 >> 15) & 6343;

    R3[118] = a0;
    R3[119] = a1;
    s -= 118;
    i = 43;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R4[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, 3212), mulhiconst(mulloconst(A0, -1979), 8476)); /* -4238...5041 */
        A0 = add(A0, S1); /* -4238...5296 */
        A0 = sub(mulhiconst(A0, 3212), mulhiconst(mulloconst(A0, -1979), 8476)); /* -4446...4497 */
        A0 = add(A0, S0); /* -4446...4752 */
        A0 = ifnegaddconst(A0, 8476); /* 0...8475 */
        A1 = add(add(shiftleftconst(A2, 14), shiftleftconst(S1, 6)), signedshiftrightconst(sub(S0, A0), 2));
        A1 = mulloconst(A1, 8567);

        /* invalid inputs might need reduction mod 8476 */
        A1 = ifgesubconst(A1, 8476);

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

    /* R3 ------> R2: reconstruct mod 238*[1473]+[6343] */

    R2[238] = R3[119];
    s -= 119;
    i = 103;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R3[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, -254), mulhiconst(mulloconst(A0, -11390), 1473)); /* -800...736 */
        A0 = add(A0, S0); /* -800...991 */
        A0 = ifnegaddconst(A0, 1473); /* 0...1472 */
        A1 = add(shiftleftconst(A2, 8), sub(S0, A0));
        A1 = mulloconst(A1, 2625);

        /* invalid inputs might need reduction mod 1473 */
        A1 = ifgesubconst(A1, 1473);

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

    /* R2 ------> R1: reconstruct mod 476*[614]+[6343] */

    R1[476] = R2[238];
    s -= 238;
    i = 222;
    for (;;) {
        A2 = A0 = _mm256_loadu_si256((__m256i *) &R2[i]);
        S0 = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i *) (s + i)));
        A0 = sub(mulhiconst(A0, 280), mulhiconst(mulloconst(A0, -27324), 614)); /* -307...377 */
        A0 = add(A0, S0); /* -307...632 */
        A0 = subconst(A0, 614); /* -921...18 */
        A0 = ifnegaddconst(A0, 614); /* -307...613 */
        A0 = ifnegaddconst(A0, 614); /* 0...613 */
        A1 = add(shiftleftconst(A2, 7), signedshiftrightconst(sub(S0, A0), 1));
        A1 = mulloconst(A1, -7685);

        /* invalid inputs might need reduction mod 614 */
        A1 = ifgesubconst(A1, 614);

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

    /* R1 ------> R0: reconstruct mod 953*[6343] */

    R0[952] = R1[476] - 3171;
    s -= 952;
    i = 460;
    for (;;) {
        A0 = _mm256_loadu_si256((__m256i *) &R1[i]);
        S0 = _mm256_loadu_si256((__m256i *) (s + 2 * i));
        S1 = _mm256_srli_epi16(S0, 8);
        S0 &= _mm256_set1_epi16(255);
        A0 = sub(mulhiconst(A0, -19), mulhiconst(mulloconst(A0, -2645), 6343)); /* -3177...3171 */
        A0 = add(A0, S1); /* -3177...3426 */
        A0 = sub(mulhiconst(A0, -19), mulhiconst(mulloconst(A0, -2645), 6343)); /* -3173...3172 */
        A0 = add(A0, S0); /* -3173...3427 */
        A0 = ifnegaddconst(A0, 6343); /* 0...6342 */
        A1 = add(shiftleftconst(S1, 8), sub(S0, A0));
        A1 = mulloconst(A1, 10487);

        /* invalid inputs might need reduction mod 6343 */
        A1 = ifgesubconst(A1, 6343);

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
