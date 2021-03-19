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


#include "gf2x_arith_mod_xPplusOne.h"
#include "rng.h"
#include <string.h>  // memcpy(...), memset(...)
#include <stdalign.h>
#include "djbsort.h"
#include "nistseedexpander.h"

void left_bit_shift_wide_n(const int length, DIGIT in[], int amount);
void rotate_bit_left(DIGIT in[]);
void rotate_bit_right(DIGIT in[]);

/*----------------------------------------------------------------------------*/

/* specialized for nin == 2 * NUM_DIGITS_GF2X_ELEMENT, as it is only used
 * by gf2x_mul */
static inline
void gf2x_mod(DIGIT out[], const DIGIT in[])
{
   DIGIT aux[NUM_DIGITS_GF2X_ELEMENT+1];
   memcpy(aux, in, (NUM_DIGITS_GF2X_ELEMENT+1)*DIGIT_SIZE_B);
#if MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS != 0
   right_bit_shift_n(NUM_DIGITS_GF2X_ELEMENT+1, aux,
                     MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS);
#endif
   gf2x_add(NUM_DIGITS_GF2X_ELEMENT,out,
            aux+1,in+NUM_DIGITS_GF2X_ELEMENT);
#if MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS != 0
   out[0] &=  ((DIGIT)1 << MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS) - 1 ;
#endif

} // end gf2x_mod

/*----------------------------------------------------------------------------*/

static
void left_bit_shift(const int length, DIGIT in[])
{

   int j;
   for (j = 0; j < length-1; j++) {
      in[j] <<= 1;                    /* logical shift does not need clearing */
      in[j] |= in[j+1] >> (DIGIT_SIZE_b-1);
   }
   in[j] <<= 1;
} // end left_bit_shift

/*----------------------------------------------------------------------------*/

static
void right_bit_shift(const int length, DIGIT in[])
{

   int j;
   for (j = length-1; j > 0 ; j--) {
      in[j] >>= 1;
      in[j] |=  (in[j-1] & (DIGIT)0x01) << (DIGIT_SIZE_b-1);
   }
   in[j] >>=1;
} // end right_bit_shift

/*----------------------------------------------------------------------------*/
/* shifts by whole digits */
static inline
void left_DIGIT_shift_n(const int length, DIGIT in[], int amount)
{
   int j;
   for (j = 0; (j + amount) < length; j++) {
      in[j] = in[j+amount];
   }
   for (; j < length; j++) {
      in[j] = (DIGIT)0;
   }
} // end left_bit_shift_n

/*----------------------------------------------------------------------------*/
/* may shift by an arbitrary amount*/

void left_bit_shift_wide_n(const int length, DIGIT in[], int amount)
{
   left_DIGIT_shift_n(length, in, amount / DIGIT_SIZE_b);
   left_bit_shift_n(length, in, amount % DIGIT_SIZE_b);
} // end left_bit_shift_n

/*----------------------------------------------------------------------------*/

#if (defined(DIGIT_IS_UINT8) || defined(DIGIT_IS_UINT16))
static
uint8_t byte_reverse_with_less32bitDIGIT(uint8_t b)
{
   uint8_t r = b;
   int s = (sizeof(b) << 3) - 1;
   for (b >>= 1; b; b >>= 1) {
      r <<= 1;
      r |= b & 1;
      s--;
   }
   r <<= s;
   return r;
} // end byte_reverse_less32bitDIGIT
#endif

#if defined(DIGIT_IS_UINT32)
static
uint8_t byte_reverse_with_32bitDIGIT(uint8_t b)
{
   b = ( (b * 0x0802LU & 0x22110LU) | (b * 0x8020LU & 0x88440LU)
       ) * 0x10101LU >> 16;
   return b;
} // end byte_reverse_32bitDIGIT
#endif

#if defined(DIGIT_IS_UINT64)
static
uint8_t byte_reverse_with_64bitDIGIT(uint8_t b)
{
   b = (b * 0x0202020202ULL & 0x010884422010ULL) % 1023;
   return b;
} // end byte_reverse_64bitDIGIT
#endif

/*----------------------------------------------------------------------------*/

