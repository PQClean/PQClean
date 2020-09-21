#ifndef _BIT_H
#define _BIT_H
#include "arch.h"
#include "macro.h"
#include "simd.h"



/* Tools for the bits manipulation */


/* (2^k) - 1, k<64, and -1 for k=0  */
#define mask64(k) ((k)?(ONE64<<(k))-ONE64:MONE64)

/* (2^k) - 1, k<32, and -1 for k=0 */
#define mask32(k) ((k)?(ONE32<<(k))-ONE32:MONE32)

#define maskUINT(k) ((k)?(UINT_1<<(k))-UINT_1:UINT_M1)


/** The i-th bit of the UINT val. */
#define ITHBIT(val,i) (((val)>>(i))&UINT_1)


/** Compute the MSB position of one UINT. */
/*  Input: U an UINT
    Output: res the MSB position of U. If U is zero, res=0
*/
#define MSB_SP(res,U,j)     (res)=0;    /* Search the MSB position of one word */    for((j)=NB_BITS_UINT>>1;(j)!=0;(j)>>=1)     {        if((U)>>((res)^(j)))        {            /* To remember the choice of the high part */            (res)^=(j);        }    }

/** Compute the MSB position of a multi-precision UINT. */
/*  Input: U an multiple precision UINT, stored on nb_word words
    Output: res the MSB position of U. If U is zero, res=0
*/
#define MSB_MP(res,U,i,j,nb_word)     (i)=(nb_word)-1;    /* Search the first word different from zero */    while((i)&&(!(U)[i]))     {        --(i);    }    /* Search the MSB of one word */    MSB_SP((res),(U)[i],j);    (res)^=(i)<<LOG2_SIZE_UINT;



/* Macros about computations on the bits of a word */


/* Goal: computed in constant-time:
    - COUNTBITS64  n=Hamming_weight(n)=sum of the 64 bits of n
    -    ORBITS64: n=(n==0)?0:1
    -   NORBITS64: n=(n==0)?1:0
    -   XORBITS64: n=Hamming_weight(n) modulo 2=sum of the 64 bits of n, mod 2
*/


/* The fastest method, using popcnt */
/* Here, the performance tests assume that the constants are already in
   registers. It is not necessarily true. */
#define COUNTBITS64_POP(n)             (n)=POPCNT_U64(n);

#define COUNTBITS32_POP(n)         (n)=POPCNT_U32(n);

#define ORBITS64_POP(n)         COUNTBITS64_POP(n);         /* The result is in {0,1,...,64} */        (n)+=63;        /* Now, the result is in {63,64,...,127} */        (n)>>=6;        /* Now, the result is in {0,1,...,1} */


#define NORBITS64_POP(n)         --(n);        COUNTBITS64_POP(n);         /* If n=0, then Hamming_weight(n-1)==64, else Hamming_weight(n-1)<64 */        /* Now, the result is in {64,0,...,63} */        (n)>>=6;        /* Now, the result is in {1,0,...,0} */


#define NORBITS64_POP2(n)         COUNTBITS64_POP(n);         /* The result is in {0,1,...,64} */        --(n);        /* Now, the result is in {2^{64}-1,0,...,63} */        (n)>>=63;        /* Now, the result is in {1,0,...,0} */


#define XORBITS64_POP(n)         COUNTBITS64_POP(n);         (n)&=ONE64;


/* The second fastest method, based on tricks
   with right shift of i == integer division by 2^i */


/* 5 logical operations */
#define ORBITS64_SHORT(n)     (n)|=(n) << 32U;    (n)>>=32U;    (n)+=((uint64_t)0xFFFFFFFF);    (n)>>=32U;


/* 5 logical operations */
#define NORBITS64_SHORT(n)     (n)|=(n) << 32U;    (n)>>=32U;    --(n);    (n)>>=63U;


/* The third fastest method, based on the variable-precision SWAR algorithm */
/* https://stackoverflow.com/questions/2709430/
   count-number-of-bits-in-a-64-bit-long-big-integer */


/* 12 logical operations */
#define COUNTBITS64_SWAR(n)     (n)-=((n) >> 1U) & ((uint64_t)0x5555555555555555);    (n)=((n) & ((uint64_t)0x3333333333333333))       + (((n) >> 2U) & ((uint64_t)0x3333333333333333));    (n)=((((n) + ((n) >> 4U)) & ((uint64_t)0xF0F0F0F0F0F0F0F))        * ((uint64_t)0x101010101010101)) >> 56U;


/* 13 logical operations */
#define ORBITS64_SWAR(n)     (n)-=((n) >> 1U) & ((uint64_t)0x5555555555555555);    (n)=((n) & ((uint64_t)0x3333333333333333))       + (((n) >> 2U) & ((uint64_t)0x3333333333333333));    /* We change ((n/(2^56))+63)/64 in (n+63*(2^56))/(2^62) */    (n)=(((((n) + ((n) >> 4U)) & ((uint64_t)0xF0F0F0F0F0F0F0F))       * ((uint64_t)0x101010101010101)) + ((uint64_t)0x3F00000000000000)) >> 62U;


/* 13 logical operations */
#define NORBITS64_SWAR(n)     --(n);    (n)-=((n) >> 1U) & ((uint64_t)0x5555555555555555);    (n)=((n) & ((uint64_t)0x3333333333333333))       + (((n) >> 2U) & ((uint64_t)0x3333333333333333));    (n)=(((((n) + ((n) >> 4U)) & ((uint64_t)0xF0F0F0F0F0F0F0F))         * ((uint64_t)0x101010101010101))) >> 62U;


