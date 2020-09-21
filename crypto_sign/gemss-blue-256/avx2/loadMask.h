#ifndef _LOADMASK_H
#define _LOADMASK_H
#include "arch.h"



/* Apply the mask b to a, and xor this to res. */
#define XORLOADMASK1(res,a,b,SIZE)     FOR_LOOP((res)[RESERVED_VARIABLE]^=(a)[RESERVED_VARIABLE]&(b),SIZE);


/* Inlined version */
#define XORLOADMASK1_1(res,a,b)     (res)[0]^=(a)[0]&(b);

#define XORLOADMASK1_2(res,a,b)     XORLOADMASK1_1(res,a,b);    (res)[1]^=(a)[1]&(b);

#define XORLOADMASK1_3(res,a,b)     XORLOADMASK1_2(res,a,b);    (res)[2]^=(a)[2]&(b);

#define XORLOADMASK1_4(res,a,b)     XORLOADMASK1_3(res,a,b);    (res)[3]^=(a)[3]&(b);

#define XORLOADMASK1_5(res,a,b)     XORLOADMASK1_4(res,a,b);    (res)[4]^=(a)[4]&(b);

#define XORLOADMASK1_6(res,a,b)     XORLOADMASK1_5(res,a,b);    (res)[5]^=(a)[5]&(b);

#define XORLOADMASK1_7(res,a,b)     XORLOADMASK1_6(res,a,b);    (res)[6]^=(a)[6]&(b);


#endif
