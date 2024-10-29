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
#ifndef FSMSW_DILITHIUM3_POLYVEC_H
#define FSMSW_DILITHIUM3_POLYVEC_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Types.h"
#include "FsmSw_Dilithium3_params.h"
#include "FsmSw_Dilithium3_poly.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct {
    poly_D3 vec[L_DILITHIUM3];
} polyvecl_D3;

/* Vectors of polynomials of length K */
typedef struct {
    poly_D3 vec[K_DILITHIUM3];
} polyveck_D3;

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_Dilithium3_polyvecl_uniform_eta(polyvecl_D3 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium3_polyvecl_uniform_gamma1(polyvecl_D3 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium3_polyvecl_reduce(polyvecl_D3 *v);
void FsmSw_Dilithium3_polyvecl_add(polyvecl_D3 *w, const polyvecl_D3 *u, const polyvecl_D3 *v);
void FsmSw_Dilithium3_polyvecl_ntt(polyvecl_D3 *v);
void FsmSw_Dilithium3_polyvecl_invntt_tomont(polyvecl_D3 *v);
void FsmSw_Dilithium3_polyvecl_pointwise_poly_montgomery(polyvecl_D3 *r, const poly_D3 *a, const polyvecl_D3 *v);
sint8 FsmSw_Dilithium3_polyvecl_chknorm(const polyvecl_D3 *v, sint32 bound);
void FsmSw_Dilithium3_polyveck_uniform_eta(polyveck_D3 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium3_polyveck_reduce(polyveck_D3 *v);
void FsmSw_Dilithium3_polyveck_caddq(polyveck_D3 *v);
void FsmSw_Dilithium3_polyveck_add(polyveck_D3 *w, const polyveck_D3 *u, const polyveck_D3 *v);
void FsmSw_Dilithium3_polyveck_sub(polyveck_D3 *w, const polyveck_D3 *u, const polyveck_D3 *v);
void FsmSw_Dilithium3_polyveck_shiftl(polyveck_D3 *v);
void FsmSw_Dilithium3_polyveck_ntt(polyveck_D3 *v);
void FsmSw_Dilithium3_polyveck_invntt_tomont(polyveck_D3 *v);
void FsmSw_Dilithium3_polyveck_pointwise_poly_montgomery(polyveck_D3 *r, const poly_D3 *a, const polyveck_D3 *v);
sint8 FsmSw_Dilithium3_polyveck_chknorm(const polyveck_D3 *v, sint32 bound);
void FsmSw_Dilithium3_polyveck_power2round(polyveck_D3 *v1, polyveck_D3 *v0, const polyveck_D3 *v);
void FsmSw_Dilithium3_polyveck_decompose(polyveck_D3 *v1, polyveck_D3 *v0, const polyveck_D3 *v);
uint32 FsmSw_Dilithium3_polyveck_make_hint(polyveck_D3 *h, const polyveck_D3 *v0, const polyveck_D3 *v1);
void FsmSw_Dilithium3_polyveck_use_hint(polyveck_D3 *w, const polyveck_D3 *u, const polyveck_D3 *h);
void FsmSw_Dilithium3_polyveck_pack_w1(uint8 r[K_DILITHIUM3 * POLYW1_PACKEDBYTES_DILITHIUM3], const polyveck_D3 *w1);
void FsmSw_Dilithium3_polyvec_matrix_expand(polyvecl_D3 mat[K_DILITHIUM3], const uint8 rho[SEEDBYTES_DILITHIUM]);
void FsmSw_Dilithium3_polyvec_matrix_pointwise_montgomery(polyveck_D3 *t, const polyvecl_D3 mat[K_DILITHIUM3],
                                                          const polyvecl_D3 *v);

#endif /* FSMSW_DILITHIUM3_POLYVEC_H */
