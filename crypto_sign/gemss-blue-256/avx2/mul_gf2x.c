#include "mul_gf2x.h"
#include "simd.h"


/* C=A*B in GF(2)[x] for each function */

/***********************************************************************/
/***********************************************************************/
/********************** version without vectorization ******************/
/***********************************************************************/
/***********************************************************************/


uint64_t PQCLEAN_GEMSSBLUE256_AVX2_mul32_no_simd_gf2x(uint64_t A, uint64_t B) {
    uint64_t C;
    MUL32_NO_SIMD_GF2X(C, A, B);
    return C;
}

uint64_t PQCLEAN_GEMSSBLUE256_AVX2_mul64low_no_simd_gf2x(uint64_t A, uint64_t B) {
    uint64_t C;
    MUL64LOW_NO_SIMD_GF2X(C, A, B);
    return C;
}

void PQCLEAN_GEMSSBLUE256_AVX2_mul64_no_simd_gf2x(uint64_t C[2], const uint64_t A[1],
        const uint64_t B[1]) {
    uint64_t tmp;
    MUL64_NO_SIMD_GF2X(C, *A, *B, tmp);
}

void PQCLEAN_GEMSSBLUE256_AVX2_mul96_no_simd_gf2x(uint64_t C[3], const uint64_t A[2],
        const uint64_t B[2]) {
    uint64_t tmp, AA, BB;
    MUL96_NO_SIMD_GF2X(C, A, B, tmp, AA, BB);
}

