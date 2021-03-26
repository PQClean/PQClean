#ifndef BF_DECODING_H
#define BF_DECODING_H

#include "qc_ldpc_parameters.h"
#include "gf2x_limbs.h"
#define  ITERATIONS_MAX   (2)

#define T_BAR 10
#define B0 39

int PQCLEAN_LEDACRYPT23371_CLEAN_bf_decoding(DIGIT err[],
                                             const POSITION_T HtrPosOnes[N0][V],
                                             DIGIT privateSyndrome[]       //  1 polynomial  -- param. in/out
);
/* thresholds and tbars for imaxin = 0imaxout = 2 CCA ledadecoder */


#endif