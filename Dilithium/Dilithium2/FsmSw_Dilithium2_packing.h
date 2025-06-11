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
#ifndef FSMSW_DILITHIUM2_PACKING_H
#define FSMSW_DILITHIUM2_PACKING_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Dilithium2_params.h"
#include "FsmSw_Dilithium2_polyvec.h"
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
void FsmSw_Dilithium2_Pack_Pk(uint8 pk[FSMSW_DILITHIUM2_CRYPTO_PUBLICKEYBYTES], const uint8 rho[SEEDBYTES_DILITHIUM],
                              const polyveck_D2 *t1);

void FsmSw_Dilithium2_Pack_Sk(uint8 sk[FSMSW_DILITHIUM2_CRYPTO_SECRETKEYBYTES], const uint8 rho[SEEDBYTES_DILITHIUM],
                              const uint8 tr[TRBYTES_DILITHIUM], const uint8 key[SEEDBYTES_DILITHIUM],
                              const polyveck_D2 *t0, const polyvecl_D2 *s1, const polyveck_D2 *s2);

void FsmSw_Dilithium2_Pack_Sig(uint8 sig[FSMSW_DILITHIUM2_CRYPTO_BYTES], const uint8 c[CTILDEBYTES_DILITHIUM2],
                               const polyvecl_D2 *z, const polyveck_D2 *h);

void FsmSw_Dilithium2_Unpack_Pk(uint8 rho[SEEDBYTES_DILITHIUM], polyveck_D2 *t1,
                                const uint8 pk[FSMSW_DILITHIUM2_CRYPTO_PUBLICKEYBYTES]);

void FsmSw_Dilithium2_Unpack_Sk(uint8 rho[SEEDBYTES_DILITHIUM], uint8 tr[TRBYTES_DILITHIUM],
                                uint8 key[SEEDBYTES_DILITHIUM], polyveck_D2 *t0, polyvecl_D2 *s1, polyveck_D2 *s2,
                                const uint8 sk[FSMSW_DILITHIUM2_CRYPTO_SECRETKEYBYTES]);

sint8 FsmSw_Dilithium2_Unpack_Sig(uint8 c[CTILDEBYTES_DILITHIUM2], polyvecl_D2 *z, polyveck_D2 *h,
                                  const uint8 sig[FSMSW_DILITHIUM2_CRYPTO_BYTES]);

#endif /* FSMSW_DILITHIUM2_PACKING_H */
