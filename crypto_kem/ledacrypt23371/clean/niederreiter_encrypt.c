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
#include "niederreiter_encrypt.h"
#include "qc_ldpc_parameters.h"
#include "gf2x_arith_mod_xPplusOne.h"
#include "gf2x_limbs.h"
#include "rng.h"
#include "fips202.h"
#include <string.h> // memset(...)
#include "randombytes.h"

static
void encrypt_niederreiter(DIGIT syndrome[],                // 1  polynomial
                          const publicKeyNiederreiter_t *const pk,
                          const POSITION_T errorPos[NUM_ERRORS_T], // positions of asserted bits
                          const DIGIT err[])
{
   uint32_t i;
   DIGIT saux[NUM_DIGITS_GF2X_ELEMENT];
   unsigned int filled;

   memset(syndrome, 0x00, NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B);
   POSITION_T blkErrorPos[NUM_ERRORS_T];
   for (i = 0; i < N0-1; i++) {
      filled=0;
      for (int j = 0 ; j < NUM_ERRORS_T; j ++) {
         if(errorPos[j] / P == i) {
            blkErrorPos[filled] =  errorPos[j] % P;
            filled++;
         }
      }
      gf2x_mod_mul_dense_to_sparse(saux,
                                   pk->Mtr+i*NUM_DIGITS_GF2X_ELEMENT,
                                   blkErrorPos,
                                   filled);
      gf2x_mod_add(syndrome, syndrome, saux);
   }  // end for
   gf2x_mod_add(syndrome, syndrome, err+(N0-1)*NUM_DIGITS_GF2X_ELEMENT);
} // end encrypt_niederreiter


/*----------------------------------------------------------------------------*/

void encrypt_niederreiter_indcca2(unsigned char *const
                                  ct,  /* ciphertext - output    */
                                  unsigned char *const ss,  /* shared secret - output */
                                  const publicKeyNiederreiter_t *const pk)
{

   unsigned char seed[TRNG_BYTE_LENGTH];
   randombytes(seed, TRNG_BYTE_LENGTH);

   unsigned char ss_input[2*TRNG_BYTE_LENGTH] = {0};
   memcpy(ss_input, seed, TRNG_BYTE_LENGTH);
   HASH_FUNCTION((uint8_t *) ss,
                 (const uint8_t *) ss_input, // input
                 2*TRNG_BYTE_LENGTH                // input Length
                 );

   // in api.h PQCLEAN_LEDACRYPTXXXX_CLEAN_CRYPTO_CIPHERTEXTBYTES is defined for KEMLT
   // as (NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B+TRNG_BYTE_LENGTH)
   // ct will be the octect seq. made of the syndrome concatenated with
   //    the blinded seed value


   uint8_t hashedSeed[HASH_BYTE_LENGTH];
   uint8_t hashedAndTruncatedSeed[TRNG_BYTE_LENGTH] = {0};
   HASH_FUNCTION((uint8_t *) hashedSeed,
                 (const uint8_t *) seed,// input
                 TRNG_BYTE_LENGTH             // input Length
                 );

#if (TRNG_BYTE_LENGTH <= HASH_BYTE_LENGTH)
   memcpy(hashedAndTruncatedSeed, hashedSeed, TRNG_BYTE_LENGTH);
#else
   memcpy(hashedAndTruncatedSeed, hashedSeed, HASH_BYTE_LENGTH);
#endif

   AES_XOF_struct hashedAndTruncatedSeed_expander;
   memset(&hashedAndTruncatedSeed_expander, 0x00, sizeof(AES_XOF_struct));
   seedexpander_from_trng(&hashedAndTruncatedSeed_expander,
                          hashedAndTruncatedSeed);

   POSITION_T errorPos[NUM_ERRORS_T];
   rand_error_pos(errorPos, &hashedAndTruncatedSeed_expander);

   DIGIT error_vector[N0*NUM_DIGITS_GF2X_ELEMENT];
   expand_error(error_vector, errorPos);

   uint8_t hashedErrorVector[HASH_BYTE_LENGTH];
   HASH_FUNCTION((uint8_t *) hashedErrorVector,
                 (const uint8_t *) error_vector,      // input
                 (N0*NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B)  // input Length
                 );

   uint8_t hashedAndTruncaedErrorVector[TRNG_BYTE_LENGTH] = {0};
#if (TRNG_BYTE_LENGTH <= HASH_BYTE_LENGTH)
   memcpy(hashedAndTruncaedErrorVector, hashedErrorVector, TRNG_BYTE_LENGTH);
#else
   memcpy(hashedAndTruncaedErrorVector, hashedErrorVector, HASH_BYTE_LENGTH);
#endif

   uint8_t maskedSeed[TRNG_BYTE_LENGTH];
   for (int i = 0; i < TRNG_BYTE_LENGTH; ++i) {
      maskedSeed[i] = seed[i] ^ hashedAndTruncaedErrorVector[i];
   }
   encrypt_niederreiter((DIGIT *) ct,
                        pk,
                        errorPos,
                        error_vector);
   memcpy(ct+(NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B), // ct+syndrome_length
          maskedSeed,
          TRNG_BYTE_LENGTH);

} // end encrypt_niederreiter_indcca2

/*----------------------------------------------------------------------------*/

