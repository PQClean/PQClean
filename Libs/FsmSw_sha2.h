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
#ifndef FSMSW_SHA2_H
#define FSMSW_SHA2_H

/* The incremental API allows hashing of individual input blocks; these blocks must be exactly 64 bytes each.
 * Use the 'finalize' functions for any remaining bytes (possibly over 64). */

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Types.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define PQC_SHA256CTX_BYTES 40
#define PQC_SHA512CTX_BYTES 72

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
/* polyspace +4 MISRA2012:D4.8 [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* Structure for the incremental API */
typedef struct
{
    uint8 ctx[PQC_SHA256CTX_BYTES];
} sha224ctx;

/* polyspace +4 MISRA2012:D4.8 [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* Structure for the incremental API */
typedef struct
{
    uint8 ctx[PQC_SHA256CTX_BYTES];
} sha256ctx;

/* polyspace +4 MISRA2012:D4.8 [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* Structure for the incremental API */
typedef struct
{
    uint8 ctx[PQC_SHA512CTX_BYTES];
} sha384ctx;

/* polyspace +4 MISRA2012:D4.8 [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
/* Structure for the incremental API */
typedef struct
{
    uint8 ctx[PQC_SHA512CTX_BYTES];
} sha512ctx;

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
/* SHA224 API */
void FsmSw_sha224_inc_ctx_clone(sha224ctx *stateout, const sha224ctx *statein);
void FsmSw_sha224_inc_blocks(sha224ctx *state, const uint8 *in, uint32 inblocks);
void FsmSw_sha224(uint8 *out, const uint8 *in, uint32 inlen);

/* SHA256 API */
void FsmSw_sha256_inc_init(sha256ctx *state);
void FsmSw_sha256_inc_ctx_clone(sha256ctx *stateout, const sha256ctx *statein);
void FsmSw_sha256_inc_blocks(sha256ctx *state, const uint8 *in, uint32 inblocks);
void FsmSw_sha256_inc_finalize(uint8 *out, sha256ctx *state, const uint8 *in, uint32 inlen);
void FsmSw_sha256(uint8 *out, const uint8 *in, uint32 inlen);

/* SHA384 API */
void FsmSw_sha384_inc_ctx_clone(sha384ctx *stateout, const sha384ctx *statein);
void FsmSw_sha384_inc_blocks(sha384ctx *state, const uint8 *in, uint32 inblocks);
void FsmSw_sha384(uint8 *out, const uint8 *in, uint32 inlen);


/* SHA512 API */
void FsmSw_sha512_inc_init(sha512ctx *state);
void FsmSw_sha512_inc_ctx_clone(sha512ctx *stateout, const sha512ctx *statein);
void FsmSw_sha512_inc_blocks(sha512ctx *state, const uint8 *in, uint32 inblocks);
void FsmSw_sha512_inc_finalize(uint8 *out, sha512ctx *state, const uint8 *in, uint32 inlen);
void FsmSw_sha512(uint8 *out, const uint8 *in, uint32 inlen);

#endif
