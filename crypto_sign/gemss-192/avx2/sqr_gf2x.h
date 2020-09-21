#ifndef _SQR_GF2X_H
#define _SQR_GF2X_H
#include "arch.h"
#include <stdint.h>

/* Squaring in GF(2)[x] */
/* (a+b*x^64)^2 = a^2 + (b^2)*x^128 */
/* The squaring is equivalent to put a zero bit between each bit */


/* Difference between SQR32 and SQR64LOW:
    - SQR32: we assume the 32-bit higher part of A is 0.
    - SQR64LOW: we do not the previous assumption.
*/


/***********************************************************************/
/***********************************************************************/
/********************** version without vectorization ******************/
/***********************************************************************/
/***********************************************************************/


/* Assume we want to insert a null bit between each bit of 0x00000000FFFFFFFF.
   We do as following:
    0x00000000FFFFFFFF (it is already an insertion of a zero 32-bit packed)
    0x0000FFFF0000FFFF (insertion by pack of 16 bits)
    0x00FF00FF00FF00FF (insertion by pack of 8 bits)
    0x0F0F0F0F0F0F0F0F (insertion by pack of 4 bits)
    0x3333333333333333 (insertion by pack of 2 bits)
    0x5555555555555555 (insertion by pack of 1 bit)
*/


uint64_t PQCLEAN_GEMSS192_AVX2_sqr32_no_simd_gf2x(uint64_t A);
uint64_t PQCLEAN_GEMSS192_AVX2_sqr64low_no_simd_gf2x(uint64_t A);


/* log_2(32)*3 = 5*3 = 15 instructions */
#define SQR32_NO_SIMD_GF2X(C,A)     (C)=((A)^(((uint64_t)(A))<<16))&((uint64_t)0x0000FFFF0000FFFF);    (C)=((C)^((C)<<8))&((uint64_t)0x00FF00FF00FF00FF);    (C)=((C)^((C)<<4))&((uint64_t)0x0F0F0F0F0F0F0F0F);    (C)=((C)^((C)<<2))&((uint64_t)0x3333333333333333);    (C)=((C)^((C)<<1))&((uint64_t)0x5555555555555555);

/* 1+log_2(32)*3 = 1+5*3 = 16 instructions */
#define SQR64LOW_NO_SIMD_GF2X(C,A)     (C)=(((A)&((uint64_t)0xFFFFFFFF))^((A)<<16))        &((uint64_t)0x0000FFFF0000FFFF);    (C)=((C)^((C)<<8))&((uint64_t)0x00FF00FF00FF00FF);    (C)=((C)^((C)<<4))&((uint64_t)0x0F0F0F0F0F0F0F0F);    (C)=((C)^((C)<<2))&((uint64_t)0x3333333333333333);    (C)=((C)^((C)<<1))&((uint64_t)0x5555555555555555);

#define SQR64_NO_SIMD_GF2X(C,A)     SQR32_NO_SIMD_GF2X((C)[1],(A)>>32);    SQR64LOW_NO_SIMD_GF2X((C)[0],A);

#define SQR96_NO_SIMD_GF2X(C,A)     SQR32_NO_SIMD_GF2X((C)[2],(A)[1]);    SQR64_NO_SIMD_GF2X(C,(A)[0]);

#define SQR128_NO_SIMD_GF2X(C,A)     SQR64_NO_SIMD_GF2X((C)+2,(A)[1]);    SQR64_NO_SIMD_GF2X(C,(A)[0]);

#define SQR160_NO_SIMD_GF2X(C,A)     SQR32_NO_SIMD_GF2X((C)[4],(A)[2]);    SQR128_NO_SIMD_GF2X(C,A);

#define SQR192_NO_SIMD_GF2X(C,A)     SQR64_NO_SIMD_GF2X((C)+4,(A)[2]);    SQR128_NO_SIMD_GF2X(C,A);

#define SQR224_NO_SIMD_GF2X(C,A)     SQR96_NO_SIMD_GF2X((C)+4,(A)+2);    SQR128_NO_SIMD_GF2X(C,A);

#define SQR256_NO_SIMD_GF2X(C,A)     SQR128_NO_SIMD_GF2X((C)+4,(A)+2);    SQR128_NO_SIMD_GF2X(C,A);

#define SQR288_NO_SIMD_GF2X(C,A)     SQR32_NO_SIMD_GF2X((C)[8],(A)[4]);    SQR256_NO_SIMD_GF2X(C,A);

#define SQR320_NO_SIMD_GF2X(C,A)     SQR64_NO_SIMD_GF2X((C)+8,(A)[4]);    SQR256_NO_SIMD_GF2X(C,A);

#define SQR352_NO_SIMD_GF2X(C,A)     SQR96_NO_SIMD_GF2X((C)+8,(A)+4);    SQR256_NO_SIMD_GF2X(C,A);

#define SQR384_NO_SIMD_GF2X(C,A)     SQR128_NO_SIMD_GF2X((C)+8,(A)+4);    SQR256_NO_SIMD_GF2X(C,A);

#define SQR416_NO_SIMD_GF2X(C,A)     SQR160_NO_SIMD_GF2X((C)+8,(A)+4);    SQR256_NO_SIMD_GF2X(C,A);

