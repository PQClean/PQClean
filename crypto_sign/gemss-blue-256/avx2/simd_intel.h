#ifndef _SIMD_INTEL_H
#define _SIMD_INTEL_H

/* Intel intrinsics */
#include "arch.h"



/* 128-bit integer */
#define M128i __m128i

/* Set to 0 */
#define PSETZERO_PS() _mm_setzero_ps()

#define PSHUFFLE_PS(A,B,i) \
    ((__m128i)_mm_shuffle_ps((__m128)(A),(__m128)(B),i))

#define PMOVE_PS(A,B) ((__m128i)_mm_move_ss((__m128)(A),(__m128)(B)))


/* These intrinsics are available on SSE and SSE2 */
/* Set to 0 */
#define PSETZERO() _mm_setzero_si128()

/* Bitwise operator */
#define PAND_(A,B) _mm_and_si128(A,B)
#define PANDNOT(A,B) _mm_andnot_si128(A,B)
#define PXOR_(A,B) _mm_xor_si128(A,B)

/* Load */
#define PLOADL(A) _mm_loadl_epi64((__m128i*)(A))
#define PLOAD(A) _mm_load_si128((__m128i*)(A))
#define PLOADU(A) _mm_loadu_si128((__m128i*)(A))

/* Store */
#define PSTOREL(res,A) _mm_storel_epi64((__m128i*)(res),A)
#define PSTORE(res,A) _mm_store_si128((__m128i*)(res),A);
#define PSTOREU(res,A) _mm_storeu_si128((__m128i*)(res),A);

/* Here, a 128-bit register is splitted in four 32-bit registers: 3|2|1|0.
   The next operation changes the order of these registers. */
#define PSHUFFLE_32(A,i) _mm_shuffle_epi32(A,i)

/* Unpack */
#define PUNPACKLO_32(A,B) _mm_unpacklo_epi32(A,B)
#define PUNPACKLO_64(A,B) _mm_unpacklo_epi64(A,B)
#define PUNPACKHI_32(A,B) _mm_unpackhi_epi32(A,B)
#define PUNPACKHI_64(A,B) _mm_unpackhi_epi64(A,B)


/* Shuffle */
#define PSHUFFLE_PD(A,B,i) \
    ((__m128i)_mm_shuffle_pd((__m128d)(A),(__m128d)(B),i))

/* Add */
#define PADD_64(A,B) _mm_add_epi64(A,B)

/* Sub */
#define PSUB_64(A,B) _mm_sub_epi64(A,B)

/* Set */
#define PSET_8(AF,AE,AD,AC,AB,AA,A9,A8,A7,A6,A5,A4,A3,A2,A1,A0) \
    _mm_set_epi8(AF,AE,AD,AC,AB,AA,A9,A8,A7,A6,A5,A4,A3,A2,A1,A0)
#define PSET_16(A7,A6,A5,A4,A3,A2,A1,A0) \
    _mm_set_epi16(A7,A6,A5,A4,A3,A2,A1,A0)
#define PSET_32(A3,A2,A1,A0) _mm_set_epi32(A3,A2,A1,A0)
#define PSET_64(A1,A0) _mm_set_epi64x(A1,A0)

/* Set 1 */
#define PSET1_8(A0) _mm_set1_epi8(A0)
#define PSET1_16(A0) _mm_set1_epi16(A0)
#define PSET1_32(A0) _mm_set1_epi32(A0)
#define PSET1_64(A0) _mm_set1_epi64x(A0)

/* Shift */
#define PSLLI_16(A,i) _mm_slli_epi16(A,i)
#define PSLLI_32(A,i) _mm_slli_epi32(A,i)
#define PSLLI_64(A,i) _mm_slli_epi64(A,i)
#define PSLLI_128(A,i) _mm_slli_si128(A,i)

#define PSRLI_16(A,i) _mm_srli_epi16(A,i)
#define PSRLI_32(A,i) _mm_srli_epi32(A,i)
#define PSRLI_64(A,i) _mm_srli_epi64(A,i)
#define PSRLI_128(A,i) _mm_srli_si128(A,i)

