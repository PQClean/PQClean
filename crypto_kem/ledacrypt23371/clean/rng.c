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


#include "rng.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h> // void srand(unsigned int seed); int rand(void); RAND_MAX
#include <string.h> // void *memset(void *s, int c, size_t n);
#include <time.h> // struct timespec; clock_gettime(...); CLOCK_REALTIME

#include "aes256.h"
#include "qc_ldpc_parameters.h"

#include "randombytes.h"
#include "nistseedexpander.h"


/******************************************************************************/
/*----------------------------------------------------------------------------*/
/*              start PSEUDO-RAND GENERATOR ROUTINES for rnd.h                */
/*----------------------------------------------------------------------------*/


void initialize_pseudo_random_generator_seed(int seedFixed, char *seed)
{

   if (seedFixed == 1)
      srand(atoi(seed));
   else {
      uint8_t r[4];
      randombytes(r, 4);
      srand(*r);
   } // end else-if
   unsigned char pseudo_entropy[48];
   for (int i=0; i< 48; i++) pseudo_entropy[i] = rand() & 0xff;
    randombytes_init(pseudo_entropy,
                     NULL);


} // end initilize_pseudo_random_sequence_seed


/*----------------------------------------------------------------------------*/

/* Initializes a dedicated DRBG context to avoid conflicts with the global one
 * declared by NIST for KATs. Provides the output of the DRBG in output, for
 * the given length */


/*----------------------------------------------------------------------------*/
/*              end PSEUDO-RAND GENERATOR ROUTINES for rnd.h                  */
/*----------------------------------------------------------------------------*/

AES256_CTR_DRBG_struct  DRBG_ctx;

void    AES256_ECB(unsigned char *key, unsigned char *ctr,
                   unsigned char *buffer);


// Use whatever AES implementation you have. This uses AES from openSSL library
//    key - 256-bit AES key
//    ptx - a 128-bit plaintext value
//    ctx - a 128-bit ciphertext value

void
AES256_ECB(unsigned char *key, unsigned char *ptx, unsigned char *ctx)
{
   uint32_t round_key[4*(NROUNDS + 1)] = {0x00};
   rijndaelKeySetupEnc(round_key, key);
   rijndaelEncrypt(round_key, NROUNDS, ptx, ctx);
}

void
randombytes_init(unsigned char *entropy_input, unsigned char *personalization_string)
{
   unsigned char   seed_material[48];

   memcpy(seed_material, entropy_input, 48);
   if (personalization_string)
      for (int i=0; i<48; i++)
         seed_material[i] ^= personalization_string[i];
   memset(DRBG_ctx.Key, 0x00, 32);
   memset(DRBG_ctx.Vee, 0x00, 16);
   AES256_CTR_DRBG_Update(seed_material, DRBG_ctx.Key, DRBG_ctx.Vee);
   DRBG_ctx.reseed_counter = 1;
}

void
AES256_CTR_DRBG_Update(unsigned char *provided_data,
                       unsigned char *Key,
                       unsigned char *Vee)
{
   unsigned char   temp[48];

   for (int i=0; i<3; i++) {
      //increment V
      for (int j=15; j>=0; j--) {
         if ( Vee[j] == 0xff )
            Vee[j] = 0x00;
         else {
            Vee[j]++;
            break;
         }
      }

      AES256_ECB(Key, Vee, temp+16*i);
   }
   if ( provided_data != NULL )
      for (int i=0; i<48; i++)
         temp[i] ^= provided_data[i];
   memcpy(Key, temp, 32);
   memcpy(Vee, temp+32, 16);
}



void deterministic_random_byte_generator(unsigned char *const output,
      const unsigned long long output_len,
      const unsigned char *const seed,
      const unsigned long long seed_length
                                        )
{
   /* DRBG context initialization */
   AES256_CTR_DRBG_struct ctx;
   unsigned char   seed_material[48];
   memset(seed_material, 0x00, 48);
   memcpy(seed_material, seed, seed_length);

   memset(ctx.Key, 0x00, 32);
   memset(ctx.Vee, 0x00, 16);
   AES256_CTR_DRBG_Update(seed_material, ctx.Key, ctx.Vee);
   ctx.reseed_counter = 1;

   /* Actual DRBG computation as from the randombytes(unsigned char *x,
    * unsigned long long xlen) from NIST */

   unsigned char   block[16];
   int             i = 0, length_remaining;

   length_remaining=output_len;

   while ( length_remaining > 0 ) {
      //increment V
      for (int j=15; j>=0; j--) {
         if ( ctx.Vee[j] == 0xff )
            ctx.Vee[j] = 0x00;
         else {
            ctx.Vee[j]++;
            break;
         }
      }
      AES256_ECB(ctx.Key, ctx.Vee, block);
      if ( length_remaining > 15 ) {
         memcpy(output+i, block, 16);
         i += 16;
         length_remaining -= 16;
      } else {
         memcpy(output+i, block, length_remaining);
         length_remaining = 0;
      }
   }
   AES256_CTR_DRBG_Update(NULL, ctx.Key, ctx.Vee);
   ctx.reseed_counter++;

} // end deterministic_random_byte_generator

