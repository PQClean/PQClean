#ifndef _CONFIG_HFE_H
#define _CONFIG_HFE_H
#include "choice_crypto.h"
#include "parameters_HFE.h"
#include "tools_gf2m.h"



/* The public and secret keys are the same for encryption and signature */
#define encrypt_keypairHFE PQCLEAN_GEMSSBLUE256_CLEAN_sign_keypairHFE


/***** Public-key *****/


/* Compressed pk */
#define FORMAT_HYBRID_CPK8 1
#define FORMAT_MONOMIAL_PK8 1

#define EVAL_HYBRID_CPK8_UNCOMP 1

/* The verification does not need constant-time evaluation */
#define evalMQS_gf2_HFE evalMQShybrid8_nocst_gf2_m


/***** Secret-key, seed? *****/


/* Generation of the secret-key with a seed */
/* This option can be set to 1 or 0 */
#define ENABLED_SEED_SK 1


/***** Secret-key (S,s,T,t) *****/


/* Choose the method to generate random invertible matrices S and T */
/* Choose !=0 for LU and 0 for classical */
#define GEN_INV_MATRIX_TRIAL_ERROR 0
#define GEN_INVERTIBLE_MATRIX_LU 1

/* Set to 1 is faster for the signing process. */
#define GEN_INVERSE_IN_FIRST 1

/* To use an affine transformation (change of variable): *S+s */
#define AFFINE_TRANSFORMATION_BY_s 0

/* To use an affine transformation (mix equations): pk = (MQS)*T +t */
#define AFFINE_TRANSFORMATION_BY_t 0


/***** Secret-key F *****/


/****************** PARAMETERS FOR THE KEYPAIR GENERATION ******************/

/* To use precomputed canonical basis and these powers.
   Requires the prec_cbasis_gf2n.h file, which depends on HFEn. */
#define PRECOMPUTED_CBASIS 0

/* XXX This part is disabled XXX */

/* To use precomputed products of elements of canonical basis and these powers
   The compilation becomes very slow!
   PRECOMPUTE2 removes the effect of PRECOMPUTED_CBASIS. */
#define PRECOMPUTE2 0




#endif
