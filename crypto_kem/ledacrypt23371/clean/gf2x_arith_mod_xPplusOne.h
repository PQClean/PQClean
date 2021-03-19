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

#ifndef GF2X_ARITH_MOD_XPPLUSONE_H
#define GF2X_ARITH_MOD_XPPLUSONE_H

#include "gf2x_limbs.h"
#include "qc_ldpc_parameters.h"

#include "gf2x_arith.h"
#include "rng.h"
#include "nistseedexpander.h"

/*----------------------------------------------------------------------------*/

#define                NUM_BITS_GF2X_ELEMENT (P)
#define              NUM_DIGITS_GF2X_ELEMENT ((P+DIGIT_SIZE_b-1)/DIGIT_SIZE_b)
#define MSb_POSITION_IN_MSB_DIGIT_OF_ELEMENT ( (P % DIGIT_SIZE_b) ? (P % DIGIT_SIZE_b)-1 : DIGIT_SIZE_b-1 )

#define                NUM_BITS_GF2X_MODULUS (P+1)
#define              NUM_DIGITS_GF2X_MODULUS ((P+1+DIGIT_SIZE_b-1)/DIGIT_SIZE_b)
#define MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS (P-DIGIT_SIZE_b*(NUM_DIGITS_GF2X_MODULUS-1))

#define                    INVALID_POS_VALUE (P)

#define IS_REPRESENTABLE_IN_D_BITS(D, N)                \
  (((unsigned long) N >= (1UL << (D - 1)) && (unsigned long) N < (1UL << D)) ? D : -1)

