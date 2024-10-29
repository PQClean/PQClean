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
#ifndef SHAKE_OFFSETS_H_
#define SHAKE_OFFSETS_H_

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
/* The byte used to specify the Merkle tree layer */
#define FSMSW_SPHINCSSHAKE_OFFSET_LAYER       3
/* The start of the 8 byte field used to specify the tree */
#define FSMSW_SPHINCSSHAKE_OFFSET_TREE        8
/* The byte used to specify the hash type (reason) */
#define FSMSW_SPHINCSSHAKE_OFFSET_TYPE       19
 /* The high byte used to specify the key pair (which one-time signature) */
#define FSMSW_SPHINCSSHAKE_OFFSET_KP_ADDR2   22
 /* The low byte used to specify the key pair */
#define FSMSW_SPHINCSSHAKE_OFFSET_KP_ADDR1   23

 /* The byte used to specify the chain address (which Winternitz chain) */
#define FSMSW_SPHINCSSHAKE_OFFSET_CHAIN_ADDR 27
/* The byte used to specify the hash address (where in the Winternitz chain) */
#define FSMSW_SPHINCSSHAKE_OFFSET_HASH_ADDR  31
/* The byte used to specify the height of this node in the FORS or Merkle tree */
#define FSMSW_SPHINCSSHAKE_OFFSET_TREE_HGT   27
/* The start of the 4 byte field used to specify the node in the FORS or Merkle tree */
#define FSMSW_SPHINCSSHAKE_OFFSET_TREE_INDEX 28

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

#endif /* SHAKE_OFFSETS_H_ */
