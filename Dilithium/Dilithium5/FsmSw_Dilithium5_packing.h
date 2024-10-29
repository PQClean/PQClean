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
#ifndef FSMSW_DILITHIUM5_PACKING_H
#define FSMSW_DILITHIUM5_PACKING_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Types.h"
#include "FsmSw_Dilithium5_params.h"
#include "FsmSw_Dilithium5_polyvec.h"

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
void FsmSw_Dilithium5_pack_pk(uint8 pk[FSMSW_DILITHIUM5_CRYPTO_PUBLICKEYBYTES], const uint8 rho[SEEDBYTES_DILITHIUM],
                              const polyveck_D5 *t1);

void FsmSw_Dilithium5_pack_sk(uint8 sk[FSMSW_DILITHIUM5_CRYPTO_SECRETKEYBYTES],
                              const uint8 rho[SEEDBYTES_DILITHIUM],
                              const uint8 tr[TRBYTES_DILITHIUM],
                              const uint8 key[SEEDBYTES_DILITHIUM],
                              const polyveck_D5 *t0,
                              const polyvecl_D5 *s1,
                              const polyveck_D5 *s2);

void FsmSw_Dilithium5_pack_sig(uint8 sig[FSMSW_DILITHIUM5_CRYPTO_BYTES], const uint8 c[CTILDEBYTES_DILITHIUM5],
                               const polyvecl_D5 *z, const polyveck_D5 *h);

void FsmSw_Dilithium5_unpack_pk(uint8 rho[SEEDBYTES_DILITHIUM], polyveck_D5 *t1,
                                const uint8 pk[FSMSW_DILITHIUM5_CRYPTO_PUBLICKEYBYTES]);

void FsmSw_Dilithium5_unpack_sk(uint8 rho[SEEDBYTES_DILITHIUM],
                                uint8 tr[TRBYTES_DILITHIUM],
                                uint8 key[SEEDBYTES_DILITHIUM],
                                polyveck_D5 *t0,
                                polyvecl_D5 *s1,
                                polyveck_D5 *s2,
                                const uint8 sk[FSMSW_DILITHIUM5_CRYPTO_SECRETKEYBYTES]);

sint8 FsmSw_Dilithium5_unpack_sig(uint8 c[CTILDEBYTES_DILITHIUM5], polyvecl_D5 *z, polyveck_D5 *h,
                                  const uint8 sig[FSMSW_DILITHIUM5_CRYPTO_BYTES]);

#endif /* FSMSW_DILITHIUM5_PACKING_H */