static
DIGIT reverse_digit(const DIGIT b)
{
   int i;
   union toReverse_t {
      uint8_t inByte[DIGIT_SIZE_B];
      DIGIT digitValue;
   } toReverse;

   toReverse.digitValue = b;
#if defined(DIGIT_IS_UINT64)
   for (i = 0; i < DIGIT_SIZE_B; i++)
      toReverse.inByte[i] = byte_reverse_with_64bitDIGIT(toReverse.inByte[i]);
   return __builtin_bswap64(toReverse.digitValue);
#elif defined(DIGIT_IS_UINT32)
   for (i = 0; i < DIGIT_SIZE_B; i++)
      toReverse.inByte[i] = byte_reverse_with_32bitDIGIT(toReverse.inByte[i]);
   return __builtin_bswap32(toReverse.digitValue);
#elif defined(DIGIT_IS_UINT16)
   for (i = 0; i < DIGIT_SIZE_B; i++)
      toReverse.inByte[i] = byte_reverse_with_less32bitDIGIT(toReverse.inByte[i]);
   reversed = __builtin_bswap16(toReverse.digitValue);
#elif defined(DIGIT_IS_UINT8)
   return byte_reverse_with_less32bitDIGIT(toReverse.inByte[0]);
#else
#error "Missing implementation for reverse_digit(...) \
with this CPU word bitsize !!! "
#endif
   return toReverse.digitValue;
} // end reverse_digit


/*----------------------------------------------------------------------------*/

void gf2x_transpose_in_place(DIGIT A[])
{
   /* it keeps the lsb in the same position and
    * inverts the sequence of the remaining bits
    */

   DIGIT mask = (DIGIT)0x1;
   DIGIT rev1, rev2, a00;
   int i, slack_bits_amount = NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_b - P;

   if (NUM_DIGITS_GF2X_ELEMENT == 1) {
      a00 = A[0] & mask;
      right_bit_shift(1, A);
      rev1 = reverse_digit(A[0]);
#if (NUM_DIGITS_GF2X_MOD_P_ELEMENT*DIGIT_SIZE_b - P)
      rev1 >>= (DIGIT_SIZE_b-(P%DIGIT_SIZE_b));
#endif
      A[0] = (rev1 & (~mask)) | a00;
      return;
   }

   a00 = A[NUM_DIGITS_GF2X_ELEMENT-1] & mask;
   right_bit_shift(NUM_DIGITS_GF2X_ELEMENT, A);

   for (i = NUM_DIGITS_GF2X_ELEMENT-1; i >= (NUM_DIGITS_GF2X_ELEMENT+1)/2; i--) {
      rev1 = reverse_digit(A[i]);
      rev2 = reverse_digit(A[NUM_DIGITS_GF2X_ELEMENT-1-i]);
      A[i] = rev2;
      A[NUM_DIGITS_GF2X_ELEMENT-1-i] = rev1;
   }
   if (NUM_DIGITS_GF2X_ELEMENT % 2 == 1)
      A[NUM_DIGITS_GF2X_ELEMENT/2] = reverse_digit(A[NUM_DIGITS_GF2X_ELEMENT/2]);

   if (slack_bits_amount)
      right_bit_shift_n(NUM_DIGITS_GF2X_ELEMENT, A,slack_bits_amount);
   A[NUM_DIGITS_GF2X_ELEMENT-1] = (A[NUM_DIGITS_GF2X_ELEMENT-1] & (~mask)) | a00;
} // end transpose_in_place

/*----------------------------------------------------------------------------*/

void rotate_bit_left(DIGIT in[])   /*  equivalent to x * in(x) mod x^P+1 */
{

   DIGIT mask,rotated_bit;

   if (NUM_DIGITS_GF2X_MODULUS == NUM_DIGITS_GF2X_ELEMENT) {

      int msb_offset_in_digit = MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS-1;
      mask = ((DIGIT)0x1) << msb_offset_in_digit;
      rotated_bit = !!(in[0] & mask);
      in[0] &= ~mask;                     /* clear shifted bit */
      left_bit_shift(NUM_DIGITS_GF2X_ELEMENT, in);
   } else {
      /* NUM_DIGITS_GF2X_MODULUS == 1 + NUM_DIGITS_GF2X_ELEMENT and
              * MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS == 0
              */
      mask =  ((DIGIT)0x1) << (DIGIT_SIZE_b-1);
      rotated_bit = !!(in[0] & mask);
      in[0] &= ~mask;                     /* clear shifted bit */
      left_bit_shift(NUM_DIGITS_GF2X_ELEMENT, in);

   }
   in[NUM_DIGITS_GF2X_ELEMENT-1] |= rotated_bit;
} // end rotate_bit_left



