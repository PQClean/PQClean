#pragma once
#include "qc_ldpc_parameters.h"
#include "gf2x_limbs.h"
#define  ITERATIONS_MAX   (2)


int bf_decoding(DIGIT err[],
                const POSITION_T HtrPosOnes[N0][DV],
                const POSITION_T QtrPosOnes[N0][M], // N0 vectors containing exp.s of Qtr ones
                DIGIT privateSyndrome[]       //  1 polynomial  -- param. in/out
               );

/*  Definitions for DFR level 2^-64 */
#if (CATEGORY == 1) && (N0 == 2) && (DFR_SL_LEVEL == 0)
#define B0 44
#define T_BAR 4
#endif

#if ((CATEGORY == 2) || (CATEGORY == 3)) && (N0 == 2) && (DFR_SL_LEVEL == 0)
#define B0 64
#define T_BAR 5
#endif

#if ((CATEGORY == 4) || (CATEGORY == 5)) && (N0 == 2) && (DFR_SL_LEVEL == 0)
#define B0 89
#define T_BAR 6
#endif

/*  Definitions for DFR level 2^-SL */
#if (CATEGORY == 1) && (N0 == 2) && (DFR_SL_LEVEL == 1)
#define B0 43
#define T_BAR 4
#endif

#if ((CATEGORY == 2) || (CATEGORY == 3)) && (N0 == 2) && (DFR_SL_LEVEL == 1)
#define B0 64
#define T_BAR 5
#endif

#if ((CATEGORY == 4) || (CATEGORY == 5)) && (N0 == 2) && (DFR_SL_LEVEL == 1)
#define B0 88
#define T_BAR 6
#endif
