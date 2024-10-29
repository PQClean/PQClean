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
#ifndef FSMSW_DILITHIUM5_POLYVEC_H
#define FSMSW_DILITHIUM5_POLYVEC_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Types.h"
#include "FsmSw_Dilithium5_params.h"
#include "FsmSw_Dilithium5_poly.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct {
    poly_D5 vec[L_DILITHIUM5];
} polyvecl_D5;

/* Vectors of polynomials of length K */
typedef struct {
    poly_D5 vec[K_DILITHIUM5];
} polyveck_D5;

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_Dilithium5_polyvecl_uniform_eta(polyvecl_D5 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium5_polyvecl_uniform_gamma1(polyvecl_D5 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium5_polyvecl_reduce(polyvecl_D5 *v);
void FsmSw_Dilithium5_polyvecl_add(polyvecl_D5 *w, const polyvecl_D5 *u, const polyvecl_D5 *v);
void FsmSw_Dilithium5_polyvecl_ntt(polyvecl_D5 *v);
void FsmSw_Dilithium5_polyvecl_invntt_tomont(polyvecl_D5 *v);
void FsmSw_Dilithium5_polyvecl_pointwise_poly_montgomery(polyvecl_D5 *r, const poly_D5 *a, const polyvecl_D5 *v);
sint8 FsmSw_Dilithium5_polyvecl_chknorm(const polyvecl_D5 *v, sint32 bound);
void FsmSw_Dilithium5_polyveck_uniform_eta(polyveck_D5 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium5_polyveck_reduce(polyveck_D5 *v);
void FsmSw_Dilithium5_polyveck_caddq(polyveck_D5 *v);
void FsmSw_Dilithium5_polyveck_add(polyveck_D5 *w, const polyveck_D5 *u, const polyveck_D5 *v);
void FsmSw_Dilithium5_polyveck_sub(polyveck_D5 *w, const polyveck_D5 *u, const polyveck_D5 *v);
void FsmSw_Dilithium5_polyveck_shiftl(polyveck_D5 *v);
void FsmSw_Dilithium5_polyveck_ntt(polyveck_D5 *v);
void FsmSw_Dilithium5_polyveck_invntt_tomont(polyveck_D5 *v);
void FsmSw_Dilithium5_polyveck_pointwise_poly_montgomery(polyveck_D5 *r, const poly_D5 *a, const polyveck_D5 *v);
sint8 FsmSw_Dilithium5_polyveck_chknorm(const polyveck_D5 *v, sint32 bound);
void FsmSw_Dilithium5_polyveck_power2round(polyveck_D5 *v1, polyveck_D5 *v0, const polyveck_D5 *v);
void FsmSw_Dilithium5_polyveck_decompose(polyveck_D5 *v1, polyveck_D5 *v0, const polyveck_D5 *v);
uint32 FsmSw_Dilithium5_polyveck_make_hint(polyveck_D5 *h, const polyveck_D5 *v0, const polyveck_D5 *v1);
void FsmSw_Dilithium5_polyveck_use_hint(polyveck_D5 *w, const polyveck_D5 *u, const polyveck_D5 *h);
void FsmSw_Dilithium5_polyveck_pack_w1(uint8 r[K_DILITHIUM5 * POLYW1_PACKEDBYTES_DILITHIUM5], const polyveck_D5 *w1);
void FsmSw_Dilithium5_polyvec_matrix_expand(polyvecl_D5 mat[K_DILITHIUM5], const uint8 rho[SEEDBYTES_DILITHIUM]);
void FsmSw_Dilithium5_polyvec_matrix_pointwise_montgomery(polyveck_D5 *t, const polyvecl_D5 mat[K_DILITHIUM5],
                                                          const polyvecl_D5 *v);

#endif /* FSMSW_DILITHIUM5_POLYVEC_H */
