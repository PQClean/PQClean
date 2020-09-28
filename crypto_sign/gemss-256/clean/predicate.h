#ifndef _PREDICATE_H
#define _PREDICATE_H
#include "arch.h"



/* Compare two 64-bit words in constant-time */
#define CMP_LT_U64(a,b) \
    (((((a)>>63U)^((b)>>63U))    &((((a)>>63U)-((b)>>63U))>>63U))\
     ^    ((((a)>>63U)^((b)>>63U)^ONE64)\
           &((((a)&((uint64_t)0x7FFFFFFFFFFFFFFF))\
              -((b)&((uint64_t)0x7FFFFFFFFFFFFFFF)))>>63U)))

/* Compare two 32-bit words in constant-time */
#define CMP_LT_U32(a,b) \
    (((((a)>>31U)^((b)>>31U))    &((((a)>>31U)-((b)>>31U))>>31U))\
     ^    ((((a)>>31U)^((b)>>31U)^ONE32)\
           &((((a)&((uint32_t)0x7FFFFFFF))\
              -((b)&((uint32_t)0x7FFFFFFF)))>>31U)))

/* Compare two UINT in constant-time */
#define CMP_LT_UINT CONCAT(CMP_LT_U,NB_BITS_UINT)


/* Constant-time version */
int PQCLEAN_GEMSS256_CLEAN_ISZERO(const UINT *a, unsigned int size);
int PQCLEAN_GEMSS256_CLEAN_ISNOTZERO(const UINT *a, unsigned int size);
int PQCLEAN_GEMSS256_CLEAN_ISONE(const UINT *a, unsigned int size);
int PQCLEAN_GEMSS256_CLEAN_ISEQUAL(const UINT *a, const UINT *b, unsigned int size);
int PQCLEAN_GEMSS256_CLEAN_CMP_LT(const UINT *a, const UINT *b, unsigned int size);


#define f_CMP_GT(a,b,size) f_CMP_LT(b,a,size)


/* Variable-time version */
int PQCLEAN_GEMSS256_CLEAN_ISZERO_NOCST(const UINT *a, unsigned int size);
int PQCLEAN_GEMSS256_CLEAN_ISONE_NOCST(const UINT *a, unsigned int size);
int PQCLEAN_GEMSS256_CLEAN_ISEQUAL_NOCST(const UINT *a, const UINT *b, unsigned int size);
int PQCLEAN_GEMSS256_CLEAN_CMP_LT_NOCST(const UINT *a, const UINT *b, unsigned int size);


#define f_CMP_GT_NOCST(a,b,size) f_CMP_LT_NOCST(b,a,size)


/* Inlined version */
/* Equal to 0 */
#define PQCLEAN_GEMSS256_CLEAN_ISZERO1_NOCST(a) ((a)[0]==0)
#define PQCLEAN_GEMSS256_CLEAN_ISZERO2_NOCST(a) (PQCLEAN_GEMSS256_CLEAN_ISZERO1_NOCST(a)&&((a)[1]==0))
#define PQCLEAN_GEMSS256_CLEAN_ISZERO3_NOCST(a) (PQCLEAN_GEMSS256_CLEAN_ISZERO2_NOCST(a)&&((a)[2]==0))
#define PQCLEAN_GEMSS256_CLEAN_ISZERO4_NOCST(a) (PQCLEAN_GEMSS256_CLEAN_ISZERO3_NOCST(a)&&((a)[3]==0))
#define PQCLEAN_GEMSS256_CLEAN_ISZERO5_NOCST(a) (PQCLEAN_GEMSS256_CLEAN_ISZERO4_NOCST(a)&&((a)[4]==0))
#define PQCLEAN_GEMSS256_CLEAN_ISZERO6_NOCST(a) (PQCLEAN_GEMSS256_CLEAN_ISZERO5_NOCST(a)&&((a)[5]==0))
#define PQCLEAN_GEMSS256_CLEAN_ISZERO7_NOCST(a) (PQCLEAN_GEMSS256_CLEAN_ISZERO6_NOCST(a)&&((a)[6]==0))
#define PQCLEAN_GEMSS256_CLEAN_ISZERO8_NOCST(a) (PQCLEAN_GEMSS256_CLEAN_ISZERO7_NOCST(a)&&((a)[7]==0))
#define PQCLEAN_GEMSS256_CLEAN_ISZERO9_NOCST(a) (PQCLEAN_GEMSS256_CLEAN_ISZERO8_NOCST(a)&&((a)[8]==0))


/* Equal to 1 */
#define PQCLEAN_GEMSS256_CLEAN_ISONE1_NOCST(a) ((a)[0]==1)
#define PQCLEAN_GEMSS256_CLEAN_ISONE2_NOCST(a) (PQCLEAN_GEMSS256_CLEAN_ISONE1_NOCST(a)&&((a)[1]==0))
#define PQCLEAN_GEMSS256_CLEAN_ISONE3_NOCST(a) (PQCLEAN_GEMSS256_CLEAN_ISONE2_NOCST(a)&&((a)[2]==0))
#define PQCLEAN_GEMSS256_CLEAN_ISONE4_NOCST(a) (PQCLEAN_GEMSS256_CLEAN_ISONE3_NOCST(a)&&((a)[3]==0))
#define PQCLEAN_GEMSS256_CLEAN_ISONE5_NOCST(a) (PQCLEAN_GEMSS256_CLEAN_ISONE4_NOCST(a)&&((a)[4]==0))
#define PQCLEAN_GEMSS256_CLEAN_ISONE6_NOCST(a) (PQCLEAN_GEMSS256_CLEAN_ISONE5_NOCST(a)&&((a)[5]==0))
#define PQCLEAN_GEMSS256_CLEAN_ISONE7_NOCST(a) (PQCLEAN_GEMSS256_CLEAN_ISONE6_NOCST(a)&&((a)[6]==0))
#define PQCLEAN_GEMSS256_CLEAN_ISONE8_NOCST(a) (PQCLEAN_GEMSS256_CLEAN_ISONE7_NOCST(a)&&((a)[7]==0))
#define PQCLEAN_GEMSS256_CLEAN_ISONE9_NOCST(a) (PQCLEAN_GEMSS256_CLEAN_ISONE8_NOCST(a)&&((a)[8]==0))


