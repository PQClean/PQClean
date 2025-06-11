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

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Sphincs_sha2_address.h"
#include "FsmSw_CommonLib.h"
#include "FsmSw_Sphincs_sha2_offsets.h"
#include "FsmSw_Sphincs_utils.h"

#include "FsmSw_Sphincs_sha2_address.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

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
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_set_layer_addr
*
* Description: Specify which level of Merkle tree (the "layer") we're working on
*
* Arguments:   - uint32 addr[8]: t.b.d.
*              - uint32 layer:   t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsSha2_SetLayerAddr(uint32 addr[8], uint32 layer)
{
  ((uint8 *)addr)[FSMSW_SPHINCSSHA2_OFFSET_LAYER] = (uint8)layer;
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_SetTreeAddr
*
* Description: Specify which Merkle tree within the level (the "tree address") we're working on
*
* Arguments:   - uint32 addr[8]: t.b.d.
*              - uint64 tree:    t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsSha2_SetTreeAddr(uint32 addr[8], uint64 tree)
{
  FsmSw_Sphincs_UllToBytes(&((uint8 *)addr)[FSMSW_SPHINCSSHA2_OFFSET_TREE], 8, tree);
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_set_layer_addr
*
* Description: Specify the reason we'll use this address structure for, that is, what hash will we compute with it.
*              This is used so that unrelated types of hashes don't accidentally get the same address structure.
*              The type will be one of the SPX_ADDR_TYPE constants.
*
* Arguments:   - uint32 addr[8]: t.b.d.
*              - uint32 type:    t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsSha2_SetType(uint32 addr[8], uint32 type)
{
  ((uint8 *)addr)[FSMSW_SPHINCSSHA2_OFFSET_TYPE] = (uint8)type;
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_copy_subtree_addr
*
* Description: Copy the layer and tree fields of the address structure. This is used when we're doing multiple types
*              of hashes within the same Merkle tree
*
* Arguments:   -       uint32 out[8]: t.b.d.
*              - const uint32 in[8]:  t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsSha2_CopySubTreeAddr(uint32 out[8], const uint32 in[8])
{
  FsmSw_CommonLib_MemCpy(out, in, FSMSW_SPHINCSSHA2_OFFSET_TREE + 8);
}

/* These functions are used for OTS addresses. */

/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_SetKeyPairAddr1Byte
*
* Description: Specify which Merkle leaf we're working on; that is, which OTS keypair we're talking about.
*
* Arguments:   - uint32 addr[8]: t.b.d.
*              - uint32 keypair: t.b.d
*
***********************************************************************************************************************/
void FsmSw_SphincsSha2_SetKeyPairAddr1Byte(uint32 addr[8], uint32 keypair)
{
  ((uint8 *)addr)[FSMSW_SPHINCSSHA2_OFFSET_KP_ADDR1] = (uint8)keypair;
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_SetKeyPairAddr2Byte
*
* Description: Specify which Merkle leaf we're working on; that is, which OTS keypair we're talking about.
*
* Arguments:   - uint32 addr[8]: t.b.d.
*              - uint32 keypair: t.b.d.
*
***********************************************************************************************************************/
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsSha2_SetKeyPairAddr2Byte(uint32 addr[8], uint32 keypair)
{
  /* We have > 256 OTS at the bottom of the Merkle tree; to specify which one, we'd need to express it in two bytes */
  ((uint8 *)addr)[FSMSW_SPHINCSSHA2_OFFSET_KP_ADDR2] = (uint8)(keypair >> 8);
  ((uint8 *)addr)[FSMSW_SPHINCSSHA2_OFFSET_KP_ADDR1] = (uint8)keypair;
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_CopyKeyPairAddr1Byte
*
* Description: Copy the layer, tree and keypair fields of the address structure. This is used when we're doing
*              multiple things within the same OTS keypair
*
* Arguments:   -       uint32 out[8]: t.b.d.
*              - const uint32 in[8]:  t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsSha2_CopyKeyPairAddr1Byte(uint32 out[8], const uint32 in[8])
{
  FsmSw_CommonLib_MemCpy(out, in, FSMSW_SPHINCSSHA2_OFFSET_TREE + 8);
  ((uint8 *)out)[FSMSW_SPHINCSSHA2_OFFSET_KP_ADDR1] = ((const uint8 *)in)[FSMSW_SPHINCSSHA2_OFFSET_KP_ADDR1];
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_CopyKeyPairAddr2Byte
*
* Description: Copy the layer, tree and keypair fields of the address structure. This is used when we're doing
*              multiple things within the same OTS keypair
*
* Arguments:   -       uint32 out[8]: t.b.d.
*              - const uint32 in[8]:  t.b.d.
*
***********************************************************************************************************************/
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsSha2_CopyKeyPairAddr2Byte(uint32 out[8], const uint32 in[8])
{
  FsmSw_CommonLib_MemCpy(out, in, FSMSW_SPHINCSSHA2_OFFSET_TREE + 8);
  ((uint8 *)out)[FSMSW_SPHINCSSHA2_OFFSET_KP_ADDR2] = ((const uint8 *)in)[FSMSW_SPHINCSSHA2_OFFSET_KP_ADDR2];
  ((uint8 *)out)[FSMSW_SPHINCSSHA2_OFFSET_KP_ADDR1] = ((const uint8 *)in)[FSMSW_SPHINCSSHA2_OFFSET_KP_ADDR1];
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_set_chain_addr
*
* Description: Specify which Merkle chain within the OTS we're working with (the chain address)
*
* Arguments:   - uint32 addr[8]: t.b.d.
*              - uint32 chain:   t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsSha2_SetChainAddr(uint32 addr[8], uint32 chain)
{
  ((uint8 *)addr)[FSMSW_SPHINCSSHA2_OFFSET_CHAIN_ADDR] = (uint8)chain;
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_set_hash_addr
*
* Description: Specify where in the Merkle chain we are (the hash address)
*
* Arguments:   - uint32 addr[8]: t.b.d.
*              - uint32 hash:    t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsSha2_SetHashAddr(uint32 addr[8], uint32 hash)
{
  ((uint8 *)addr)[FSMSW_SPHINCSSHA2_OFFSET_HASH_ADDR] = (uint8)hash;
}

/* These functions are used for all hash tree addresses (including FORS). */

/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_set_tree_height
*
* Description: Specify the height of the node in the Merkle/FORS tree we are in (the tree height)
*
* Arguments:   - uint32 addr[8]:     t.b.d.
*              - uint32 tree_height: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsSha2_SetTreeHeight(uint32 addr[8], uint32 tree_height)
{
  ((uint8 *)addr)[FSMSW_SPHINCSSHA2_OFFSET_TREE_HGT] = (uint8)tree_height;
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_set_tree_index
*
* Description: Specify the distance from the left edge of the node in the Merkle/FORS tree (the tree index)
*
* Arguments:   - uint32 addr[8]:    t.b.d.
*              - uint32 tree_index: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsSha2_SetTreeIndex(uint32 addr[8], uint32 tree_index)
{
  FsmSw_Sphincs_U32ToBytes(&((uint8 *)addr)[FSMSW_SPHINCSSHA2_OFFSET_TREE_INDEX], tree_index);
}