/* Unpack */
#define PUNPACKLO_8(A,B) _mm_unpacklo_epi8(A,B)
#define PUNPACKLO_16(A,B) _mm_unpacklo_epi16(A,B)
/* unpacklo_32 is already available in SSE */
/* unpacklo_64 is already available in SSE */

#define PUNPACKHI_8(A,B) _mm_unpackhi_epi8(A,B)
#define PUNPACKHI_16(A,B) _mm_unpackhi_epi16(A,B)
/* unpackhi_32 is already available in SSE */
/* unpackhi_64 is already available in SSE */

/* For x in {0,1}, compute -x as (x==1)?(-1):0. */

/* Use: "one=PSET1_8(1);" to initialize the variable one */
#define PMASK8_ONE(x,one) _mm_cmpeq_epi8(x,one)
/* Use: "one=PSET1_16(1);" to initialize the variable one */
#define PMASK16_ONE(x,one) _mm_cmpeq_epi16(x,one)
/* Use: "one=PSET1_32(1);" to initialize the variable one */
#define PMASK32_ONE(x,one) _mm_cmpeq_epi32(x,one)


/*** We implement here other functions, available on SSE and SSE2 ***/
/* Left shift without 0 padding */
#define PLMOVE64(x) PSLLI_128(x,8)

/* Right shift without 0 padding */
#define PRMOVE64(x) PSRLI_128(x,8)

/* Left shift with 0 padding */
#define PLSHIFT64(x) PSLLI_128(x,8)

/* Right shift with 0 padding */
#define PRSHIFT64(x) PSRLI_128(x,8)

/* Here, a 128-bit register is splitted in four 32-bit registers: 3|2|1|0.
   The next operations change the order of these registers. */

/* Duplicate one of the two 64-bit registers. */
#define PSHUFFLE_32_1010(A) PSHUFFLE_32(A,0x44)
#define PSHUFFLE_32_3232(A) PSHUFFLE_32(A,0xEE)

#define PSETL_64(A0) PSET_64(0,A0)
#define PSETH_64(A0) PSET_64(A0,0)


/*** We implement here other functions ***/

/* Load */
#define PINIT64(x,A) x=PLOADL(A);
#define PINIT128(x,A) x=PLOADU(A);

#define PINIT192(x1,x2,A) \
    PINIT128(x1,A);\
    PINIT64(x2,(A)+2);

#define PINIT256(x1,x2,A) \
    PINIT128(x1,A);\
    PINIT128(x2,(A)+2);

#define PINIT320(x1,x2,x3,A) \
    PINIT256(x1,x2,A);\
    PINIT64(x3,(A)+4);

#define PINIT384(x1,x2,x3,A) \
    PINIT256(x1,x2,A);\
    PINIT128(x3,(A)+4);

#define PINIT448(x1,x2,x3,x4,A) \
    PINIT256(x1,x2,A);\
    PINIT192(x3,x4,(A)+4);

#define PINIT512(x1,x2,x3,x4,A) \
    PINIT256(x1,x2,A);\
    PINIT256(x3,x4,(A)+4);

#define PINIT576(x1,x2,x3,x4,x5,A) \
    PINIT512(x1,x2,x3,x4,A);\
    PINIT64(x5,(A)+8);

#define PINIT640(x1,x2,x3,x4,x5,A) \
    PINIT512(x1,x2,x3,x4,A);\
    PINIT128(x5,(A)+8);

#define PINIT704(x1,x2,x3,x4,x5,x6,A) \
    PINIT512(x1,x2,x3,x4,A);\
    PINIT192(x5,x6,(A)+8);

#define PINIT768(x1,x2,x3,x4,x5,x6,A) \
    PINIT512(x1,x2,x3,x4,A);\
    PINIT256(x5,x6,(A)+8);

#define PINIT832(x1,x2,x3,x4,x5,x6,x7,A) \
    PINIT512(x1,x2,x3,x4,A);\
    PINIT320(x5,x6,x7,(A)+8);