#define SQR448_NO_SIMD_GF2X(C,A)     SQR192_NO_SIMD_GF2X((C)+8,(A)+4);    SQR256_NO_SIMD_GF2X(C,A);

#define SQR480_NO_SIMD_GF2X(C,A)     SQR224_NO_SIMD_GF2X((C)+8,(A)+4);    SQR256_NO_SIMD_GF2X(C,A);

#define SQR512_NO_SIMD_GF2X(C,A)     SQR256_NO_SIMD_GF2X((C)+8,(A)+4);    SQR256_NO_SIMD_GF2X(C,A);

#define SQR544_NO_SIMD_GF2X(C,A)     SQR32_NO_SIMD_GF2X((C)[16],(A)[8]);    SQR512_NO_SIMD_GF2X(C,A);

#define SQR576_NO_SIMD_GF2X(C,A)     SQR64_NO_SIMD_GF2X((C)+16,(A)[8]);    SQR512_NO_SIMD_GF2X(C,A);


/***********************************************************************/
/***********************************************************************/
/*************************** sse2 version ******************************/
/***********************************************************************/
/***********************************************************************/


uint64_t PQCLEAN_GEMSS192_AVX2_psqr64low_gf2x(uint64_t A);
void PQCLEAN_GEMSS192_AVX2_psqr64_gf2x(uint64_t C[2], const uint64_t A[1]);
void PQCLEAN_GEMSS192_AVX2_psqr96_gf2x(uint64_t C[3], const uint64_t A[2]);
void PQCLEAN_GEMSS192_AVX2_psqr128_gf2x(uint64_t C[4], const uint64_t A[2]);
void PQCLEAN_GEMSS192_AVX2_psqr160_gf2x(uint64_t C[5], const uint64_t A[3]);
void PQCLEAN_GEMSS192_AVX2_psqr192_gf2x(uint64_t C[6], const uint64_t A[3]);
void PQCLEAN_GEMSS192_AVX2_psqr224_gf2x(uint64_t C[7], const uint64_t A[4]);
void PQCLEAN_GEMSS192_AVX2_psqr256_gf2x(uint64_t C[8], const uint64_t A[4]);
void PQCLEAN_GEMSS192_AVX2_psqr288_gf2x(uint64_t C[9], const uint64_t A[5]);
void PQCLEAN_GEMSS192_AVX2_psqr320_gf2x(uint64_t C[10], const uint64_t A[5]);
void PQCLEAN_GEMSS192_AVX2_psqr352_gf2x(uint64_t C[11], const uint64_t A[6]);
void PQCLEAN_GEMSS192_AVX2_psqr384_gf2x(uint64_t C[12], const uint64_t A[6]);
void PQCLEAN_GEMSS192_AVX2_psqr416_gf2x(uint64_t C[13], const uint64_t A[7]);
void PQCLEAN_GEMSS192_AVX2_psqr448_gf2x(uint64_t C[14], const uint64_t A[7]);
void PQCLEAN_GEMSS192_AVX2_psqr480_gf2x(uint64_t C[15], const uint64_t A[8]);
void PQCLEAN_GEMSS192_AVX2_psqr512_gf2x(uint64_t C[16], const uint64_t A[8]);
void PQCLEAN_GEMSS192_AVX2_psqr544_gf2x(uint64_t C[17], const uint64_t A[9]);
void PQCLEAN_GEMSS192_AVX2_psqr576_gf2x(uint64_t C[18], const uint64_t A[9]);

#define psqr32_gf2x PQCLEAN_GEMSS192_AVX2_psqr64low_gf2x


/* 11 instructions */
#define PSQR64_LO_GF2X(C1,A1,RA,R1)     (R1)=PUNPACKLO_8(A1,RA);    (C1)=PAND_(R1,PSET1_8(0xF));    (R1)=PAND_(PXOR_(C1,PSLLI_16(C1,2)),PSET1_8(0x33));    (C1)=PAND_(PXOR_(R1,PSLLI_16(R1,1)),PSET1_8(0x55));

/* 11 instructions */
#define PSQR64_HI_GF2X(C1,A1,RA,R1)     (R1)=PUNPACKHI_8(A1,RA);    (C1)=PAND_(R1,PSET1_8(0xF));    (R1)=PAND_(PXOR_(C1,PSLLI_16(C1,2)),PSET1_8(0x33));    (C1)=PAND_(PXOR_(R1,PSLLI_16(R1,1)),PSET1_8(0x55));

/* 12 instructions */
#define PSQR64_GF2X(C1,A1,RA,R1)     RA=PSRLI_16(A1,4);    (R1)=PUNPACKLO_8(A1,RA);    (C1)=PAND_(R1,PSET1_8(0xF));    (R1)=PAND_(PXOR_(C1,PSLLI_16(C1,2)),PSET1_8(0x33));    (C1)=PAND_(PXOR_(R1,PSLLI_16(R1,1)),PSET1_8(0x55));

#define PSQR128_GF2X(C1,C2,A1,RA,R1)     RA=PSRLI_16(A1,4);    PSQR64_LO_GF2X(C1,A1,RA,R1);    PSQR64_HI_GF2X(C2,A1,RA,R1);

