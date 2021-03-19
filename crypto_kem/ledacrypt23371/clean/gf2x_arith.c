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

#include "gf2x_arith.h"
#include <string.h>  // memset(...)

void gf2x_mul_comb(const int nr, DIGIT Res[],
                   const int na, const DIGIT A[],
                   const int nb, const DIGIT B[]);
void gf2x_mul_Kar(const int nr, DIGIT Res[],
                  const int na, const DIGIT A[],
                  const int nb, const DIGIT B[]);
/*----------------------------------------------------------------------------*/

void gf2x_mul_comb(const int nr, DIGIT Res[],
                   const int na, const DIGIT A[],
                   const int nb, const DIGIT B[])
{
   int i, j, k;
   DIGIT u, h;

   memset(Res, 0x00, nr*sizeof(DIGIT));

   for (k = DIGIT_SIZE_b-1; k > 0; k--) {
      for (i = na-1; i >= 0; i--)
         if ( A[i] & (((DIGIT)0x1) << k) )
            for (j = nb-1; j >= 0; j--) Res[i+j+1] ^= B[j];


      u = Res[na+nb-1];
      Res[na+nb-1] = u << 0x1;
      for (j = 1; j < na+nb; ++j) {
         h = u >> (DIGIT_SIZE_b-1);
         u = Res[na+nb-1-j];
         Res[na+nb-1-j] = h^(u << 0x1);
      }
   }
   for (i = na-1; i >= 0; i--)
      if ( A[i] & ((DIGIT)0x1) )
         for (j = nb-1; j >= 0; j--) Res[i+j+1] ^= B[j];
}
/*----------------------------------------------------------------------------*/

/* allows the second operand to be shorter than the first */
/* the result should be as large as the first operand*/
static inline void gf2x_add_asymm(DIGIT Res[], const int na, const DIGIT A[], const int nb, const DIGIT B[])
{
   int delta = na-nb;
   memcpy(Res,A,delta*DIGIT_SIZE_B);
   gf2x_add(nb, Res+delta,
            A+delta, B);
} // end gf2x_add

/*----------------------------------------------------------------------------*/

/* PRE: MAX ALLOWED ROTATION AMOUNT : DIGIT_SIZE_b */

void right_bit_shift_n(const int length, DIGIT in[], const int amount)
{
   if ( amount == 0 ) return;
   int j;
   DIGIT mask;
   mask = ((DIGIT)0x01 << amount) - 1;
   for (j = length-1; j > 0 ; j--) {
      in[j] >>= amount;
      in[j] |=  (in[j-1] & mask) << (DIGIT_SIZE_b - amount);
   }
   in[j] >>= amount;
} // end right_bit_shift_n

/*----------------------------------------------------------------------------*/

/* PRE: MAX ALLOWED ROTATION AMOUNT : DIGIT_SIZE_b */
void left_bit_shift_n(const int length, DIGIT in[], const int amount)
{
   if ( amount == 0 ) return;
   int j;
   DIGIT mask;
   mask = ~(((DIGIT)0x01 << (DIGIT_SIZE_b - amount)) - 1);
   for (j = 0 ; j < length-1 ; j++) {
      in[j] <<= amount;
      in[j] |=  (in[j+1] & mask) >> (DIGIT_SIZE_b - amount);
   }
   in[j] <<= amount;
} // end left_bit_shift_n

/*---------------------------------------------------------------------------*/

void gf2x_mul_Kar(const int nr, DIGIT Res[],
                  const int na, const DIGIT A[],
                  const int nb, const DIGIT B[])
{

   if(na < MIN_KAR_DIGITS || nb < MIN_KAR_DIGITS) {
      /* fall back to schoolbook */
      gf2x_mul_comb(nr, Res, na, A, nb, B);
      return;
   }

   if(na %2 == 0) {
      unsigned bih = na/2;
      DIGIT middle[2*bih], sumA[bih], sumB[bih];
      gf2x_add(bih,sumA,
               A,A+bih);
      gf2x_add(bih,sumB,
               B,B+bih);
      gf2x_mul_Kar(2*bih, middle,
                   bih, sumA,
                   bih, sumB);
      gf2x_mul_Kar(2*bih, Res+2*bih,
                   bih, A+bih,
                   bih, B+bih);
      gf2x_add(2*bih, middle,
               middle, Res+2*bih);
      gf2x_mul_Kar(2*bih, Res,
                   bih, A,
                   bih, B);
      gf2x_add(2*bih, middle,
               middle, Res);
      gf2x_add(2*bih, Res+bih,
               Res+bih,middle);
   } else {
      unsigned bih = na/2 + 1;
      DIGIT middle[2*bih], sumA[bih], sumB[bih];
       gf2x_add_asymm(sumA,
                      bih, A + bih - 1,
                      bih - 1, A);
       gf2x_add_asymm(sumB,
                      bih, B + bih - 1,
                      bih - 1, B);
      gf2x_mul_Kar(2*bih, middle,
                   bih, sumA,
                   bih, sumB);
      gf2x_mul_Kar(2*bih, Res+2*(bih-1),
                   bih, A+bih-1,
                   bih, B+bih-1);
      gf2x_add(2*bih, middle,
               middle, Res+2*(bih-1));
      gf2x_mul_Kar(2*(bih-1), Res,
                   (bih-1), A,
                   (bih-1), B);
       gf2x_add_asymm(middle,
                      2 * bih, middle,
                      2 * (bih - 1), Res);
      gf2x_add(2*bih, Res+bih-2,
               Res+bih-2,middle);
   }
}