void seedexpander_from_trng(AES_XOF_struct *ctx,
                            const unsigned char *trng_entropy
                            /* TRNG_BYTE_LENGTH wide buffer */)
{

   /*the NIST seedexpander will however access 32B from this buffer */
   unsigned int prng_buffer_size = TRNG_BYTE_LENGTH < 32 ? 32 : TRNG_BYTE_LENGTH;
   unsigned char prng_buffer[TRNG_BYTE_LENGTH < 32 ? 32 : TRNG_BYTE_LENGTH] = { 0x00 };
   memcpy(prng_buffer,
          trng_entropy,
          TRNG_BYTE_LENGTH < prng_buffer_size ? TRNG_BYTE_LENGTH : prng_buffer_size);
   /* if extra entropy is provided, add it to the diversifier */
#if TRNG_BYTE_LENGTH == 40
   unsigned char *diversifier = ((unsigned char *)trng_entropy)+32;
#else
   unsigned char diversifier[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
#endif
   /* the required seed expansion will be quite small, set the max number of
    * bytes conservatively to 10 MiB*/
   seedexpander_init(ctx,prng_buffer,diversifier,10*1024*1024);
}

/*----------------------------------------------------------------------------*/

/* Return a uniform random value in the range 0..n-1 inclusive,
 * applying a rejection sampling strategy and exploiting as a random source
 * the NIST seedexpander seeded with the proper key.
 * Assumes that the maximum value for the range n is 2^32-1
 */
uint32_t rand_range(const int n, const int logn, AES_XOF_struct *seed_expander_ctx)
{

   unsigned long required_rnd_bytes = (logn+7)/8;
   unsigned char rnd_char_buffer[4];
   uint32_t rnd_value;
   uint32_t mask = ( (uint32_t)1 << logn) - 1;

   do {
      seedexpander(seed_expander_ctx, rnd_char_buffer, required_rnd_bytes);
      /* obtain an endianness independent representation of the generated random
       bytes into an unsigned integer */
      rnd_value =  ((uint32_t)rnd_char_buffer[3] << 24) +
                   ((uint32_t)rnd_char_buffer[2] << 16) +
                   ((uint32_t)rnd_char_buffer[1] <<  8) +
                   ((uint32_t)rnd_char_buffer[0] <<  0) ;
      rnd_value = mask & rnd_value;
   } while (rnd_value >= (uint32_t)n);

   return rnd_value;
} // end rand_range

void shake_seedexpander_init(xof_shake_t *st,
                             const unsigned char *trng_entropy
                             /* TRNG_BYTE_LENGTH wide buffer */)
{
   memset(st, 0x00, sizeof(xof_shake_t));
#if SHAKE_FUNCTION == shake_128
    shake128_inc_init(&(st->state));
    shake128_inc_absorb(&(st->state),
                        (const uint8_t *) trng_entropy,
                        (size_t) (TRNG_BYTE_LENGTH*8)
    );
    shake128_inc_finalize(&(st->state)
    );
    st->last_filled_byte_idx = 15;
#elif SHAKE_FUNCTION == shake_256
    shake256_inc_init(&(st->state));
    shake256_inc_absorb(&(st->state),
                     (const uint8_t *) trng_entropy,
                     (size_t) (TRNG_BYTE_LENGTH*8)
                    );
    shake256_inc_finalize(&(st->state)
                   );
    st->last_filled_byte_idx = 31;
#else
#error "SHAKE FUNCTION SELECTION FAILED !"
#endif

   st->idx = 0;
} // end shake_seedexpander_init

void shake_seedexpander_extract(xof_shake_t *st,
                                unsigned char *output,
                                unsigned int outputByteLen)
{
   int remaining_bytes_to_output = outputByteLen;
   uint32_t outIdx = 0;
   for ( ; st->idx <= st->last_filled_byte_idx &&
         outIdx < outputByteLen;
         (st->idx)++ ) {
      output[outIdx] = st->buffer[st->idx];
      remaining_bytes_to_output -= 1;
      outIdx += 1;
   } // end for
   while (remaining_bytes_to_output > 0) {
#if SHAKE_FUNCTION == shake_128
       shake128_inc_squeeze((st->buffer),
                         (size_t) (SHAKE_BUFFER_LENGTH*8),
                         &(st->state)
                        );
#elif SHAKE_FUNCTION == shake_256
       shake256_inc_squeeze((st->buffer),
                         (size_t) (SHAKE_BUFFER_LENGTH*8),
                         &(st->state)
                        );
#else
#error "SHAKE FUNCTION SELECTION FAILED !"
#endif
      st->last_filled_byte_idx = SHAKE_BUFFER_LENGTH-1;
      st->idx = 0;
      for ( ; st->idx <= st->last_filled_byte_idx &&
            outIdx < outputByteLen;
            (st->idx)++ ) {
         output[outIdx] = st->buffer[st->idx];
         remaining_bytes_to_output -= 1;
         outIdx += 1;
      } // end for
   } // end while
} // end shake_seedexpander_extract

int rand_range_shake(const int n, const int logn, xof_shake_t *st)
{
   unsigned long required_rnd_bytes = (logn+7)/8;
   unsigned char rnd_char_buffer[4];
   uint32_t rnd_value;
   uint32_t mask = ( (uint32_t)1 << logn) - 1;

   do {
      shake_seedexpander_extract(st, rnd_char_buffer, required_rnd_bytes);
      /* obtain an endianness independent representation of the generated random
       bytes into an unsigned integer */
      rnd_value =  ((uint32_t)rnd_char_buffer[3] << 24) +
                   ((uint32_t)rnd_char_buffer[2] << 16) +
                   ((uint32_t)rnd_char_buffer[1] <<  8) +
                   ((uint32_t)rnd_char_buffer[0] <<  0) ;
      rnd_value = mask & rnd_value;
   } while (rnd_value >= (uint32_t)n);

   return rnd_value;
} // end rand_range_shake
