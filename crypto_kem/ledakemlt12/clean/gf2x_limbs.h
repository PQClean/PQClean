#ifndef GF2X_LIMBS_H
#define GF2X_LIMBS_H

#include "qc_ldpc_parameters.h"

#include <inttypes.h>
#include <limits.h>
#include <stddef.h>

typedef uint64_t DIGIT;
#define DIGIT_IS_UINT64
#define DIGIT_IS_ULLONG
#define DIGIT_SIZE_B (8)
#define DIGIT_SIZE_b (DIGIT_SIZE_B << 3)

#define POSITION_T uint32_t

#endif
