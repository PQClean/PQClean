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
#include "FsmSw_Dilithium5_params.h"
#include "FsmSw_Dilithium5_poly.h"
#include "FsmSw_Types.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct
{
  poly_D5 vec[L_DILITHIUM5];
} polyvecl_D5;

/* Vectors of polynomials of length K */
typedef struct
{
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
void FsmSw_Dilithium5_Polyvecl_Uniform_Eta(polyvecl_D5 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_Dilithium5_Polyvecl_Uniform_Gamma1(polyvecl_D5 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium5_Polyvecl_Reduce(polyvecl_D5 *v);
void FsmSw_Dilithium5_Polyvecl_Add(polyvecl_D5 *w, const polyvecl_D5 *u, const polyvecl_D5 *v);
void FsmSw_Dilithium5_Polyvecl_Ntt(polyvecl_D5 *v);
void FsmSw_Dilithium5_Polyvecl_Invntt_Tomont(polyvecl_D5 *v);
void FsmSw_Dilithium5_Polyvecl_Pointwise_Poly_Montgomery(polyvecl_D5 *r, const poly_D5 *a, const polyvecl_D5 *v);
sint8 FsmSw_Dilithium5_Polyvecl_Chknorm(const polyvecl_D5 *v, sint32 bound);
void FsmSw_Dilithium5_Polyveck_Uniform_Eta(polyveck_D5 *v, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium5_Polyveck_Reduce(polyveck_D5 *v);
void FsmSw_Dilithium5_Polyveck_CAddQ(polyveck_D5 *v);
void FsmSw_Dilithium5_Polyveck_Add(polyveck_D5 *w, const polyveck_D5 *u, const polyveck_D5 *v);
void FsmSw_Dilithium5_Polyveck_Sub(polyveck_D5 *w, const polyveck_D5 *u, const polyveck_D5 *v);
void FsmSw_Dilithium5_Polyveck_Shiftl(polyveck_D5 *v);
void FsmSw_Dilithium5_Polyveck_Ntt(polyveck_D5 *v);
void FsmSw_Dilithium5_Polyveck_Invntt_Tomont(polyveck_D5 *v);
void FsmSw_Dilithium5_Polyveck_Pointwise_Poly_Montgomery(polyveck_D5 *r, const poly_D5 *a, const polyveck_D5 *v);
sint8 FsmSw_Dilithium5_Polyveck_Chknorm(const polyveck_D5 *v, sint32 bound);
void FsmSw_Dilithium5_Polyveck_Power2Round(polyveck_D5 *v1, polyveck_D5 *v0, const polyveck_D5 *v);
void FsmSw_Dilithium5_Polyveck_Decompose(polyveck_D5 *v1, polyveck_D5 *v0, const polyveck_D5 *v);
uint32 FsmSw_Dilithium5_Polyveck_Make_Hint(polyveck_D5 *h, const polyveck_D5 *v0, const polyveck_D5 *v1);
void FsmSw_Dilithium5_Polyveck_Use_Hint(polyveck_D5 *w, const polyveck_D5 *u, const polyveck_D5 *h);
void FsmSw_Dilithium5_Polyveck_Pack_W1(uint8 r[K_DILITHIUM5 * POLYW1_PACKEDBYTES_DILITHIUM5], const polyveck_D5 *w1);
void FsmSw_Dilithium5_Polyvec_Matrix_Expand(polyvecl_D5 mat[K_DILITHIUM5], const uint8 rho[SEEDBYTES_DILITHIUM]);
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_Dilithium5_Polyvec_Matrix_Pointwise_Montgomery(polyveck_D5 *t, const polyvecl_D5 mat[K_DILITHIUM5],
                                                          const polyvecl_D5 *v);

#endif /* FSMSW_DILITHIUM5_POLYVEC_H */
