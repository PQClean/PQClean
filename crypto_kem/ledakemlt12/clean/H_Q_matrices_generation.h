#pragma once
#include "qc_ldpc_parameters.h"
#include "gf2x_limbs.h"
#include "rng.h"
/*----------------------------------------------------------------------------*/


void generateHPosOnes_HtrPosOnes(POSITION_T HPosOnes[N0][DV],
                                 POSITION_T HtrPosOnes[N0][DV],
                                 AES_XOF_struct *niederreiter_keys_expander);

/*----------------------------------------------------------------------------*/

void generateQsparse(POSITION_T pos_ones[N0][M],
                     AES_XOF_struct *niederreiter_keys_expander);

/*----------------------------------------------------------------------------*/