#define PINIT896(x1,x2,x3,x4,x5,x6,x7,A) \
    PINIT512(x1,x2,x3,x4,A);\
    PINIT384(x5,x6,x7,(A)+8);

#define PINIT960(x1,x2,x3,x4,x5,x6,x7,x8,A) \
    PINIT512(x1,x2,x3,x4,A);\
    PINIT448(x5,x6,x7,x8,(A)+8);

#define PINIT1024(x1,x2,x3,x4,x5,x6,x7,x8,A) \
    PINIT512(x1,x2,x3,x4,A);\
    PINIT512(x5,x6,x7,x8,(A)+8);

#define PINIT1088(x1,x2,x3,x4,x5,x6,x7,x8,x9,A) \
    PINIT1024(x1,x2,x3,x4,x5,x6,x7,x8,A);\
    PINIT64(x9,(A)+16);

#define PINIT1152(x1,x2,x3,x4,x5,x6,x7,x8,x9,A) \
    PINIT1024(x1,x2,x3,x4,x5,x6,x7,x8,A);\
    PINIT128(x9,(A)+16);


/* Store */
#define PSTORE128(A,x1) PSTOREU(A,x1)

#define PSTORE192(A,x1,x2) \
    PSTORE128(A,x1);\
    PSTOREL((A)+2,x2);

#define PSTORE256(A,x1,x2) \
    PSTORE128(A,x1);\
    PSTORE128((A)+2,x2);

#define PSTORE320(A,x1,x2,x3) \
    PSTORE256(A,x1,x2);\
    PSTOREL((A)+4,x3);

#define PSTORE384(A,x1,x2,x3) \
    PSTORE256(A,x1,x2);\
    PSTORE128((A)+4,x3);

#define PSTORE448(A,x1,x2,x3,x4) \
    PSTORE256(A,x1,x2);\
    PSTORE192((A)+4,x3,x4);

#define PSTORE512(A,x1,x2,x3,x4) \
    PSTORE256(A,x1,x2);\
    PSTORE256((A)+4,x3,x4);

#define PSTORE576(A,x1,x2,x3,x4,x5) \
    PSTORE512(A,x1,x2,x3,x4);\
    PSTOREL((A)+8,x5);

#define PSTORE640(A,x1,x2,x3,x4,x5) \
    PSTORE512(A,x1,x2,x3,x4);\
    PSTORE128((A)+8,x5);

#define PSTORE704(A,x1,x2,x3,x4,x5,x6) \
    PSTORE512(A,x1,x2,x3,x4);\
    PSTORE192((A)+8,x5,x6);

#define PSTORE768(A,x1,x2,x3,x4,x5,x6) \
    PSTORE512(A,x1,x2,x3,x4);\
    PSTORE256((A)+8,x5,x6);

#define PSTORE832(A,x1,x2,x3,x4,x5,x6,x7) \
    PSTORE512(A,x1,x2,x3,x4);\
    PSTORE320((A)+8,x5,x6,x7);

#define PSTORE896(A,x1,x2,x3,x4,x5,x6,x7) \
    PSTORE512(A,x1,x2,x3,x4);\
    PSTORE384((A)+8,x5,x6,x7);

#define PSTORE960(A,x1,x2,x3,x4,x5,x6,x7,x8) \
    PSTORE512(A,x1,x2,x3,x4);\
    PSTORE448((A)+8,x5,x6,x7,x8);

#define PSTORE1024(A,x1,x2,x3,x4,x5,x6,x7,x8) \
    PSTORE512(A,x1,x2,x3,x4);\
    PSTORE512((A)+8,x5,x6,x7,x8);

#define PSTORE1088(A,x1,x2,x3,x4,x5,x6,x7,x8,x9) \
    PSTORE1024(A,x1,x2,x3,x4,x5,x6,x7,x8);\
    PSTOREL((A)+16,x9);

#define PSTORE1152(A,x1,x2,x3,x4,x5,x6,x7,x8,x9) \
    PSTORE1024(A,x1,x2,x3,x4,x5,x6,x7,x8);\
    PSTORE128((A)+16,x9);


