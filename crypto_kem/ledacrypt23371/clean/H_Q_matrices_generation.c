#include "H_Q_matrices_generation.h"
#include "gf2x_arith_mod_xPplusOne.h"
#include "nistseedexpander.h"

/*----------------------------------------------------------------------------*/

void PQCLEAN_LEDACRYPT23371_CLEAN_generateHPosOnes(POSITION_T HPosOnes[N0][V],
                                                   AES_XOF_struct *keys_expander
)
{
   for (int i = 0; i < N0; i++) {
      /* Generate a random block of Htr */
       PQCLEAN_LEDACRYPT23371_CLEAN_rand_circulant_sparse_block(&HPosOnes[i][0],
                                                                V,
                                                                keys_expander);
   }
} // end generateHtr_HtrPosOnes

void PQCLEAN_LEDACRYPT23371_CLEAN_transposeHPosOnes(POSITION_T HtrPosOnes[N0][V], /* output*/
                                                    POSITION_T const HPosOnes[N0][V]
)
{
   for (int i = 0; i < N0; i++) {
      /* Obtain directly the sparse representation of the block of H */
      for (int k = 0; k < V; k++) {
         HtrPosOnes[i][k] = (P - HPosOnes[i][k])  % P; /* transposes indexes */
      }// end for k
   }
} // end PQCLEAN_LEDACRYPT23371_CLEAN_transposeHPosOnes

/*----------------------------------------------------------------------------*/
