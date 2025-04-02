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
#ifndef FSMSW_SPHINCSSHAKE_ADDRESS_H
#define FSMSW_SPHINCSSHAKE_ADDRESS_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Types.h"
#include "FsmSw_Sphincs_address.h"

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
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_SphincsShake_set_layer_addr(uint32 addr[8], uint32 layer);

void FsmSw_SphincsShake_set_tree_addr(uint32 addr[8], uint64 tree);

void FsmSw_SphincsShake_set_type(uint32 addr[8], uint32 type);

void FsmSw_SphincsShake_copy_subtree_addr(uint32 out[8], const uint32 in[8]);

void FsmSw_SphincsShake_set_keypair_addr_1_byte(uint32 addr[8], uint32 keypair);

/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsShake_set_keypair_addr_2_byte(uint32 addr[8], uint32 keypair);

void FsmSw_SphincsShake_set_chain_addr(uint32 addr[8], uint32 chain);

void FsmSw_SphincsShake_set_hash_addr(uint32 addr[8], uint32 hash);

void FsmSw_SphincsShake_copy_keypair_addr_1_byte(uint32 out[8], const uint32 in[8]);

/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsShake_copy_keypair_addr_2_byte(uint32 out[8], const uint32 in[8]);

void FsmSw_SphincsShake_set_tree_height(uint32 addr[8], uint32 tree_height);

void FsmSw_SphincsShake_set_tree_index(uint32 addr[8], uint32 tree_index);

#endif /* FSMSW_SPHINCSSHAKE_ADDRESS_H */