/* Bitwise operator */
#define PAND1_2(C,A) C=PAND_(A,C);

#define PXOR1_(C,A,B) C=PXOR_(A,B);
#define PXOR2_(C1,C2,A1,A2,B1,B2) \
    PXOR1_(C1,A1,B1)\
    PXOR1_(C2,A2,B2)
#define PXOR3_(C1,C2,C3,A1,A2,A3,B1,B2,B3) \
    PXOR1_(C1,A1,B1)\
    PXOR1_(C2,A2,B2)\
    PXOR1_(C3,A3,B3)
#define PXOR4_(C1,C2,C3,C4,A1,A2,A3,A4,B1,B2,B3,B4) \
    PXOR1_(C1,A1,B1)\
    PXOR1_(C2,A2,B2)\
    PXOR1_(C3,A3,B3)\
    PXOR1_(C4,A4,B4)

#define PXOR1_2(C,A) C=PXOR_(A,C);
#define PXOR2_2(C1,C2,A1,A2) \
    PXOR1_2(C1,A1)\
    PXOR1_2(C2,A2)
#define PXOR3_2(C1,C2,C3,A1,A2,A3) \
    PXOR1_2(C1,A1)\
    PXOR1_2(C2,A2)\
    PXOR1_2(C3,A3)
#define PXOR4_2(C1,C2,C3,C4,A1,A2,A3,A4) \
    PXOR1_2(C1,A1)\
    PXOR1_2(C2,A2)\
    PXOR1_2(C3,A3)\
    PXOR1_2(C4,A4)
#define PXOR5_2(C1,C2,C3,C4,C5,A1,A2,A3,A4,A5) \
    PXOR1_2(C1,A1)\
    PXOR1_2(C2,A2)\
    PXOR1_2(C3,A3)\
    PXOR1_2(C4,A4)\
    PXOR1_2(C5,A5)


/* Shuffle */
#define PSHUFFLE_8(A,B) _mm_shuffle_epi8(A,B)


#define PALIGNR(A2,A1,NB_BYTES) _mm_alignr_epi8(A2,A1,NB_BYTES)

/*** We implement here other functions ***/

#define PALIGNR_4(A2,A1) PALIGNR(A2,A1,4)
#define PALIGNR_12(A2,A1) PALIGNR(A2,A1,12)
#define PMIDDLE(A1,A2) PALIGNR(A2,A1,8)


/* Use: "one=PSET1_64(1);" to initialize the variable one */
#define PMASK64_ONE(x,one) _mm_cmpeq_epi64(x,one)


#define PCVT_64(C,A) (C)=_mm_cvtsi128_si64(A);

#define EXTRACT64(A) _mm_extract_epi64(A,1)


/* These intrinsics are available on AVX and AVX2 */
/* Bitwise operators */
#define VPAND_(A,B) _mm256_and_si256(A,B)
#define VPXOR_(A,B) _mm256_xor_si256(A,B)

/* Load + mask */
#define PMASKLOAD(data,mask) \
    _mm_maskload_epi64((long long int*)(data),mask)
#define VPMASKLOAD(data,mask) \
    _mm256_maskload_epi64((long long int*)(data),mask)

/* Unpack */
#define VPUNPACKLO_32(A,B) _mm256_unpacklo_epi32(A,B)
#define VPUNPACKLO_64(A,B) _mm256_unpacklo_epi64(A,B)

#define VPUNPACKHI_32(A,B) _mm256_unpackhi_epi32(A,B)
#define VPUNPACKHI_64(A,B) _mm256_unpackhi_epi64(A,B)

#define VPBLEND_32(A,B,i) _mm256_blend_epi32(A,B,i)

/* Extract A128 in the higher 128-bit of DST256 */
#define VPINSERT_128(DST256,A128) _mm256_inserti128_si256(DST256,A128,1)

/* Extract the higher 128-bit register */
#define VPEXTRACT128(A) _mm256_extracti128_si256(A,1)


/* These intrinsics are available on AVX */
/* 256-bit integer */
#define M256i __m256i

