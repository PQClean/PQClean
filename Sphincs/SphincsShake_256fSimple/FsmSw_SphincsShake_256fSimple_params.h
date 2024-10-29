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
#ifndef FSMSW_SPHINCSSHAKE_256FSIMPLE_PARAMS_H
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_PARAMS_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Sphincs_shake_offsets.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
/* Hash output length in bytes. */
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_N              32u
/* Height of the hypertree. */
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_FULL_HEIGHT    68u
/* Number of subtree layer. */
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_D              17u
/* FORS tree dimensions. */
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_FORS_HEIGHT     9u

#define FSMSW_SPHINCSSHAKE_256FSIMPLE_FORS_TREES     35u
/* Winternitz parameter, */
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_WOTS_W         16u
/* For clarity */
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_ADDR_BYTES     32u
/* WOTS parameters. */
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_WOTS_LOGW       4u

#define FSMSW_SPHINCSSHAKE_256FSIMPLE_WOTS_LEN1      (8u * FSMSW_SPHINCSSHAKE_256FSIMPLE_N / \
                                                      FSMSW_SPHINCSSHAKE_256FSIMPLE_WOTS_LOGW)

/* FSMSW_SPHINCSSHAKE_256FSIMPLE_WOTS_LEN2 is floor(log(len_1 * (w - 1)) / log(w)) + 1 we precompute */
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_WOTS_LEN2       3u

#define FSMSW_SPHINCSSHAKE_256FSIMPLE_WOTS_LEN          (FSMSW_SPHINCSSHAKE_256FSIMPLE_WOTS_LEN1 + \
                                                         FSMSW_SPHINCSSHAKE_256FSIMPLE_WOTS_LEN2)
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_WOTS_BYTES        (FSMSW_SPHINCSSHAKE_256FSIMPLE_WOTS_LEN * \
                                                         FSMSW_SPHINCSSHAKE_256FSIMPLE_N)

/* Subtree size. */
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_TREE_HEIGHT       (FSMSW_SPHINCSSHAKE_256FSIMPLE_FULL_HEIGHT / \
                                                         FSMSW_SPHINCSSHAKE_256FSIMPLE_D)

/* FORS parameters. */
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_FORS_MSG_BYTES   (((FSMSW_SPHINCSSHAKE_256FSIMPLE_FORS_HEIGHT * \
                                                         FSMSW_SPHINCSSHAKE_256FSIMPLE_FORS_TREES) + 7u) / 8u)
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_FORS_BYTES       ((FSMSW_SPHINCSSHAKE_256FSIMPLE_FORS_HEIGHT + 1u) * \
                                                         FSMSW_SPHINCSSHAKE_256FSIMPLE_FORS_TREES * \
                                                         FSMSW_SPHINCSSHAKE_256FSIMPLE_N)

/* Resulting SPX sizes. */
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_BYTES             (FSMSW_SPHINCSSHAKE_256FSIMPLE_N + \
                                                         FSMSW_SPHINCSSHAKE_256FSIMPLE_FORS_BYTES + \
                                                         (FSMSW_SPHINCSSHAKE_256FSIMPLE_D * \
                                                         FSMSW_SPHINCSSHAKE_256FSIMPLE_WOTS_BYTES) +\
                                                         (FSMSW_SPHINCSSHAKE_256FSIMPLE_FULL_HEIGHT * \
                                                         FSMSW_SPHINCSSHAKE_256FSIMPLE_N))
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_PK_BYTES          (2u * FSMSW_SPHINCSSHAKE_256FSIMPLE_N)
/* polyspace +2 MISRA2012:2.5 [Justified:]"This define is reserved for the future." */
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_SK_BYTES          (2 * FSMSW_SPHINCSSHAKE_256FSIMPLE_N + \
                                                         FSMSW_SPHINCSSHAKE_256FSIMPLE_PK_BYTES)

#define FSMSW_SPHINCSSHAKE_256FSIMPLE_CRYPTO_SEEDBYTES       (3u*FSMSW_SPHINCSSHAKE_256FSIMPLE_N)

/* Defines for max array length */
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_THASH_BUF_LEN            67u
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_TREEHASH_BUF_LEN          1u /* Buffer with this length is currently not used */
#define FSMSW_SPHINCSSHAKE_256FSIMPLE_TREEHASHX1_BUF_LEN        9u

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

#endif /* FSMSW_SPHINCSSHAKE_256FSIMPLE_PARAMS_H */
