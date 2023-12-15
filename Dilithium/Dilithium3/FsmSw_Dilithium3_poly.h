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
#ifndef FSMSW_DILITHIUM3_POLY_H
#define FSMSW_DILITHIUM3_POLY_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
#include "FsmSw_Dilithium3_params.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct {
    sint32 coeffs[N_DILITHIUM];
} poly_D3;

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_Dilithium3_poly_reduce(poly_D3 *a);
void FsmSw_Dilithium3_poly_caddq(poly_D3 *a);

void FsmSw_Dilithium3_poly_add(poly_D3 *c, const poly_D3 *a, const poly_D3 *b);
void FsmSw_Dilithium3_poly_sub(poly_D3 *c, const poly_D3 *a, const poly_D3 *b);
void FsmSw_Dilithium3_poly_shiftl(poly_D3 *a);

void FsmSw_Dilithium3_poly_ntt(poly_D3 *a);
void FsmSw_Dilithium3_poly_invntt_tomont(poly_D3 *a);
void FsmSw_Dilithium3_poly_pointwise_montgomery(poly_D3 *c, const poly_D3 *a, const poly_D3 *b);

void FsmSw_Dilithium3_poly_power2round(poly_D3 *a1, poly_D3 *a0, const poly_D3 *a);
void FsmSw_Dilithium3_poly_decompose(poly_D3 *a1, poly_D3 *a0, const poly_D3 *a);
uint32 FsmSw_Dilithium3_poly_make_hint(poly_D3 *h, const poly_D3 *a0, const poly_D3 *a1);
void FsmSw_Dilithium3_poly_use_hint(poly_D3 *b, const poly_D3 *a, const poly_D3 *h);

sint8 FsmSw_Dilithium3_poly_chknorm(const poly_D3 *a, sint32 B);
void FsmSw_Dilithium3_poly_uniform(poly_D3 *a, const uint8 seed[SEEDBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium3_poly_uniform_eta(poly_D3 *a, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium3_poly_uniform_gamma1(poly_D3 *a, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium3_poly_challenge(poly_D3 *c, const uint8 seed[SEEDBYTES_DILITHIUM]);

void FsmSw_Dilithium3_polyeta_pack(uint8 *r, const poly_D3 *a);
void FsmSw_Dilithium3_polyeta_unpack(poly_D3 *r, const uint8 *a);

void FsmSw_Dilithium3_polyt1_pack(uint8 *r, const poly_D3 *a);
void FsmSw_Dilithium3_polyt1_unpack(poly_D3 *r, const uint8 *a);

void FsmSw_Dilithium3_polyt0_pack(uint8 *r, const poly_D3 *a);
void FsmSw_Dilithium3_polyt0_unpack(poly_D3 *r, const uint8 *a);

void FsmSw_Dilithium3_polyz_pack(uint8 *r, const poly_D3 *a);
void FsmSw_Dilithium3_polyz_unpack(poly_D3 *r, const uint8 *a);

void FsmSw_Dilithium3_polyw1_pack(uint8 *r, const poly_D3 *a);

#endif /* FSMSW_DILITHIUM3_POLY_H */
