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

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_CommonLib.h"
#include "FsmSw_Kyber512_indcpa.h"
#include "FsmSw_Kyber512_params.h"
#include "FsmSw_Kyber_symmetric.h"
#include "FsmSw_Kyber_verify.h"
#include "FsmSw_Types.h"

#include "FsmSw_Kyber512_kem.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        FsmSw_Kyber512_Crypto_Kem_KeyPair
*
* Description: Generates public and private key
*              for CCA-secure Kyber key encapsulation mechanism
*
* Arguments:   - uint8 *pk: pointer to output public key (of length KYBER512_PUBLICKEYBYTES bytes)
*              - uint8 *sk: pointer to output private key (of length KYBER512_SECRETKEYBYTES bytes)
*
* Returns 0 (success)
***********************************************************************************************************************/
uint8 FsmSw_Kyber512_Crypto_Kem_KeyPair(uint8 *pk, uint8 *sk)
{
  uint32 i = 0;

  FsmSw_Kyber512_Indcpa_KeyPair(pk, sk);

  for (i = 0; i < KYBER512_INDCPA_PUBLICKEYBYTES; i++)
  {
    sk[i + KYBER512_INDCPA_SECRETKEYBYTES] = pk[i];
  }

  FsmSw_Fips202_Sha3_256(&sk[KYBER512_SECRETKEYBYTES - (2u * KYBER_SYMBYTES)], pk, KYBER512_PUBLICKEYBYTES);
  /* Value z for pseudo-random output on reject */
  (void)FsmSw_CommonLib_RandomBytes(&sk[KYBER512_SECRETKEYBYTES - KYBER_SYMBYTES], KYBER_SYMBYTES);

  return 0;
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber512_Crypto_Kem_Enc
*
* Description: Generates cipher text and shared
*              secret for given public key
*
* Arguments:   -       uint8 *ct: pointer to output cipher text (of length KYBER512_CIPHERTEXTBYTES bytes)
*              -       uint8 *ss: pointer to output shared secret (of length KYBER_SSBYTES bytes)
*              - const uint8 *pk: pointer to input public key (of length KYBER512_PUBLICKEYBYTES bytes)
* Returns 0 (success)
***********************************************************************************************************************/
uint8 FsmSw_Kyber512_Crypto_Kem_Enc(uint8 *ct, uint8 *ss, const uint8 *pk)
{
  uint8 buf[2u * KYBER_SYMBYTES] = {0};
  /* Will contain key, coins */
  uint8 kr[2u * KYBER_SYMBYTES] = {0};

  (void)FsmSw_CommonLib_RandomBytes(buf, KYBER_SYMBYTES);
  /* Don't release system RNG output */
  FsmSw_Fips202_Sha3_256(buf, buf, KYBER_SYMBYTES);

  /* Multitarget countermeasure for coins + contributory KEM */
  FsmSw_Fips202_Sha3_256(&buf[KYBER_SYMBYTES], pk, KYBER512_PUBLICKEYBYTES);
  FsmSw_Fips202_Sha3_512(kr, buf, 2u * KYBER_SYMBYTES);

  /* coins are in kr[KYBER_SYMBYTES] */
  FsmSw_Kyber512_Indcpa_Enc(ct, buf, pk, &kr[KYBER_SYMBYTES]);

  /* overwrite coins in kr with H(c) */
  FsmSw_Fips202_Sha3_256(&kr[KYBER_SYMBYTES], ct, KYBER512_CIPHERTEXTBYTES);
  /* hash concatenation of pre-k and H(c) to k */
  FsmSw_Fips202_Shake256(ss, KYBER_SSBYTES, kr, 2u * KYBER_SYMBYTES);

  return 0;
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber512_Crypto_Kem_Dec
*
* Description: Generates shared secret for given
*              cipher text and private key
*
* Arguments:   -       uint8 *ss: pointer to output shared secret (of length KYBER_SSBYTES bytes)
*              - const uint8 *ct: pointer to input cipher text (of length KYBER512_CIPHERTEXTBYTES bytes)
*              - const uint8 *sk: pointer to input private key (of length KYBER512_SECRETKEYBYTES bytes)
*
* Returns 0.
*
* On failure, ss will contain a pseudo-random value.
***********************************************************************************************************************/
uint8 FsmSw_Kyber512_Crypto_Kem_Dec(uint8 *ss, const uint8 *ct, const uint8 *sk)
{
  uint32 i                       = 0;
  uint8 fail                     = 0;
  uint8 buf[2u * KYBER_SYMBYTES] = {0};
  /* Will contain key, coins */
  uint8 kr[2u * KYBER_SYMBYTES]       = {0};
  uint8 cmp[KYBER512_CIPHERTEXTBYTES] = {0};
  const uint8 *pk                     = &sk[KYBER512_INDCPA_SECRETKEYBYTES];

  FsmSw_Kyber512_Indcpa_Dec(buf, ct, sk);

  /* Multitarget countermeasure for coins + contributory KEM */
  for (i = 0; i < KYBER_SYMBYTES; i++)
  {
    buf[KYBER_SYMBYTES + i] = sk[KYBER512_SECRETKEYBYTES - (2u * KYBER_SYMBYTES) + i];
  }

  FsmSw_Fips202_Sha3_512(kr, buf, 2u * KYBER_SYMBYTES);

  /* coins are in kr+KYBER_SYMBYTES */
  FsmSw_Kyber512_Indcpa_Enc(cmp, buf, pk, &kr[KYBER_SYMBYTES]);

  fail = FsmSw_Kyber_Verify(ct, cmp, KYBER512_CIPHERTEXTBYTES);

  /* overwrite coins in kr with H(c) */
  FsmSw_Fips202_Sha3_256(&kr[KYBER_SYMBYTES], ct, KYBER512_CIPHERTEXTBYTES);

  /* Overwrite pre-k with z on re-encryption failure */
  FsmSw_Kyber_Cmov(kr, &sk[KYBER512_SECRETKEYBYTES - KYBER_SYMBYTES], KYBER_SYMBYTES, fail);

  /* hash concatenation of pre-k and H(c) to k */
  FsmSw_Fips202_Shake256(ss, KYBER_SSBYTES, kr, 2u * KYBER_SYMBYTES);

  return 0;
}