#define PSQR192_GF2X(C1,C2,C3,A1,A2,RA,R1)     PSQR128_GF2X(C1,C2,A1,RA,R1);    PSQR64_GF2X(C3,A2,RA,R1);

#define PSQR256_GF2X(C1,C2,C3,C4,A1,A2,RA,R1)     PSQR128_GF2X(C1,C2,A1,RA,R1);    PSQR128_GF2X(C3,C4,A2,RA,R1);

#define PSQR320_GF2X(C1,C2,C3,C4,C5,A1,A2,A3,RA,R1)     PSQR256_GF2X(C1,C2,C3,C4,A1,A2,RA,R1);    PSQR64_GF2X(C5,A3,RA,R1);

#define PSQR384_GF2X(C1,C2,C3,C4,C5,C6,A1,A2,A3,RA,R1)     PSQR256_GF2X(C1,C2,C3,C4,A1,A2,RA,R1);    PSQR128_GF2X(C5,C6,A3,RA,R1);

#define PSQR448_GF2X(C1,C2,C3,C4,C5,C6,C7,A1,A2,A3,A4,RA,R1)     PSQR256_GF2X(C1,C2,C3,C4,A1,A2,RA,R1);    PSQR192_GF2X(C5,C6,C7,A3,A4,RA,R1);

#define PSQR512_GF2X(C1,C2,C3,C4,C5,C6,C7,C8,A1,A2,A3,A4,RA,R1)     PSQR256_GF2X(C1,C2,C3,C4,A1,A2,RA,R1);    PSQR256_GF2X(C5,C6,C7,C8,A3,A4,RA,R1);

#define PSQR576_GF2X(C1,C2,C3,C4,C5,C6,C7,C8,C9,A1,A2,A3,A4,A5,RA,R1)     PSQR512_GF2X(C1,C2,C3,C4,C5,C6,C7,C8,A1,A2,A3,A4,RA,R1);    PSQR64_GF2X(C9,A5,RA,R1);


/***********************************************************************/
/***********************************************************************/
/*************************** shuffle version ***************************/
/***********************************************************************/
/***********************************************************************/


uint64_t PQCLEAN_GEMSS192_AVX2_psqr64low_shuffle_gf2x(uint64_t A);
void PQCLEAN_GEMSS192_AVX2_psqr64_shuffle_gf2x(uint64_t C[2], const uint64_t A[1]);
void PQCLEAN_GEMSS192_AVX2_psqr96_shuffle_gf2x(uint64_t C[3], const uint64_t A[2]);
void PQCLEAN_GEMSS192_AVX2_psqr128_shuffle_gf2x(uint64_t C[4], const uint64_t A[2]);
void PQCLEAN_GEMSS192_AVX2_psqr160_shuffle_gf2x(uint64_t C[5], const uint64_t A[3]);
void PQCLEAN_GEMSS192_AVX2_psqr192_shuffle_gf2x(uint64_t C[6], const uint64_t A[3]);
void PQCLEAN_GEMSS192_AVX2_psqr224_shuffle_gf2x(uint64_t C[7], const uint64_t A[4]);
void PQCLEAN_GEMSS192_AVX2_psqr256_shuffle_gf2x(uint64_t C[8], const uint64_t A[4]);
void PQCLEAN_GEMSS192_AVX2_psqr288_shuffle_gf2x(uint64_t C[9], const uint64_t A[5]);
void PQCLEAN_GEMSS192_AVX2_psqr320_shuffle_gf2x(uint64_t C[10], const uint64_t A[5]);
void PQCLEAN_GEMSS192_AVX2_psqr352_shuffle_gf2x(uint64_t C[11], const uint64_t A[6]);
void PQCLEAN_GEMSS192_AVX2_psqr384_shuffle_gf2x(uint64_t C[12], const uint64_t A[6]);
void PQCLEAN_GEMSS192_AVX2_psqr416_shuffle_gf2x(uint64_t C[13], const uint64_t A[7]);
void PQCLEAN_GEMSS192_AVX2_psqr448_shuffle_gf2x(uint64_t C[14], const uint64_t A[7]);
void PQCLEAN_GEMSS192_AVX2_psqr480_shuffle_gf2x(uint64_t C[15], const uint64_t A[8]);
void PQCLEAN_GEMSS192_AVX2_psqr512_shuffle_gf2x(uint64_t C[16], const uint64_t A[8]);
void PQCLEAN_GEMSS192_AVX2_psqr544_shuffle_gf2x(uint64_t C[17], const uint64_t A[9]);
void PQCLEAN_GEMSS192_AVX2_psqr576_shuffle_gf2x(uint64_t C[18], const uint64_t A[9]);


#define psqr32_shuffle_gf2x PQCLEAN_GEMSS192_AVX2_psqr64low_shuffle_gf2x


#define PSQR_INIT_SHUFFLE_GF2X(M,T)     (M)=PSET1_8(0x0F);    (T)=PSET_64((uint64_t)0x5554515045444140,(uint64_t)0x1514111005040100);