/* Equality */
#define PQCLEAN_GEMSS256_CLEAN_ISEQUAL1_NOCST(a,b) ((a)[0]==(b)[0])
#define PQCLEAN_GEMSS256_CLEAN_ISEQUAL2_NOCST(a,b) (PQCLEAN_GEMSS256_CLEAN_ISEQUAL1_NOCST(a,b)&&((a)[1]==(b)[1]))
#define PQCLEAN_GEMSS256_CLEAN_ISEQUAL3_NOCST(a,b) (PQCLEAN_GEMSS256_CLEAN_ISEQUAL2_NOCST(a,b)&&((a)[2]==(b)[2]))
#define PQCLEAN_GEMSS256_CLEAN_ISEQUAL4_NOCST(a,b) (PQCLEAN_GEMSS256_CLEAN_ISEQUAL3_NOCST(a,b)&&((a)[3]==(b)[3]))
#define PQCLEAN_GEMSS256_CLEAN_ISEQUAL5_NOCST(a,b) (PQCLEAN_GEMSS256_CLEAN_ISEQUAL4_NOCST(a,b)&&((a)[4]==(b)[4]))
#define PQCLEAN_GEMSS256_CLEAN_ISEQUAL6_NOCST(a,b) (PQCLEAN_GEMSS256_CLEAN_ISEQUAL5_NOCST(a,b)&&((a)[5]==(b)[5]))
#define PQCLEAN_GEMSS256_CLEAN_ISEQUAL7_NOCST(a,b) (PQCLEAN_GEMSS256_CLEAN_ISEQUAL6_NOCST(a,b)&&((a)[6]==(b)[6]))
#define PQCLEAN_GEMSS256_CLEAN_ISEQUAL8_NOCST(a,b) (PQCLEAN_GEMSS256_CLEAN_ISEQUAL7_NOCST(a,b)&&((a)[7]==(b)[7]))
#define PQCLEAN_GEMSS256_CLEAN_ISEQUAL9_NOCST(a,b) (PQCLEAN_GEMSS256_CLEAN_ISEQUAL8_NOCST(a,b)&&((a)[8]==(b)[8]))


/* Comparison, less than */
#define PQCLEAN_GEMSS256_CLEAN_CMP_LT1_NOCST(a,b) ((a)[0]<(b)[0])
#define PQCLEAN_GEMSS256_CLEAN_CMP_LT2_NOCST(a,b) (((a)[1]==(b)[1])?PQCLEAN_GEMSS256_CLEAN_CMP_LT1_NOCST(a,b)\
        :((a)[1]<(b)[1]))
#define PQCLEAN_GEMSS256_CLEAN_CMP_LT3_NOCST(a,b) (((a)[2]==(b)[2])?PQCLEAN_GEMSS256_CLEAN_CMP_LT2_NOCST(a,b)\
        :((a)[2]<(b)[2]))
#define PQCLEAN_GEMSS256_CLEAN_CMP_LT4_NOCST(a,b) (((a)[3]==(b)[3])?PQCLEAN_GEMSS256_CLEAN_CMP_LT3_NOCST(a,b)\
        :((a)[3]<(b)[3]))
#define PQCLEAN_GEMSS256_CLEAN_CMP_LT5_NOCST(a,b) (((a)[4]==(b)[4])?PQCLEAN_GEMSS256_CLEAN_CMP_LT4_NOCST(a,b)\
        :((a)[4]<(b)[4]))
#define PQCLEAN_GEMSS256_CLEAN_CMP_LT6_NOCST(a,b) (((a)[5]==(b)[5])?PQCLEAN_GEMSS256_CLEAN_CMP_LT5_NOCST(a,b)\
        :((a)[5]<(b)[5]))
#define PQCLEAN_GEMSS256_CLEAN_CMP_LT7_NOCST(a,b) (((a)[6]==(b)[6])?PQCLEAN_GEMSS256_CLEAN_CMP_LT6_NOCST(a,b)\
        :((a)[6]<(b)[6]))
#define PQCLEAN_GEMSS256_CLEAN_CMP_LT8_NOCST(a,b) (((a)[7]==(b)[7])?PQCLEAN_GEMSS256_CLEAN_CMP_LT7_NOCST(a,b)\
        :((a)[7]<(b)[7]))
#define PQCLEAN_GEMSS256_CLEAN_CMP_LT9_NOCST(a,b) (((a)[8]==(b)[8])?PQCLEAN_GEMSS256_CLEAN_CMP_LT8_NOCST(a,b)\
        :((a)[8]<(b)[8]))


#endif