/* 13 logical operations */
#define NORBITS64_SWAR2(n)     (n)-=((n) >> 1U) & ((uint64_t)0x5555555555555555);    (n)=((n) & ((uint64_t)0x3333333333333333))       + (((n) >> 2U) & ((uint64_t)0x3333333333333333));    /* We remark that ({0,1,...,63}+255)<<56 ={255<<56,0<<56,...,62<<56}*2^56*/    /* So, the 63-th bit is 1 iff Hamming_weight(n)=0 */    (n)=(((((n) + ((n) >> 4U)) & ((uint64_t)0xF0F0F0F0F0F0F0F))       * ((uint64_t)0x101010101010101)) + ((uint64_t)0xFF00000000000000)) >> 63U;


/* Slow, 13 logical operations */
#define XORBITS64_SWAR2(n)     COUNTBITS64_SWAR(n);     (n)&=ONE64;


/* A special algorithm with 7 logical operations */
#define XORBITS64_SWAR(n)     /*  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*  */    (n)^=((n) << 1);    /*  +***+***+***+***+***+***+***+***+***+***+***+***+***+***+***+***  */    (n)^=((n) << 2);    /*  +000+000+000+000+000+000+000+000+000+000+000+000+000+000+000+000  */    /* Then, we sum the 16 bits and store them in the bits 63 to 67. */    /* So the 63-th bit in the bit of parity. */    (n)=(((n) & ((uint64_t)0x8888888888888888)) *((uint64_t)0x1111111111111111))      >> 63;


/* A special algorithm with 7 logical operations */
#define XORBITS32_SWAR(n)     /*  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*  */    (n)^=((n) << 1);    /*  +***+***+***+***+***+***+***+***  */    (n)^=((n) << 2);    /*  +000+000+000+000+000+000+000+000  */    /* Then, we sum the 8 bits and store them in the bits 31 to 34. */    /* So the 31-th bit in the bit of parity. */    (n)=(((n) & ((uint32_t)0x88888888)) *((uint32_t)0x11111111)) >> 31;


/* A special algorithm with 7 logical operations */
#define XORBITS16_SWAR(n)     /*  +*+*+*+*+*+*+*+*  */    (n)^=((n) << 1);    /*  +***+***+***+***  */    (n)^=((n) << 2);    /*  +000+000+000+000  */    /* Then, we sum the 4 bits and store them in the bits 15 to 17. */    /* So the 15-th bit in the bit of parity. */    (n)=(((n) & ((uint16_t)0x8888)) *((uint16_t)0x1111)) >> 15;


/* A special algorithm with 5 logical operations */
#define XORBITS8_SWAR(n)     /*  +*+*+*+*  */    (n)^=((n) << 1);    /*  +0+0+0+0  */    /* Then, we sum the 4 bits and store them in the bits 7 to 9. */    /* So the 15-th bit in the bit of parity. */    (n)=(((n) & ((uint8_t)0xAA)) *((uint8_t)0x55)) >> 7;


/* The slowest method, based on the "dichotomic xor/or" */


/* A generic method using the dichotomic principle */
#define ORBITS(n,SIZE)     FOR_LOOP_COMPLETE(SIZE,RESERVED_VARIABLE>0,RESERVED_VARIABLE>>1U,                      (n)|=(n)>>RESERVED_VARIABLE)    (n)&=UINT_1;


#define NORBITS(n,SIZE)     FOR_LOOP_COMPLETE(SIZE,RESERVED_VARIABLE>0,RESERVED_VARIABLE>>1U,                      (n)|=(n)>>RESERVED_VARIABLE)    (n)=~(n);    (n)&=UINT_1;


#define XORBITS(n,SIZE)     FOR_LOOP_COMPLETE(SIZE,RESERVED_VARIABLE>0,RESERVED_VARIABLE>>1U,                      (n)^=(n)>>RESERVED_VARIABLE)    (n)&=UINT_1;


/* 13 logical operations */
#define ORBITS64_DICHO(n)     (n)|=(n) >> 32U;    (n)|=(n) >> 16U;    (n)|=(n) >> 8U;    (n)|=(n) >> 4U;    (n)|=(n) >> 2U;    (n)|=(n) >> 1U;    (n)&=ONE64;


/* 14 logical operations */
#define NORBITS64_DICHO(n)     (n)|=(n) >> 32U;    (n)|=(n) >> 16U;    (n)|=(n) >> 8U;    (n)|=(n) >> 4U;    (n)|=(n) >> 2U;    (n)|=(n) >> 1U;    (n)=~(n);    (n)&=ONE64;


/* 13 logical operations */
#define XORBITS64_DICHO(n)     (n)^=(n) >> 32U;    (n)^=(n) >> 16U;    (n)^=(n) >> 8U;    (n)^=(n) >> 4U;    (n)^=(n) >> 2U;    (n)^=(n) >> 1U;    (n)&=ONE64;


/* Choose the best method */
/* This code is also the reference version. */
#define COUNTBITS64 COUNTBITS64_POP
#define    ORBITS64    ORBITS64_POP
#define   NORBITS64   NORBITS64_POP
#define   XORBITS64   XORBITS64_POP

#define COUNTBITS32 COUNTBITS32_POP


#define COUNTBITS_UINT CONCAT(COUNTBITS,NB_BITS_UINT)
#define    ORBITS_UINT CONCAT(   ORBITS,NB_BITS_UINT)
#define   NORBITS_UINT CONCAT(  NORBITS,NB_BITS_UINT)
#define   XORBITS_UINT CONCAT(  XORBITS,NB_BITS_UINT)




#endif
