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
PQCLEAN_LEDACRYPT23371_CLEAN_AES256_CTR_DRBG_Update(unsigned char *provided_data,
                                                    unsigned char *Key,
                                                    unsigned char *Vee);

void
PQCLEAN_LEDACRYPT23371_CLEAN_randombytes_init(unsigned char *entropy_input, unsigned char *personalization_string);


/******  End of NIST supplied code ****************/
#include "fips202.h"

void PQCLEAN_LEDACRYPT23371_CLEAN_initialize_pseudo_random_generator_seed(int seedFixed, char *seed);

void PQCLEAN_LEDACRYPT23371_CLEAN_deterministic_random_byte_generator(unsigned char *const output,
                                                                      const unsigned long long output_len,
                                                                      const unsigned char *const seed,
                                                                      const unsigned long long seed_length);

void PQCLEAN_LEDACRYPT23371_CLEAN_seedexpander_from_trng(AES_XOF_struct *ctx,
                                                         const unsigned char *trng_entropy
                            /* TRNG_BYTE_LENGTH wide buffer */);

/* Return a uniform random value in the range 0..n-1 inclusive,
 * applying a rejection sampling strategy and exploiting as a random source
 * the NIST seedexpander seeded with the proper key.
 * Assumes that the maximum value for the range n is 2^32-1
 */
uint32_t PQCLEAN_LEDACRYPT23371_CLEAN_rand_range(const int n, const int logn, AES_XOF_struct *seed_expander_ctx);

///////////////////////////////////////////////////////////////////////////////

typedef
struct xof_shake {
#define SHAKE_BUFFER_LENGTH (168)
    shake128incctx state;
    uint8_t buffer[SHAKE_BUFFER_LENGTH];
    int last_filled_byte_idx;
    int idx;
} xof_shake_t;


void PQCLEAN_LEDACRYPT23371_CLEAN_shake_seedexpander_init(xof_shake_t *st,
                                                          const unsigned char *trng_entropy
                             /* TRNG_BYTE_LENGTH wide buffer */);

void PQCLEAN_LEDACRYPT23371_CLEAN_shake_seedexpander_extract(xof_shake_t *st,
                                                             unsigned char *output,
                                                             unsigned int outputByteLen);

int PQCLEAN_LEDACRYPT23371_CLEAN_rand_range_shake(const int n, const int logn, xof_shake_t *st);

#endif