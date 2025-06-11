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
#ifndef FSMSW_SPHINCSSHAKE_128SSIMPLE_UTILS_H
#define FSMSW_SPHINCSSHAKE_128SSIMPLE_UTILS_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_SphincsShake_128sSimple_context.h"
#include "FsmSw_SphincsShake_128sSimple_params.h"
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
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsShake_128sSimple_ComputeRoot(uint8 *root, const uint8 *leaf, uint32 leaf_idx, uint32 idx_offset,
                                               const uint8 *auth_path, uint32 tree_height,
                                               const sphincs_shake_128s_ctx *ctx, uint32 addr[8]);

/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsShake_128sSimple_TreeHash(uint8 *root, uint8 *auth_path, const sphincs_shake_128s_ctx *ctx,
                                            uint32 leaf_idx, uint32 idx_offset, uint32 tree_height,
                                            void (*gen_leaf)(uint8 *leaf, const sphincs_shake_128s_ctx *ctx,
                                                             uint32 addr_idx, const uint32 tree_addr[8]),
                                            uint32 tree_addr[8]);

#endif /* FSMSW_SPHINCSSHAKE_128SSIMPLE_UTILS_H */
