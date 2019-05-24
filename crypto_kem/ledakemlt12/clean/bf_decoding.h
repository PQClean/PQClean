#ifndef BF_DECODING_H
#define BF_DECODING_H

#include "qc_ldpc_parameters.h"
#include "gf2x_limbs.h"

/*  Definitions for DFR level 2^-SL with SL=128 */
#define ITERATIONS_MAX  (2)
#define B0              (43)
#define T_BAR           (4)

int PQCLEAN_LEDAKEMLT12_CLEAN_bf_decoding(DIGIT err[],
        const POSITION_T HtrPosOnes[N0][DV],
        const POSITION_T QtrPosOnes[N0][M], // N0 vectors containing exp.s of Qtr ones
        DIGIT privateSyndrome[]);

#endif