void PQCLEAN_GEMSSBLUE256_AVX2_mul128_no_simd_gf2x(uint64_t C[4], const uint64_t A[2],
        const uint64_t B[2]) {
    uint64_t tmp, AA, BB;
    MUL128_NO_SIMD_GF2X(C, A, B, tmp, AA, BB);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_mul160_no_simd_gf2x(uint64_t C[5], const uint64_t A[3],
        const uint64_t B[3]) {
    uint64_t tmp, AA, BB;
    MUL160_NO_SIMD_GF2X(C, A, B, tmp, AA, BB);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_mul192_no_simd_gf2x(uint64_t C[6], const uint64_t A[3],
        const uint64_t B[3]) {
    uint64_t tmp, AA, BB;
    MUL192_NO_SIMD_GF2X(C, A, B, tmp, AA, BB);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_mul224_no_simd_gf2x(uint64_t C[7], const uint64_t A[4],
        const uint64_t B[4]) {
    uint64_t tmp, AA[2], BB[2], tmp1, tmp2;
    MUL224_NO_SIMD_GF2X(C, A, B, tmp, AA, BB, tmp1, tmp2);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_mul256_no_simd_gf2x(uint64_t C[8], const uint64_t A[4],
        const uint64_t B[4]) {
    uint64_t tmp, AA[2], BB[2], tmp1, tmp2;
    MUL256_NO_SIMD_GF2X(C, A, B, tmp, AA, BB, tmp1, tmp2);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_mul288_no_simd_gf2x(uint64_t C[9], const uint64_t A[5],
        const uint64_t B[5]) {
    uint64_t tmp, AA[3], BB[3], tmp1, tmp2;
    MUL288_NO_SIMD_GF2X(C, A, B, tmp, AA, BB, tmp1, tmp2);
}


// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_mul320_no_simd_gf2x(uint64_t C[10], const uint64_t A[5],
        const uint64_t B[5]) {
    uint64_t tmp, AA[3], BB[3], tmp1, tmp2;
    MUL320_NO_SIMD_GF2X(C, A, B, tmp, AA, BB, tmp1, tmp2);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_mul352_no_simd_gf2x(uint64_t C[11], const uint64_t A[6],
        const uint64_t B[6]) {
    uint64_t tmp, AA[3], BB[3], tmp1, tmp2;
    MUL352_NO_SIMD_GF2X(C, A, B, tmp, AA, BB, tmp1, tmp2);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_mul384_no_simd_gf2x(uint64_t C[12], const uint64_t A[6],
        const uint64_t B[6]) {
    uint64_t tmp, AA[3], BB[3], tmp1, tmp2;
    MUL384_NO_SIMD_GF2X(C, A, B, tmp, AA, BB, tmp1, tmp2);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_mul416_no_simd_gf2x(uint64_t C[13], const uint64_t A[7],
        const uint64_t B[7]) {
    uint64_t tmp, AA[4], BB[4], tmp3[2], tmp4[2], tmp1, tmp2;
    MUL416_NO_SIMD_GF2X(C, A, B, tmp, AA, BB, tmp3, tmp4, tmp1, tmp2);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_mul448_no_simd_gf2x(uint64_t C[14], const uint64_t A[7],
        const uint64_t B[7]) {
    uint64_t tmp, AA[4], BB[4], tmp3[2], tmp4[2], tmp1, tmp2;
    MUL448_NO_SIMD_GF2X(C, A, B, tmp, AA, BB, tmp3, tmp4, tmp1, tmp2);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_mul480_no_simd_gf2x(uint64_t C[15], const uint64_t A[8],
        const uint64_t B[8]) {
    uint64_t tmp, AA[4], BB[4], tmp3[2], tmp4[2], tmp1, tmp2;
    MUL480_NO_SIMD_GF2X(C, A, B, tmp, AA, BB, tmp3, tmp4, tmp1, tmp2);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_mul512_no_simd_gf2x(uint64_t C[16], const uint64_t A[8],
        const uint64_t B[8]) {
    uint64_t tmp, AA[4], BB[4], tmp3[2], tmp4[2], tmp1, tmp2;
    MUL512_NO_SIMD_GF2X(C, A, B, tmp, AA, BB, tmp3, tmp4, tmp1, tmp2);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_mul544_no_simd_gf2x(uint64_t C[17], const uint64_t A[9],
        const uint64_t B[9]) {
    uint64_t tmp, AA[5], BB[5], tmp3[3], tmp4[3], tmp1, tmp2;
    MUL544_NO_SIMD_GF2X(C, A, B, tmp, AA, BB, tmp3, tmp4, tmp1, tmp2);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_mul576_no_simd_gf2x(uint64_t C[18], const uint64_t A[9],
        const uint64_t B[9]) {
    uint64_t tmp, AA[5], BB[5], tmp3[3], tmp4[3], tmp1, tmp2;
    MUL576_NO_SIMD_GF2X(C, A, B, tmp, AA, BB, tmp3, tmp4, tmp1, tmp2);
}


/***********************************************************************/
/***********************************************************************/
/*************************** sse2 version ******************************/
/***********************************************************************/
/***********************************************************************/


uint64_t PQCLEAN_GEMSSBLUE256_AVX2_pmul32_gf2x(uint64_t A, uint64_t B) {
    __m128i C1, RA, RB, CL, CH, M, one32;
    uint64_t C;
    one32 = PSET1_32(1);

    PMUL32_WS_CLAS_GF2X(C1, A, B, RA, RB, CL, CH, M, one32);

    PCVT_64(C, C1);
    return C;
}

void PQCLEAN_GEMSSBLUE256_AVX2_pmul64_gf2x(uint64_t C[2], const uint64_t A[1],
        const uint64_t B[1]) {
    __m128i C1, RA, RAM, RB, CL, CH, M, CLM, CHM, one32;
    one32 = PSET1_32(1);

    PMUL64_WS_CLAS2_GF2X(C1, *A, *B, RA, RAM, RB, CL, CH, M, CLM, CHM, one32);

    PSTORE128(C, C1);
}

void PQCLEAN_GEMSSBLUE256_AVX2_pmul96_gf2x(uint64_t C[3], const uint64_t A[2],
        const uint64_t B[2]) {
    __m128i C1, C2, RA, RB, CL, CH, M, M2, CM, one32;
    one32 = PSET1_32(1);

    PMUL96_WS_KAR_GF2X(C1, C2, A, B, RA, RB, CL, CH, M, M2, CM, one32);

    PSTORE192(C, C1, C2);
}

void PQCLEAN_GEMSSBLUE256_AVX2_pmul128_gf2x(uint64_t C[4], const uint64_t A[2],
        const uint64_t B[2]) {
    __m128i C1, C2, RA, RB, CL, CH, M, M2, CM, one32;
    one32 = PSET1_32(1);

    PMUL128_WS_KAR_GF2X(C1, C2, A, B, RA, RB, CL, CH, M, M2, CM, one32);

    PSTORE256(C, C1, C2);
}

void PQCLEAN_GEMSSBLUE256_AVX2_pmul160_gf2x(uint64_t C[5], const uint64_t A[3],
        const uint64_t B[3]) {
    __m128i C1, C2, C3, RA, RB, CL, CH, M, M2, CM, res1, res2, one32;
    one32 = PSET1_32(1);

    PMUL160_WS_KAR6_GF2X(C1, C2, C3, A, B, RA, RB, CL, CH, M, M2, CM, res1, res2, one32);

    PSTORE320(C, C1, C2, C3);
}

void PQCLEAN_GEMSSBLUE256_AVX2_pmul192_gf2x(uint64_t C[6], const uint64_t A[3],
        const uint64_t B[3]) {
    __m128i C1, C2, C3, RA, RB, CL, CH, M, M2, CM, res1, res2, one32;
    one32 = PSET1_32(1);

    PMUL192_WS_KAR6_GF2X(C1, C2, C3, A, B, RA, RB, CL, CH, M, M2, CM, res1, res2, one32);

    PSTORE384(C, C1, C2, C3);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_pmul224_gf2x(uint64_t C[7], const uint64_t A[4],
        const uint64_t B[4]) {
    __m128i C1, C2, C3, C4, RA, RB, CL, CH, M, MM, CM, M1, M2, one32;
    one32 = PSET1_32(1);

    PMUL224_WS_KAR_GF2X(C1, C2, C3, C4, A, B, RA, RB, CL, CH, M, MM, CM, M1, M2, one32);

    PSTORE448(C, C1, C2, C3, C4);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_pmul256_gf2x(uint64_t C[8], const uint64_t A[4],
        const uint64_t B[4]) {
    __m128i C1, C2, C3, C4, RA, RB, CL, CH, M, MM, CM, M1, M2, one32;
    one32 = PSET1_32(1);

    PMUL256_WS_KAR_GF2X(C1, C2, C3, C4, A, B, RA, RB, CL, CH, M, MM, CM, M1, M2, one32);

    PSTORE512(C, C1, C2, C3, C4);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_pmul288_gf2x(uint64_t C[9], const uint64_t A[5],
        const uint64_t B[5]) {
    __m128i C1, C2, C3, C4, C5, RA, RB, CL, CH, M, MM, CM, M1, M2, M3, res1, res2, one32;
    one32 = PSET1_32(1);

    PMUL288_WS_KAR_GF2X(C1, C2, C3, C4, C5, A, B, RA, RB, CL, CH, M, MM, CM, M1, M2, M3,
                        res1, res2, one32);

    PSTORE576(C, C1, C2, C3, C4, C5);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_pmul320_gf2x(uint64_t C[10], const uint64_t A[5],
        const uint64_t B[5]) {
    __m128i C1, C2, C3, C4, C5, RA, RB, CL, CH, M, MM, CM, M1, M2, M3, res1, res2, one32;
    one32 = PSET1_32(1);

    PMUL320_WS_KAR_GF2X(C1, C2, C3, C4, C5, A, B, RA, RB, CL, CH, M, MM, CM, M1, M2, M3,
                        res1, res2, one32);

    PSTORE640(C, C1, C2, C3, C4, C5);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_pmul352_gf2x(uint64_t C[11], const uint64_t A[6],
        const uint64_t B[6]) {
    __m128i C1, C2, C3, C4, C5, C6, RA, RB, CL, CH, M, MM, CM, M1, M2, M3, res1, res2, one32;
    one32 = PSET1_32(1);

    PMUL384_WS_KAR_GF2X(C1, C2, C3, C4, C5, C6, A, B, RA, RB, CL, CH, M, MM, CM, M1, M2, M3,
                        res1, res2, one32)

    PSTORE704(C, C1, C2, C3, C4, C5, C6);
}


// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_pmul384_gf2x(uint64_t C[12], const uint64_t A[6],
        const uint64_t B[6]) {
    __m128i C1, C2, C3, C4, C5, C6, RA, RB, CL, CH, M, MM, CM, M1, M2, M3, res1, res2, one32;
    one32 = PSET1_32(1);

    PMUL384_WS_KAR_GF2X(C1, C2, C3, C4, C5, C6, A, B, RA, RB, CL, CH, M, MM, CM, M1, M2, M3,
                        res1, res2, one32)

    PSTORE768(C, C1, C2, C3, C4, C5, C6);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_pmul416_gf2x(uint64_t C[13], const uint64_t A[7],
        const uint64_t B[7]) {
    __m128i C1, C2, C3, C4, C5, C6, C7, RA, RB, CL, CH, M, MM, CM, CM1, CM2,
            M1, M2, M3, M4, one32;
    one32 = PSET1_32(1);

    PMUL416_WS_KAR_GF2X(C1, C2, C3, C4, C5, C6, C7, A, B, RA, RB, CL, CH, M, MM, CM, CM1,
                        CM2, M1, M2, M3, M4, one32)

    PSTORE832(C, C1, C2, C3, C4, C5, C6, C7);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_pmul448_gf2x(uint64_t C[14], const uint64_t A[7],
        const uint64_t B[7]) {
    __m128i C1, C2, C3, C4, C5, C6, C7, RA, RB, CL, CH, M, MM, CM, CM1, CM2,
            M1, M2, M3, M4, one32;
    one32 = PSET1_32(1);

    PMUL448_WS_KAR_GF2X(C1, C2, C3, C4, C5, C6, C7, A, B, RA, RB, CL, CH, M, MM, CM, CM1,
                        CM2, M1, M2, M3, M4, one32)

    PSTORE896(C, C1, C2, C3, C4, C5, C6, C7);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_pmul480_gf2x(uint64_t C[15], const uint64_t A[8],
        const uint64_t B[8]) {
    __m128i C1, C2, C3, C4, C5, C6, C7, C8, RA, RB, CL, CH, M, MM, CM, CM1, CM2,
            M1, M2, M3, M4, one32;
    one32 = PSET1_32(1);

    PMUL480_WS_KAR_GF2X(C1, C2, C3, C4, C5, C6, C7, C8, A, B, RA, RB, CL, CH, M, MM, CM, CM1,
                        CM2, M1, M2, M3, M4, one32)

    PSTORE960(C, C1, C2, C3, C4, C5, C6, C7, C8);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_pmul512_gf2x(uint64_t C[16], const uint64_t A[8],
        const uint64_t B[8]) {
    __m128i C1, C2, C3, C4, C5, C6, C7, C8, RA, RB, CL, CH, M, MM, CM, CM1, CM2,
            M1, M2, M3, M4, one32;
    one32 = PSET1_32(1);

    PMUL512_WS_KAR_GF2X(C1, C2, C3, C4, C5, C6, C7, C8, A, B, RA, RB, CL, CH, M, MM, CM, CM1,
                        CM2, M1, M2, M3, M4, one32)

    PSTORE1024(C, C1, C2, C3, C4, C5, C6, C7, C8);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_pmul544_gf2x(uint64_t C[17], const uint64_t A[9],
        const uint64_t B[9]) {
    __m128i C1, C2, C3, C4, C5, C6, C7, C8, C9, RA, RB, CL, CH, M, MM, CM, CM1, CM2, CM3,
            M1, M2, M3, M4, M5, res1, res2, one32;
    one32 = PSET1_32(1);

    PMUL544_WS_KAR_GF2X(C1, C2, C3, C4, C5, C6, C7, C8, C9, A, B, RA, RB, CL, CH, M, MM, CM, CM1,
                        CM2, CM3, M1, M2, M3, M4, M5, res1, res2, one32);

    PSTORE1088(C, C1, C2, C3, C4, C5, C6, C7, C8, C9);
}

// NOLINTNEXTLINE(google-readability-function-size)
void PQCLEAN_GEMSSBLUE256_AVX2_pmul576_gf2x(uint64_t C[18], const uint64_t A[9],
        const uint64_t B[9]) {
    __m128i C1, C2, C3, C4, C5, C6, C7, C8, C9, RA, RB, CL, CH, M, MM, CM, CM1, CM2, CM3,
            M1, M2, M3, M4, M5, res1, res2, one32;
    one32 = PSET1_32(1);

    PMUL576_WS_KAR_GF2X(C1, C2, C3, C4, C5, C6, C7, C8, C9, A, B, RA, RB, CL, CH, M, MM, CM, CM1,
                        CM2, CM3, M1, M2, M3, M4, M5, res1, res2, one32);

    PSTORE1152(C, C1, C2, C3, C4, C5, C6, C7, C8, C9);
}


/***********************************************************************/
/***********************************************************************/
/**************************** pclmul version ***************************/
/***********************************************************************/
/***********************************************************************/


uint64_t PQCLEAN_GEMSSBLUE256_AVX2_pclmul64low_gf2x(uint64_t A, uint64_t B) {
    __m128i x, y;
    uint64_t C;
    PINIT64(x, &A);
    PINIT64(y, &B);
    PCLMUL64LOW_GF2X(C, x, y, 0);
    return C;
}

void PQCLEAN_GEMSSBLUE256_AVX2_pclmul64_gf2x(uint64_t C[2], const uint64_t A[1],
        const uint64_t B[1]) {
    __m128i x, y, z;
    PINIT64(x, A);
    PINIT64(y, B);
    PCLMUL64_GF2X(C, x, y, z, 0);
}

void PQCLEAN_GEMSSBLUE256_AVX2_pclmul96_gf2x(uint64_t C[3], const uint64_t A[2],
        const uint64_t B[2]) {
    __m128i x, y, z1, z2, sum, res_low, res_high;
    PINIT128(x, A);
    PINIT128(y, B);
    PCLMUL96_GF2X(C, x, y, z1, z2, sum, res_low, res_high);
}

void PQCLEAN_GEMSSBLUE256_AVX2_pclmul128_gf2x(uint64_t C[4], const uint64_t A[2],
        const uint64_t B[2]) {
    __m128i x, y, z1, z2, sum, res_low, res_high;
    PINIT128(x, A);
    PINIT128(y, B);
    PCLMUL128_GF2X(C, x, y, z1, z2, sum, res_low, res_high);
}

void PQCLEAN_GEMSSBLUE256_AVX2_pclmul160_gf2x(uint64_t C[5], const uint64_t A[3],
        const uint64_t B[3]) {
    __m128i x1, x2, y1, y2, z1, z2, sum, res1, res2;
    PINIT192(x1, x2, A);
    PINIT192(y1, y2, B);
    PCLMUL160_GF2X(C, x1, x2, y1, y2, z1, z2, sum, res1, res2);
}

void PQCLEAN_GEMSSBLUE256_AVX2_pclmul192_gf2x(uint64_t C[6], const uint64_t A[3],
        const uint64_t B[3]) {
    __m128i x1, x2, y1, y2, z1, z2, sum, res1, res2;
    PINIT192(x1, x2, A);
    PINIT192(y1, y2, B);
    PCLMUL192_GF2X(C, x1, x2, y1, y2, z1, z2, sum, res1, res2);
}

void PQCLEAN_GEMSSBLUE256_AVX2_pclmul224_gf2x(uint64_t C[7], const uint64_t A[4],
        const uint64_t B[4]) {
    __m128i x1, x2, y1, y2, z1, z2, sum, res1, res2;
    PINIT256(x1, x2, A);
    PINIT256(y1, y2, B);
    PCLMUL224_GF2X(C, x1, x2, y1, y2, z1, z2, sum, res1, res2);
}

void PQCLEAN_GEMSSBLUE256_AVX2_pclmul256_gf2x(uint64_t C[8], const uint64_t A[4],
        const uint64_t B[4]) {
    __m128i x1, x2, y1, y2, z1, z2, sum, res1, res2;
    PINIT256(x1, x2, A);
    PINIT256(y1, y2, B);
    PCLMUL256_GF2X(C, x1, x2, y1, y2, z1, z2, sum, res1, res2);
}

void PQCLEAN_GEMSSBLUE256_AVX2_pclmul288_gf2x(uint64_t C[9], const uint64_t A[5],
        const uint64_t B[5]) {
    __m128i x1, x2, x3, y1, y2, y3, z1, z2, sum, res1, res2;
    PINIT320(x1, x2, x3, A);
    PINIT320(y1, y2, y3, B);
    PCLMUL288_GF2X(C, x1, x2, x3, y1, y2, y3, z1, z2, sum, res1, res2);
}

void PQCLEAN_GEMSSBLUE256_AVX2_pclmul320_gf2x(uint64_t C[10], const uint64_t A[5],
        const uint64_t B[5]) {
    __m128i x1, x2, x3, y1, y2, y3, z1, z2, sum, res1, res2;
    PINIT320(x1, x2, x3, A);
    PINIT320(y1, y2, y3, B);
    PCLMUL320_GF2X(C, x1, x2, x3, y1, y2, y3, z1, z2, sum, res1, res2);
}

void PQCLEAN_GEMSSBLUE256_AVX2_pclmul352_gf2x(uint64_t C[11], const uint64_t A[6],
        const uint64_t B[6]) {
    __m128i x1, x2, x3, y1, y2, y3, z1, z2, sum, res1, res2;
    PINIT384(x1, x2, x3, A);
    PINIT384(y1, y2, y3, B);
    PCLMUL352_GF2X(C, x1, x2, x3, y1, y2, y3, z1, z2, sum, res1, res2);
}

void PQCLEAN_GEMSSBLUE256_AVX2_pclmul384_gf2x(uint64_t C[12], const uint64_t A[6],
        const uint64_t B[6]) {
    __m128i x1, x2, x3, y1, y2, y3, z1, z2, sum, res1, res2;
    PINIT384(x1, x2, x3, A);
    PINIT384(y1, y2, y3, B);
    PCLMUL384_GF2X(C, x1, x2, x3, y1, y2, y3, z1, z2, sum, res1, res2);
}

void PQCLEAN_GEMSSBLUE256_AVX2_pclmul416_gf2x(uint64_t C[13], const uint64_t A[7],
        const uint64_t B[7]) {
    __m128i x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, sum, res1, res2;
    PINIT448(x1, x2, x3, x4, A);
    PINIT448(y1, y2, y3, y4, B);
    PCLMUL416_GF2X(C, x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, sum, res1, res2);
}

void PQCLEAN_GEMSSBLUE256_AVX2_pclmul448_gf2x(uint64_t C[14], const uint64_t A[7],
        const uint64_t B[7]) {
    __m128i x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, sum, res1, res2;
    PINIT448(x1, x2, x3, x4, A);
    PINIT448(y1, y2, y3, y4, B);
    PCLMUL448_GF2X(C, x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, sum, res1, res2);
}

void PQCLEAN_GEMSSBLUE256_AVX2_pclmul480_gf2x(uint64_t C[15], const uint64_t A[8],
        const uint64_t B[8]) {
    __m128i x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, sum, res1, res2;
    PINIT512(x1, x2, x3, x4, A);
    PINIT512(y1, y2, y3, y4, B);
    PCLMUL480_GF2X(C, x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, sum, res1, res2);
}

void PQCLEAN_GEMSSBLUE256_AVX2_pclmul512_gf2x(uint64_t C[16], const uint64_t A[8],
        const uint64_t B[8]) {
    __m128i x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, sum, res1, res2;
    PINIT512(x1, x2, x3, x4, A);
    PINIT512(y1, y2, y3, y4, B);
    PCLMUL512_GF2X(C, x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, sum, res1, res2);
}

void PQCLEAN_GEMSSBLUE256_AVX2_pclmul544_gf2x(uint64_t C[17], const uint64_t A[9],
        const uint64_t B[9]) {
    __m128i x1, x2, x3, x4, x5, y1, y2, y3, y4, y5, z1, z2, sum, res1, res2;
    PINIT576(x1, x2, x3, x4, x5, A);
    PINIT576(y1, y2, y3, y4, y5, B);
    PCLMUL544_GF2X(C, x1, x2, x3, x4, x5, y1, y2, y3, y4, y5, z1, z2, sum, res1, res2);
}

void PQCLEAN_GEMSSBLUE256_AVX2_pclmul576_gf2x(uint64_t C[18], const uint64_t A[9],
        const uint64_t B[9]) {
    __m128i x1, x2, x3, x4, x5, y1, y2, y3, y4, y5, z1, z2, sum, res1, res2;
    PINIT576(x1, x2, x3, x4, x5, A);
    PINIT576(y1, y2, y3, y4, y5, B);
    PCLMUL576_GF2X(C, x1, x2, x3, x4, x5, y1, y2, y3, y4, y5, z1, z2, sum, res1, res2);
}