/* 6 instructions */
#define PSQR64_SHUFFLE_V1_GF2X(E0,A128,M,T)     {__m128i C0,C1,D0,D1;    (C0)=PAND_(A128,M);    (C1)=PAND_(PSRLI_16(A128,4),M);    (D0)=PSHUFFLE_8(T,C0);    (D1)=PSHUFFLE_8(T,C1);    (E0)=PUNPACKLO_8(D0,D1);}

/* 4 instructions, faster than PSQR64_SHUFFLE_V1_GF2X */
#define PSQR64_SHUFFLE_GF2X(E0,A128,M,T)     {__m128i C0,D0;    (E0)=PSRLI_16(A128,4);    (C0)=PUNPACKLO_8(A128,E0);    (D0)=PAND_(C0,M);    (E0)=PSHUFFLE_8(T,D0);}

/* 7 instructions */
#define PSQR128_SHUFFLE_V1_GF2X(E0,E1,A128,M,T)     {__m128i C0,C1,D0,D1;    (E0)=PSRLI_16(A128,4);    (C0)=PUNPACKLO_8(A128,E0);    (C1)=PUNPACKHI_8(A128,E0);    (D0)=PAND_(C0,M);    (D1)=PAND_(C1,M);    (E0)=PSHUFFLE_8(T,D0);    (E1)=PSHUFFLE_8(T,D1);}

/* 7 instructions, faster than PSQR128_SHUFFLE_V1_GF2X */
#define PSQR128_SHUFFLE_GF2X(E0,E1,A128,M,T)     {__m128i C0,C1,D0,D1;    (C0)=PAND_(A128,M);    (C1)=PAND_(PSRLI_16(A128,4),M);    (D0)=PSHUFFLE_8(T,C0);    (D1)=PSHUFFLE_8(T,C1);    (E0)=PUNPACKLO_8(D0,D1);    (E1)=PUNPACKHI_8(D0,D1);}

/* General macros */
#define PSQR192_SHUFFLE_GF2X(E1,E2,E3,A1,A2,M,T)     PSQR128_SHUFFLE_GF2X(E1,E2,A1,M,T);    PSQR64_SHUFFLE_GF2X(E3,A2,M,T);

#define PSQR256_SHUFFLE_GF2X(E1,E2,E3,E4,A1,A2,M,T)     PSQR128_SHUFFLE_GF2X(E1,E2,A1,M,T);    PSQR128_SHUFFLE_GF2X(E3,E4,A2,M,T);

#define PSQR320_SHUFFLE_GF2X(E1,E2,E3,E4,E5,A1,A2,A3,M,T)     PSQR256_SHUFFLE_GF2X(E1,E2,E3,E4,A1,A2,M,T);    PSQR64_SHUFFLE_GF2X(E5,A3,M,T);

#define PSQR384_SHUFFLE_GF2X(E1,E2,E3,E4,E5,E6,A1,A2,A3,M,T)     PSQR256_SHUFFLE_GF2X(E1,E2,E3,E4,A1,A2,M,T);    PSQR128_SHUFFLE_GF2X(E5,E6,A3,M,T);

#define PSQR448_SHUFFLE_GF2X(E1,E2,E3,E4,E5,E6,E7,A1,A2,A3,A4,M,T)     PSQR256_SHUFFLE_GF2X(E1,E2,E3,E4,A1,A2,M,T);    PSQR192_SHUFFLE_GF2X(E5,E6,E7,A3,A4,M,T);

#define PSQR512_SHUFFLE_GF2X(E1,E2,E3,E4,E5,E6,E7,E8,A1,A2,A3,A4,M,T)     PSQR256_SHUFFLE_GF2X(E1,E2,E3,E4,A1,A2,M,T);    PSQR256_SHUFFLE_GF2X(E5,E6,E7,E8,A3,A4,M,T);

#define PSQR576_SHUFFLE_GF2X(E1,E2,E3,E4,E5,E6,E7,E8,E9,A1,A2,A3,A4,A5,M,T)     PSQR512_SHUFFLE_GF2X(E1,E2,E3,E4,E5,E6,E7,E8,A1,A2,A3,A4,M,T);    PSQR64_SHUFFLE_GF2X(E9,A5,M,T);


