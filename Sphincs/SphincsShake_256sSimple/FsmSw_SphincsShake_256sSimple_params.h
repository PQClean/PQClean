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
#ifndef FSMSW_SPHINCSSHAKE_256SSIMPLE_PARAMS_H
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_PARAMS_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Sphincs_shake_offsets.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
/* Hash output length in bytes. */
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_N              32u
/* Height of the hypertree. */
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_FULL_HEIGHT    64u
/* Number of subtree layer. */
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_D               8u
/* FORS tree dimensions. */
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_FORS_HEIGHT    14u
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_FORS_TREES     22u
/* Winternitz parameter, */
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_WOTS_W         16u

/* For clarity */
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_ADDR_BYTES     32u

/* WOTS parameters. */
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_WOTS_LOGW       4u

#define FSMSW_SPHINCSSHAKE_256SSIMPLE_WOTS_LEN1      (8u * FSMSW_SPHINCSSHAKE_256SSIMPLE_N / \
                                                      FSMSW_SPHINCSSHAKE_256SSIMPLE_WOTS_LOGW)

/* FSMSW_SPHINCSSHAKE_256SSIMPLE_WOTS_LEN2 is floor(log(len_1 * (w - 1)) / log(w)) + 1; we precompute */
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_WOTS_LEN2       3u

#define FSMSW_SPHINCSSHAKE_256SSIMPLE_WOTS_LEN        (FSMSW_SPHINCSSHAKE_256SSIMPLE_WOTS_LEN1 + \
                                                       FSMSW_SPHINCSSHAKE_256SSIMPLE_WOTS_LEN2)
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_WOTS_BYTES      (FSMSW_SPHINCSSHAKE_256SSIMPLE_WOTS_LEN * \
                                                       FSMSW_SPHINCSSHAKE_256SSIMPLE_N)
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_WOTS_PK_BYTES    FSMSW_SPHINCSSHAKE_256SSIMPLE_WOTS_BYTES

/* Subtree size. */
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_TREE_HEIGHT     (FSMSW_SPHINCSSHAKE_256SSIMPLE_FULL_HEIGHT / \
                                                       FSMSW_SPHINCSSHAKE_256SSIMPLE_D)

/* FORS parameters. */
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_FORS_MSG_BYTES ((FSMSW_SPHINCSSHAKE_256SSIMPLE_FORS_HEIGHT * \
                                                       FSMSW_SPHINCSSHAKE_256SSIMPLE_FORS_TREES + 7u) / 8u)
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_FORS_BYTES     ((FSMSW_SPHINCSSHAKE_256SSIMPLE_FORS_HEIGHT + 1u) * \
                                                       FSMSW_SPHINCSSHAKE_256SSIMPLE_FORS_TREES * \
                                                       FSMSW_SPHINCSSHAKE_256SSIMPLE_N)
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_FORS_PK_BYTES    FSMSW_SPHINCSSHAKE_256SSIMPLE_N

/* Resulting SPX sizes. */
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_BYTES           (FSMSW_SPHINCSSHAKE_256SSIMPLE_N + \
                                                       FSMSW_SPHINCSSHAKE_256SSIMPLE_FORS_BYTES + \
                                                       FSMSW_SPHINCSSHAKE_256SSIMPLE_D * \
                                                       FSMSW_SPHINCSSHAKE_256SSIMPLE_WOTS_BYTES +\
                                                       FSMSW_SPHINCSSHAKE_256SSIMPLE_FULL_HEIGHT * \
                                                       FSMSW_SPHINCSSHAKE_256SSIMPLE_N)
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_PK_BYTES        (2u * FSMSW_SPHINCSSHAKE_256SSIMPLE_N)
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_SK_BYTES        (2 * FSMSW_SPHINCSSHAKE_256SSIMPLE_N + \
                                                       FSMSW_SPHINCSSHAKE_256SSIMPLE_PK_BYTES)

#define FSMSW_SPHINCSSHAKE_256SSIMPLE_CRYPTO_SECRETKEYBYTES  FSMSW_SPHINCSSHAKE_256SSIMPLE_SK_BYTES
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_CRYPTO_PUBLICKEYBYTES  FSMSW_SPHINCSSHAKE_256SSIMPLE_PK_BYTES
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_CRYPTO_BYTES           FSMSW_SPHINCSSHAKE_256SSIMPLE_BYTES
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_CRYPTO_SEEDBYTES       (3u*FSMSW_SPHINCSSHAKE_256SSIMPLE_N)

/* Defines for max array length */
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_THASH_BUF_LEN            67u
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_TREEHASH_BUF_LEN          1u /* Buffer with this length is currently not used */
#define FSMSW_SPHINCSSHAKE_256SSIMPLE_TREEHASHX1_BUF_LEN       14u

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

#endif /* FSMSW_SPHINCSSHAKE_256SSIMPLE_PARAMS_H */
