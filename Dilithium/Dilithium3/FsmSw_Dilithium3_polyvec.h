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
#include "FsmSw_Dilithium3_params.h"
#include "FsmSw_Dilithium3_poly.h"
#include "FsmSw_Types.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct
{
  poly_D3 vec[L_DILITHIUM3];
} polyvecl_D3;

/* Vectors of polynomials of length K */
typedef struct
{
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
void FsmSw_Dilithium3_Polyvecl_Uniform_Eta(polyvecl_D3 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_Dilithium3_Polyvecl_UniformGamma1(polyvecl_D3 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium3_Polyvecl_Reduce(polyvecl_D3 *v);
void FsmSw_Dilithium3_Polyvecl_Add(polyvecl_D3 *w, const polyvecl_D3 *u, const polyvecl_D3 *v);
void FsmSw_Dilithium3_Polyvecl_Ntt(polyvecl_D3 *v);
void FsmSw_Dilithium3_Polyvecl_InvnttTomont(polyvecl_D3 *v);
void FsmSw_Dilithium3_Polyvecl_PointwisePolyMontgomery(polyvecl_D3 *r, const poly_D3 *a, const polyvecl_D3 *v);
sint8 FsmSw_Dilithium3_Polyvecl_Chknorm(const polyvecl_D3 *v, sint32 bound);
void FsmSw_Dilithium3_Polyveck_UniformEta(polyveck_D3 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium3_Polyveck_Reduce(polyveck_D3 *v);
void FsmSw_Dilithium3_Polyveck_CAddQ(polyveck_D3 *v);
void FsmSw_Dilithium3_Polyveck_Add(polyveck_D3 *w, const polyveck_D3 *u, const polyveck_D3 *v);
void FsmSw_Dilithium3_Polyveck_Sub(polyveck_D3 *w, const polyveck_D3 *u, const polyveck_D3 *v);
void FsmSw_Dilithium3_Polyveck_Shiftl(polyveck_D3 *v);
void FsmSw_Dilithium3_Polyveck_Ntt(polyveck_D3 *v);
void FsmSw_Dilithium3_Polyveck_InvnttTomont(polyveck_D3 *v);
void FsmSw_Dilithium3_Polyveck_PointwisePolyMontgomery(polyveck_D3 *r, const poly_D3 *a, const polyveck_D3 *v);
sint8 FsmSw_Dilithium3_Polyveck_Chknorm(const polyveck_D3 *v, sint32 bound);
void FsmSw_Dilithium3_Polyveck_Power2Round(polyveck_D3 *v1, polyveck_D3 *v0, const polyveck_D3 *v);
void FsmSw_Dilithium3_Polyveck_Decompose(polyveck_D3 *v1, polyveck_D3 *v0, const polyveck_D3 *v);
uint32 FsmSw_Dilithium3_Polyveck_MakeHint(polyveck_D3 *h, const polyveck_D3 *v0, const polyveck_D3 *v1);
void FsmSw_Dilithium3_Polyveck_UseHint(polyveck_D3 *w, const polyveck_D3 *u, const polyveck_D3 *h);
void FsmSw_Dilithium3_Polyveck_PackW1(uint8 r[K_DILITHIUM3 * POLYW1_PACKEDBYTES_DILITHIUM3], const polyveck_D3 *w1);
void FsmSw_Dilithium3_Polyvec_MatrixExpand(polyvecl_D3 mat[K_DILITHIUM3], const uint8 rho[SEEDBYTES_DILITHIUM]);
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_Dilithium3_Polyvec_MatrixPointwiseMontgomery(polyveck_D3 *t, const polyvecl_D3 mat[K_DILITHIUM3],
                                                          const polyvecl_D3 *v);

#endif /* FSMSW_DILITHIUM3_POLYVEC_H */