/*----------------------------------------------------------------------------*/

void rotate_bit_right(DIGIT in[])   /*  x^{-1} * in(x) mod x^P+1 */
{

   DIGIT rotated_bit = in[NUM_DIGITS_GF2X_ELEMENT-1] & ((DIGIT)0x1);
   right_bit_shift(NUM_DIGITS_GF2X_ELEMENT, in);

   if (NUM_DIGITS_GF2X_MODULUS == NUM_DIGITS_GF2X_ELEMENT) {
      int msb_offset_in_digit = MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS-1;
      rotated_bit = rotated_bit << msb_offset_in_digit;
   } else {
      /* NUM_DIGITS_GF2X_MODULUS == 1 + NUM_DIGITS_GF2X_ELEMENT and
              * MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS == 0
              */
      rotated_bit = rotated_bit << (DIGIT_SIZE_b-1);
   }
   in[0] |= rotated_bit;
} // end rotate_bit_right

static
void gf2x_swap(const int length,
               DIGIT f[],
               DIGIT s[])
{
   DIGIT t;
   for (int i = length-1; i >= 0; i--) {
      t = f[i];
      f[i] = s[i];
      s[i] = t;
   }
}  // end gf2x_swap

/*----------------------------------------------------------------------------*/
/*
 * Optimized extended GCD algorithm to compute the multiplicative inverse of
 * a non-zero element in GF(2)[x] mod x^P+1, in polyn. representation.
 *
 * H. Brunner, A. Curiger, and M. Hofstetter. 1993.
 * On Computing Multiplicative Inverses in GF(2^m).
 * IEEE Trans. Comput. 42, 8 (August 1993), 1010-1015.
 * DOI=http://dx.doi.org/10.1109/12.238496
 *
 *
 * Henri Cohen, Gerhard Frey, Roberto Avanzi, Christophe Doche, Tanja Lange,
 * Kim Nguyen, and Frederik Vercauteren. 2012.
 * Handbook of Elliptic and Hyperelliptic Curve Cryptography,
 * Second Edition (2nd ed.). Chapman & Hall/CRC.
 * (Chapter 11 -- Algorithm 11.44 -- pag 223)
 *
 */


int gf2x_mod_inverse(DIGIT out[], const DIGIT in[])     /* in^{-1} mod x^P-1 */
{
   int i;
   long int delta = 0;
   alignas(32) DIGIT u[NUM_DIGITS_GF2X_ELEMENT] = {0},
         v[NUM_DIGITS_GF2X_ELEMENT] = {0},
                                      s[NUM_DIGITS_GF2X_MODULUS] = {0},
                                            f[NUM_DIGITS_GF2X_MODULUS] = {0};

   DIGIT mask;
   u[NUM_DIGITS_GF2X_ELEMENT-1] = 0x1;
   v[NUM_DIGITS_GF2X_ELEMENT-1] = 0x0;

   s[NUM_DIGITS_GF2X_MODULUS-1] = 0x1;
   if (MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS == 0)
      mask = 0x1;
   else
      mask = (((DIGIT)0x1) << MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS);
   s[0] |= mask;

   for (i = NUM_DIGITS_GF2X_ELEMENT-1; i>=0 && in[i] == 0; i--);
   if (i < 0) return 0;

   if (NUM_DIGITS_GF2X_MODULUS == 1 + NUM_DIGITS_GF2X_ELEMENT)
      for (i = NUM_DIGITS_GF2X_MODULUS-1; i >= 1 ; i--) f[i] = in[i-1];
   else  /* they are equal */
      for (i = NUM_DIGITS_GF2X_MODULUS-1; i >= 0 ; i--) f[i] = in[i];

   for (i = 1; i <= 2*P; i++) {
      if ( (f[0] & mask) == 0 ) {
         left_bit_shift(NUM_DIGITS_GF2X_MODULUS, f);
         rotate_bit_left(u);
         delta += 1;
      } else {
         if ( (s[0] & mask) != 0) {
            gf2x_add(NUM_DIGITS_GF2X_MODULUS, s,
                     s,f);
            gf2x_mod_add(v, v, u);
         }
         left_bit_shift(NUM_DIGITS_GF2X_MODULUS, s);
         if ( delta == 0 ) {
            gf2x_swap(NUM_DIGITS_GF2X_MODULUS, f, s);
            gf2x_swap(NUM_DIGITS_GF2X_ELEMENT, u, v);
            rotate_bit_left(u);
            delta = 1;
         } else {
            rotate_bit_right(u);
            delta = delta - 1;
         }
      }
   }
   for (i = NUM_DIGITS_GF2X_ELEMENT-1; i >= 0 ; i--) out[i] = u[i];
   return (delta == 0);
} // end gf2x_mod_inverse


