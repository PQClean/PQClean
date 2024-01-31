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
#ifndef FSMSW_SPHINCSSHA2_256FSIMPLE_PARAMS_H
#define FSMSW_SPHINCSSHA2_256FSIMPLE_PARAMS_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Sphincs_sha2_offsets.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/* Hash output length in bytes. */
#define FSMSW_SPHINCSSHA2_256FSIMPLE_N              32u
/* Height of the hypertree. */
#define FSMSW_SPHINCSSHA2_256FSIMPLE_FULL_HEIGHT    68u
/* Number of subtree layer. */
#define FSMSW_SPHINCSSHA2_256FSIMPLE_D              17u
/* FORS tree dimensions. */
#define FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_HEIGHT     9u
#define FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_TREES     35u
/* Winternitz parameter, */
#define FSMSW_SPHINCSSHA2_256FSIMPLE_WOTS_W         16u

/* This is a SHA2-based parameter set, hence whether we use SHA-256 exclusively or we use both SHA-256 and SHA-512 is
 * controlled by the following #define */
#define FSMSW_SPHINCSSHA2_256FSIMPLE_SHA512      1  /* Use SHA-512 for H and T_l, l >= 2 */

/* For clarity */
#define FSMSW_SPHINCSSHA2_256FSIMPLE_ADDR_BYTES 32

/* WOTS parameters. */
#define FSMSW_SPHINCSSHA2_256FSIMPLE_WOTS_LOGW   4u

#define FSMSW_SPHINCSSHA2_256FSIMPLE_WOTS_LEN1  (8u * FSMSW_SPHINCSSHA2_256FSIMPLE_N / \
                                                 FSMSW_SPHINCSSHA2_256FSIMPLE_WOTS_LOGW)

/* FSMSW_SPHINCSSHA2_256FSIMPLE_WOTS_LEN2 is floor(log(len_1 * (w - 1)) / log(w)) + 1; we precompute */
#define FSMSW_SPHINCSSHA2_256FSIMPLE_WOTS_LEN2   3u

#define FSMSW_SPHINCSSHA2_256FSIMPLE_WOTS_LEN          (FSMSW_SPHINCSSHA2_256FSIMPLE_WOTS_LEN1 + \
                                                        FSMSW_SPHINCSSHA2_256FSIMPLE_WOTS_LEN2)
#define FSMSW_SPHINCSSHA2_256FSIMPLE_WOTS_BYTES        (FSMSW_SPHINCSSHA2_256FSIMPLE_WOTS_LEN * \
                                                        FSMSW_SPHINCSSHA2_256FSIMPLE_N)
#define FSMSW_SPHINCSSHA2_256FSIMPLE_WOTS_PK_BYTES      FSMSW_SPHINCSSHA2_256FSIMPLE_WOTS_BYTES

/* Subtree size. */
#define FSMSW_SPHINCSSHA2_256FSIMPLE_TREE_HEIGHT       (FSMSW_SPHINCSSHA2_256FSIMPLE_FULL_HEIGHT / \
                                                        FSMSW_SPHINCSSHA2_256FSIMPLE_D)

/* FORS parameters. */
#define FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_MSG_BYTES   ((FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_HEIGHT * \
                                                        FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_TREES + 7u) / 8u)
#define FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_BYTES       ((FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_HEIGHT + 1u) * \
                                                        FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_TREES * \
                                                        FSMSW_SPHINCSSHA2_256FSIMPLE_N)
#define FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_PK_BYTES      FSMSW_SPHINCSSHA2_256FSIMPLE_N

/* Resulting SPX sizes. */
#define FSMSW_SPHINCSSHA2_256FSIMPLE_BYTES             (FSMSW_SPHINCSSHA2_256FSIMPLE_N + \
                                                        FSMSW_SPHINCSSHA2_256FSIMPLE_FORS_BYTES + \
                                                        FSMSW_SPHINCSSHA2_256FSIMPLE_D * \
                                                        FSMSW_SPHINCSSHA2_256FSIMPLE_WOTS_BYTES +\
                                                        FSMSW_SPHINCSSHA2_256FSIMPLE_FULL_HEIGHT * \
                                                        FSMSW_SPHINCSSHA2_256FSIMPLE_N)
#define FSMSW_SPHINCSSHA2_256FSIMPLE_PK_BYTES          (2u * FSMSW_SPHINCSSHA2_256FSIMPLE_N)
#define FSMSW_SPHINCSSHA2_256FSIMPLE_SK_BYTES          (2 * FSMSW_SPHINCSSHA2_256FSIMPLE_N + \
                                                        FSMSW_SPHINCSSHA2_256FSIMPLE_PK_BYTES)

#define FSMSW_SPHINCSSHA2_256FSIMPLE_CRYPTO_SECRETKEYBYTES  FSMSW_SPHINCSSHA2_256FSIMPLE_SK_BYTES
#define FSMSW_SPHINCSSHA2_256FSIMPLE_CRYPTO_PUBLICKEYBYTES  FSMSW_SPHINCSSHA2_256FSIMPLE_PK_BYTES
#define FSMSW_SPHINCSSHA2_256FSIMPLE_CRYPTO_BYTES           FSMSW_SPHINCSSHA2_256FSIMPLE_BYTES
#define FSMSW_SPHINCSSHA2_256FSIMPLE_CRYPTO_SEEDBYTES       (3u*FSMSW_SPHINCSSHA2_256FSIMPLE_N)

/* Defines for max array length */
#define FSMSW_SPHINCSSHA2_256FSIMPLE_MGF1_256_BUF_LEN          1u /* Buffer with this length is currently not used */
#define FSMSW_SPHINCSSHA2_256FSIMPLE_MGF1_512_BUF_LEN        128u
#define FSMSW_SPHINCSSHA2_256FSIMPLE_THASH_BUF_LEN            67u
#define FSMSW_SPHINCSSHA2_256FSIMPLE_TREEHASH_BUF_LEN          1u /* Buffer with this length is currently not used */
#define FSMSW_SPHINCSSHA2_256FSIMPLE_TREEHASHX1_BUF_LEN        9u

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

#endif /* FSMSW_SPHINCSSHA2_256FSIMPLE_PARAMS_H */
