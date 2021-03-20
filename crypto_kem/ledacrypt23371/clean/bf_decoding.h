#ifndef BF_DECODING_H
#define BF_DECODING_H

#include "qc_ldpc_parameters.h"
#include "gf2x_limbs.h"
#define  ITERATIONS_MAX   (2)

int bf_decoding(DIGIT err[],
                const POSITION_T HtrPosOnes[N0][V],
                DIGIT privateSyndrome[]       //  1 polynomial  -- param. in/out
               );
/* thresholds and tbars for imaxin = 0imaxout = 2 CCA ledadecoder */
#if (CATEGORY == 1) && (N0 == 2) && (DFR_SL_LEVEL == 0)
#define T_BAR 10
#define B0 39
#endif

#if (CATEGORY == 1) && (N0 == 3) && (DFR_SL_LEVEL == 0)
#define T_BAR 9
#define B0 43
#endif

#if (CATEGORY == 1) && (N0 == 4) && (DFR_SL_LEVEL == 0)
#define T_BAR 8
#define B0 46
#endif

#if (CATEGORY == 1) && (N0 == 2) && (DFR_SL_LEVEL == 1)
#define T_BAR 11
#define B0 38
#endif

#if (CATEGORY == 1) && (N0 == 3) && (DFR_SL_LEVEL == 1)
#define T_BAR 10
#define B0 43
#endif

#if (CATEGORY == 1) && (N0 == 4) && (DFR_SL_LEVEL == 1)
#define T_BAR 9
#define B0 45
#endif

#if (CATEGORY == 3) && (N0 == 2) && (DFR_SL_LEVEL == 0)
#define T_BAR 13
#define B0 56
#endif

#if (CATEGORY == 3) && (N0 == 3) && (DFR_SL_LEVEL == 0)
#define T_BAR 11
#define B0 63
#endif

#if (CATEGORY == 3) && (N0 == 4) && (DFR_SL_LEVEL == 0)
#define T_BAR 11
#define B0 67
#endif

#if (CATEGORY == 3) && (N0 == 2) && (DFR_SL_LEVEL == 1)
#define T_BAR 15
#define B0 55
#endif

#if (CATEGORY == 3) && (N0 == 3) && (DFR_SL_LEVEL == 1)
#define T_BAR 13
#define B0 62
#endif

#if (CATEGORY == 3) && (N0 == 4) && (DFR_SL_LEVEL == 1)
#define T_BAR 12
#define B0 66
#endif

#if (CATEGORY == 5) && (N0 == 2) && (DFR_SL_LEVEL == 0)
#define T_BAR 17
#define B0 74
#endif

#if (CATEGORY == 5) && (N0 == 3) && (DFR_SL_LEVEL == 0)
#define T_BAR 14
#define B0 82
#endif

#if (CATEGORY == 5) && (N0 == 4) && (DFR_SL_LEVEL == 0)
#define T_BAR 13
#define B0 88
#endif

#if (CATEGORY == 5) && (N0 == 2) && (DFR_SL_LEVEL == 1)
#define T_BAR 18
#define B0 72
#endif

#if (CATEGORY == 5) && (N0 == 3) && (DFR_SL_LEVEL == 1)
#define T_BAR 16
#define B0 81
#endif

#if (CATEGORY == 5) && (N0 == 4) && (DFR_SL_LEVEL == 1)
#define T_BAR 15
#define B0 85
#endif

#endif