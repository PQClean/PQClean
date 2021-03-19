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

#include "niederreiter_keygen.h"
#include "H_Q_matrices_generation.h"
#include "gf2x_arith_mod_xPplusOne.h"
#include "rng.h"
#include "dfr_test.h"
#include <string.h>
#include "randombytes.h"
#include "nistseedexpander.h"
/*----------------------------------------------------------------------------*/
/* Implementation that should never be optimized out by the compiler */
static inline void zeroize( void *v, size_t n )
{
   volatile unsigned char *p = v;
   while( n-- ) *p++ = 0;
} // end zeroize

/*----------------------------------------------------------------------------*/

void key_gen_niederreiter(publicKeyNiederreiter_t   *const pk,
                          privateKeyNiederreiter_t *const sk)
{
   AES_XOF_struct keys_expander;
   memset(&keys_expander,0x00,sizeof(AES_XOF_struct));
   randombytes(sk->prng_seed, TRNG_BYTE_LENGTH);
   seedexpander_from_trng(&keys_expander, sk->prng_seed);

   POSITION_T HPosOnes[N0][V];

   int isDFRok;
   sk->rejections = (uint8_t) 0;
   do {
      generateHPosOnes(HPosOnes, &keys_expander);
      sk->rejections = sk->rejections + 1;
      isDFRok = DFR_test(HPosOnes, &(sk->secondIterThreshold));
   } while(!isDFRok);

   sk->rejections = sk->rejections - 1;

   seedexpander(&keys_expander,
                sk->decryption_failure_secret,
                (unsigned long)TRNG_BYTE_LENGTH);

   DIGIT Ln0dense[NUM_DIGITS_GF2X_ELEMENT] = {0x00};
   for(int j = 0; j < V; j++) {
      gf2x_set_coeff(Ln0dense,HPosOnes[N0-1][j],1);
   }
   DIGIT Ln0Inv[NUM_DIGITS_GF2X_ELEMENT] = {0x00};
   GF2X_DIGIT_MOD_INVERSE(Ln0Inv, Ln0dense);
   for (int i = 0; i < N0-1; i++) {
      gf2x_mod_mul_dense_to_sparse(pk->Mtr+i*NUM_DIGITS_GF2X_ELEMENT,
                                   Ln0Inv,
                                   HPosOnes[i],
                                   V);
   }

   for (int i = 0; i < N0-1; i++) {
      gf2x_transpose_in_place(pk->Mtr+i*NUM_DIGITS_GF2X_ELEMENT);
   }
} // end key_gen_niederreiter

/*----------------------------------------------------------------------------*/

void publicKey_deletion_niederreiter(publicKeyNiederreiter_t   *const pk)
{

   zeroize(pk,sizeof(publicKeyNiederreiter_t));
} // publicKey_deletion_niederreiter

/*----------------------------------------------------------------------------*/

void privateKey_deletion_niederreiter(privateKeyNiederreiter_t *const sk)
{
   zeroize(sk, sizeof(privateKeyNiederreiter_t));
} // privateKey_deletion_niederreiter

/*----------------------------------------------------------------------------*/
