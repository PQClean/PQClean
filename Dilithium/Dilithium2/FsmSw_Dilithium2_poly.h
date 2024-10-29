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
#ifndef FSMSW_DILITHIUM2_POLY_H
#define FSMSW_DILITHIUM2_POLY_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Types.h"
#include "FsmSw_Dilithium2_params.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct {
    sint32 coeffs[N_DILITHIUM];
} poly_D2;

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_Dilithium2_poly_reduce(poly_D2 *a);
void FsmSw_Dilithium2_poly_caddq(poly_D2 *a);

void FsmSw_Dilithium2_poly_add(poly_D2 *c, const poly_D2 *a, const poly_D2 *b);
void FsmSw_Dilithium2_poly_sub(poly_D2 *c, const poly_D2 *a, const poly_D2 *b);
void FsmSw_Dilithium2_poly_shiftl(poly_D2 *a);

void FsmSw_Dilithium2_poly_ntt(poly_D2 *a);
void FsmSw_Dilithium2_poly_invntt_tomont(poly_D2 *a);
void FsmSw_Dilithium2_poly_pointwise_montgomery(poly_D2 *c, const poly_D2 *a, const poly_D2 *b);

void FsmSw_Dilithium2_poly_power2round(poly_D2 *a1, poly_D2 *a0, const poly_D2 *a);
void FsmSw_Dilithium2_poly_decompose(poly_D2 *a1, poly_D2 *a0, const poly_D2 *a);
uint32 FsmSw_Dilithium2_poly_make_hint(poly_D2 *h, const poly_D2 *a0, const poly_D2 *a1);
void FsmSw_Dilithium2_poly_use_hint(poly_D2 *b, const poly_D2 *a, const poly_D2 *h);

sint8 FsmSw_Dilithium2_poly_chknorm(const poly_D2 *a, sint32 B);
void FsmSw_Dilithium2_poly_uniform(poly_D2 *a, const uint8 seed[SEEDBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium2_poly_uniform_eta(poly_D2 *a, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium2_poly_uniform_gamma1(poly_D2 *a, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium2_poly_challenge(poly_D2 *c, const uint8 seed[SEEDBYTES_DILITHIUM]);

void FsmSw_Dilithium2_polyeta_pack(uint8 *r, const poly_D2 *a);
void FsmSw_Dilithium2_polyeta_unpack(poly_D2 *r, const uint8 *a);

void FsmSw_Dilithium2_polyt1_pack(uint8 *r, const poly_D2 *a);
void FsmSw_Dilithium2_polyt1_unpack(poly_D2 *r, const uint8 *a);

void FsmSw_Dilithium2_polyt0_pack(uint8 *r, const poly_D2 *a);
void FsmSw_Dilithium2_polyt0_unpack(poly_D2 *r, const uint8 *a);

void FsmSw_Dilithium2_polyz_pack(uint8 *r, const poly_D2 *a);
void FsmSw_Dilithium2_polyz_unpack(poly_D2 *r, const uint8 *a);

void FsmSw_Dilithium2_polyw1_pack(uint8 *r, const poly_D2 *a);

#endif /* FSMSW_DILITHIUM2_POLY_H */
