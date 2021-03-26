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


#define DIGIT_IS_UINT64
#define DIGIT_SIZE_B 8


#define DIGIT_SIZE_b  (DIGIT_SIZE_B << 3)
#define SIGNED_DIGIT int64_t

#define    POSITION_T uint32_t
#define    SIGNED_POSITION_T int32_t

/*----------------------------------------------------------------------------*/

#endif