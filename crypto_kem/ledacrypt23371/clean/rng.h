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
#ifndef RNG_H
#define RNG_H

/******  From this point on, the code was supplied by NIST ****************/
//  Created by Bassham, Lawrence E (Fed) on 8/29/17.
//  Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
//
/******    from NIST  ****************/

#include <stdio.h>
#include "nistseedexpander.h"

typedef struct {
   unsigned char   Key[32];
   unsigned char   Vee[16];
   int             reseed_counter;
} AES256_CTR_DRBG_struct;


void
AES256_CTR_DRBG_Update(unsigned char *provided_data,
                       unsigned char *Key,
                       unsigned char *Vee);

void
randombytes_init(unsigned char *entropy_input, unsigned char *personalization_string);


/******  End of NIST supplied code ****************/
#include "fips202.h"

void initialize_pseudo_random_generator_seed(int seedFixed, char *seed);

void deterministic_random_byte_generator(unsigned char *const output,
      const unsigned long long output_len,
      const unsigned char *const seed,
      const unsigned long long seed_length);

void seedexpander_from_trng(AES_XOF_struct *ctx,
                            const unsigned char *trng_entropy
                            /* TRNG_BYTE_LENGTH wide buffer */);

/* Return a uniform random value in the range 0..n-1 inclusive,
 * applying a rejection sampling strategy and exploiting as a random source
 * the NIST seedexpander seeded with the proper key.
 * Assumes that the maximum value for the range n is 2^32-1
 */
uint32_t rand_range(const int n, const int logn, AES_XOF_struct *seed_expander_ctx);

///////////////////////////////////////////////////////////////////////////////

typedef
struct xof_shake {
#if SHAKE_FUNCTION == shake_128
#define SHAKE_BUFFER_LENGTH (168)
    shake128incctx state;
#elif SHAKE_FUNCTION == shake_256
#define SHAKE_BUFFER_LENGTH (136)
    shake256incctx state;
#else
#error "SHAKE FUNCTION SELECTION FAILED !"
#endif
   uint8_t buffer[SHAKE_BUFFER_LENGTH];
   int last_filled_byte_idx;
   int idx;
} xof_shake_t;


void shake_seedexpander_init(xof_shake_t *st,
                             const unsigned char *trng_entropy
                             /* TRNG_BYTE_LENGTH wide buffer */);

void shake_seedexpander_extract(xof_shake_t *st,
                                unsigned char *output,
                                unsigned int outputByteLen);

int rand_range_shake(const int n, const int logn, xof_shake_t *st);

#endif