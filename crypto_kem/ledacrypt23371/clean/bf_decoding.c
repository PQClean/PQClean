#include "bf_decoding.h"
#include "gf2x_arith_mod_xPplusOne.h"
#include <string.h>

#define ROTBYTE(a)   ( (a << 8) | (a >> (DIGIT_SIZE_b - 8)) )
#define ROTUPC(a)   ( (a >> 8) | (a << (DIGIT_SIZE_b - 8)) )

/* The second threshold is updated before decoding retrieving it from the
 * private key field secondIterThreshold*/
int PQCLEAN_LEDACRYPT23371_CLEAN_thresholds[2] = {B0, (V) / 2 + 1};


int PQCLEAN_LEDACRYPT23371_CLEAN_bf_decoding(DIGIT out[], // N0 polynomials
                                             const POSITION_T HtrPosOnes[N0][V],
                                             DIGIT privateSyndrome[]  //  1 polynomial
)
{

   uint8_t unsatParityChecks[N0*P];
   DIGIT currSyndrome[NUM_DIGITS_GF2X_ELEMENT];
   int check;
   int iteration = 0;

   do {
      gf2x_copy(currSyndrome, privateSyndrome);
      memset(unsatParityChecks,0x00,N0*P*sizeof(uint8_t));
      for (int i = 0; i < N0; i++) {
         for (int valueIdx = 0; valueIdx < P; valueIdx++) {
            for(int HtrOneIdx = 0; HtrOneIdx < V; HtrOneIdx++) {
               POSITION_T tmp = (HtrPosOnes[i][HtrOneIdx]+valueIdx) >= P ?
                                (HtrPosOnes[i][HtrOneIdx]+valueIdx) -P : (HtrPosOnes[i][HtrOneIdx]+valueIdx);
               if (gf2x_get_coeff(currSyndrome, tmp))
                  unsatParityChecks[i*P+valueIdx]++;
            }
         }
      }

      /* iteration based threshold determination*/
      int corrt_syndrome_based= PQCLEAN_LEDACRYPT23371_CLEAN_thresholds[iteration];

      //Computation of correlation  with a full Q matrix
      for (int i = 0; i < N0; i++) {
         for (int j = 0; j < P; j++) {
            /* Correlation based flipping */
            if (unsatParityChecks[i*P+j] >= corrt_syndrome_based) {
               gf2x_toggle_coeff(out+NUM_DIGITS_GF2X_ELEMENT*i, j);
               for (int k = 0; k < V; k++) {
                  unsigned syndromePosToFlip;
                  for (int HtrOneIdx = 0; HtrOneIdx < V; HtrOneIdx++) {
                     syndromePosToFlip = (HtrPosOnes[i][k] + j) % P;
                     gf2x_toggle_coeff(privateSyndrome, syndromePosToFlip);
                  }
               } // end for v
            } // end if
         } // end for j
      } // end for i

      iteration = iteration + 1;
      check = 0;
      while (check < NUM_DIGITS_GF2X_ELEMENT && privateSyndrome[check++] == 0);

   } while (iteration < ITERATIONS_MAX && check < NUM_DIGITS_GF2X_ELEMENT);

   return (check == NUM_DIGITS_GF2X_ELEMENT);
}
