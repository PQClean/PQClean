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
#include "FsmSw_Dilithium2_params.h"
#include "FsmSw_Dilithium2_poly.h"
#include "FsmSw_Types.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct
{
  poly_D2 vec[L_DILITHIUM2];
} polyvecl_D2;

/* Vectors of polynomials of length K */
typedef struct
{
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
void FsmSw_Dilithium2_Polyvecl_UniformEta(polyvecl_D2 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_Dilithium2_Polyvecl_UniformGamma1(polyvecl_D2 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium2_Polyvecl_Reduce(polyvecl_D2 *v);
void FsmSw_Dilithium2_Polyvecl_Add(polyvecl_D2 *w, const polyvecl_D2 *u, const polyvecl_D2 *v);
void FsmSw_Dilithium2_Polyvecl_Ntt(polyvecl_D2 *v);
void FsmSw_Dilithium2_Polyvecl_InvnttTomont(polyvecl_D2 *v);
void FsmSw_Dilithium2_Polyvecl_PointwisePolyMontgomery(polyvecl_D2 *r, const poly_D2 *a, const polyvecl_D2 *v);
sint8 FsmSw_Dilithium2_Polyvecl_Chknorm(const polyvecl_D2 *v, sint32 bound);
void FsmSw_Dilithium2_Polyveck_UniformEta(polyveck_D2 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium2_Polyveck_Reduce(polyveck_D2 *v);
void FsmSw_Dilithium2_Polyveck_CAddQ(polyveck_D2 *v);
void FsmSw_Dilithium2_Polyveck_Add(polyveck_D2 *w, const polyveck_D2 *u, const polyveck_D2 *v);
void FsmSw_Dilithium2_Polyveck_Sub(polyveck_D2 *w, const polyveck_D2 *u, const polyveck_D2 *v);
void FsmSw_Dilithium2_Polyveck_Shiftl(polyveck_D2 *v);
void FsmSw_Dilithium2_Polyveck_Ntt(polyveck_D2 *v);
void FsmSw_Dilithium2_Polyveck_InvnttTomont(polyveck_D2 *v);
void FsmSw_Dilithium2_Polyveck_PointwisePolyMontgomery(polyveck_D2 *r, const poly_D2 *a, const polyveck_D2 *v);
sint8 FsmSw_Dilithium2_Polyveck_Chknorm(const polyveck_D2 *v, sint32 bound);
void FsmSw_Dilithium2_Polyveck_Power2Round(polyveck_D2 *v1, polyveck_D2 *v0, const polyveck_D2 *v);
void FsmSw_Dilithium2_Polyveck_Decompose(polyveck_D2 *v1, polyveck_D2 *v0, const polyveck_D2 *v);
uint32 FsmSw_Dilithium2_Polyveck_MakeHint(polyveck_D2 *h, const polyveck_D2 *v0, const polyveck_D2 *v1);
void FsmSw_Dilithium2_Polyveck_UseHint(polyveck_D2 *w, const polyveck_D2 *u, const polyveck_D2 *h);
void FsmSw_Dilithium2_Polyveck_PackW1(uint8 r[K_DILITHIUM2 * POLYW1_PACKEDBYTES_DILITHIUM2], const polyveck_D2 *w1);
void FsmSw_Dilithium2_Polyvec_MatrixExpand(polyvecl_D2 mat[K_DILITHIUM2], const uint8 rho[SEEDBYTES_DILITHIUM]);
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_Dilithium2_Polyvec_MatrixPointwiseMontgomery(polyveck_D2 *t, const polyvecl_D2 mat[K_DILITHIUM2],
                                                          const polyvecl_D2 *v);

#endif /* FSMSW_DILITHIUM2_POLYVEC_H */
