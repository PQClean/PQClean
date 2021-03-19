/**
 *
 * Reference ISO-C11 Implementation of LEDAcrypt.
 *
 * @version 3.0 (May 2020)
 *
 * In alphabetical order:
 *
 * @author Marco Baldi <m.baldi@univpm.it>
 * @author Alessandro Barenghi <alessandro.barenghi@polimi.it>
 * @author Franco Chiaraluce <f.chiaraluce@univpm.it>
 * @author Gerardo Pelosi <gerardo.pelosi@polimi.it>
 * @author Paolo Santini <p.santini@pm.univpm.it>
 *
 * This code is hereby placed in the public domain.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **/

#include "qc_ldpc_parameters.h"
#include "gf2x_arith_mod_xPplusOne.h"
#include "bf_decoding.h"
#include "djbsort.h"
#include <string.h>
#include "dfr_test.h"
/*---------------------------------------------------------------------------*/
/* Tests if the current code attains the desired DFR. If that is the case,
 * computes the threshold for the second iteration of the decoder and stores
 * it in the globally accessible vector*/

int DFR_test(POSITION_T LSparse[N0][V],
             uint8_t *secondIterThreshold)
{

   POSITION_T LSparse_loc[N0][V];  /* vector of N_0 sparse blocks */

   /* transpose blocks of L, we need its columns */
   for(int i = 0; i < N0; i++) {
      for(int j = 0; j < V; j++) {
         LSparse_loc[i][j] = (P-LSparse[i][j]) % P;
      }
      int32_sort((int32_t *)LSparse_loc[i],V);
   }
   /* Gamma matrix: an N0 x N0 block circulant matrix with block size p
    * gamma[a][b][c] stores the intersection of the first column of the a-th
    * block of L  with the c-th column of the b-th block of L */
   /* Gamma computation can be accelerated employing symmetry and QC properties */
   int gamma[N0][N0][P] = {{{0}}};

   for(int i=0; i < N0; i++ ) {
      for(int j=0; j < N0; j++ ) {
         for(int k=0; k < (V); k++) {
            for(int l=0; l < (V); l++) {
               gamma[i][j][ (P+ LSparse_loc[i][k] - LSparse_loc[j][l]) % P ]++;
            }
         }
      }
   }

   for(int i=0; i < N0; i++ ) {
      gamma[i][i][0] = 0;
   }

   /* build histogram of values in gamma */
   unsigned int gammaHist[N0][V+1] = {{0}};
   for(int i=0; i < N0; i++ ) {
      for(int j=0; j < N0; j++ ) {
         for(int k=0; k < P; k++) {
            gammaHist[i][gamma[i][j][k]]++;
         }
      }
   }

   int maxMut[N0],maxMutMinusOne[N0];
   int allBlockMaxSumst,allBlockMaxSumstMinusOne;

   for(int gammaBlockRowIdx=0; gammaBlockRowIdx < N0; gammaBlockRowIdx++) {
      int toAdd = T_BAR-1;
      maxMutMinusOne[gammaBlockRowIdx]=0;
      int histIdx= V;
      while( (histIdx > 0) && (toAdd > 0)) {
         if(gammaHist[gammaBlockRowIdx][histIdx] > (uint32_t)toAdd ) {
            maxMutMinusOne[gammaBlockRowIdx] += histIdx*toAdd;
            toAdd = 0;
         } else {
            maxMutMinusOne[gammaBlockRowIdx] +=
               histIdx*gammaHist[gammaBlockRowIdx][histIdx];
            toAdd -= gammaHist[gammaBlockRowIdx][histIdx];
            histIdx--;
         }
      }
      maxMut[gammaBlockRowIdx] = histIdx + maxMutMinusOne[gammaBlockRowIdx];
   }



   /*seek max values across all gamma blocks */
   allBlockMaxSumst = maxMut[0];
   allBlockMaxSumstMinusOne = maxMutMinusOne[0];
   for(int gammaBlockRowIdx=0; gammaBlockRowIdx < N0 ; gammaBlockRowIdx++) {
      allBlockMaxSumst = allBlockMaxSumst < maxMut[gammaBlockRowIdx] ?
                         maxMut[gammaBlockRowIdx] :
                         allBlockMaxSumst;
      allBlockMaxSumstMinusOne = allBlockMaxSumstMinusOne <
                                 maxMutMinusOne[gammaBlockRowIdx] ?
                                 maxMutMinusOne[gammaBlockRowIdx] :
                                 allBlockMaxSumstMinusOne;
   }
   if (V > (allBlockMaxSumstMinusOne + allBlockMaxSumst)) {
      *secondIterThreshold= allBlockMaxSumst+1;
      return 1;
   }
   return 0;

}
