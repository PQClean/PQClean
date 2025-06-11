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
#ifndef FSMSW_DILITHIUM3_PACKING_H
#define FSMSW_DILITHIUM3_PACKING_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Dilithium3_params.h"
#include "FsmSw_Dilithium3_polyvec.h"
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
void FsmSw_Dilithium3_PackPk(uint8 pk[FSMSW_DILITHIUM3_CRYPTO_PUBLICKEYBYTES], const uint8 rho[SEEDBYTES_DILITHIUM],
                             const polyveck_D3 *t1);

void FsmSw_Dilithium3_PackSk(uint8 sk[FSMSW_DILITHIUM3_CRYPTO_SECRETKEYBYTES], const uint8 rho[SEEDBYTES_DILITHIUM],
                             const uint8 tr[TRBYTES_DILITHIUM], const uint8 key[SEEDBYTES_DILITHIUM],
                             const polyveck_D3 *t0, const polyvecl_D3 *s1, const polyveck_D3 *s2);

void FsmSw_Dilithium3_PackSig(uint8 sig[FSMSW_DILITHIUM3_CRYPTO_BYTES], const uint8 c[CTILDEBYTES_DILITHIUM3],
                              const polyvecl_D3 *z, const polyveck_D3 *h);

void FsmSw_Dilithium3_UnpackPk(uint8 rho[SEEDBYTES_DILITHIUM], polyveck_D3 *t1,
                               const uint8 pk[FSMSW_DILITHIUM3_CRYPTO_PUBLICKEYBYTES]);

void FsmSw_Dilithium3_UnpackSk(uint8 rho[SEEDBYTES_DILITHIUM], uint8 tr[TRBYTES_DILITHIUM],
                               uint8 key[SEEDBYTES_DILITHIUM], polyveck_D3 *t0, polyvecl_D3 *s1, polyveck_D3 *s2,
                               const uint8 sk[FSMSW_DILITHIUM3_CRYPTO_SECRETKEYBYTES]);

sint8 FsmSw_Dilithium3_UnpackSig(uint8 c[CTILDEBYTES_DILITHIUM3], polyvecl_D3 *z, polyveck_D3 *h,
                                 const uint8 sig[FSMSW_DILITHIUM3_CRYPTO_BYTES]);

#endif /* FSMSW_DILITHIUM3_PACKING_H */