/* Set to 0 */
#define VPSETZERO() _mm256_setzero_si256()

/* Set */
#define VPSET_32(A7,A6,A5,A4,A3,A2,A1,A0) \
    _mm256_set_epi32(A7,A6,A5,A4,A3,A2,A1,A0)
#define VPSET_64(A3,A2,A1,A0) _mm256_set_epi64x(A3,A2,A1,A0)

/* Set 1 */
#define VPSET1_8(A0) _mm256_set1_epi8(A0)
#define VPSET1_16(A0) _mm256_set1_epi16(A0)
#define VPSET1_32(A0) _mm256_set1_epi32(A0)
#define VPSET1_64(A0) _mm256_set1_epi64x(A0)

/* Conversion */
#define VPCVT_128(A) _mm256_castsi256_si128(A)
#define PCVT_256(A) _mm256_castsi128_si256(A)

/* Extraction */
#define VPEXTRACT64(A,i) _mm256_extract_epi64(A,i)

/* Load */
#define VPLOAD(A) _mm256_load_si256((__m256i*)(A))
#define VPLOADU(A) _mm256_loadu_si256((__m256i*)(A))

/* Store */
#define VPSTORE(res,A) _mm256_store_si256((__m256i*)(res),A);
#define VPSTOREU(res,A) _mm256_storeu_si256((__m256i*)(res),A);


/*** We implement here other functions ***/


#define VPSETL_64(A0) VPSET_64(0,0,0,A0)
#define VPSETL2_64(A1,A0) VPSET_64(0,0,A1,A0)
#define VPSETL3_64(A2,A1,A0) VPSET_64(0,A2,A1,A0)

#define VPSET_FFF0 VPSET_64(MONE64,MONE64,MONE64,0)


/* Load */
#define VPINITU256(x,A) x=VPLOADU(A);

#define VPINIT64(x,A) \
    x=PCVT_256(PLOADL(A));

#define VPINIT128(x,A) \
    x=PCVT_256(PLOADU(A));

#define VPINIT192(x,A) \
    x=VPINSERT_128(PCVT_256(PLOADU(A)),PLOADL((A)+2));

#define VPINIT256(x,A) x=VPLOADU(A);

#define VPINIT320(x1,x2,A) \
    VPINIT256(x1,A);\
    VPINIT64(x2,(A)+4);

#define VPINIT384(x1,x2,A) \
    VPINIT256(x1,A);\
    VPINIT128(x2,(A)+4);

#define VPINIT448(x1,x2,A) \
    VPINIT256(x1,A);\
    VPINIT192(x2,(A)+4);

#define VPINIT512(x1,x2,A) \
    VPINIT256(x1,A);\
    VPINIT256(x2,(A)+4);

#define VPINIT576(x1,x2,x3,A) \
    VPINIT512(x1,x2,A);\
    VPINIT64(x3,(A)+8);

#define VPINIT640(x1,x2,x3,A) \
    VPINIT512(x1,x2,A);\
    VPINIT128(x3,(A)+8);

#define VPINIT704(x1,x2,x3,A) \
    VPINIT512(x1,x2,A);\
    VPINIT192(x3,(A)+8);

#define VPINIT768(x1,x2,x3,A) \
    VPINIT512(x1,x2,A);\
    VPINIT256(x3,(A)+8);

#define VPINIT832(x1,x2,x3,x4,A) \
    VPINIT512(x1,x2,A);\
    VPINIT320(x3,x4,(A)+8);

#define VPINIT896(x1,x2,x3,x4,A) \
    VPINIT512(x1,x2,A);\
    VPINIT384(x3,x4,(A)+8);

#define VPINIT960(x1,x2,x3,x4,A) \
    VPINIT512(x1,x2,A);\
    VPINIT448(x3,x4,(A)+8);

#define VPINIT1024(x1,x2,x3,x4,A) \
    VPINIT512(x1,x2,A);\
    VPINIT512(x3,x4,(A)+8);

