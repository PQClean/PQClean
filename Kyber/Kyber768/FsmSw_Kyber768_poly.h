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
#ifndef FSMSW_KYBER768_POLY_H
#define FSMSW_KYBER768_POLY_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_Types.h"
#include "FsmSw_Kyber768_params.h"

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
} poly768;

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_Kyber768_poly_compress(uint8 r[KYBER768_POLYCOMPRESSEDBYTES], const poly768 *a);
void FsmSw_Kyber768_poly_decompress(poly768 *r, const uint8 a[KYBER768_POLYCOMPRESSEDBYTES]);

void FsmSw_Kyber768_poly_tobytes(uint8 r[KYBER_POLYBYTES], const poly768 *a);
void FsmSw_Kyber768_poly_frombytes(poly768 *r, const uint8 a[KYBER_POLYBYTES]);

void FsmSw_Kyber768_poly_frommsg(poly768 *r, const uint8 msg[KYBER768_INDCPA_MSGBYTES]);
void FsmSw_Kyber768_poly_tomsg(uint8 msg[KYBER768_INDCPA_MSGBYTES], const poly768 *a);

void FsmSw_Kyber768_poly_getnoise_eta1(poly768 *r, const uint8 seed[KYBER_SYMBYTES], uint8 nonce);

void FsmSw_Kyber768_poly_getnoise_eta2(poly768 *r, const uint8 seed[KYBER_SYMBYTES], uint8 nonce);

void FsmSw_Kyber768_poly_ntt(poly768 *r);
void FsmSw_Kyber768_poly_invntt_tomont(poly768 *r);
void FsmSw_Kyber768_poly_basemul_montgomery(poly768 *r, const poly768 *a, const poly768 *b);
void FsmSw_Kyber768_poly_tomont(poly768 *r);

void FsmSw_Kyber768_poly_reduce(poly768 *r);

void FsmSw_Kyber768_poly_add(poly768 *r, const poly768 *a, const poly768 *b);
void FsmSw_Kyber768_poly_sub(poly768 *r, const poly768 *a, const poly768 *b);

#endif /* FSMSW_KYBER768_POLY_H */
