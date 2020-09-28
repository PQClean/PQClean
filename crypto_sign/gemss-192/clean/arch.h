#ifndef _ARCH_H
#define _ARCH_H
#include "macro.h"
#include <inttypes.h>
#include <stdint.h>
#include <string.h>
/* For a portable format of printf. */


/****************** uintXX_t for compatibility  ******************/

/* Minus one */
#define MONE64 ((uint64_t)0xFFFFFFFFFFFFFFFF)
#define ZERO64 ((uint64_t)0)
#define ONE64  ((uint64_t)1)

#define MONE32 ((uint32_t)0xFFFFFFFF)
#define ZERO32 ((uint32_t)0)
#define ONE32  ((uint32_t)1)

#define MONE16 ((uint16_t)0xFFFF)
#define ZERO16 ((uint16_t)0)
#define ONE16  ((uint16_t)1)

#define MONE8 ((uint8_t)0xFF)
#define ZERO8 ((uint8_t)0)
#define ONE8  ((uint8_t)1)

/****************** Definition of an UINT  ******************/

/* XXX For the moment, this parameter cannot be modified. XXX */
/** Number of bits of an UINT. */
#define NB_BITS_UINT 64


/** Type of an UINT. */
typedef uint64_t UINT;
/** Size of an UINT in bytes. */
#define SIZEOF_UINT 8
#define LOG2_SIZE_UINT 6


#define UINT_M1 ((UINT)-1)
#define UINT_0 ((UINT)0)
#define UINT_1 ((UINT)1)

/** Print an UINT. */
#define PRINT_UINT(a) CONCAT(PRINT_X,NB_BITS_UINT)(a);

/** A reserved variable to do a for loop on a buffer of UINT. */
#define RESERVED_VARIABLE reserved_variable

#define FOR_LOOP(OP,SIZE) \
    {unsigned int RESERVED_VARIABLE; \
        for(RESERVED_VARIABLE=0U;RESERVED_VARIABLE<(SIZE);++RESERVED_VARIABLE) \
        { \
            OP;\
        } \
    }

#define FOR_LOOP_COMPLETE(INIT,CMP,INC,OP) \
    {unsigned int RESERVED_VARIABLE; \
        for(RESERVED_VARIABLE=INIT;CMP;INC) \
        { \
            OP;\
        } \
    }

/** Load a UINT from unsigned char * **/

#define LOAD_UINT(a, p) \
    (a) = (p)[7]; (a) <<= 8;\
    (a) |= (p)[6]; (a) <<= 8;\
    (a) |= (p)[5]; (a) <<= 8;\
    (a) |= (p)[4]; (a) <<= 8;\
    (a) |= (p)[3]; (a) <<= 8;\
    (a) |= (p)[2]; (a) <<= 8;\
    (a) |= (p)[1]; (a) <<= 8;\
    (a) |= (p)[0];

#define LOAD_UINT_ARRAY(a, p, N) \
    FOR_LOOP(LOAD_UINT((a)[RESERVED_VARIABLE], &(p)[8*RESERVED_VARIABLE]), (N))

/** Store a UINT to an unsigned char * **/
#define STORE_UINT(p, a) \
    (p)[0] = ((a) >> 0x00) & 0xff; \
    (p)[1] = ((a) >> 0x08) & 0xff; \
    (p)[2] = ((a) >> 0x10) & 0xff; \
    (p)[3] = ((a) >> 0x18) & 0xff; \
    (p)[4] = ((a) >> 0x20) & 0xff; \
    (p)[5] = ((a) >> 0x28) & 0xff; \
    (p)[6] = ((a) >> 0x30) & 0xff; \
    (p)[7] = ((a) >> 0x38) & 0xff;

#define STORE_UINT_ARRAY(a, p, N) \
    FOR_LOOP(STORE_UINT(&(p)[8*RESERVED_VARIABLE], (a)[RESERVED_VARIABLE]), (N))



/****************** C++ compatibility ******************/

/** Compatibility with C++. */
#define BEGIN_EXTERNC
/** Compatibility with C++. */
#define END_EXTERNC


/********************** Security **********************/

/** To enable a constant-time implementation (when it is possible) */
#define CONSTANT_TIME 1


/********************** Memory optimization **********************/

/** Set to 1 to optimize the memory, else 0 to optimize the speed */
#define OPT_MEM 0


/********************** Architecture **********************/

#define MODE_64_BITS


/********************** Reference implementation **********************/

/** To enable the reference implementation of MQsoft (option in progress) */
#define MQSOFT_REF


/****************** To choose vectorial optimizations ******************/

/* Choice of the processor */
#define INTEL_PROCESSOR 1

/* To choose the best algorithm in function of the processor */
#define PROC_HASWELL 1
#define PROC_SKYLAKE 0


/****************** gf2x library ******************/

/** Set to 1 to use the multiplication in GF(2)[x] of the gf2x library. */
/* Only when set to 1, the installation of gf2x is required */
/* Be careful because this library can be in variable-time and so vulnerable
   to the timing attacks. */
#define ENABLED_GF2X 0


/********************* Allocation *********************/

/** A negative integer for the failure of a memory allocation. */
#define ERROR_ALLOC (-2)

/** Verify if the allocation by malloc or calloc succeeds.
 *  Exit in the failure case. */
#define VERIFY_ALLOC(p) \
    if(!(p)) \
    {\
        exit(ERROR_ALLOC);\
    }

/** Verify if the allocation by malloc or calloc succeeds.
 *  Return ERROR_ALLOC in the failure case. */
#define VERIFY_ALLOC_RET(p) \
    if(!(p)) \
    {\
        return(ERROR_ALLOC);\
    }


/****************** Memory alignment ******************/



/** Free an aligned memory. */
#define ALIGNED_FREE(p) free(p);


/** Align the data on 16 bytes, useful for sse. */
#define ALIGNED16 __attribute__((aligned(16)))

#define ALIGNED16_MALLOC(p,type,nmemb,size) \
    if(posix_memalign((void**)(&(p)),16,(nmemb)*(size)))\
    {\
        exit(1);\
    }

#define ALIGNED16_CALLOC(p,type,nmemb,size) \
    ALIGNED16_MALLOC(p,type,nmemb,size);\
    memset((void*)(p),0,(nmemb)*(size))


/** Align the data on 32 bytes, useful for avx. */
#define ALIGNED32 __attribute__((aligned(32)))

#define ALIGNED32_MALLOC(p,type,nmemb,size) \
    if(posix_memalign((void**)(&(p)),32,(nmemb)*(size)))\
    {\
        exit(1);\
    }

#define ALIGNED32_CALLOC(p,type,nmemb,size) \
    ALIGNED32_MALLOC(p,type,nmemb,size);\
    memset((void*)(p),0,(nmemb)*(size));


#define NO_ALIGNED_MALLOC(p,type,nmemb,size) p=(type)malloc((nmemb)*(size));
#define NO_ALIGNED_CALLOC(p,type,nmemb,size) p=(type)calloc(nmemb,size);
/** Free a no aligned memory. */
#define NO_ALIGNED_FREE(p) free(p);



#endif
