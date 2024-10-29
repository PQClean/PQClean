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
#ifndef FSMSW_DILITHIUM2_POLYVEC_H
#define FSMSW_DILITHIUM2_POLYVEC_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Types.h"
#include "FsmSw_Dilithium2_params.h"
#include "FsmSw_Dilithium2_poly.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct {
    poly_D2 vec[L_DILITHIUM2];
} polyvecl_D2;

/* Vectors of polynomials of length K */
typedef struct {
    poly_D2 vec[K_DILITHIUM2];
} polyveck_D2;

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_Dilithium2_polyvecl_uniform_eta(polyvecl_D2 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium2_polyvecl_uniform_gamma1(polyvecl_D2 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium2_polyvecl_reduce(polyvecl_D2 *v);
void FsmSw_Dilithium2_polyvecl_add(polyvecl_D2 *w, const polyvecl_D2 *u, const polyvecl_D2 *v);
void FsmSw_Dilithium2_polyvecl_ntt(polyvecl_D2 *v);
void FsmSw_Dilithium2_polyvecl_invntt_tomont(polyvecl_D2 *v);
void FsmSw_Dilithium2_polyvecl_pointwise_poly_montgomery(polyvecl_D2 *r, const poly_D2 *a, const polyvecl_D2 *v);
sint8 FsmSw_Dilithium2_polyvecl_chknorm(const polyvecl_D2 *v, sint32 bound);
void FsmSw_Dilithium2_polyveck_uniform_eta(polyveck_D2 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium2_polyveck_reduce(polyveck_D2 *v);
void FsmSw_Dilithium2_polyveck_caddq(polyveck_D2 *v);
void FsmSw_Dilithium2_polyveck_add(polyveck_D2 *w, const polyveck_D2 *u, const polyveck_D2 *v);
void FsmSw_Dilithium2_polyveck_sub(polyveck_D2 *w, const polyveck_D2 *u, const polyveck_D2 *v);
void FsmSw_Dilithium2_polyveck_shiftl(polyveck_D2 *v);
void FsmSw_Dilithium2_polyveck_ntt(polyveck_D2 *v);
void FsmSw_Dilithium2_polyveck_invntt_tomont(polyveck_D2 *v);
void FsmSw_Dilithium2_polyveck_pointwise_poly_montgomery(polyveck_D2 *r, const poly_D2 *a, const polyveck_D2 *v);
sint8 FsmSw_Dilithium2_polyveck_chknorm(const polyveck_D2 *v, sint32 bound);
void FsmSw_Dilithium2_polyveck_power2round(polyveck_D2 *v1, polyveck_D2 *v0, const polyveck_D2 *v);
void FsmSw_Dilithium2_polyveck_decompose(polyveck_D2 *v1, polyveck_D2 *v0, const polyveck_D2 *v);
uint32 FsmSw_Dilithium2_polyveck_make_hint(polyveck_D2 *h, const polyveck_D2 *v0, const polyveck_D2 *v1);
void FsmSw_Dilithium2_polyveck_use_hint(polyveck_D2 *w, const polyveck_D2 *u, const polyveck_D2 *h);
void FsmSw_Dilithium2_polyveck_pack_w1(uint8 r[K_DILITHIUM2 * POLYW1_PACKEDBYTES_DILITHIUM2], const polyveck_D2 *w1);
void FsmSw_Dilithium2_polyvec_matrix_expand(polyvecl_D2 mat[K_DILITHIUM2], const uint8 rho[SEEDBYTES_DILITHIUM]);
void FsmSw_Dilithium2_polyvec_matrix_pointwise_montgomery(polyveck_D2 *t, const polyvecl_D2 mat[K_DILITHIUM2],
                                                          const polyvecl_D2 *v);

#endif /* FSMSW_DILITHIUM2_POLYVEC_H */
