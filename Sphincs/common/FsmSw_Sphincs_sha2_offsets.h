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
#ifndef FSMSW_SPHINCS_SHA2_OFFSETS_H
#define FSMSW_SPHINCS_SHA2_OFFSETS_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define FSMSW_SPHINCS_SHA256_BLOCK_BYTES     64u
#define FSMSW_SPHINCS_SHA256_OUTPUT_BYTES    32u
#define FSMSW_SPHINCS_SHA512_BLOCK_BYTES    128u
#define FSMSW_SPHINCS_SHA512_OUTPUT_BYTES    64u
/* The byte used to specify the Merkle tree layer */
#define FSMSW_SPHINCSSHA2_OFFSET_LAYER        0
/* The start of the 8 byte field used to specify the tree */
#define FSMSW_SPHINCSSHA2_OFFSET_TREE         1
/* The byte used to specify the hash type (reason) */
#define FSMSW_SPHINCSSHA2_OFFSET_TYPE         9
/* The high byte used to specify the key pair (which one-time signature) */
#define FSMSW_SPHINCSSHA2_OFFSET_KP_ADDR2    12
/* The low byte used to specify the key pair */
#define FSMSW_SPHINCSSHA2_OFFSET_KP_ADDR1    13
/* The byte used to specify the chain address (which Winternitz chain) */
#define FSMSW_SPHINCSSHA2_OFFSET_CHAIN_ADDR  17
/* The byte used to specify the hash address (where in the Winternitz chain) */
#define FSMSW_SPHINCSSHA2_OFFSET_HASH_ADDR   21
/* The byte used to specify the height of this node in the FORS or Merkle tree */
#define FSMSW_SPHINCSSHA2_OFFSET_TREE_HGT    17
/* The start of the 4 byte field used to specify the node in the FORS or Merkle tree */
#define FSMSW_SPHINCSSHA2_OFFSET_TREE_INDEX  18

#define FSMSW_SPHINCSSHA2_SHA2                1
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

#endif /* FSMSW_SPHINCS_SHA2_OFFSETS_H */