#define BITS_TO_REPRESENT(N)                            \
  (N == 0 ? 1 : (31                                     \
                 + IS_REPRESENTABLE_IN_D_BITS( 1, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 2, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 3, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 4, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 5, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 6, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 7, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 8, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 9, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(10, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(11, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(12, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(13, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(14, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(15, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(16, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(17, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(18, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(19, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(20, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(21, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(22, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(23, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(24, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(25, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(26, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(27, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(28, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(29, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(30, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(31, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(32, N)    \
                 )                                      \
   )

/*----------------------------------------------------------------------------*/

static inline void gf2x_copy(DIGIT dest[], const DIGIT in[])
{
   for (int i = NUM_DIGITS_GF2X_ELEMENT-1; i >= 0; i--)
      dest[i] = in[i];
} // end gf2x_copy

/*---------------------------------------------------------------------------*/

void gf2x_mod_mul(DIGIT Res[], const DIGIT A[], const DIGIT B[]);

/*---------------------------------------------------------------------------*/

static inline void gf2x_mod_add(DIGIT Res[], const DIGIT A[], const DIGIT B[])
{
   gf2x_add(NUM_DIGITS_GF2X_ELEMENT, Res,
            A, B);
} // end gf2x_mod_add

/*----------------------------------------------------------------------------*/

void gf2x_transpose_in_place(DIGIT
                             A[]); /* in place bit-transp. of a(x) % x^P+1  *
                                      * e.g.: a3 a2 a1 a0 --> a1 a2 a3 a0     */

/*---------------------------------------------------------------------------*/

/* population count for a single polynomial */
static inline int population_count(DIGIT upc[])
{
   int ret = 0;
   for(int i = NUM_DIGITS_GF2X_ELEMENT - 1; i >= 0; i--) {
#if defined(DIGIT_IS_ULLONG)
      ret += __builtin_popcountll((unsigned long long int) (upc[i]));
#elif defined(DIGIT_IS_ULONG)
      ret += __builtin_popcountl((unsigned long int) (upc[i]));
#elif defined(DIGIT_IS_UINT)
      ret += __builtin_popcount((unsigned int) (upc[i]));
#elif defined(DIGIT_IS_UCHAR)
      const unsigned char split_lookup[] = {
         0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4
      };
      ret += split_lookup[upc[i]&0xF] + split_lookup[upc[i]>>4];
#else
#error "Missing implementation for population_count(...) \
with this CPU word bitsize !!! "
#endif
   }
   return ret;
} // end population_count

/*--------------------------------------------------------------------------*/
/* returns the coefficient of the x^exponent term as the LSB of a digit */
static inline
DIGIT gf2x_get_coeff(const DIGIT poly[], const unsigned int exponent)
{
   unsigned int straightIdx = (NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_b -1) - exponent;
   unsigned int digitIdx = straightIdx / DIGIT_SIZE_b;
   unsigned int inDigitIdx = straightIdx % DIGIT_SIZE_b;
   return (poly[digitIdx] >> (DIGIT_SIZE_b-1-inDigitIdx)) & ((DIGIT) 1) ;
}

/*--------------------------------------------------------------------------*/

/* sets the coefficient of the x^exponent term as the LSB of a digit */
static inline
void gf2x_set_coeff(DIGIT poly[], const unsigned int exponent, DIGIT value)
{
   int straightIdx = (NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_b -1) - exponent;
   int digitIdx = straightIdx / DIGIT_SIZE_b;

   unsigned int inDigitIdx = straightIdx % DIGIT_SIZE_b;

   /* clear given coefficient */
   DIGIT mask = ~( ((DIGIT) 1) << (DIGIT_SIZE_b-1-inDigitIdx));
   poly[digitIdx] = poly[digitIdx] & mask;
   poly[digitIdx] = poly[digitIdx] | (( value & ((DIGIT) 1)) <<
                                      (DIGIT_SIZE_b-1-inDigitIdx));
}
/*--------------------------------------------------------------------------*/

/* toggles (flips) the coefficient of the x^exponent term as the LSB of a digit */
static inline
void gf2x_toggle_coeff(DIGIT poly[], const unsigned int exponent)
{

   int straightIdx = (NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_b -1) - exponent;
   int digitIdx = straightIdx / DIGIT_SIZE_b;
   unsigned int inDigitIdx = straightIdx % DIGIT_SIZE_b;

   /* clear given coefficient */
   DIGIT mask = ( ((DIGIT) 1) << (DIGIT_SIZE_b-1-inDigitIdx));
   poly[digitIdx] = poly[digitIdx] ^ mask;
}
/*--------------------------------------------------------------------------*/

void rand_circulant_sparse_block(POSITION_T *pos_ones,
                                 const int countOnes,
                                 AES_XOF_struct *seed_expander_ctx);
/*--------------------------------------------------------------------------*/

void rand_circulant_blocks_sequence(DIGIT sequence[N0*NUM_DIGITS_GF2X_ELEMENT],
                                    const int countOnes,
                                    AES_XOF_struct *seed_expander_ctx
                                   );

/*---------------------------------------------------------------------------*/
void rand_error_pos(POSITION_T errorPos[NUM_ERRORS_T],
                    AES_XOF_struct *seed_expander_ctx);

/*---------------------------------------------------------------------------*/
void rand_error_pos_shake(POSITION_T errorPos[NUM_ERRORS_T],
                          xof_shake_t *state);

/*---------------------------------------------------------------------------*/
void expand_error(DIGIT sequence[N0*NUM_DIGITS_GF2X_ELEMENT],
                  POSITION_T errorPos[NUM_ERRORS_T]);

/*----------------------------------------------------------------------------*/

void gf2x_transpose_in_place_sparse(int sizeA, POSITION_T A[]);

/*----------------------------------------------------------------------------*/
void gf2x_mod_mul_dense_to_sparse(DIGIT Res[],
                                  const DIGIT dense[],
                                  const POSITION_T sparse[],
                                  unsigned int nPos);
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

static inline
void gf2x_mod_densify_CT(DIGIT dense[NUM_DIGITS_GF2X_ELEMENT],
                         const POSITION_T exponent[],
                         int num_exponents)
{
   POSITION_T digit_pos[num_exponents];
   DIGIT indigit_bitmask[num_exponents];

   for(int i = 0; i < num_exponents; i++) {
      int straightIdx = (NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_b -1) - exponent[i];
      digit_pos[i] = straightIdx / DIGIT_SIZE_b;
      indigit_bitmask[i] = ((DIGIT)1 << (DIGIT_SIZE_b-1)) >>
                           (straightIdx % DIGIT_SIZE_b);
   }

   for(unsigned int digit_to_set_idx=0; digit_to_set_idx < NUM_DIGITS_GF2X_ELEMENT;
         digit_to_set_idx++) {
      for(int i = 0; i < num_exponents; i++) {
         /*Note: this automatically deals with invalid positions which are
          * represented as position P */
         DIGIT mask = (DIGIT) 0 - (DIGIT)(digit_to_set_idx == digit_pos[i]);
         dense[digit_to_set_idx] |= ((mask&(indigit_bitmask[i])) | (~mask&((DIGIT)0) ));
      }
   }
}



/*----------------------------------------------------------------------------*/

static inline
void gf2x_mod_densify_VT(DIGIT dense[NUM_DIGITS_GF2X_ELEMENT],
                         const POSITION_T exponent[],
                         int num_exponents)
{
   for(int j=0; j<num_exponents; j++) {
      gf2x_set_coeff(dense, exponent[j], (DIGIT) 1);
   }
}
/*----------------------------------------------------------------------------*/

static inline
void gf2x_mod_sparsify_error_CT(const DIGIT dense[N0*NUM_DIGITS_GF2X_ELEMENT],
                                POSITION_T positionsOut[]
                                )
{
   POSITION_T pos_being_written_idx = 0;
   for (int i = 0; i < N0 ; i++) {
      for(int j = 0; j< NUM_DIGITS_GF2X_ELEMENT ; j++) {
         for(int inDigitIdx = 0; inDigitIdx < DIGIT_SIZE_b; inDigitIdx++) {
            POSITION_T exponent = P*(i)
                                  + NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_b-j*DIGIT_SIZE_b
                                  - (DIGIT_SIZE_b-inDigitIdx);
            DIGIT extracted_bit = (dense[NUM_DIGITS_GF2X_ELEMENT*i+j] >> inDigitIdx) &1;
            positionsOut[pos_being_written_idx] = exponent;
            /* move forward only if the extracted bit is 1 */
            pos_being_written_idx += extracted_bit;
         }
      }
   }
}


/*----------------------------------------------------------------------------*/

#define GF2X_DIGIT_MOD_INVERSE gf2x_mod_inverse
int gf2x_mod_inverse(DIGIT out[], const DIGIT in[]);
/*----------------------------------------------------------------------------*/

#endif