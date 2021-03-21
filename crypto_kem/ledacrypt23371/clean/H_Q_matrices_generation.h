#ifndef H_Q_MATRICES_GENERATION_H
#define H_Q_MATRICES_GENERATION_H
#include "qc_ldpc_parameters.h"
#include "gf2x_limbs.h"
#include "rng.h"
#include "nistseedexpander.h"

/*----------------------------------------------------------------------------*/

void PQCLEAN_LEDACRYPT23371_CLEAN_generateHPosOnes(POSITION_T HPosOnes[N0][V], /* output*/
                                                   AES_XOF_struct *niederreiter_keys_expander);

/*----------------------------------------------------------------------------*/

void PQCLEAN_LEDACRYPT23371_CLEAN_transposeHPosOnes(POSITION_T HtrPosOnes[N0][V], /* output*/
                                                    POSITION_T const HPosOnes[N0][V]);

/*----------------------------------------------------------------------------*/

#endif