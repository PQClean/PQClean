#ifndef GF2X_LIMBS_H
#define GF2X_LIMBS_H

/*----------------------------------------------------------------------------*/

#include <stdint.h>
#include <limits.h>
#include <stddef.h>
#include "qc_ldpc_parameters.h"

/*----------------------------------------------------------------------------*/
/* limb size definitions for the multi-precision GF(2^x) library              */
/*----------------------------------------------------------------------------*/

typedef uint64_t DIGIT;
#define DIGIT_MAX SIZE_MAX


#if (DIGIT_MAX == ULLONG_MAX)
#define DIGIT_IS_ULLONG
#elif (DIGIT_MAX == ULONG_MAX)
#define DIGIT_IS_ULONG
#elif (DIGIT_MAX == UINT_MAX)
#define DIGIT_IS_UINT
#elif (DIGIT_MAX == UCHAR_MAX)
#define DIGIT_IS_UCHAR
#else
#error "unable to find the type of CPU_WORD_BITS"
#endif

#if (DIGIT_MAX == UINT64_MAX)
#define DIGIT_IS_UINT64
#define DIGIT_SIZE_B 8
#elif (DIGIT_MAX == UINT32_MAX)
#define DIGIT_IS_UINT32
#define DIGIT_SIZE_B 4
#elif (DIGIT_MAX == UINT16_MAX)
#define DIGIT_IS_UINT16
#define DIGIT_SIZE_B 2
#elif (DIGIT_MAX == UINT8_MAX)
#define DIGIT_IS_UINT8
#define DIGIT_SIZE_B 1
#else
#error "unable to find the bitsize of size_t"
#endif

#define DIGIT_SIZE_b  (DIGIT_SIZE_B << 3)
#define SIGNED_DIGIT int64_t

#define    POSITION_T uint32_t
#define    SIGNED_POSITION_T int32_t

/*----------------------------------------------------------------------------*/

#endif