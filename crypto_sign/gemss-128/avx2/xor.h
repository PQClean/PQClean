#ifndef _XOR_H
#define _XOR_H
#include "arch.h"



/* Simple precision. */
#define XOR_WORD(c,a) (c)=(a)^(b);
#define XOR_WORD_2(c,a) (c)^=(a);


/* Xor of buffers. */
#define XOR(c,a,b,SIZE) \
    FOR_LOOP((c)[RESERVED_VARIABLE]=(a)[RESERVED_VARIABLE]\
                                    ^(b)[RESERVED_VARIABLE],SIZE);

#define XOR_2(c,a,SIZE) \
    FOR_LOOP((c)[RESERVED_VARIABLE]^=(a)[RESERVED_VARIABLE],SIZE);


/* Inlined version, c[i]=a[i]^b[i]. */
#define XOR1(c,a,b) \
    (c)[0]=(a)[0]^(b)[0];

#define XOR2(c,a,b) \
    XOR1(c,a,b);\
    (c)[1]=(a)[1]^(b)[1];

#define XOR3(c,a,b) \
    XOR2(c,a,b);\
    (c)[2]=(a)[2]^(b)[2];

#define XOR4(c,a,b) \
    XOR3(c,a,b);\
    (c)[3]=(a)[3]^(b)[3];

#define XOR5(c,a,b) \
    XOR4(c,a,b);\
    (c)[4]=(a)[4]^(b)[4];

#define XOR6(c,a,b) \
    XOR5(c,a,b);\
    (c)[5]=(a)[5]^(b)[5];

#define XOR7(c,a,b) \
    XOR6(c,a,b);\
    (c)[6]=(a)[6]^(b)[6];

#define XOR8(c,a,b) \
    XOR7(c,a,b);\
    (c)[7]=(a)[7]^(b)[7];

#define XOR9(c,a,b) \
    XOR8(c,a,b);\
    (c)[8]=(a)[8]^(b)[8];


/* Inlined version, c[i]^=a[i]. */
#define XOR1_2(c,a) \
    (c)[0]^=(a)[0];

#define XOR2_2(c,a) \
    XOR1_2(c,a);\
    (c)[1]^=(a)[1];

#define XOR3_2(c,a) \
    XOR2_2(c,a);\
    (c)[2]^=(a)[2];

#define XOR4_2(c,a) \
    XOR3_2(c,a);\
    (c)[3]^=(a)[3];

#define XOR5_2(c,a) \
    XOR4_2(c,a);\
    (c)[4]^=(a)[4];

#define XOR6_2(c,a) \
    XOR5_2(c,a);\
    (c)[5]^=(a)[5];

#define XOR7_2(c,a) \
    XOR6_2(c,a);\
    (c)[6]^=(a)[6];

#define XOR8_2(c,a) \
    XOR7_2(c,a);\
    (c)[7]^=(a)[7];

#define XOR9_2(c,a) \
    XOR8_2(c,a);\
    (c)[8]^=(a)[8];

#define XOR10_2(c,a) \
    XOR9_2(c,a);\
    (c)[9]^=(a)[9];

#define XOR11_2(c,a) \
    XOR10_2(c,a);\
    (c)[10]^=(a)[10];

#define XOR12_2(c,a) \
    XOR11_2(c,a);\
    (c)[11]^=(a)[11];

#define XOR13_2(c,a) \
    XOR12_2(c,a);\
    (c)[12]^=(a)[12];

#define XOR14_2(c,a) \
    XOR13_2(c,a);\
    (c)[13]^=(a)[13];

#define XOR15_2(c,a) \
    XOR14_2(c,a);\
    (c)[14]^=(a)[14];

#define XOR16_2(c,a) \
    XOR15_2(c,a);\
    (c)[15]^=(a)[15];

#define XOR17_2(c,a) \
    XOR16_2(c,a);\
    (c)[16]^=(a)[16];

#define XOR18_2(c,a) \
    XOR17_2(c,a);\
    (c)[17]^=(a)[17];


#endif