uint64_t PQCLEAN_GEMSS192_AVX2_vpsqr64low_shuffle_gf2x(uint64_t A);
void PQCLEAN_GEMSS192_AVX2_vpsqr64_shuffle_gf2x(uint64_t C[2], const uint64_t A[1]);
void PQCLEAN_GEMSS192_AVX2_vpsqr96_shuffle_gf2x(uint64_t C[3], const uint64_t A[2]);
void PQCLEAN_GEMSS192_AVX2_vpsqr128_shuffle_gf2x(uint64_t C[4], const uint64_t A[2]);
void PQCLEAN_GEMSS192_AVX2_vpsqr160_shuffle_gf2x(uint64_t C[5], const uint64_t A[3]);
void PQCLEAN_GEMSS192_AVX2_vpsqr192_shuffle_gf2x(uint64_t C[6], const uint64_t A[3]);
void PQCLEAN_GEMSS192_AVX2_vpsqr224_shuffle_gf2x(uint64_t C[7], const uint64_t A[4]);
void PQCLEAN_GEMSS192_AVX2_vpsqr256_shuffle_gf2x(uint64_t C[8], const uint64_t A[4]);
void PQCLEAN_GEMSS192_AVX2_vpsqr288_shuffle_gf2x(uint64_t C[9], const uint64_t A[5]);
void PQCLEAN_GEMSS192_AVX2_vpsqr320_shuffle_gf2x(uint64_t C[10], const uint64_t A[5]);
void PQCLEAN_GEMSS192_AVX2_vpsqr352_shuffle_gf2x(uint64_t C[11], const uint64_t A[6]);
void PQCLEAN_GEMSS192_AVX2_vpsqr384_shuffle_gf2x(uint64_t C[12], const uint64_t A[6]);
void PQCLEAN_GEMSS192_AVX2_vpsqr416_shuffle_gf2x(uint64_t C[13], const uint64_t A[7]);
void PQCLEAN_GEMSS192_AVX2_vpsqr448_shuffle_gf2x(uint64_t C[14], const uint64_t A[7]);
void PQCLEAN_GEMSS192_AVX2_vpsqr480_shuffle_gf2x(uint64_t C[15], const uint64_t A[8]);
void PQCLEAN_GEMSS192_AVX2_vpsqr512_shuffle_gf2x(uint64_t C[16], const uint64_t A[8]);
void PQCLEAN_GEMSS192_AVX2_vpsqr544_shuffle_gf2x(uint64_t C[17], const uint64_t A[9]);
void PQCLEAN_GEMSS192_AVX2_vpsqr576_shuffle_gf2x(uint64_t C[18], const uint64_t A[9]);

#define vpsqr32_shuffle_gf2x PQCLEAN_GEMSS192_AVX2_vpsqr64low_shuffle_gf2x


#define VPSQR_INIT_SHUFFLE_GF2X(M,T)     (M)=VPSET1_8(0x0F);    (T)=VPSET_64((uint64_t)0x5554515045444140,(uint64_t)0x1514111005040100,               (uint64_t)0x5554515045444140,(uint64_t)0x1514111005040100);

/* 4 instructions */
#define VPSQR64_SHUFFLE_GF2X(E0,A256,M,T)     {__m256i B1,C0,D0;    (B1)=VPSRLI_16(A256,4);    (C0)=VPUNPACKLO_8(A256,B1);    (D0)=VPAND_(C0,M);    (E0)=VPSHUFFLE_8(T,D0);}

/* 5 instructions */
#define VPSQR128_SHUFFLE_GF2X(E0,A256,M,T)     {__m256i B0,B1,C0,D0;    (B0)=VPPERMUTE4x64(A256,0xD8);    (B1)=VPSRLI_16(B0,4);    (C0)=VPUNPACKLO_8(B0,B1);    (D0)=VPAND_(C0,M);    (E0)=VPSHUFFLE_8(T,D0);}

/* unpack after */
/* 9 instructions */
#define VPSQR256_SHUFFLE_V1_GF2X(E0,E1,A256,M,T)     {__m256i B0,B1,C0,C1,D0,D1;    (B0)=VPAND_(A256,M);    (B1)=VPAND_(VPSRLI_16(A256,4),M);    (C0)=VPSHUFFLE_8(T,B0);    (C1)=VPSHUFFLE_8(T,B1);    (D0)=VPUNPACKLO_8(C0,C1);    (D1)=VPUNPACKHI_8(C0,C1);    (E0)=VPPERMUTE2x128(D0,D1,0x20);    (E1)=VPPERMUTE2x128(D0,D1,0x31);}

/* unpack before */
/* 8 instructions, faster than VPSQR256_SHUFFLE_V1_GF2X */
#define VPSQR256_SHUFFLE_GF2X(E0,E1,A256,M,T)     {__m256i B0,B1,C0,C1,D0,D1;    (B0)=VPPERMUTE4x64(A256,0xD8);    (B1)=VPSRLI_16(B0,4);    (C0)=VPUNPACKLO_8(B0,B1);    (C1)=VPUNPACKHI_8(B0,B1);    (D0)=VPAND_(C0,M);    (D1)=VPAND_(C1,M);    (E0)=VPSHUFFLE_8(T,D0);    (E1)=VPSHUFFLE_8(T,D1);}

#define VPSQR192_SHUFFLE_GF2X VPSQR256_SHUFFLE_GF2X

#define VPSQR320_SHUFFLE_GF2X(E1,E2,E3,A1,A2,M,T)             VPSQR256_SHUFFLE_GF2X(E1,E2,A1,M,T);            VPSQR64_SHUFFLE_GF2X(E3,A2,M,T);

#define VPSQR384_SHUFFLE_GF2X(E1,E2,E3,A1,A2,M,T)             VPSQR256_SHUFFLE_GF2X(E1,E2,A1,M,T);            VPSQR128_SHUFFLE_GF2X(E3,A2,M,T);

#define VPSQR512_SHUFFLE_GF2X(E1,E2,E3,E4,A1,A2,M,T)             VPSQR256_SHUFFLE_GF2X(E1,E2,A1,M,T);            VPSQR256_SHUFFLE_GF2X(E3,E4,A2,M,T);

#define VPSQR448_SHUFFLE_GF2X VPSQR512_SHUFFLE_GF2X