/*----------------------------------------------------------------------------*/

void gf2x_mod_mul(DIGIT Res[], const DIGIT A[], const DIGIT B[])
{

   DIGIT aux[2*NUM_DIGITS_GF2X_ELEMENT];
   GF2X_MUL(2*NUM_DIGITS_GF2X_ELEMENT, aux,
            NUM_DIGITS_GF2X_ELEMENT, A,
            NUM_DIGITS_GF2X_ELEMENT, B);
    gf2x_mod(Res, aux);

} // end gf2x_mod_mul

/*----------------------------------------------------------------------------*/
/* computes operand*x^shiftAmt + Res. assumes res is
 * wide and operand is NUM_DIGITS_GF2X_ELEMENT with blank slack bits */
static inline
void gf2x_fmac(DIGIT Res[],
               const DIGIT operand[],
               const unsigned int shiftAmt)
{
   unsigned int digitShift = shiftAmt / DIGIT_SIZE_b;
   unsigned int inDigitShift= shiftAmt % DIGIT_SIZE_b;
   DIGIT tmp,prevLo=0;
   int i;
   SIGNED_DIGIT inDigitShiftMask = ((SIGNED_DIGIT) (inDigitShift>0)  <<
                                    (DIGIT_SIZE_b-1)) >> (DIGIT_SIZE_b-1);
   for(i = NUM_DIGITS_GF2X_ELEMENT-1; i>=0 ; i--) {
      tmp = operand[i];
      Res[NUM_DIGITS_GF2X_ELEMENT+i-digitShift] ^= prevLo | (tmp << inDigitShift);
      prevLo = (tmp >> (DIGIT_SIZE_b - inDigitShift)) & inDigitShiftMask;
   }
   Res[NUM_DIGITS_GF2X_ELEMENT+i-digitShift] ^= prevLo;
}

/*----------------------------------------------------------------------------*/

/*PRE: the representation of the sparse coefficients is sorted in increasing
 order of the coefficients themselves */
void gf2x_mod_mul_dense_to_sparse(DIGIT Res[],
                                  const DIGIT dense[],
                                  const POSITION_T sparse[],
                                  unsigned int nPos)
{
   DIGIT resDouble[2*NUM_DIGITS_GF2X_ELEMENT] = {0};

   for (unsigned int i = 0; i < nPos; i++) {
      if (sparse[i] != INVALID_POS_VALUE) {
         gf2x_fmac(resDouble, dense,sparse[i]);
      }
   }

    gf2x_mod(Res, resDouble);

} // end gf2x_mod_mul

/*----------------------------------------------------------------------------*/


void gf2x_transpose_in_place_sparse(int sizeA, POSITION_T A[])
{

   POSITION_T t;
   int i = 0, j;

   if (A[i] == 0) {
      i = 1;
   }
   j = i;

   for (; i < sizeA && A[i] != INVALID_POS_VALUE; i++) {
      A[i] = P-A[i];
   }

   for (i -= 1; j < i; j++, i--) {
      t = A[j];
      A[j] = A[i];
      A[i] = t;
   }

} // end gf2x_transpose_in_place_sparse

