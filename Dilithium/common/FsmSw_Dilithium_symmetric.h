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
 **********************************************************************************************************************/
#ifndef FSMSW_DILITHIUM_SYMMETRIC_H
#define FSMSW_DILITHIUM_SYMMETRIC_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
#include "FsmSw_Fips202.h"
#include "FsmSw_Dilithium_params.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define STREAM128_BLOCKBYTES SHAKE128_RATE
#define STREAM256_BLOCKBYTES SHAKE256_RATE

#define stream128_init(STATE, SEED, NONCE)              FsmSw_Dilithium_shake128_stream_init(STATE, SEED, NONCE)
#define stream128_squeezeblocks(OUT, OUTBLOCKS, STATE)  FsmSw_Fips202_shake128_inc_squeeze(OUT, (OUTBLOCKS) * \
                                                                                                (SHAKE128_RATE), STATE)

#define stream256_init(STATE, SEED, NONCE)              FsmSw_Dilithium_shake256_stream_init(STATE, SEED, NONCE)
#define stream256_squeezeblocks(OUT, OUTBLOCKS, STATE)  FsmSw_Fips202_shake256_inc_squeeze(OUT, (OUTBLOCKS) * \
                                                                                                (SHAKE256_RATE), STATE)

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef shake128incctx FsmSw_Dilithium_stream128_state;
typedef shake256incctx FsmSw_Dilithium_stream256_state;

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_Dilithium_shake128_stream_init(shake128incctx *state, const uint8 seed[SEEDBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium_shake256_stream_init(shake256incctx *state, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);

#endif /* FSMSW_DILITHIUM_SYMMETRIC_H */