#define VPSQR576_SHUFFLE_GF2X(E1,E2,E3,E4,E5,A1,A2,A3,M,T)             VPSQR512_SHUFFLE_GF2X(E1,E2,E3,E4,A1,A2,M,T);            VPSQR64_SHUFFLE_GF2X(E5,A3,M,T);


/***********************************************************************/
/***********************************************************************/
/**************************** pclmul version ***************************/
/***********************************************************************/
/***********************************************************************/


/* Here, A is an integer */
#define sqr64low_inlined_pclmul_gf2x(C,A)     PCVT_64(C,PCLMUL(PSETL_64(A),PSETL_64(A),0))

/* The pclmul instruction does not exploit that the 32 last bits are null */
#define sqr32_inlined_pclmul_gf2x sqr64low_inlined_pclmul_gf2x

/* Here, A is an integer and C is a pointer */
#define sqr64_inlined2_pclmul_gf2x(C,A)     PSTORE128(C,PCLMUL(PSETL_64(A),PSETL_64(A),0))

/* Here, A,C are pointers */
#define sqr64_inlined_pclmul_gf2x(C,A)     PSTORE128(C,PCLMUL(PLOADL(A),PLOADL(A),0))


uint64_t PQCLEAN_GEMSS192_AVX2_sqr64low_pclmul_gf2x(uint64_t A);
void PQCLEAN_GEMSS192_AVX2_sqr64_pclmul_gf2x(uint64_t C[2], const uint64_t A[1]);
void PQCLEAN_GEMSS192_AVX2_sqr96_pclmul_gf2x(uint64_t C[3], const uint64_t A[2]);
void PQCLEAN_GEMSS192_AVX2_sqr128_pclmul_gf2x(uint64_t C[4], const uint64_t A[2]);
void PQCLEAN_GEMSS192_AVX2_sqr160_pclmul_gf2x(uint64_t C[5], const uint64_t A[3]);
void PQCLEAN_GEMSS192_AVX2_sqr192_pclmul_gf2x(uint64_t C[6], const uint64_t A[3]);
void PQCLEAN_GEMSS192_AVX2_sqr224_pclmul_gf2x(uint64_t C[7], const uint64_t A[4]);
void PQCLEAN_GEMSS192_AVX2_sqr256_pclmul_gf2x(uint64_t C[8], const uint64_t A[4]);
void PQCLEAN_GEMSS192_AVX2_sqr288_pclmul_gf2x(uint64_t C[9], const uint64_t A[5]);
void PQCLEAN_GEMSS192_AVX2_sqr320_pclmul_gf2x(uint64_t C[10], const uint64_t A[5]);
void PQCLEAN_GEMSS192_AVX2_sqr352_pclmul_gf2x(uint64_t C[11], const uint64_t A[6]);
void PQCLEAN_GEMSS192_AVX2_sqr384_pclmul_gf2x(uint64_t C[12], const uint64_t A[6]);
void PQCLEAN_GEMSS192_AVX2_sqr416_pclmul_gf2x(uint64_t C[13], const uint64_t A[7]);
void PQCLEAN_GEMSS192_AVX2_sqr448_pclmul_gf2x(uint64_t C[14], const uint64_t A[7]);
void PQCLEAN_GEMSS192_AVX2_sqr480_pclmul_gf2x(uint64_t C[15], const uint64_t A[8]);
void PQCLEAN_GEMSS192_AVX2_sqr512_pclmul_gf2x(uint64_t C[16], const uint64_t A[8]);
void PQCLEAN_GEMSS192_AVX2_sqr544_pclmul_gf2x(uint64_t C[17], const uint64_t A[9]);
void PQCLEAN_GEMSS192_AVX2_sqr576_pclmul_gf2x(uint64_t C[18], const uint64_t A[9]);

#define sqr32_pclmul_gf2x PQCLEAN_GEMSS192_AVX2_sqr64low_pclmul_gf2x


/***********************************************************************/
/***********************************************************************/
/********************** store at the end version ***********************/
/***********************************************************************/
/***********************************************************************/


/* WS: without store */
/* 1 pclmul */
#define SQR64_WS_PCLMUL_GF2X(z,x,pos)     z=PCLMUL(x,x,pos);

/* 2 pclmul */
#define SQR128_WS_PCLMUL_GF2X(z1,z2,x)     SQR64_WS_PCLMUL_GF2X(z1,x,0);    SQR64_WS_PCLMUL_GF2X(z2,x,17);

/* 3 pclmul */
#define SQR192_WS_PCLMUL_GF2X(z1,z2,z3,x1,x2)     SQR128_WS_PCLMUL_GF2X(z1,z2,x1);    SQR64_WS_PCLMUL_GF2X(z3,x2,0);

/* 4 pclmul */
#define SQR256_WS_PCLMUL_GF2X(z1,z2,z3,z4,x1,x2)     SQR128_WS_PCLMUL_GF2X(z1,z2,x1);    SQR128_WS_PCLMUL_GF2X(z3,z4,x2);

/* 5 pclmul */
#define SQR320_WS_PCLMUL_GF2X(z1,z2,z3,z4,z5,x1,x2,x3)     SQR256_WS_PCLMUL_GF2X(z1,z2,z3,z4,x1,x2);    SQR64_WS_PCLMUL_GF2X(z5,x3,0);

