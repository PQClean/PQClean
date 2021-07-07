#ifndef _INIT_H
#define _INIT_H
#include "xor.h"



/* Swap */
/* a,b = b,a */
#define SWAP(XOR_2,a,b,SIZE) \
    XOR_2(a,b,SIZE); \
    XOR_2(b,a,SIZE); \
    XOR_2(a,b,SIZE);


/* Inlined version */
#define SWAP_MACRO(XOR_2,a,b) \
    XOR_2(a,b); \
    XOR_2(b,a); \
    XOR_2(a,b);

#define SWAP_WORD(a,b) SWAP_MACRO(XOR_WORD_2,a,b);
#define SWAP1(a,b) SWAP_MACRO(XOR1_2,a,b);
#define SWAP2(a,b) SWAP_MACRO(XOR2_2,a,b);
#define SWAP3(a,b) SWAP_MACRO(XOR3_2,a,b);
#define SWAP4(a,b) SWAP_MACRO(XOR4_2,a,b);
#define SWAP5(a,b) SWAP_MACRO(XOR5_2,a,b);
#define SWAP6(a,b) SWAP_MACRO(XOR6_2,a,b);
#define SWAP7(a,b) SWAP_MACRO(XOR7_2,a,b);


/* Copy */
#define COPY(c,a,SIZE) \
    FOR_LOOP((c)[RESERVED_VARIABLE]=(a)[RESERVED_VARIABLE],SIZE)


/* Inlined version */
#define COPY1(c,a) \
    (c)[0]=(a)[0];

#define COPY2(c,a) \
    COPY1(c,a);\
    (c)[1]=(a)[1];

#define COPY3(c,a) \
    COPY2(c,a); \
    (c)[2]=(a)[2];

#define COPY4(c,a) \
    COPY3(c,a); \
    (c)[3]=(a)[3];

#define COPY5(c,a) \
    COPY4(c,a); \
    (c)[4]=(a)[4];

#define COPY6(c,a) \
    COPY5(c,a); \
    (c)[5]=(a)[5];

#define COPY7(c,a) \
    COPY6(c,a); \
    (c)[6]=(a)[6];

#define COPY8(c,a) \
    COPY7(c,a); \
    (c)[7]=(a)[7];

#define COPY9(c,a) \
    COPY8(c,a); \
    (c)[8]=(a)[8];


/* Init to 0 */
#define SET0(c,SIZE) \
    FOR_LOOP((c)[RESERVED_VARIABLE]=0,SIZE)


/* Inlined version */
#define SET0_1(c) \
    (c)[0]=0;

#define SET0_2(c) \
    SET0_1(c);\
    (c)[1]=0;

#define SET0_3(c) \
    SET0_2(c);\
    (c)[2]=0;

#define SET0_4(c) \
    SET0_3(c);\
    (c)[3]=0;

#define SET0_5(c) \
    SET0_4(c);\
    (c)[4]=0;

#define SET0_6(c) \
    SET0_5(c);\
    (c)[5]=0;

#define SET0_7(c) \
    SET0_6(c);\
    (c)[6]=0;


/* Init to 1 */
#define SET1(c,SIZE) \
    (c)[0]=1;\
    FOR_LOOP((c)[RESERVED_VARIABLE+1]=0,(SIZE)-1)


/* Inlined version */
#define SET1_1(c) \
    (c)[0]=1; \

#define SET1_2(c) \
    SET1_1(c);\
    SET0_1((c)+1);

#define SET1_3(c) \
    SET1_1(c);\
    SET0_2((c)+1);

#define SET1_4(c) \
    SET1_1(c);\
    SET0_3((c)+1);

#define SET1_5(c) \
    SET1_1(c);\
    SET0_4((c)+1);

#define SET1_6(c) \
    SET1_1(c);\
    SET0_5((c)+1);


#endif
