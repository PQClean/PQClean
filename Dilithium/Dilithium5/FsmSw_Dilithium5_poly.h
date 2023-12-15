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
#ifndef FSMSW_DILITHIUM5_POLY_H
#define FSMSW_DILITHIUM5_POLY_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
#include "FsmSw_Dilithium5_params.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct {
    sint32 coeffs[N_DILITHIUM];
} poly_D5;

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_Dilithium5_poly_reduce(poly_D5 *a);
void FsmSw_Dilithium5_poly_caddq(poly_D5 *a);

void FsmSw_Dilithium5_poly_add(poly_D5 *c, const poly_D5 *a, const poly_D5 *b);
void FsmSw_Dilithium5_poly_sub(poly_D5 *c, const poly_D5 *a, const poly_D5 *b);
void FsmSw_Dilithium5_poly_shiftl(poly_D5 *a);

void FsmSw_Dilithium5_poly_ntt(poly_D5 *a);
void FsmSw_Dilithium5_poly_invntt_tomont(poly_D5 *a);
void FsmSw_Dilithium5_poly_pointwise_montgomery(poly_D5 *c, const poly_D5 *a, const poly_D5 *b);

void FsmSw_Dilithium5_poly_power2round(poly_D5 *a1, poly_D5 *a0, const poly_D5 *a);
void FsmSw_Dilithium5_poly_decompose(poly_D5 *a1, poly_D5 *a0, const poly_D5 *a);
uint32 FsmSw_Dilithium5_poly_make_hint(poly_D5 *h, const poly_D5 *a0, const poly_D5 *a1);
void FsmSw_Dilithium5_poly_use_hint(poly_D5 *b, const poly_D5 *a, const poly_D5 *h);

sint8 FsmSw_Dilithium5_poly_chknorm(const poly_D5 *a, sint32 B);
void FsmSw_Dilithium5_poly_uniform(poly_D5 *a, const uint8 seed[SEEDBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium5_poly_uniform_eta(poly_D5 *a, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium5_poly_uniform_gamma1(poly_D5 *a, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium5_poly_challenge(poly_D5 *c, const uint8 seed[SEEDBYTES_DILITHIUM]);

void FsmSw_Dilithium5_polyeta_pack(uint8 *r, const poly_D5 *a);
void FsmSw_Dilithium5_polyeta_unpack(poly_D5 *r, const uint8 *a);

void FsmSw_Dilithium5_polyt1_pack(uint8 *r, const poly_D5 *a);
void FsmSw_Dilithium5_polyt1_unpack(poly_D5 *r, const uint8 *a);

void FsmSw_Dilithium5_polyt0_pack(uint8 *r, const poly_D5 *a);
void FsmSw_Dilithium5_polyt0_unpack(poly_D5 *r, const uint8 *a);

void FsmSw_Dilithium5_polyz_pack(uint8 *r, const poly_D5 *a);
void FsmSw_Dilithium5_polyz_unpack(poly_D5 *r, const uint8 *a);

void FsmSw_Dilithium5_polyw1_pack(uint8 *r, const poly_D5 *a);

#endif /* FSMSW_DILITHIUM5_POLY_H */