/* 6 pclmul */
#define SQR384_WS_PCLMUL_GF2X(z1,z2,z3,z4,z5,z6,x1,x2,x3)     SQR256_WS_PCLMUL_GF2X(z1,z2,z3,z4,x1,x2);    SQR128_WS_PCLMUL_GF2X(z5,z6,x3);

/* 7 pclmul */
#define SQR448_WS_PCLMUL_GF2X(z1,z2,z3,z4,z5,z6,z7,x1,x2,x3,x4)     SQR256_WS_PCLMUL_GF2X(z1,z2,z3,z4,x1,x2);    SQR192_WS_PCLMUL_GF2X(z5,z6,z7,x3,x4);

/* 8 pclmul */
#define SQR512_WS_PCLMUL_GF2X(z1,z2,z3,z4,z5,z6,z7,z8,x1,x2,x3,x4)     SQR256_WS_PCLMUL_GF2X(z1,z2,z3,z4,x1,x2);    SQR256_WS_PCLMUL_GF2X(z5,z6,z7,z8,x3,x4);

/* 9 pclmul */
#define SQR576_WS_PCLMUL_GF2X(z1,z2,z3,z4,z5,z6,z7,z8,z9,x1,x2,x3,x4,x5)     SQR512_WS_PCLMUL_GF2X(z1,z2,z3,z4,z5,z6,z7,z8,x1,x2,x3,x4);    SQR64_WS_PCLMUL_GF2X(z9,x5,0);


/***********************************************************************/
/***********************************************************************/
/********************** square and store version ***********************/
/***********************************************************************/
/***********************************************************************/


/* 1 pclmul */
#define SQR64LOW_PCLMUL_GF2X(C,x,pos) PCVT_64(C,PCLMUL(x,x,pos))
#define SQR64LOW_TAB_PCLMUL_GF2X(C,x,pos) PSTOREL(C,PCLMUL(x,x,pos))

#define SQR32_PCLMUL_GF2X(C,x) SQR64LOW_PCLMUL_GF2X(C,x,0)

#define SQR64_PCLMUL_GF2X(C,x,z,pos)     SQR64_WS_PCLMUL_GF2X(z,x,pos);    PSTORE128(C,z);

/* 2 pclmul */
#define SQR96_PCLMUL_GF2X(C,x,z)     SQR64_PCLMUL_GF2X(C,x,z,0);    SQR64LOW_TAB_PCLMUL_GF2X((C)+2,x,17);

#define SQR128_PCLMUL_GF2X(C,x,z)     SQR64_PCLMUL_GF2X(C,x,z,0);    SQR64_PCLMUL_GF2X((C)+2,x,z,17);

/* 3 pclmul */
#define SQR160_PCLMUL_GF2X(C,x1,x2,z)     SQR128_PCLMUL_GF2X(C,x1,z);    SQR64LOW_TAB_PCLMUL_GF2X((C)+4,x2,0);

#define SQR192_PCLMUL_GF2X(C,x1,x2,z)     SQR128_PCLMUL_GF2X(C,x1,z);    SQR64_PCLMUL_GF2X((C)+4,x2,z,0);

/* 4 pclmul */
#define SQR224_PCLMUL_GF2X(C,x1,x2,z)     SQR128_PCLMUL_GF2X(C,x1,z);    SQR96_PCLMUL_GF2X((C)+4,x2,z);

#define SQR256_PCLMUL_GF2X(C,x1,x2,z)     SQR128_PCLMUL_GF2X(C,x1,z);    SQR128_PCLMUL_GF2X((C)+4,x2,z);

/* 5 pclmul */
#define SQR288_PCLMUL_GF2X(C,x1,x2,x3,z)     SQR256_PCLMUL_GF2X(C,x1,x2,z);    SQR64LOW_TAB_PCLMUL_GF2X((C)+8,x3,0);

#define SQR320_PCLMUL_GF2X(C,x1,x2,x3,z)     SQR256_PCLMUL_GF2X(C,x1,x2,z);    SQR64_PCLMUL_GF2X((C)+8,x3,z,0);

/* 6 pclmul */
#define SQR352_PCLMUL_GF2X(C,x1,x2,x3,z)     SQR256_PCLMUL_GF2X(C,x1,x2,z);    SQR96_PCLMUL_GF2X((C)+8,x3,z);

#define SQR384_PCLMUL_GF2X(C,x1,x2,x3,z)     SQR256_PCLMUL_GF2X(C,x1,x2,z);    SQR128_PCLMUL_GF2X((C)+8,x3,z);

/* 7 pclmul */
#define SQR416_PCLMUL_GF2X(C,x1,x2,x3,x4,z)     SQR256_PCLMUL_GF2X(C,x1,x2,z);    SQR160_PCLMUL_GF2X((C)+8,x3,x4,z);

#define SQR448_PCLMUL_GF2X(C,x1,x2,x3,x4,z)     SQR256_PCLMUL_GF2X(C,x1,x2,z);    SQR192_PCLMUL_GF2X((C)+8,x3,x4,z);