/*----------------------------------------------------------------------------*/
/* Obtains fresh randomness and seed-expands it until all the required positions
 * for the '1's in the circulant block are obtained */

void rand_circulant_sparse_block(POSITION_T *pos_ones,
                                 const int countOnes,
                                 AES_XOF_struct *seed_expander_ctx)
{

   int duplicated, placedOnes = 0;

   while (placedOnes < countOnes) {
      int p = rand_range(NUM_BITS_GF2X_ELEMENT,
                         BITS_TO_REPRESENT(P),
                         seed_expander_ctx);
      duplicated = 0;
      for (int j = 0; j < placedOnes; j++) if (pos_ones[j] == (uint32_t)p) duplicated = 1;
      if (duplicated == 0) {
         pos_ones[placedOnes] = p;
         placedOnes++;
      }
   }
} // rand_circulant_sparse_block

/*----------------------------------------------------------------------------*/

void rand_circulant_blocks_sequence(DIGIT sequence[N0*NUM_DIGITS_GF2X_ELEMENT],
                                    const int countOnes,
                                    AES_XOF_struct *seed_expander_ctx)
{

   int rndPos[countOnes],  duplicated, counter = 0;
   memset(sequence, 0x00, N0*NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B);


   while (counter < countOnes) {
      int p = rand_range(N0*NUM_BITS_GF2X_ELEMENT,BITS_TO_REPRESENT(P),
                         seed_expander_ctx);
      duplicated = 0;
      for (int j = 0; j < counter; j++) if (rndPos[j] == p) duplicated = 1;
      if (duplicated == 0) {
         rndPos[counter] = p;
         counter++;
      }
   }
   for (int j = 0; j < counter; j++) {
      int polyIndex = rndPos[j] / P;
      int exponent = rndPos[j] % P;
      gf2x_set_coeff( sequence + NUM_DIGITS_GF2X_ELEMENT*polyIndex, exponent,
                      ( (DIGIT) 1));
   }
} // end rand_circulant_blocks_sequence
/*----------------------------------------------------------------------------*/

void rand_error_pos(POSITION_T errorPos[NUM_ERRORS_T],
                    AES_XOF_struct *seed_expander_ctx)
{

   int duplicated, counter = 0;

   while (counter < NUM_ERRORS_T) {
      int p = rand_range(N0*NUM_BITS_GF2X_ELEMENT,BITS_TO_REPRESENT(P),
                         seed_expander_ctx);
      duplicated = 0;
      for (int j = 0; j < counter; j++) if (errorPos[j] == (uint32_t)p) duplicated = 1;
      if (duplicated == 0) {
         errorPos[counter] = p;
         counter++;
      }
   }
} // end rand_error_pos

/*----------------------------------------------------------------------------*/

void rand_error_pos_shake(POSITION_T errorPos[NUM_ERRORS_T],
                          xof_shake_t *state)
{

   int duplicated, counter = 0;

   while (counter < NUM_ERRORS_T) {
      int p = rand_range_shake(N0*NUM_BITS_GF2X_ELEMENT,BITS_TO_REPRESENT(P),
                               state);
      duplicated = 0;
      for (int j = 0; j < counter; j++) if (errorPos[j] == (uint32_t)p) duplicated = 1;
      if (duplicated == 0) {
         errorPos[counter] = p;
         counter++;
      }
   }
} // end rand_error_pos_shake

/*----------------------------------------------------------------------------*/

void expand_error(DIGIT sequence[N0*NUM_DIGITS_GF2X_ELEMENT],
                  POSITION_T errorPos[NUM_ERRORS_T])
{
   memset(sequence, 0x00, N0*NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B);

   for (int j = 0; j < NUM_ERRORS_T; j++) {
      int polyIndex = errorPos[j] / P;
      int exponent = errorPos[j] % P;
      gf2x_set_coeff( sequence + NUM_DIGITS_GF2X_ELEMENT*polyIndex, exponent,
                      ( (DIGIT) 1));
   }
} // end rand_circulant_blocks_sequence

/*----------------------------------------------------------------------------*/
