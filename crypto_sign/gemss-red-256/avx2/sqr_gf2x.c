#include "simd.h"
#include "sqr_gf2x.h"


/* C=A*A in GF(2)[x] for each function */

/***********************************************************************/
/***********************************************************************/
/********************** version without vectorization ******************/
/***********************************************************************/
/***********************************************************************/


uint64_t PQCLEAN_GEMSSRED256_AVX2_sqr32_no_simd_gf2x(uint64_t A) {
    uint64_t C;
    SQR32_NO_SIMD_GF2X(C, A);
    return C;
}

uint64_t PQCLEAN_GEMSSRED256_AVX2_sqr64low_no_simd_gf2x(uint64_t A) {
    uint64_t C;
    SQR64LOW_NO_SIMD_GF2X(C, A);
    return C;
}


/***********************************************************************/
/***********************************************************************/
/*************************** sse2 version ******************************/
/***********************************************************************/
/***********************************************************************/


uint64_t PQCLEAN_GEMSSRED256_AVX2_psqr64low_gf2x(uint64_t A) {
    __m128i A1, C1, RA, R1;
    uint64_t C;
    A1 = PSETL_64(A);

    PSQR64_GF2X(C1, A1, RA, R1);

    PCVT_64(C, C1);
    return C;
}

void PQCLEAN_GEMSSRED256_AVX2_psqr64_gf2x(uint64_t C[2], const uint64_t A[1]) {
    __m128i A1, C1, RA, R1;
    PINIT64(A1, A);

    PSQR64_GF2X(C1, A1, RA, R1);

    PSTORE128(C, C1);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr96_gf2x(uint64_t C[3], const uint64_t A[2]) {
    __m128i A1, C1, C2, RA, R1;
    PINIT128(A1, A);

    PSQR128_GF2X(C1, C2, A1, RA, R1);

    PSTORE192(C, C1, C2);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr128_gf2x(uint64_t C[4], const uint64_t A[2]) {
    __m128i A1, C1, C2, RA, R1;
    PINIT128(A1, A);

    PSQR128_GF2X(C1, C2, A1, RA, R1);

    PSTORE256(C, C1, C2);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr160_gf2x(uint64_t C[5], const uint64_t A[3]) {
    __m128i A1, A2, C1, C2, C3, RA, R1;
    PINIT192(A1, A2, A);

    PSQR192_GF2X(C1, C2, C3, A1, A2, RA, R1);

    PSTORE320(C, C1, C2, C3);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr192_gf2x(uint64_t C[6], const uint64_t A[3]) {
    __m128i A1, A2, C1, C2, C3, RA, R1;
    PINIT192(A1, A2, A);

    PSQR192_GF2X(C1, C2, C3, A1, A2, RA, R1);

    PSTORE384(C, C1, C2, C3);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr224_gf2x(uint64_t C[7], const uint64_t A[4]) {
    __m128i A1, A2, C1, C2, C3, C4, RA, R1;
    PINIT256(A1, A2, A);

    PSQR256_GF2X(C1, C2, C3, C4, A1, A2, RA, R1);

    PSTORE448(C, C1, C2, C3, C4);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr256_gf2x(uint64_t C[8], const uint64_t A[4]) {
    __m128i A1, A2, C1, C2, C3, C4, RA, R1;
    PINIT256(A1, A2, A);

    PSQR256_GF2X(C1, C2, C3, C4, A1, A2, RA, R1);

    PSTORE512(C, C1, C2, C3, C4);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr288_gf2x(uint64_t C[9], const uint64_t A[5]) {
    __m128i A1, A2, A3, C1, C2, C3, C4, C5, RA, R1;
    PINIT320(A1, A2, A3, A);

    PSQR320_GF2X(C1, C2, C3, C4, C5, A1, A2, A3, RA, R1);

    PSTORE576(C, C1, C2, C3, C4, C5);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr320_gf2x(uint64_t C[10], const uint64_t A[5]) {
    __m128i A1, A2, A3, C1, C2, C3, C4, C5, RA, R1;
    PINIT320(A1, A2, A3, A);

    PSQR320_GF2X(C1, C2, C3, C4, C5, A1, A2, A3, RA, R1);

    PSTORE640(C, C1, C2, C3, C4, C5);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr352_gf2x(uint64_t C[11], const uint64_t A[6]) {
    __m128i A1, A2, A3, C1, C2, C3, C4, C5, C6, RA, R1;
    PINIT384(A1, A2, A3, A);

    PSQR384_GF2X(C1, C2, C3, C4, C5, C6, A1, A2, A3, RA, R1);

    PSTORE704(C, C1, C2, C3, C4, C5, C6);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr384_gf2x(uint64_t C[12], const uint64_t A[6]) {
    __m128i A1, A2, A3, C1, C2, C3, C4, C5, C6, RA, R1;
    PINIT384(A1, A2, A3, A);

    PSQR384_GF2X(C1, C2, C3, C4, C5, C6, A1, A2, A3, RA, R1);

    PSTORE768(C, C1, C2, C3, C4, C5, C6);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr416_gf2x(uint64_t C[13], const uint64_t A[7]) {
    __m128i A1, A2, A3, A4, C1, C2, C3, C4, C5, C6, C7, RA, R1;
    PINIT448(A1, A2, A3, A4, A);

    PSQR448_GF2X(C1, C2, C3, C4, C5, C6, C7, A1, A2, A3, A4, RA, R1);

    PSTORE832(C, C1, C2, C3, C4, C5, C6, C7);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr448_gf2x(uint64_t C[14], const uint64_t A[7]) {
    __m128i A1, A2, A3, A4, C1, C2, C3, C4, C5, C6, C7, RA, R1;
    PINIT448(A1, A2, A3, A4, A);

    PSQR448_GF2X(C1, C2, C3, C4, C5, C6, C7, A1, A2, A3, A4, RA, R1);

    PSTORE896(C, C1, C2, C3, C4, C5, C6, C7);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr480_gf2x(uint64_t C[15], const uint64_t A[8]) {
    __m128i A1, A2, A3, A4, C1, C2, C3, C4, C5, C6, C7, C8, RA, R1;
    PINIT512(A1, A2, A3, A4, A);

    PSQR512_GF2X(C1, C2, C3, C4, C5, C6, C7, C8, A1, A2, A3, A4, RA, R1);

    PSTORE960(C, C1, C2, C3, C4, C5, C6, C7, C8);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr512_gf2x(uint64_t C[16], const uint64_t A[8]) {
    __m128i A1, A2, A3, A4, C1, C2, C3, C4, C5, C6, C7, C8, RA, R1;
    PINIT512(A1, A2, A3, A4, A);

    PSQR512_GF2X(C1, C2, C3, C4, C5, C6, C7, C8, A1, A2, A3, A4, RA, R1);

    PSTORE1024(C, C1, C2, C3, C4, C5, C6, C7, C8);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr544_gf2x(uint64_t C[17], const uint64_t A[9]) {
    __m128i A1, A2, A3, A4, A5, C1, C2, C3, C4, C5, C6, C7, C8, C9, RA, R1;
    PINIT576(A1, A2, A3, A4, A5, A);

    PSQR576_GF2X(C1, C2, C3, C4, C5, C6, C7, C8, C9, A1, A2, A3, A4, A5, RA, R1);

    PSTORE1088(C, C1, C2, C3, C4, C5, C6, C7, C8, C9);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr576_gf2x(uint64_t C[18], const uint64_t A[9]) {
    __m128i A1, A2, A3, A4, A5, C1, C2, C3, C4, C5, C6, C7, C8, C9, RA, R1;
    PINIT576(A1, A2, A3, A4, A5, A);

    PSQR576_GF2X(C1, C2, C3, C4, C5, C6, C7, C8, C9, A1, A2, A3, A4, A5, RA, R1);

    PSTORE1152(C, C1, C2, C3, C4, C5, C6, C7, C8, C9);
}


/***********************************************************************/
/***********************************************************************/
/*************************** shuffle version ***************************/
/***********************************************************************/
/***********************************************************************/


uint64_t PQCLEAN_GEMSSRED256_AVX2_psqr64low_shuffle_gf2x(uint64_t A) {
    __m128i M, T, A1, E1;
    uint64_t C;
    PSQR_INIT_SHUFFLE_GF2X(M, T);
    A1 = PSETL_64(A);

    PSQR64_SHUFFLE_GF2X(E1, A1, M, T);

    PCVT_64(C, E1);
    return C;
}

void PQCLEAN_GEMSSRED256_AVX2_psqr64_shuffle_gf2x(uint64_t C[2], const uint64_t A[1]) {
    __m128i M, T, A1, E1;
    PSQR_INIT_SHUFFLE_GF2X(M, T);
    PINIT64(A1, A);

    PSQR64_SHUFFLE_GF2X(E1, A1, M, T);

    PSTORE128(C, E1);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr96_shuffle_gf2x(uint64_t C[3], const uint64_t A[2]) {
    __m128i M, T, A1, E1, E2;
    PSQR_INIT_SHUFFLE_GF2X(M, T);
    PINIT128(A1, A);

    PSQR128_SHUFFLE_GF2X(E1, E2, A1, M, T);

    PSTORE192(C, E1, E2);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr128_shuffle_gf2x(uint64_t C[4], const uint64_t A[2]) {
    __m128i M, T, A1, E1, E2;
    PSQR_INIT_SHUFFLE_GF2X(M, T);
    PINIT128(A1, A);

    PSQR128_SHUFFLE_GF2X(E1, E2, A1, M, T);

    PSTORE256(C, E1, E2);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr160_shuffle_gf2x(uint64_t C[5], const uint64_t A[3]) {
    __m128i M, T, A1, A2, E1, E2, E3;
    PSQR_INIT_SHUFFLE_GF2X(M, T);
    PINIT192(A1, A2, A);

    PSQR192_SHUFFLE_GF2X(E1, E2, E3, A1, A2, M, T);

    PSTORE320(C, E1, E2, E3);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr192_shuffle_gf2x(uint64_t C[6], const uint64_t A[3]) {
    __m128i M, T, A1, A2, E1, E2, E3;
    PSQR_INIT_SHUFFLE_GF2X(M, T);
    PINIT192(A1, A2, A);

    PSQR192_SHUFFLE_GF2X(E1, E2, E3, A1, A2, M, T);

    PSTORE384(C, E1, E2, E3);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr224_shuffle_gf2x(uint64_t C[7], const uint64_t A[4]) {
    __m128i M, T, A1, A2, E1, E2, E3, E4;
    PSQR_INIT_SHUFFLE_GF2X(M, T);
    PINIT256(A1, A2, A);

    PSQR256_SHUFFLE_GF2X(E1, E2, E3, E4, A1, A2, M, T);

    PSTORE448(C, E1, E2, E3, E4);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr256_shuffle_gf2x(uint64_t C[8], const uint64_t A[4]) {
    __m128i M, T, A1, A2, E1, E2, E3, E4;
    PSQR_INIT_SHUFFLE_GF2X(M, T);
    PINIT256(A1, A2, A);

    PSQR256_SHUFFLE_GF2X(E1, E2, E3, E4, A1, A2, M, T);

    PSTORE512(C, E1, E2, E3, E4);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr288_shuffle_gf2x(uint64_t C[9], const uint64_t A[5]) {
    __m128i M, T, A1, A2, A3, E1, E2, E3, E4, E5;
    PSQR_INIT_SHUFFLE_GF2X(M, T);
    PINIT320(A1, A2, A3, A);

    PSQR320_SHUFFLE_GF2X(E1, E2, E3, E4, E5, A1, A2, A3, M, T);

    PSTORE576(C, E1, E2, E3, E4, E5);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr320_shuffle_gf2x(uint64_t C[10], const uint64_t A[5]) {
    __m128i M, T, A1, A2, A3, E1, E2, E3, E4, E5;
    PSQR_INIT_SHUFFLE_GF2X(M, T);
    PINIT320(A1, A2, A3, A);

    PSQR320_SHUFFLE_GF2X(E1, E2, E3, E4, E5, A1, A2, A3, M, T);

    PSTORE640(C, E1, E2, E3, E4, E5);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr352_shuffle_gf2x(uint64_t C[11], const uint64_t A[6]) {
    __m128i M, T, A1, A2, A3, E1, E2, E3, E4, E5, E6;
    PSQR_INIT_SHUFFLE_GF2X(M, T);
    PINIT384(A1, A2, A3, A);

    PSQR384_SHUFFLE_GF2X(E1, E2, E3, E4, E5, E6, A1, A2, A3, M, T);

    PSTORE704(C, E1, E2, E3, E4, E5, E6);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr384_shuffle_gf2x(uint64_t C[12], const uint64_t A[6]) {
    __m128i M, T, A1, A2, A3, E1, E2, E3, E4, E5, E6;
    PSQR_INIT_SHUFFLE_GF2X(M, T);
    PINIT384(A1, A2, A3, A);

    PSQR384_SHUFFLE_GF2X(E1, E2, E3, E4, E5, E6, A1, A2, A3, M, T);

    PSTORE768(C, E1, E2, E3, E4, E5, E6);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr416_shuffle_gf2x(uint64_t C[13], const uint64_t A[7]) {
    __m128i M, T, A1, A2, A3, A4, E1, E2, E3, E4, E5, E6, E7;
    PSQR_INIT_SHUFFLE_GF2X(M, T);
    PINIT448(A1, A2, A3, A4, A);

    PSQR448_SHUFFLE_GF2X(E1, E2, E3, E4, E5, E6, E7, A1, A2, A3, A4, M, T);

    PSTORE832(C, E1, E2, E3, E4, E5, E6, E7);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr448_shuffle_gf2x(uint64_t C[14], const uint64_t A[7]) {
    __m128i M, T, A1, A2, A3, A4, E1, E2, E3, E4, E5, E6, E7;
    PSQR_INIT_SHUFFLE_GF2X(M, T);
    PINIT448(A1, A2, A3, A4, A);

    PSQR448_SHUFFLE_GF2X(E1, E2, E3, E4, E5, E6, E7, A1, A2, A3, A4, M, T);

    PSTORE896(C, E1, E2, E3, E4, E5, E6, E7);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr480_shuffle_gf2x(uint64_t C[15], const uint64_t A[8]) {
    __m128i M, T, A1, A2, A3, A4, E1, E2, E3, E4, E5, E6, E7, E8;
    PSQR_INIT_SHUFFLE_GF2X(M, T);
    PINIT512(A1, A2, A3, A4, A);

    PSQR512_SHUFFLE_GF2X(E1, E2, E3, E4, E5, E6, E7, E8, A1, A2, A3, A4, M, T);

    PSTORE960(C, E1, E2, E3, E4, E5, E6, E7, E8);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr512_shuffle_gf2x(uint64_t C[16], const uint64_t A[8]) {
    __m128i M, T, A1, A2, A3, A4, E1, E2, E3, E4, E5, E6, E7, E8;
    PSQR_INIT_SHUFFLE_GF2X(M, T);
    PINIT512(A1, A2, A3, A4, A);

    PSQR512_SHUFFLE_GF2X(E1, E2, E3, E4, E5, E6, E7, E8, A1, A2, A3, A4, M, T);

    PSTORE1024(C, E1, E2, E3, E4, E5, E6, E7, E8);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr544_shuffle_gf2x(uint64_t C[17], const uint64_t A[9]) {
    __m128i M, T, A1, A2, A3, A4, A5, E1, E2, E3, E4, E5, E6, E7, E8, E9;
    PSQR_INIT_SHUFFLE_GF2X(M, T);
    PINIT576(A1, A2, A3, A4, A5, A);

    PSQR576_SHUFFLE_GF2X(E1, E2, E3, E4, E5, E6, E7, E8, E9, A1, A2, A3, A4, A5, M, T);

    PSTORE1088(C, E1, E2, E3, E4, E5, E6, E7, E8, E9);
}

void PQCLEAN_GEMSSRED256_AVX2_psqr576_shuffle_gf2x(uint64_t C[18], const uint64_t A[9]) {
    __m128i M, T, A1, A2, A3, A4, A5, E1, E2, E3, E4, E5, E6, E7, E8, E9;
    PSQR_INIT_SHUFFLE_GF2X(M, T);
    PINIT576(A1, A2, A3, A4, A5, A);

    PSQR576_SHUFFLE_GF2X(E1, E2, E3, E4, E5, E6, E7, E8, E9, A1, A2, A3, A4, A5, M, T);

    PSTORE1152(C, E1, E2, E3, E4, E5, E6, E7, E8, E9);
}


uint64_t PQCLEAN_GEMSSRED256_AVX2_vpsqr64low_shuffle_gf2x(uint64_t A) {
    __m256i M, T, A1, E1;
    VPSQR_INIT_SHUFFLE_GF2X(M, T);

    A1 = VPSETL_64(A);
    VPSQR64_SHUFFLE_GF2X(E1, A1, M, T);

    return VPEXTRACT64(E1, 0);
}

void PQCLEAN_GEMSSRED256_AVX2_vpsqr64_shuffle_gf2x(uint64_t C[2], const uint64_t A[1]) {
    __m256i M, T, A1, E1;
    VPSQR_INIT_SHUFFLE_GF2X(M, T);

    VPINIT64(A1, A);
    VPSQR64_SHUFFLE_GF2X(E1, A1, M, T);

    VPSTORE128(C, E1);
}

void PQCLEAN_GEMSSRED256_AVX2_vpsqr96_shuffle_gf2x(uint64_t C[3], const uint64_t A[2]) {
    __m256i M, T, A1, E1;
    VPSQR_INIT_SHUFFLE_GF2X(M, T);

    VPINIT128(A1, A);
    VPSQR128_SHUFFLE_GF2X(E1, A1, M, T);

    VPSTORE192(C, E1);
}

void PQCLEAN_GEMSSRED256_AVX2_vpsqr128_shuffle_gf2x(uint64_t C[4], const uint64_t A[2]) {
    __m256i M, T, A1, E1;
    VPSQR_INIT_SHUFFLE_GF2X(M, T);

    VPINIT128(A1, A);
    VPSQR128_SHUFFLE_GF2X(E1, A1, M, T);

    VPSTORE256(C, E1);
}

void PQCLEAN_GEMSSRED256_AVX2_vpsqr160_shuffle_gf2x(uint64_t C[5], const uint64_t A[3]) {
    __m256i M, T, A1, E1, E2;
    VPSQR_INIT_SHUFFLE_GF2X(M, T);

    VPINIT192(A1, A);
    VPSQR192_SHUFFLE_GF2X(E1, E2, A1, M, T);

    VPSTORE320(C, E1, E2);
}

void PQCLEAN_GEMSSRED256_AVX2_vpsqr192_shuffle_gf2x(uint64_t C[6], const uint64_t A[3]) {
    __m256i M, T, A1, E1, E2;
    VPSQR_INIT_SHUFFLE_GF2X(M, T);

    VPINIT192(A1, A);
    VPSQR192_SHUFFLE_GF2X(E1, E2, A1, M, T);

    VPSTORE384(C, E1, E2);
}

void PQCLEAN_GEMSSRED256_AVX2_vpsqr224_shuffle_gf2x(uint64_t C[7], const uint64_t A[4]) {
    __m256i M, T, A1, E1, E2;
    VPSQR_INIT_SHUFFLE_GF2X(M, T);

    VPINIT256(A1, A);
    VPSQR256_SHUFFLE_GF2X(E1, E2, A1, M, T);

    VPSTORE448(C, E1, E2);
}

void PQCLEAN_GEMSSRED256_AVX2_vpsqr256_shuffle_gf2x(uint64_t C[8], const uint64_t A[4]) {
    __m256i M, T, A1, E1, E2;
    VPSQR_INIT_SHUFFLE_GF2X(M, T);

    VPINIT256(A1, A);
    VPSQR256_SHUFFLE_GF2X(E1, E2, A1, M, T);

    VPSTORE512(C, E1, E2);
}

void PQCLEAN_GEMSSRED256_AVX2_vpsqr288_shuffle_gf2x(uint64_t C[9], const uint64_t A[5]) {
    __m256i M, T, A1, A2, E1, E2, E3;
    VPSQR_INIT_SHUFFLE_GF2X(M, T);

    VPINIT320(A1, A2, A);
    VPSQR320_SHUFFLE_GF2X(E1, E2, E3, A1, A2, M, T);

    VPSTORE576(C, E1, E2, E3);
}

void PQCLEAN_GEMSSRED256_AVX2_vpsqr320_shuffle_gf2x(uint64_t C[10], const uint64_t A[5]) {
    __m256i M, T, A1, A2, E1, E2, E3;
    VPSQR_INIT_SHUFFLE_GF2X(M, T);

    VPINIT320(A1, A2, A);
    VPSQR320_SHUFFLE_GF2X(E1, E2, E3, A1, A2, M, T);

    VPSTORE640(C, E1, E2, E3);
}

void PQCLEAN_GEMSSRED256_AVX2_vpsqr352_shuffle_gf2x(uint64_t C[11], const uint64_t A[6]) {
    __m256i M, T, A1, A2, E1, E2, E3;
    VPSQR_INIT_SHUFFLE_GF2X(M, T);

    VPINIT384(A1, A2, A);
    VPSQR384_SHUFFLE_GF2X(E1, E2, E3, A1, A2, M, T);

    VPSTORE704(C, E1, E2, E3);
}


void PQCLEAN_GEMSSRED256_AVX2_vpsqr384_shuffle_gf2x(uint64_t C[12], const uint64_t A[6]) {
    __m256i M, T, A1, A2, E1, E2, E3;
    VPSQR_INIT_SHUFFLE_GF2X(M, T);

    VPINIT384(A1, A2, A);
    VPSQR384_SHUFFLE_GF2X(E1, E2, E3, A1, A2, M, T);

    VPSTORE768(C, E1, E2, E3);
}

void PQCLEAN_GEMSSRED256_AVX2_vpsqr416_shuffle_gf2x(uint64_t C[13], const uint64_t A[7]) {
    __m256i M, T, A1, A2, E1, E2, E3, E4;
    VPSQR_INIT_SHUFFLE_GF2X(M, T);

    VPINIT448(A1, A2, A);
    VPSQR448_SHUFFLE_GF2X(E1, E2, E3, E4, A1, A2, M, T);

    VPSTORE832(C, E1, E2, E3, E4);
}

void PQCLEAN_GEMSSRED256_AVX2_vpsqr448_shuffle_gf2x(uint64_t C[14], const uint64_t A[7]) {
    __m256i M, T, A1, A2, E1, E2, E3, E4;
    VPSQR_INIT_SHUFFLE_GF2X(M, T);

    VPINIT448(A1, A2, A);
    VPSQR448_SHUFFLE_GF2X(E1, E2, E3, E4, A1, A2, M, T);

    VPSTORE896(C, E1, E2, E3, E4);
}

void PQCLEAN_GEMSSRED256_AVX2_vpsqr480_shuffle_gf2x(uint64_t C[15], const uint64_t A[8]) {
    __m256i M, T, A1, A2, E1, E2, E3, E4;
    VPSQR_INIT_SHUFFLE_GF2X(M, T);

    VPINIT512(A1, A2, A);
    VPSQR512_SHUFFLE_GF2X(E1, E2, E3, E4, A1, A2, M, T);

    VPSTORE960(C, E1, E2, E3, E4);
}

void PQCLEAN_GEMSSRED256_AVX2_vpsqr512_shuffle_gf2x(uint64_t C[16], const uint64_t A[8]) {
    __m256i M, T, A1, A2, E1, E2, E3, E4;
    VPSQR_INIT_SHUFFLE_GF2X(M, T);

    VPINIT512(A1, A2, A);
    VPSQR512_SHUFFLE_GF2X(E1, E2, E3, E4, A1, A2, M, T);

    VPSTORE1024(C, E1, E2, E3, E4);
}

void PQCLEAN_GEMSSRED256_AVX2_vpsqr544_shuffle_gf2x(uint64_t C[17], const uint64_t A[9]) {
    __m256i M, T, A1, A2, A3, E1, E2, E3, E4, E5;
    VPSQR_INIT_SHUFFLE_GF2X(M, T);

    VPINIT576(A1, A2, A3, A);
    VPSQR576_SHUFFLE_GF2X(E1, E2, E3, E4, E5, A1, A2, A3, M, T);

    VPSTORE1088(C, E1, E2, E3, E4, E5);
}

void PQCLEAN_GEMSSRED256_AVX2_vpsqr576_shuffle_gf2x(uint64_t C[18], const uint64_t A[9]) {
    __m256i M, T, A1, A2, A3, E1, E2, E3, E4, E5;
    VPSQR_INIT_SHUFFLE_GF2X(M, T);

    VPINIT576(A1, A2, A3, A);
    VPSQR576_SHUFFLE_GF2X(E1, E2, E3, E4, E5, A1, A2, A3, M, T);

    VPSTORE1152(C, E1, E2, E3, E4, E5);
}


/***********************************************************************/
/***********************************************************************/
/**************************** pclmul version ***************************/
/***********************************************************************/
/***********************************************************************/


/***********************************************************************/
/***********************************************************************/
/********************** store at the end version ***********************/
/***********************************************************************/
/***********************************************************************/


uint64_t PQCLEAN_GEMSSRED256_AVX2_sqr64low_pclmul_gf2x(uint64_t A) {
    __m128i x;
    uint64_t C;
    PINIT64(x, &A);

    PCVT_64(C, PCLMUL(x, x, 0));
    return C;
}

void PQCLEAN_GEMSSRED256_AVX2_sqr64_pclmul_gf2x(uint64_t C[2], const uint64_t A[1]) {
    __m128i x, z;
    PINIT64(x, A);

    SQR64_WS_PCLMUL_GF2X(z, x, 0);

    PSTORE128(C, z);
}

void PQCLEAN_GEMSSRED256_AVX2_sqr96_pclmul_gf2x(uint64_t C[3], const uint64_t A[2]) {
    __m128i x, z1, z2;
    PINIT128(x, A);

    SQR128_WS_PCLMUL_GF2X(z1, z2, x);

    PSTORE192(C, z1, z2);
}

void PQCLEAN_GEMSSRED256_AVX2_sqr128_pclmul_gf2x(uint64_t C[4], const uint64_t A[2]) {
    __m128i x, z1, z2;
    PINIT128(x, A);

    SQR128_WS_PCLMUL_GF2X(z1, z2, x);

    PSTORE256(C, z1, z2);
}

void PQCLEAN_GEMSSRED256_AVX2_sqr160_pclmul_gf2x(uint64_t C[5], const uint64_t A[3]) {
    __m128i x1, x2, z1, z2, z3;
    PINIT192(x1, x2, A);

    SQR192_WS_PCLMUL_GF2X(z1, z2, z3, x1, x2);

    PSTORE320(C, z1, z2, z3);
}

void PQCLEAN_GEMSSRED256_AVX2_sqr192_pclmul_gf2x(uint64_t C[6], const uint64_t A[3]) {
    __m128i x1, x2, z1, z2, z3;
    PINIT192(x1, x2, A);

    SQR192_WS_PCLMUL_GF2X(z1, z2, z3, x1, x2);

    PSTORE384(C, z1, z2, z3);
}

void PQCLEAN_GEMSSRED256_AVX2_sqr224_pclmul_gf2x(uint64_t C[7], const uint64_t A[4]) {
    __m128i x1, x2, z1, z2, z3, z4;
    PINIT256(x1, x2, A);

    SQR256_WS_PCLMUL_GF2X(z1, z2, z3, z4, x1, x2);

    PSTORE448(C, z1, z2, z3, z4);
}

void PQCLEAN_GEMSSRED256_AVX2_sqr256_pclmul_gf2x(uint64_t C[8], const uint64_t A[4]) {
    __m128i x1, x2, z1, z2, z3, z4;
    PINIT256(x1, x2, A);

    SQR256_WS_PCLMUL_GF2X(z1, z2, z3, z4, x1, x2);

    PSTORE512(C, z1, z2, z3, z4);
}

void PQCLEAN_GEMSSRED256_AVX2_sqr288_pclmul_gf2x(uint64_t C[9], const uint64_t A[5]) {
    __m128i x1, x2, x3, z1, z2, z3, z4, z5;
    PINIT320(x1, x2, x3, A);

    SQR320_WS_PCLMUL_GF2X(z1, z2, z3, z4, z5, x1, x2, x3);

    PSTORE576(C, z1, z2, z3, z4, z5);
}

void PQCLEAN_GEMSSRED256_AVX2_sqr320_pclmul_gf2x(uint64_t C[10], const uint64_t A[5]) {
    __m128i x1, x2, x3, z1, z2, z3, z4, z5;
    PINIT320(x1, x2, x3, A);

    SQR320_WS_PCLMUL_GF2X(z1, z2, z3, z4, z5, x1, x2, x3);

    PSTORE640(C, z1, z2, z3, z4, z5);
}

void PQCLEAN_GEMSSRED256_AVX2_sqr352_pclmul_gf2x(uint64_t C[11], const uint64_t A[6]) {
    __m128i x1, x2, x3, z1, z2, z3, z4, z5, z6;
    PINIT384(x1, x2, x3, A);

    SQR384_WS_PCLMUL_GF2X(z1, z2, z3, z4, z5, z6, x1, x2, x3);

    PSTORE704(C, z1, z2, z3, z4, z5, z6);
}

void PQCLEAN_GEMSSRED256_AVX2_sqr384_pclmul_gf2x(uint64_t C[12], const uint64_t A[6]) {
    __m128i x1, x2, x3, z1, z2, z3, z4, z5, z6;
    PINIT384(x1, x2, x3, A);

    SQR384_WS_PCLMUL_GF2X(z1, z2, z3, z4, z5, z6, x1, x2, x3);

    PSTORE768(C, z1, z2, z3, z4, z5, z6);
}

void PQCLEAN_GEMSSRED256_AVX2_sqr416_pclmul_gf2x(uint64_t C[13], const uint64_t A[7]) {
    __m128i x1, x2, x3, x4, z1, z2, z3, z4, z5, z6, z7;
    PINIT448(x1, x2, x3, x4, A);

    SQR448_WS_PCLMUL_GF2X(z1, z2, z3, z4, z5, z6, z7, x1, x2, x3, x4);

    PSTORE832(C, z1, z2, z3, z4, z5, z6, z7);
}

void PQCLEAN_GEMSSRED256_AVX2_sqr448_pclmul_gf2x(uint64_t C[14], const uint64_t A[7]) {
    __m128i x1, x2, x3, x4, z1, z2, z3, z4, z5, z6, z7;
    PINIT448(x1, x2, x3, x4, A);

    SQR448_WS_PCLMUL_GF2X(z1, z2, z3, z4, z5, z6, z7, x1, x2, x3, x4);

    PSTORE896(C, z1, z2, z3, z4, z5, z6, z7);
}

void PQCLEAN_GEMSSRED256_AVX2_sqr480_pclmul_gf2x(uint64_t C[15], const uint64_t A[8]) {
    __m128i x1, x2, x3, x4, z1, z2, z3, z4, z5, z6, z7, z8;
    PINIT512(x1, x2, x3, x4, A);

    SQR512_WS_PCLMUL_GF2X(z1, z2, z3, z4, z5, z6, z7, z8, x1, x2, x3, x4);

    PSTORE960(C, z1, z2, z3, z4, z5, z6, z7, z8);
}

void PQCLEAN_GEMSSRED256_AVX2_sqr512_pclmul_gf2x(uint64_t C[16], const uint64_t A[8]) {
    __m128i x1, x2, x3, x4, z1, z2, z3, z4, z5, z6, z7, z8;
    PINIT512(x1, x2, x3, x4, A);

    SQR512_WS_PCLMUL_GF2X(z1, z2, z3, z4, z5, z6, z7, z8, x1, x2, x3, x4);

    PSTORE1024(C, z1, z2, z3, z4, z5, z6, z7, z8);
}

void PQCLEAN_GEMSSRED256_AVX2_sqr544_pclmul_gf2x(uint64_t C[17], const uint64_t A[9]) {
    __m128i x1, x2, x3, x4, x5, z1, z2, z3, z4, z5, z6, z7, z8, z9;
    PINIT576(x1, x2, x3, x4, x5, A);

    SQR576_WS_PCLMUL_GF2X(z1, z2, z3, z4, z5, z6, z7, z8, z9, x1, x2, x3, x4, x5);

    PSTORE1088(C, z1, z2, z3, z4, z5, z6, z7, z8, z9);
}

void PQCLEAN_GEMSSRED256_AVX2_sqr576_pclmul_gf2x(uint64_t C[18], const uint64_t A[9]) {
    __m128i x1, x2, x3, x4, x5, z1, z2, z3, z4, z5, z6, z7, z8, z9;
    PINIT576(x1, x2, x3, x4, x5, A);

    SQR576_WS_PCLMUL_GF2X(z1, z2, z3, z4, z5, z6, z7, z8, z9, x1, x2, x3, x4, x5);

    PSTORE1152(C, z1, z2, z3, z4, z5, z6, z7, z8, z9);
}
