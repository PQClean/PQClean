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
#include "FsmSw_Dilithium5_params.h"
#include "FsmSw_Types.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct
{
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
void FsmSw_Dilithium5_Poly_Reduce(poly_D5 *a);
void FsmSw_Dilithium5_Poly_CAddQ(poly_D5 *a);

void FsmSw_Dilithium5_Poly_Add(poly_D5 *c, const poly_D5 *a, const poly_D5 *b);
void FsmSw_Dilithium5_Poly_Sub(poly_D5 *c, const poly_D5 *a, const poly_D5 *b);
void FsmSw_Dilithium5_Poly_Shiftl(poly_D5 *a);

void FsmSw_Dilithium5_Poly_Ntt(poly_D5 *a);
void FsmSw_Dilithium5_Poly_InvnttTomont(poly_D5 *a);
void FsmSw_Dilithium5_Poly_PointwiseMontgomery(poly_D5 *c, const poly_D5 *a, const poly_D5 *b);

void FsmSw_Dilithium5_Poly_Power2Round(poly_D5 *a1, poly_D5 *a0, const poly_D5 *a);
void FsmSw_Dilithium5_poly_Decompose(poly_D5 *a1, poly_D5 *a0, const poly_D5 *a);
uint32 FsmSw_Dilithium5_Poly_MakeHint(poly_D5 *h, const poly_D5 *a0, const poly_D5 *a1);
void FsmSw_Dilithium5_Poly_UseHint(poly_D5 *b, const poly_D5 *a, const poly_D5 *h);

sint8 FsmSw_Dilithium5_Poly_Chknorm(const poly_D5 *a, sint32 B);
void FsmSw_Dilithium5_Poly_Uniform(poly_D5 *a, const uint8 seed[SEEDBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium5_Poly_UniformEta(poly_D5 *a, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium5_Poly_UniformGamma1(poly_D5 *a, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium5_Poly_Challenge(poly_D5 *c, const uint8 seed[SEEDBYTES_DILITHIUM]);

void FsmSw_Dilithium5_Polyeta_EtaPack(uint8 *r, const poly_D5 *a);
void FsmSw_Dilithium5_Polyeta_EtaUnpack(poly_D5 *r, const uint8 *a);

void FsmSw_Dilithium5_Poly_T1Pack(uint8 *r, const poly_D5 *a);
void FsmSw_Dilithium5_Poly_T1Unpack(poly_D5 *r, const uint8 *a);

void FsmSw_Dilithium5_Poly_T0Pack(uint8 *r, const poly_D5 *a);
void FsmSw_Dilithium5_Poly_T0Unpack(poly_D5 *r, const uint8 *a);

void FsmSw_Dilithium5_Poly_ZPack(uint8 *r, const poly_D5 *a);
void FsmSw_Dilithium5_Poly_ZUnpack(poly_D5 *r, const uint8 *a);

void FsmSw_Dilithium5_Poly_W1Pack(uint8 *r, const poly_D5 *a);

#endif /* FSMSW_DILITHIUM5_POLY_H */
