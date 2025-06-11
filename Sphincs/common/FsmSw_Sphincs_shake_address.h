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
#include "FsmSw_Sphincs_address.h"
#include "FsmSw_Types.h"

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
void FsmSw_SphincsShake_SetLayerAddr(uint32 addr[8], uint32 layer);

void FsmSw_SphincsShake_SetTreeAddr(uint32 addr[8], uint64 tree);

void FsmSw_SphincsShake_SetType(uint32 addr[8], uint32 type);

void FsmSw_SphincsShake_CopySubTreeAddr(uint32 out[8], const uint32 in[8]);

void FsmSw_SphincsShake_SetKeyPairAddr1Byte(uint32 addr[8], uint32 keypair);

/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsShake_SetKeyPairAddr2Byte(uint32 addr[8], uint32 keypair);

void FsmSw_SphincsShake_SetChainAddr(uint32 addr[8], uint32 chain);

void FsmSw_SphincsShake_SetHashAddr(uint32 addr[8], uint32 hash);

void FsmSw_SphincsShake_CopyKeyPairAddr1Byte(uint32 out[8], const uint32 in[8]);

/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsShake_CopyKeyPairAddr2Byte(uint32 out[8], const uint32 in[8]);

void FsmSw_SphincsShake_SetTreeHeight(uint32 addr[8], uint32 tree_height);

void FsmSw_SphincsShake_SetTreeIndex(uint32 addr[8], uint32 tree_index);

#endif /* FSMSW_SPHINCSSHAKE_ADDRESS_H */
