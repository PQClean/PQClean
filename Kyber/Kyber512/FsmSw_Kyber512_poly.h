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
#ifndef FSMSW_KYBER512_POLY_H
#define FSMSW_KYBER512_POLY_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Types.h"
#include "FsmSw_Kyber512_params.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
/* polyspace +3 MISRA2012:D4.8 [Justified:]"Structs are used across multiple files, 
making it impractical and complex to hide the implementation details." */
typedef struct 
{
    sint16 coeffs[KYBER_N];
} poly512;

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_Kyber512_poly_compress(uint8 r[KYBER512_POLYCOMPRESSEDBYTES], const poly512 *a);
void FsmSw_Kyber512_poly_decompress(poly512 *r, const uint8 a[KYBER512_POLYCOMPRESSEDBYTES]);

void FsmSw_Kyber512_poly_tobytes(uint8 r[KYBER_POLYBYTES], const poly512 *a);
void FsmSw_Kyber512_poly_frombytes(poly512 *r, const uint8 a[KYBER_POLYBYTES]);

void FsmSw_Kyber512_poly_frommsg(poly512 *r, const uint8 msg[KYBER512_INDCPA_MSGBYTES]);
void FsmSw_Kyber512_poly_tomsg(uint8 msg[KYBER512_INDCPA_MSGBYTES], const poly512 *a);

void FsmSw_Kyber512_poly_getnoise_eta1(poly512 *r, const uint8 seed[KYBER_SYMBYTES], uint8 nonce);

/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_Kyber512_poly_getnoise_eta2(poly512 *r, const uint8 seed[KYBER_SYMBYTES], uint8 nonce);

void FsmSw_Kyber512_poly_ntt(poly512 *r);
void FsmSw_Kyber512_poly_invntt_tomont(poly512 *r);
void FsmSw_Kyber512_poly_basemul_montgomery(poly512 *r, const poly512 *a, const poly512 *b);
void FsmSw_Kyber512_poly_tomont(poly512 *r);

void FsmSw_Kyber512_poly_reduce(poly512 *r);

void FsmSw_Kyber512_poly_add(poly512 *r, const poly512 *a, const poly512 *b);
void FsmSw_Kyber512_poly_sub(poly512 *r, const poly512 *a, const poly512 *b);

#endif /* FSMSW_KYBER512_POLY_H */
