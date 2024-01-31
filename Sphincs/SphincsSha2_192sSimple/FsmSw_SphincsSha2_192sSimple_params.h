/***********************************************************************************************************************
*
*                                          IAV GmbH
*
***********************************************************************************************************************/
/*
*
*  $File$
*
*  $Author$
*
*  $Date$
*
*  $Rev$
*
***********************************************************************************************************************/
#ifndef FSMSW_SPHINCSSHA2_192SSIMPLE_PARAMS_H
#define FSMSW_SPHINCSSHA2_192SSIMPLE_PARAMS_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Sphincs_sha2_offsets.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
/* Hash output length in bytes. */
#define FSMSW_SPHINCSSHA2_192SSIMPLE_N              24u
/* Height of the hypertree. */
#define FSMSW_SPHINCSSHA2_192SSIMPLE_FULL_HEIGHT    63u
/* Number of subtree layer. */
#define FSMSW_SPHINCSSHA2_192SSIMPLE_D               7u
/* FORS tree dimensions. */
#define FSMSW_SPHINCSSHA2_192SSIMPLE_FORS_HEIGHT    14u
#define FSMSW_SPHINCSSHA2_192SSIMPLE_FORS_TREES     17u
/* Winternitz parameter, */
#define FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_W         16u

/* The hash function is defined by linking a different hash.c file, as opposed
   to setting a #define constant. */

/* This is a SHA2-based parameter set, hence whether we use SHA-256
 * exclusively or we use both SHA-256 and SHA-512 is controlled by
 * the following #define */
#define FSMSW_SPHINCSSHA2_192SSIMPLE_SHA512      1  /* Use SHA-512 for H and T_l, l >= 2 */

/* For clarity */
#define FSMSW_SPHINCSSHA2_192SSIMPLE_ADDR_BYTES 32

/* WOTS parameters. */
#define FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_LOGW   4u

#define FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_LEN1  (8u * FSMSW_SPHINCSSHA2_192SSIMPLE_N / \
                                                 FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_LOGW)

/* FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_LEN2 is floor(log(len_1 * (w - 1)) / log(w)) + 1; we precompute */
#define FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_LEN2   3u

#define FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_LEN         (FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_LEN1 + \
                                                       FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_LEN2)
#define FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_BYTES       (FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_LEN * \
                                                       FSMSW_SPHINCSSHA2_192SSIMPLE_N)
#define FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_PK_BYTES     FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_BYTES

/* Subtree size. */
#define FSMSW_SPHINCSSHA2_192SSIMPLE_TREE_HEIGHT      (FSMSW_SPHINCSSHA2_192SSIMPLE_FULL_HEIGHT / \
                                                       FSMSW_SPHINCSSHA2_192SSIMPLE_D)

/* FORS parameters. */
#define FSMSW_SPHINCSSHA2_192SSIMPLE_FORS_MSG_BYTES  ((FSMSW_SPHINCSSHA2_192SSIMPLE_FORS_HEIGHT * \
                                                       FSMSW_SPHINCSSHA2_192SSIMPLE_FORS_TREES + 7u) / 8u)
#define FSMSW_SPHINCSSHA2_192SSIMPLE_FORS_BYTES      ((FSMSW_SPHINCSSHA2_192SSIMPLE_FORS_HEIGHT + 1u) * \
                                                       FSMSW_SPHINCSSHA2_192SSIMPLE_FORS_TREES * \
                                                       FSMSW_SPHINCSSHA2_192SSIMPLE_N)
#define FSMSW_SPHINCSSHA2_192SSIMPLE_FORS_PK_BYTES     FSMSW_SPHINCSSHA2_192SSIMPLE_N

/* Resulting SPX sizes. */
#define FSMSW_SPHINCSSHA2_192SSIMPLE_BYTES            (FSMSW_SPHINCSSHA2_192SSIMPLE_N + \
                                                       FSMSW_SPHINCSSHA2_192SSIMPLE_FORS_BYTES + \
                                                       FSMSW_SPHINCSSHA2_192SSIMPLE_D * \
                                                       FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_BYTES + \
                                                       FSMSW_SPHINCSSHA2_192SSIMPLE_FULL_HEIGHT * \
                                                       FSMSW_SPHINCSSHA2_192SSIMPLE_N)
#define FSMSW_SPHINCSSHA2_192SSIMPLE_PK_BYTES         (2u * FSMSW_SPHINCSSHA2_192SSIMPLE_N)
#define FSMSW_SPHINCSSHA2_192SSIMPLE_SK_BYTES         (2 * FSMSW_SPHINCSSHA2_192SSIMPLE_N + \
                                                       FSMSW_SPHINCSSHA2_192SSIMPLE_PK_BYTES)

#define FSMSW_SPHINCSSHA2_192SSIMPLE_CRYPTO_SECRETKEYBYTES  FSMSW_SPHINCSSHA2_192SSIMPLE_SK_BYTES
#define FSMSW_SPHINCSSHA2_192SSIMPLE_CRYPTO_PUBLICKEYBYTES  FSMSW_SPHINCSSHA2_192SSIMPLE_PK_BYTES
#define FSMSW_SPHINCSSHA2_192SSIMPLE_CRYPTO_BYTES           FSMSW_SPHINCSSHA2_192SSIMPLE_BYTES
#define FSMSW_SPHINCSSHA2_192SSIMPLE_CRYPTO_SEEDBYTES       (3u*FSMSW_SPHINCSSHA2_192SSIMPLE_N)

/* Defines for max array length */
#define FSMSW_SPHINCSSHA2_192SSIMPLE_MGF1_256_BUF_LEN          1u /* Buffer with this length is currently not used */
#define FSMSW_SPHINCSSHA2_192SSIMPLE_MGF1_512_BUF_LEN        112u
#define FSMSW_SPHINCSSHA2_192SSIMPLE_THASH_BUF_LEN            51u
#define FSMSW_SPHINCSSHA2_192SSIMPLE_TREEHASH_BUF_LEN          1u /* Buffer with this length is currently not used */
#define FSMSW_SPHINCSSHA2_192SSIMPLE_TREEHASHX1_BUF_LEN       14u

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/


#endif /* FSMSW_SPHINCSSHA2_192SSIMPLE_PARAMS_H */
