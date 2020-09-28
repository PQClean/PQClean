#ifndef _SIZES_HFE_H
#define _SIZES_HFE_H
#include "MQS_gf2.h"
#include "arch.h"
#include "bit.h"
#include "choice_crypto.h"
#include "config_HFE.h"
#include "gf2nx.h"
#include "hash.h"
#include "macro.h"
#include "matrix_gf2.h"
#include "parameters_HFE.h"
#include "tools_gf2n.h"
#include "tools_gf2nv.h"
#include "tools_gf2v.h"



#define SIZE_SALT_BITS 0
/* Size of the salt in bytes */
#define SIZE_SALT 0
#define SIZE_SALT_WORD 0


/* Size of the secret seed in bytes */
/* K bits */
#define SIZE_SEED_SK (K>>3)


/****************** THEORETICAL SIZES ******************/

/* Sizes in bits */
#define SIZE_PK_THEORETICAL_HFE (HFEm*(((HFEnv*(HFEnv+1))>>1)+1))

#define SIZE_SK_THEORETICAL_HFE (SIZE_SEED_SK<<3)

#define SIZE_SIGN_THEORETICAL_HFE (HFEnv+(NB_ITE-1)*(HFEnv-HFEm)+SIZE_SALT_BITS)


/****************** PRACTICAL SIZES ******************/

/* Public-key (words) */
#define SIZE_PK_HFE_WORD ((((NB_MONOMIAL_PK*HFEm+7)>>3)+7)>>3)

/* Public-key (bytes) */
/* Minimal size */
#define SIZE_PK_HFE ((NB_MONOMIAL_PK*HFEm+7)>>3)

/* Secret-key: HFE polynomial + matrix S_inv + matrix T_inv (+ vector -t) */

#define SIZE_VECTOR_t 0

#define ACCESS_MATRIX_S NB_UINT_HFEVPOLY
#define ACCESS_MATRIX_T (ACCESS_MATRIX_S+MATRIXnv_SIZE)
#define ACCESS_VECTOR_t (ACCESS_MATRIX_T+MATRIXn_SIZE)

/* Secret-key (words) */
#define SIZE_SK_HFE_UNCOMPRESSED_WORD (ACCESS_VECTOR_t+SIZE_VECTOR_t)
/* Secret-key (bytes) */
#define SIZE_SK_HFE_UNCOMPRESSED (SIZE_SK_HFE_UNCOMPRESSED_WORD<<3)

/* Secret-key (bytes) */
#define SIZE_SK_HFE SIZE_SEED_SK
/* Secret-key (words) */
#define SIZE_SK_HFE_WORD ((SIZE_SK_HFE+7)>>3)


/* Signature: NB_WORD_GF2nv for signature NB_ITE, NB_WORD_GF2nvm for others */

/* Size of signature (words): */
#define SIZE_SIGN_UNCOMPRESSED \
    (NB_WORD_GF2nv+(NB_ITE-1)*NB_WORD_GF2nvm+SIZE_SALT_WORD)
/* Size of signature (bytes): */
/* (x+7)/8 = Ceiling(x/8) */
#define SIZE_SIGN_HFE ((SIZE_SIGN_THEORETICAL_HFE+7)>>3)


#endif