/*----------------------------------------------------------------------------*/

static inline void gf2x_exact_div_x_plus_one(const int na, DIGIT A[])
{
   DIGIT t = 0;
   for (int i = na - 1; i >= 0; i--) {

      t ^= A[i];

      for (int j = 1; j <= DIGIT_SIZE_b / 2; j = j * 2) {
         t ^= t << (unsigned) j;
      }
      A[i] = t;
      t >>= DIGIT_SIZE_b - 1;
   }
} // end gf2x_exact_div_x_plus_one

/*---------------------------------------------------------------------------*/
/* Toom-Cook 3 algorithm as reported in
 * Marco Bodrato: "Towards Optimal Toom-Cook Multiplication for Univariate and
 * Multivariate Polynomials in Characteristic 2 and 0". WAIFI 2007: 116-133   */

void gf2x_mul_TC3(const int nr, DIGIT Res[],
                  const int na, const DIGIT A[],
                  const int nb, const DIGIT B[])
{

   if(na < MIN_TOOM_DIGITS || nb < MIN_TOOM_DIGITS) {
      /* fall back to schoolbook */
      gf2x_mul_Kar(nr, Res, na, A, nb, B);
      return;
   }

   unsigned bih; //number of limbs for each part.
   if (na % 3 == 0) {
      bih = na / 3;
   } else {
      bih = na / 3 + 1;
   }

   DIGIT u2[bih], *u1, *u0;

   int leading_slack = (3 - (na) % 3) %3;
//     printf("leading slack %d",leading_slack);
   int i;
   for(i = 0; i < leading_slack ; i++) {
      u2[i] = 0;
   }
   for (; (unsigned int)i < bih; ++i) {
      u2[i] = A[i-leading_slack];
   }
   u1= (DIGIT *) (A+bih-leading_slack);
   u0= (DIGIT *) (A+2*bih-leading_slack);

   DIGIT v2[bih], *v1, *v0; /* partitioned inputs */
   for(i = 0; i < leading_slack ; i++) {
      v2[i] = 0;
   }
   for (; (unsigned int)i < bih; ++i) {
      v2[i] = B[i-leading_slack];
   }
   v1=(DIGIT *) (B+bih-leading_slack);
   v0=(DIGIT *) (B+2*bih-leading_slack);

   DIGIT sum_u[bih]; /*bih digit wide*/
   gf2x_add(bih, sum_u,
            u0,u1);
   gf2x_add(bih, sum_u,
            sum_u,u2);

   DIGIT sum_v[bih]; /*bih digit wide*/
   gf2x_add(bih, sum_v,
            v0,v1);
   gf2x_add(bih, sum_v,
            sum_v,v2);

   DIGIT w1[2*bih];
   gf2x_mul_TC3(2*bih, w1,
                bih, sum_u,
                bih, sum_v);

   DIGIT u2_x2[bih + 1];
   u2_x2[0] = 0;
   memcpy(u2_x2+1,u2,bih*DIGIT_SIZE_B);
   left_bit_shift_n(bih+1, u2_x2, 2);

   DIGIT u1_x[bih + 1];
   u1_x[0] = 0;
   memcpy(u1_x+1,u1,bih*DIGIT_SIZE_B);
   left_bit_shift_n(bih+1, u1_x, 1);

   DIGIT u1_x1_u2_x2[bih + 1];
   gf2x_add(bih+1, u1_x1_u2_x2,
            u1_x,u2_x2);

   DIGIT temp_u_components[bih + 1];
    gf2x_add_asymm(temp_u_components,
                   bih + 1, u1_x1_u2_x2,
                   bih, sum_u);

   DIGIT v2_x2[bih + 1];
   v2_x2[0] = 0;
   memcpy(v2_x2+1,v2,bih*DIGIT_SIZE_B);
   left_bit_shift_n(bih+1, v2_x2, 2);

   DIGIT v1_x[bih + 1];
   v1_x[0] = 0;
   memcpy(v1_x+1,v1,bih*DIGIT_SIZE_B);
   left_bit_shift_n(bih+1, v1_x, 1);

   DIGIT v1_x1_v2_x2[bih + 1];
   gf2x_add(bih+1, v1_x1_v2_x2,
            v1_x, v2_x2);

   DIGIT temp_v_components[bih + 1];
    gf2x_add_asymm(temp_v_components,
                   bih + 1, v1_x1_v2_x2,
                   bih, sum_v);

   DIGIT w3[2*bih+2];
   gf2x_mul_TC3(2*bih+2, w3,
                bih+1, temp_u_components,
                bih+1, temp_v_components);

    gf2x_add_asymm(u1_x1_u2_x2,
                   bih + 1, u1_x1_u2_x2,
                   bih, u0);
    gf2x_add_asymm(v1_x1_v2_x2,
                   bih + 1, v1_x1_v2_x2,
                   bih, v0);

   DIGIT w2[2*bih+2];
   gf2x_mul_TC3(2*bih+2, w2,
                bih+1, u1_x1_u2_x2,
                bih+1, v1_x1_v2_x2);

   DIGIT w4[2*bih];
   gf2x_mul_TC3(2*bih, w4,
                bih, u2,
                bih, v2);
   DIGIT w0[2*bih];
   gf2x_mul_TC3(2*bih, w0,
                bih, u0,
                bih, v0);

   // Interpolation starts
   gf2x_add(2*bih+2, w3,
            w2, w3);
    gf2x_add_asymm(w2,
                   2 * bih + 2, w2,
                   2 * bih, w0);
   right_bit_shift_n(2*bih+2, w2, 1);
   gf2x_add(2*bih+2, w2,
            w2, w3);

   // w2 + (w4 * x^3+1) = w2 + w4 + w4 << 3
   DIGIT w4_x3_plus_1[2*bih+1];
   w4_x3_plus_1[0] = 0;
   memcpy(w4_x3_plus_1+1,w4,2*bih*DIGIT_SIZE_B);
   left_bit_shift_n(2*bih+1, w4_x3_plus_1, 3);
    gf2x_add_asymm(w2,
                   2 * bih + 2, w2,
                   2 * bih, w4);
    gf2x_add_asymm(w2,
                   2 * bih + 2, w2,
                   2 * bih + 1, w4_x3_plus_1);

   gf2x_exact_div_x_plus_one(2*bih+2, w2);

   gf2x_add(2*bih, w1,
            w1, w0);
    gf2x_add_asymm(w3,
                   2 * bih + 2, w3,
                   2 * bih, w1);

   right_bit_shift_n(2*bih+2, w3, 1);
   gf2x_exact_div_x_plus_one(2*bih+2, w3);

   gf2x_add(2*bih, w1,
            w1, w4);

   DIGIT w1_final[2*bih+2];
    gf2x_add_asymm(w1_final,
                   2 * bih + 2, w2,
                   2 * bih, w1);
   gf2x_add(2*bih+2, w2,
            w2, w3);

   // Result recombination starts here

   memset(Res,0,nr*DIGIT_SIZE_B);
   /* optimization: topmost slack digits should be computed, and not addedd,
    * zeroization can be avoided altogether with a proper merge of the
    * results */

   int leastSignifDigitIdx = nr - 1;
   for (unsigned int i = 0; i< 2*bih; i++) {
      Res[leastSignifDigitIdx - i] ^= w0[2*bih   - 1 - i];
   }
   leastSignifDigitIdx -= bih;
   for (unsigned int i = 0; i< 2*bih+2; i++) {
      Res[leastSignifDigitIdx - i] ^= w1_final[2*bih+2   - 1 - i];
   }
   leastSignifDigitIdx -= bih;
   for (unsigned int i = 0; i< 2*bih+2; i++) {
      Res[leastSignifDigitIdx - i] ^= w2[2*bih+2 - 1 - i];
   }
   leastSignifDigitIdx -= bih;
   for (unsigned int i = 0; i< 2*bih+2 ; i++) {
      Res[leastSignifDigitIdx - i] ^= w3[2*bih+2 - 1 - i];
   }
   leastSignifDigitIdx -= bih;
   for (int i = 0; (unsigned int)i< 2*bih && (leastSignifDigitIdx - i >= 0) ; i++) {
      Res[leastSignifDigitIdx - i] ^= w4[2*bih   - 1 - i];
   }
}
