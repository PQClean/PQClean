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
#ifndef FSMSW_SPHINCSSHAKE_128FSIMPLE_FORS_H
#define FSMSW_SPHINCSSHAKE_128FSIMPLE_FORS_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_SphincsShake_128fSimple_context.h"
#include "FsmSw_SphincsShake_128fSimple_params.h"
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
/**
 * Signs a message m, deriving the secret key from sk_seed and the FTS address.
 * Assumes m contains at least FSMSW_SPHINCSSHAKE_128FSIMPLE_FORS_HEIGHT * FSMSW_SPHINCSSHAKE_128FSIMPLE_FORS_TREES bits.
 */
void FsmSw_SphincsShake_128fSimple_Fors_Sign(uint8 *sig, uint8 *pk, const uint8 *m, const sphincs_shake_128f_ctx *ctx,
                                             const uint32 fors_addr[8]);

/**
 * Derives the FORS public key from a signature.
 * This can be used for verification by comparing to a known public key, or to
 * subsequently verify a signature on the derived public key. The latter is the
 * typical use-case when used as an FTS below an OTS in a hypertree.
 * Assumes m contains at least FSMSW_SPHINCSSHAKE_128FSIMPLE_FORS_HEIGHT * FSMSW_SPHINCSSHAKE_128FSIMPLE_FORS_TREES bits.
 */

/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsShake_128fSimple_Fors_PkFromSig(uint8 *pk, const uint8 *sig, const uint8 *m,
                                                  const sphincs_shake_128f_ctx *ctx, const uint32 fors_addr[8]);

#endif /* FSMSW_SPHINCSSHAKE_128FSIMPLE_FORS_H */