/* 8 pclmul */
#define SQR480_PCLMUL_GF2X(C,x1,x2,x3,x4,z)     SQR256_PCLMUL_GF2X(C,x1,x2,z);    SQR224_PCLMUL_GF2X((C)+8,x3,x4,z);

#define SQR512_PCLMUL_GF2X(C,x1,x2,x3,x4,z)     SQR256_PCLMUL_GF2X(C,x1,x2,z);    SQR256_PCLMUL_GF2X((C)+8,x3,x4,z);

/* 9 pclmul */
#define SQR544_PCLMUL_GF2X(C,x1,x2,x3,x4,x5,z)     SQR512_PCLMUL_GF2X(C,x1,x2,x3,x4,z);    SQR64LOW_TAB_PCLMUL_GF2X((C)+16,x5,0);

#define SQR576_PCLMUL_GF2X(C,x1,x2,x3,x4,x5,z)     SQR512_PCLMUL_GF2X(C,x1,x2,x3,x4,z);    SQR64_PCLMUL_GF2X((C)+16,x5,z,0);






/* Choice of the best constant-time squaring.
   We obtain this trade-off (without data dependencies). */
/* Pclmul */
#define best_sqr32_gf2x sqr32_inlined_pclmul_gf2x
#define best_sqr64low_gf2x sqr64low_inlined_pclmul_gf2x
#define best_sqr64_gf2x sqr64_inlined_pclmul_gf2x
/* #define best_sqr64low_gf2x PQCLEAN_GEMSS192_AVX2_sqr64low_pclmul_gf2x */
/* #define best_sqr64_gf2x PQCLEAN_GEMSS192_AVX2_sqr64_pclmul_gf2x */
#define best_sqr96_gf2x PQCLEAN_GEMSS192_AVX2_sqr96_pclmul_gf2x
#define best_sqr128_gf2x PQCLEAN_GEMSS192_AVX2_sqr128_pclmul_gf2x
#define best_sqr160_gf2x PQCLEAN_GEMSS192_AVX2_sqr160_pclmul_gf2x
#define best_sqr192_gf2x PQCLEAN_GEMSS192_AVX2_sqr192_pclmul_gf2x
#define best_sqr224_gf2x PQCLEAN_GEMSS192_AVX2_sqr224_pclmul_gf2x

/* Faster on Haswell */
#define best_sqr256_gf2x PQCLEAN_GEMSS192_AVX2_vpsqr256_shuffle_gf2x
#define best_sqr288_gf2x PQCLEAN_GEMSS192_AVX2_vpsqr288_shuffle_gf2x
#define best_sqr320_gf2x PQCLEAN_GEMSS192_AVX2_vpsqr320_shuffle_gf2x
#define best_sqr352_gf2x PQCLEAN_GEMSS192_AVX2_vpsqr352_shuffle_gf2x
#define best_sqr384_gf2x PQCLEAN_GEMSS192_AVX2_vpsqr384_shuffle_gf2x
#define best_sqr416_gf2x PQCLEAN_GEMSS192_AVX2_vpsqr416_shuffle_gf2x
#define best_sqr448_gf2x PQCLEAN_GEMSS192_AVX2_vpsqr448_shuffle_gf2x
#define best_sqr480_gf2x PQCLEAN_GEMSS192_AVX2_vpsqr480_shuffle_gf2x
#define best_sqr512_gf2x PQCLEAN_GEMSS192_AVX2_vpsqr512_shuffle_gf2x
#define best_sqr544_gf2x PQCLEAN_GEMSS192_AVX2_vpsqr544_shuffle_gf2x
#define best_sqr576_gf2x PQCLEAN_GEMSS192_AVX2_vpsqr576_shuffle_gf2x


/* Choice of the best variable-time squaring */
/* The best variable-time squaring is the constant-time squaring */
#define best_sqr_nocst_32_gf2x best_sqr32_gf2x
#define best_sqr_nocst_64low_gf2x best_sqr64low_gf2x
#define best_sqr_nocst_64_gf2x best_sqr64_gf2x
#define best_sqr_nocst_96_gf2x best_sqr96_gf2x
#define best_sqr_nocst_128_gf2x best_sqr128_gf2x
#define best_sqr_nocst_160_gf2x best_sqr160_gf2x
#define best_sqr_nocst_192_gf2x best_sqr192_gf2x
#define best_sqr_nocst_224_gf2x best_sqr224_gf2x
#define best_sqr_nocst_256_gf2x best_sqr256_gf2x
#define best_sqr_nocst_288_gf2x best_sqr288_gf2x
#define best_sqr_nocst_320_gf2x best_sqr320_gf2x
#define best_sqr_nocst_352_gf2x best_sqr352_gf2x
#define best_sqr_nocst_384_gf2x best_sqr384_gf2x
#define best_sqr_nocst_416_gf2x best_sqr416_gf2x
#define best_sqr_nocst_448_gf2x best_sqr448_gf2x
#define best_sqr_nocst_480_gf2x best_sqr480_gf2x
#define best_sqr_nocst_512_gf2x best_sqr512_gf2x
#define best_sqr_nocst_544_gf2x best_sqr544_gf2x
#define best_sqr_nocst_576_gf2x best_sqr576_gf2x



#endif