#define VPINIT1088(x1,x2,x3,x4,x5,A) \
    VPINIT1024(x1,x2,x3,x4,A);\
    VPINIT64(x5,(A)+16);

#define VPINIT1152(x1,x2,x3,x4,x5,A) \
    VPINIT1024(x1,x2,x3,x4,A);\
    VPINIT128(x5,(A)+16);


/* Store */
#define VPSTORE64(A,x) \
    PSTOREL(A,VPCVT_128(x));

#define VPSTORE128(A,x) \
    PSTOREU(A,VPCVT_128(x));

#define VPSTORE192(A,x) \
    PSTOREU(A,VPCVT_128(x));\
    PSTOREL((A)+2,VPEXTRACT128(x));

#define VPSTORE256 VPSTOREU

#define VPSTORE320(A,x1,x2) \
    VPSTORE256(A,x1);\
    VPSTORE64((A)+4,x2);

#define VPSTORE384(A,x1,x2) \
    VPSTORE256(A,x1);\
    VPSTORE128((A)+4,x2);

#define VPSTORE448(A,x1,x2) \
    VPSTORE256(A,x1);\
    VPSTORE192((A)+4,x2);

#define VPSTORE512(A,x1,x2) \
    VPSTORE256(A,x1);\
    VPSTORE256((A)+4,x2);

#define VPSTORE576(A,x1,x2,x3) \
    VPSTORE512(A,x1,x2);\
    VPSTORE64((A)+8,x3);

#define VPSTORE640(A,x1,x2,x3) \
    VPSTORE512(A,x1,x2);\
    VPSTORE128((A)+8,x3);

#define VPSTORE704(A,x1,x2,x3) \
    VPSTORE512(A,x1,x2);\
    VPSTORE192((A)+8,x3);

#define VPSTORE768(A,x1,x2,x3) \
    VPSTORE512(A,x1,x2);\
    VPSTORE256((A)+8,x3);

#define VPSTORE832(A,x1,x2,x3,x4) \
    VPSTORE512(A,x1,x2);\
    VPSTORE320((A)+8,x3,x4);

#define VPSTORE896(A,x1,x2,x3,x4) \
    VPSTORE512(A,x1,x2);\
    VPSTORE384((A)+8,x3,x4);

#define VPSTORE960(A,x1,x2,x3,x4) \
    VPSTORE512(A,x1,x2);\
    VPSTORE448((A)+8,x3,x4);

#define VPSTORE1024(A,x1,x2,x3,x4) \
    VPSTORE512(A,x1,x2);\
    VPSTORE512((A)+8,x3,x4);

#define VPSTORE1088(A,x1,x2,x3,x4,x5) \
    VPSTORE1024(A,x1,x2,x3,x4);\
    VPSTORE64((A)+16,x5);

#define VPSTORE1152(A,x1,x2,x3,x4,x5) \
    VPSTORE1024(A,x1,x2,x3,x4);\
    VPSTORE128((A)+16,x5);


/* These intrinsics are available on AVX2 only */
/* Add */
#define VPADD_64(A,B) _mm256_add_epi64(A,B)

/* Sub */
#define VPSUB_64(A,B) _mm256_sub_epi64(A,B)

/* Shift */
#define VPSLLI_16(A,i) _mm256_slli_epi16(A,i)
#define VPSLLI_32(A,i) _mm256_slli_epi32(A,i)
#define VPSLLI_64(A,i) _mm256_slli_epi64(A,i)
#define VPSLLI_128(A,i) _mm256_slli_si256(A,i)

#define VPSRLI_16(A,i) _mm256_srli_epi16(A,i)
#define VPSRLI_32(A,i) _mm256_srli_epi32(A,i)
#define VPSRLI_64(A,i) _mm256_srli_epi64(A,i)
#define VPSRLI_128(A,i) _mm256_srli_si256(A,i)

