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
#ifndef FSMSW_DILITHIUM2_PARAMS_H
#define FSMSW_DILITHIUM2_PARAMS_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Dilithium_params.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define K_DILITHIUM2               4u
#define L_DILITHIUM2               4u
#define ETA_DILITHIUM2             2u
#define TAU_DILITHIUM2             39u
#define BETA_DILITHIUM2            78u
/* (1u << 17u) = 131072 */
#define GAMMA1_DILITHIUM2          131072u
/* MISRA check for GAMMA2_DILITHIUM2 is ongoing. You get an error if GAMMA2_DILITHIUM2 get a type cast. */
#define GAMMA2_DILITHIUM2          ((Q_DILITHIUM-1)/88)
#define OMEGA_DILITHIUM2           80u
#define CTILDEBYTES_DILITHIUM2     32u

#define POLYVECH_PACKEDBYTES_DILITHIUM2 (OMEGA_DILITHIUM2 + K_DILITHIUM2)

#define POLYZ_PACKEDBYTES_DILITHIUM2   576u
#define POLYW1_PACKEDBYTES_DILITHIUM2  192u
#define POLYETA_PACKEDBYTES_DILITHIUM2  96u

#define FSMSW_DILITHIUM2_CRYPTO_PUBLICKEYBYTES (SEEDBYTES_DILITHIUM + K_DILITHIUM2*POLYT1_PACKEDBYTES_DILITHIUM)

#define FSMSW_DILITHIUM2_CRYPTO_SECRETKEYBYTES (  2u*SEEDBYTES_DILITHIUM                      \
                                                + TRBYTES_DILITHIUM                           \
                                                + L_DILITHIUM2*POLYETA_PACKEDBYTES_DILITHIUM2 \
                                                + K_DILITHIUM2*POLYETA_PACKEDBYTES_DILITHIUM2 \
                                                + K_DILITHIUM2*POLYT0_PACKEDBYTES_DILITHIUM)

#define FSMSW_DILITHIUM2_CRYPTO_BYTES (  CTILDEBYTES_DILITHIUM2                    \
                                       + L_DILITHIUM2*POLYZ_PACKEDBYTES_DILITHIUM2 \
                                       + POLYVECH_PACKEDBYTES_DILITHIUM2)

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

#endif /* FSMSW_DILITHIUM2_PARAMS_H */
