#include "niederreiter_decrypt.h"

#include "qc_ldpc_parameters.h"
#include "gf2x_arith_mod_xPplusOne.h"
#include "H_Q_matrices_generation.h"

#include "bf_decoding.h"
#include "dfr_test.h"
#include "rng.h"
#include "fips202.h"
#include <string.h>

extern int PQCLEAN_LEDACRYPT23371_CLEAN_thresholds[2];
/*----------------------------------------------------------------------------*/
static
int decrypt_niederreiter(DIGIT *err,            // N0 circ poly
                         const privateKeyNiederreiter_t *const sk,
                         const DIGIT *syndrome  // 1 circ poly
                        )
{
   AES_XOF_struct niederreiter_decrypt_expander;
    PQCLEAN_LEDACRYPT23371_CLEAN_seedexpander_from_trng(&niederreiter_decrypt_expander,
                                                        sk->prng_seed);
   /* rebuild secret key values */
   POSITION_T HPosOnes[N0][V];

   int rejections =  sk->rejections;
   PQCLEAN_LEDACRYPT23371_CLEAN_thresholds[1] = sk->secondIterThreshold;

   do {
      PQCLEAN_LEDACRYPT23371_CLEAN_generateHPosOnes(HPosOnes, &niederreiter_decrypt_expander);
      rejections--;
   } while(rejections>=0);

   POSITION_T HtrPosOnes[N0][V];
   PQCLEAN_LEDACRYPT23371_CLEAN_transposeHPosOnes(HtrPosOnes, (const POSITION_T(*)[V]) HPosOnes);

   DIGIT privateSyndrome[NUM_DIGITS_GF2X_ELEMENT];
   int decryptOk = 0;
   DIGIT err_computed[N0*NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B] = {0};
    PQCLEAN_LEDACRYPT23371_CLEAN_gf2x_mod_mul_dense_to_sparse(privateSyndrome,
                                                              syndrome,
                                                              HtrPosOnes[N0 - 1],
                                                              V);

   decryptOk = PQCLEAN_LEDACRYPT23371_CLEAN_bf_decoding(err_computed,
                                                        (const POSITION_T (*)[V]) HtrPosOnes,
                                                        privateSyndrome);
   int err_weight = 0;
   for (int i =0 ; i < N0; i++) {
      err_weight += population_count(err_computed+(NUM_DIGITS_GF2X_ELEMENT*i));
   }
   decryptOk = decryptOk && (err_weight == NUM_ERRORS_T);

   /* prepare mockup error vector in case a decoding failure occurs */
   DIGIT err_mockup[N0*NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B];
   memcpy(err_mockup, syndrome, NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B);
   memcpy(err_mockup+NUM_DIGITS_GF2X_ELEMENT, sk->decryption_failure_secret,
          TRNG_BYTE_LENGTH);
   memset( ((unsigned char *) err_mockup)+(NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B)
           +TRNG_BYTE_LENGTH,
           0x00,
           (N0-1)*NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B-TRNG_BYTE_LENGTH
         );

   if (!decryptOk) {
      memcpy(err, err_mockup, N0*NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B);
   } else {
      memcpy(err, err_computed, N0*NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B);
   }

   return decryptOk;
} // end decrypt_niederreiter


/*----------------------------------------------------------------------------*/

