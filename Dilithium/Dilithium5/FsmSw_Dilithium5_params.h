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
#ifndef FSMSW_DILITHIUM5_PARAMS_H
#define FSMSW_DILITHIUM5_PARAMS_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Dilithium_params.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define K_DILITHIUM5               8u
#define L_DILITHIUM5               7u
#define ETA_DILITHIUM5             2u
#define TAU_DILITHIUM5             60u
#define BETA_DILITHIUM5            120u
/* (1 << 19) = 524288u */
#define GAMMA1_DILITHIUM5          524288u
/* MISRA check for GAMMA2_DILITHIUM5 is ongoing. You get an error if GAMMA2_DILITHIUM5 get a type cast. */
#define GAMMA2_DILITHIUM5          ((Q_DILITHIUM-1)/32)
#define OMEGA_DILITHIUM5           75u
#define CTILDEBYTES_DILITHIUM5     64u

#define POLYVECH_PACKEDBYTES_DILITHIUM5 (OMEGA_DILITHIUM5 + K_DILITHIUM5)

#define POLYZ_PACKEDBYTES_DILITHIUM5   640u
#define POLYW1_PACKEDBYTES_DILITHIUM5  128u
#define POLYETA_PACKEDBYTES_DILITHIUM5  96u

#define FSMSW_DILITHIUM5_CRYPTO_PUBLICKEYBYTES (SEEDBYTES_DILITHIUM + (K_DILITHIUM5*POLYT1_PACKEDBYTES_DILITHIUM))

#define FSMSW_DILITHIUM5_CRYPTO_SECRETKEYBYTES (  (2u*SEEDBYTES_DILITHIUM)                       \
                                                + TRBYTES_DILITHIUM                           \
                                                + (L_DILITHIUM5*POLYETA_PACKEDBYTES_DILITHIUM5) \
                                                + (K_DILITHIUM5*POLYETA_PACKEDBYTES_DILITHIUM5) \
                                                + (K_DILITHIUM5*POLYT0_PACKEDBYTES_DILITHIUM))

#define FSMSW_DILITHIUM5_CRYPTO_BYTES (  CTILDEBYTES_DILITHIUM5                    \
                                       + (L_DILITHIUM5*POLYZ_PACKEDBYTES_DILITHIUM5) \
                                       + POLYVECH_PACKEDBYTES_DILITHIUM5)

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

#endif /* FSMSW_DILITHIUM5_PARAMS_H */
