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
#include "FsmSw_Types.h"
#include "FsmSw_Dilithium2_params.h"
#include "FsmSw_Dilithium2_polyvec.h"

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
void FsmSw_Dilithium2_pack_pk(uint8 pk[FSMSW_DILITHIUM2_CRYPTO_PUBLICKEYBYTES], const uint8 rho[SEEDBYTES_DILITHIUM],
                              const polyveck_D2 *t1);

void FsmSw_Dilithium2_pack_sk(uint8 sk[FSMSW_DILITHIUM2_CRYPTO_SECRETKEYBYTES],
                              const uint8 rho[SEEDBYTES_DILITHIUM],
                              const uint8 tr[TRBYTES_DILITHIUM],
                              const uint8 key[SEEDBYTES_DILITHIUM],
                              const polyveck_D2 *t0,
                              const polyvecl_D2 *s1,
                              const polyveck_D2 *s2);

void FsmSw_Dilithium2_pack_sig(uint8 sig[FSMSW_DILITHIUM2_CRYPTO_BYTES], const uint8 c[CTILDEBYTES_DILITHIUM2],
                               const polyvecl_D2 *z, const polyveck_D2 *h);

void FsmSw_Dilithium2_unpack_pk(uint8 rho[SEEDBYTES_DILITHIUM], polyveck_D2 *t1,
                                const uint8 pk[FSMSW_DILITHIUM2_CRYPTO_PUBLICKEYBYTES]);

void FsmSw_Dilithium2_unpack_sk(uint8 rho[SEEDBYTES_DILITHIUM],
                                uint8 tr[TRBYTES_DILITHIUM],
                                uint8 key[SEEDBYTES_DILITHIUM],
                                polyveck_D2 *t0,
                                polyvecl_D2 *s1,
                                polyveck_D2 *s2,
                                const uint8 sk[FSMSW_DILITHIUM2_CRYPTO_SECRETKEYBYTES]);

sint8 FsmSw_Dilithium2_unpack_sig(uint8 c[CTILDEBYTES_DILITHIUM2], polyvecl_D2 *z, polyveck_D2 *h,
                                  const uint8 sig[FSMSW_DILITHIUM2_CRYPTO_BYTES]);

#endif /* FSMSW_DILITHIUM2_PACKING_H */