void PQCLEAN_LEDACRYPT23371_CLEAN_decrypt_niederreiter_indcca2(unsigned char *const ss,
                                                               const unsigned char *const ct,
                                                               const privateKeyNiederreiter_t *const sk)
{
   DIGIT decoded_error_vector[N0*NUM_DIGITS_GF2X_ELEMENT];

   int decode_ok = decrypt_niederreiter(decoded_error_vector,
                                        sk,
                                        (DIGIT *) ct);
   // the masked seed is appended to the syndrome (ct) by the encryptor ...
   // this call to decrypt_niederreiter(....) is ok!

   uint8_t hashedErrorVector[HASH_BYTE_LENGTH];
   HASH_FUNCTION((uint8_t *) hashedErrorVector,
                 (const uint8_t *) decoded_error_vector, // input
                 (N0*NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B) // input Length
                 );

   uint8_t hashedAndTruncaedErrorVector[TRNG_BYTE_LENGTH] = {0};
#if (TRNG_BYTE_LENGTH <= HASH_BYTE_LENGTH)
   memcpy(hashedAndTruncaedErrorVector, hashedErrorVector, TRNG_BYTE_LENGTH);
#else
   memcpy(hashedAndTruncaedErrorVector, hashedErrorVector, HASH_BYTE_LENGTH);
#endif

   uint8_t decoded_seed[TRNG_BYTE_LENGTH];
   for (int i = 0; i < TRNG_BYTE_LENGTH; ++i) {
      decoded_seed[i] = ct[(NUM_DIGITS_GF2X_ELEMENT*DIGIT_SIZE_B)+i] ^
                        hashedAndTruncaedErrorVector[i];
   }

   uint8_t hashed_decoded_seed[HASH_BYTE_LENGTH];
   uint8_t hashedAndTruncaed_decoded_seed[TRNG_BYTE_LENGTH] = {0};
   HASH_FUNCTION((uint8_t *) hashed_decoded_seed,
                 (const uint8_t *) decoded_seed,     // input
                 TRNG_BYTE_LENGTH                    // input Length
                 );

#if (TRNG_BYTE_LENGTH <= HASH_BYTE_LENGTH)
   memcpy(hashedAndTruncaed_decoded_seed,
          hashed_decoded_seed,
          TRNG_BYTE_LENGTH);
#else
   memcpy(hashedAndTruncaed_decoded_seed,
          hashed_decoded_seed,
          HASH_BYTE_LENGTH);
#endif

   AES_XOF_struct hashedAndTruncaedSeed_expander;
   memset(&hashedAndTruncaedSeed_expander, 0x00, sizeof(AES_XOF_struct));
    PQCLEAN_LEDACRYPT23371_CLEAN_seedexpander_from_trng(&hashedAndTruncaedSeed_expander,
                                                        hashed_decoded_seed);

   POSITION_T reconstructed_errorPos[NUM_ERRORS_T];
    PQCLEAN_LEDACRYPT23371_CLEAN_rand_error_pos(reconstructed_errorPos, &hashedAndTruncaedSeed_expander);

   DIGIT reconstructed_error_vector[N0*NUM_DIGITS_GF2X_ELEMENT];
    PQCLEAN_LEDACRYPT23371_CLEAN_expand_error(reconstructed_error_vector, reconstructed_errorPos);

   int equal = (0 == memcmp((const unsigned char *) decoded_error_vector,
                            (const unsigned char *) reconstructed_error_vector,
                            N0*NUM_DIGITS_GF2X_ELEMENT));
   // equal == 1, if the reconstructed error vector match !!!

   int decryptOk = (decode_ok == 1 && equal == 1);

   unsigned char ss_input[2*TRNG_BYTE_LENGTH],
            tail[TRNG_BYTE_LENGTH] = {0};
   memcpy(ss_input,
          decoded_seed,
          TRNG_BYTE_LENGTH);

   if (decryptOk == 1) {
      memcpy(ss_input+sizeof(decoded_seed),
             tail,
             TRNG_BYTE_LENGTH);
   } else { // decryption failure
      memcpy(ss_input+sizeof(decoded_seed),
             sk->decryption_failure_secret,
             TRNG_BYTE_LENGTH);
   }

   HASH_FUNCTION((uint8_t *) ss,
                 (const uint8_t *) ss_input,       // input
                 2*TRNG_BYTE_LENGTH                // input Length
                 );

} // end PQCLEAN_LEDACRYPT23371_CLEAN_decrypt_niederreiter_indcca2