/* Use: "one=VPSET1_8(1);" to initialize the variable one */
#define VPMASK8_ONE(x,one) _mm256_cmpeq_epi8(x,one)
/* Use: "one=VPSET1_16(1);" to initialize the variable one */
#define VPMASK16_ONE(x,one) _mm256_cmpeq_epi16(x,one)
/* Use: "one=VPSET1_32(1);" to initialize the variable one */
#define VPMASK32_ONE(x,one) _mm256_cmpeq_epi32(x,one)
/* Use: "one=VPSET1_64(1);" to initialize the variable one */
#define VPMASK64_ONE(x,one) _mm256_cmpeq_epi64(x,one)

/* Shuffle */
#define VPSHUFFLE_8(A,B) _mm256_shuffle_epi8(A,B)

/* Unpack */
#define VPUNPACKLO_8(A,B) _mm256_unpacklo_epi8(A,B)
#define VPUNPACKLO_16(A,B) _mm256_unpacklo_epi16(A,B)

#define VPUNPACKHI_8(A,B) _mm256_unpackhi_epi8(A,B)
#define VPUNPACKHI_16(A,B) _mm256_unpackhi_epi16(A,B)

/* Permute */
#define VPPERMUTE4x64(A,i) _mm256_permute4x64_epi64(A,i)
#define VPPERMUTE2x128(A,B,i) _mm256_permute2x128_si256(A,B,i)


/*** We implement here other functions ***/


/* Here, a 256-bit register is splitted in four 64-bit registers: 3|2|1|0.
   The next operations change the order of these registers. */

/* Rotate Through Carry Left, Rotate Through Carry Right (circular shift) */
#define VPPERMUTE4x64_0321(A) VPPERMUTE4x64(A,0x39)
#define VPPERMUTE4x64_1032(A) VPPERMUTE4x64(A,0x4E)
#define VPPERMUTE4x64_2103(A) VPPERMUTE4x64(A,0x93)

/* Duplicate one of the four 64-bit registers. */
#define VPPERMUTE4x64_0000(A) VPPERMUTE4x64(A,0)
#define VPPERMUTE4x64_1111(A) VPPERMUTE4x64(A,0x55)
#define VPPERMUTE4x64_2222(A) VPPERMUTE4x64(A,0xAA)
#define VPPERMUTE4x64_3333(A) VPPERMUTE4x64(A,0xFF)


/*** We implement here other functions ***/
#define VPSET_128(A1,A0) VPINSERT_128(PCVT_256(A0),A1)

#define VPAND1_2(C,A) C=VPAND_(A,C);

#define VPXOR1_(C,A,B) C=VPXOR_(A,B);
#define VPXOR2_(C1,C2,A1,A2,B1,B2) \
    VPXOR1_(C1,A1,B1)\
    VPXOR1_(C2,A2,B2)
#define VPXOR3_(C1,C2,C3,A1,A2,A3,B1,B2,B3) \
    VPXOR1_(C1,A1,B1)\
    VPXOR1_(C2,A2,B2)\
    VPXOR1_(C3,A3,B3)
#define VPXOR4_(C1,C2,C3,C4,A1,A2,A3,A4,B1,B2,B3,B4) \
    VPXOR1_(C1,A1,B1)\
    VPXOR1_(C2,A2,B2)\
    VPXOR1_(C3,A3,B3)\
    VPXOR1_(C4,A4,B4)

#define VPXOR1_2(C,A) C=VPXOR_(A,C);
#define VPXOR2_2(C1,C2,A1,A2) \
    VPXOR1_2(C1,A1)\
    VPXOR1_2(C2,A2)
#define VPXOR3_2(C1,C2,C3,A1,A2,A3) \
    VPXOR1_2(C1,A1)\
    VPXOR1_2(C2,A2)\
    VPXOR1_2(C3,A3)
#define VPXOR4_2(C1,C2,C3,C4,A1,A2,A3,A4) \
    VPXOR1_2(C1,A1)\
    VPXOR1_2(C2,A2)\
    VPXOR1_2(C3,A3)\
    VPXOR1_2(C4,A4)


/* Carry less multiplication */
#define PCLMUL(A,B,i) _mm_clmulepi64_si128(A,B,i)


/* Hamming weight, sum of the bits */
#define POPCNT_U64 _mm_popcnt_u64



#endif
