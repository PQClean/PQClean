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
#include "FsmSw_Dilithium2_params.h"
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
void FsmSw_Dilithium2_Poly_Reduce(poly_D2 *a);
void FsmSw_Dilithium2_Poly_CAddQ(poly_D2 *a);

void FsmSw_Dilithium2_Poly_Add(poly_D2 *c, const poly_D2 *a, const poly_D2 *b);
void FsmSw_Dilithium2_Poly_Sub(poly_D2 *c, const poly_D2 *a, const poly_D2 *b);
void FsmSw_Dilithium2_Poly_Shiftl(poly_D2 *a);

void FsmSw_Dilithium2_Poly_Ntt(poly_D2 *a);
void FsmSw_Dilithium2_Poly_InvnttTomont(poly_D2 *a);
void FsmSw_Dilithium2_Poly_PointwiseMontgomery(poly_D2 *c, const poly_D2 *a, const poly_D2 *b);

void FsmSw_Dilithium2_Poly_Power2Round(poly_D2 *a1, poly_D2 *a0, const poly_D2 *a);
void FsmSw_Dilithium2_Poly_Decompose(poly_D2 *a1, poly_D2 *a0, const poly_D2 *a);
uint32 FsmSw_Dilithium2_Poly_MakeHint(poly_D2 *h, const poly_D2 *a0, const poly_D2 *a1);
void FsmSw_Dilithium2_Poly_UseHint(poly_D2 *b, const poly_D2 *a, const poly_D2 *h);

sint8 FsmSw_Dilithium2_Poly_Chknorm(const poly_D2 *a, sint32 B);
void FsmSw_Dilithium2_Poly_Uniform(poly_D2 *a, const uint8 seed[SEEDBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium2_Poly_UniformEta(poly_D2 *a, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium2_Poly_UniformGamma1(poly_D2 *a, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce);
void FsmSw_Dilithium2_Poly_Challenge(poly_D2 *c, const uint8 seed[SEEDBYTES_DILITHIUM]);

void FsmSw_Dilithium2_Polyeta_EtaPack(uint8 *r, const poly_D2 *a);
void FsmSw_Dilithium2_Polyeta_EtaUnpack(poly_D2 *r, const uint8 *a);

void FsmSw_Dilithium2_Poly_T1Pack(uint8 *r, const poly_D2 *a);
void FsmSw_Dilithium2_Poly_T1Unpack(poly_D2 *r, const uint8 *a);

void FsmSw_Dilithium2_Poly_T0Pack(uint8 *r, const poly_D2 *a);
void FsmSw_Dilithium2_Poly_T0Unpack(poly_D2 *r, const uint8 *a);

void FsmSw_Dilithium2_Poly_ZPack(uint8 *r, const poly_D2 *a);
void FsmSw_Dilithium2_Poly_ZUnpack(poly_D2 *r, const uint8 *a);

void FsmSw_Dilithium2_Poly_W1Pack(uint8 *r, const poly_D2 *a);

#endif /* FSMSW_DILITHIUM2_POLY_H */
