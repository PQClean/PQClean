/***********************************************************************************************************************
*
*                                          IAV GmbH
*                          All rights reserved - Alle Rechte vorbehalten
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
#ifndef FSMSW_KYBER1024_POLY_H
#define FSMSW_KYBER1024_POLY_H

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
#include "FsmSw_Kyber1024_params.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct {
    sint16 coeffs[KYBER_N];
} poly1024;

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES                                                                                         */
/**********************************************************************************************************************/
void FsmSw_Kyber1024_poly_compress(uint8 r[KYBER1024_POLYCOMPRESSEDBYTES], const poly1024 *a);
void FsmSw_Kyber1024_poly_decompress(poly1024 *r, const uint8 a[KYBER1024_POLYCOMPRESSEDBYTES]);

void FsmSw_Kyber1024_poly_tobytes(uint8 r[KYBER_POLYBYTES], const poly1024 *a);
void FsmSw_Kyber1024_poly_frombytes(poly1024 *r, const uint8 a[KYBER_POLYBYTES]);

void FsmSw_Kyber1024_poly_frommsg(poly1024 *r, const uint8 msg[KYBER1024_INDCPA_MSGBYTES]);
void FsmSw_Kyber1024_poly_tomsg(uint8 msg[KYBER1024_INDCPA_MSGBYTES], const poly1024 *a);

void FsmSw_Kyber1024_poly_getnoise_eta1(poly1024 *r, const uint8 seed[KYBER_SYMBYTES], uint8 nonce);
void FsmSw_Kyber1024_poly_getnoise_eta2(poly1024 *r, const uint8 seed[KYBER_SYMBYTES], uint8 nonce);

void FsmSw_Kyber1024_poly_ntt(poly1024 *r);
void FsmSw_Kyber1024_poly_invntt_tomont(poly1024 *r);
void FsmSw_Kyber1024_poly_basemul_montgomery(poly1024 *r, const poly1024 *a, const poly1024 *b);
void FsmSw_Kyber1024_poly_tomont(poly1024 *r);

void FsmSw_Kyber1024_poly_reduce(poly1024 *r);

void FsmSw_Kyber1024_poly_add(poly1024 *r, const poly1024 *a, const poly1024 *b);
void FsmSw_Kyber1024_poly_sub(poly1024 *r, const poly1024 *a, const poly1024 *b);

#endif /* FSMSW_KYBER1024_POLY_H */
