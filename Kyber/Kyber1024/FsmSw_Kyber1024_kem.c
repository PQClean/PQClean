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
#include "Platform_Types.h"
#include "FsmSw_CommonLib.h"
#include "FsmSw_Kyber_symmetric.h"
#include "FsmSw_Kyber_verify.h"
#include "FsmSw_Kyber1024_params.h"
#include "FsmSw_Kyber1024_indcpa.h"
#include "FsmSw_Kyber1024_kem.h"

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
* Name:        FsmSw_Kyber1024_crypto_kem_keypair
*
* Description: Generates public and private key
*              for CCA-secure Kyber key encapsulation mechanism
*
* Arguments:   - uint8 *pk: pointer to output public key (of length KYBER1024_PUBLICKEYBYTES bytes)
*              - uint8 *sk: pointer to output private key (of length KYBER1024_SECRETKEYBYTES bytes)
*
* Returns 0 (success)
***********************************************************************************************************************/
uint8 FsmSw_Kyber1024_crypto_kem_keypair(uint8 *pk, uint8 *sk)
{
    uint32 i;

    FsmSw_Kyber1024_indcpa_keypair(pk, sk);

    for (i = 0; i < KYBER1024_INDCPA_PUBLICKEYBYTES; i++)
    {
        sk[i + KYBER1024_INDCPA_SECRETKEYBYTES] = pk[i];
    }

    hash_h(&sk[KYBER1024_SECRETKEYBYTES - (2u * KYBER_SYMBYTES)], pk, KYBER1024_PUBLICKEYBYTES);

    /* Value z for pseudo-random output on reject */
    (void)FsmSw_CommonLib_randombytes(&sk[KYBER1024_SECRETKEYBYTES - KYBER_SYMBYTES], KYBER_SYMBYTES);

    return 0;
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_crypto_kem_enc
*
* Description: Generates cipher text and shared
*              secret for given public key
*
* Arguments:   - uint8       *ct: pointer to output cipher text (of length KYBER1024_CIPHERTEXTBYTES bytes)
*              - uint8       *ss: pointer to output shared secret (of length KYBER_SSBYTES bytes)
*              - const uint8 *pk: pointer to input public key (of length KYBER1024_PUBLICKEYBYTES bytes)
*
* Returns 0 (success)
***********************************************************************************************************************/
uint8 FsmSw_Kyber1024_crypto_kem_enc(uint8 *ct, uint8 *ss, const uint8 *pk)
{
    uint8 buf[2u * KYBER_SYMBYTES];
    /* Will contain key, coins */
    uint8 kr[2u * KYBER_SYMBYTES];

    (void)FsmSw_CommonLib_randombytes(buf, KYBER_SYMBYTES);
    /* Don't release system RNG output */
    hash_h(buf, buf, KYBER_SYMBYTES);

    /* Multitarget countermeasure for coins + contributory KEM */
    hash_h(&buf[KYBER_SYMBYTES], pk, KYBER1024_PUBLICKEYBYTES);
    hash_g(kr, buf, 2u * KYBER_SYMBYTES);

    /* coins are in kr+KYBER_SYMBYTES */
    FsmSw_Kyber1024_indcpa_enc(ct, buf, pk, &kr[KYBER_SYMBYTES]);

    /* overwrite coins in kr with H(c) */
    hash_h(&kr[KYBER_SYMBYTES], ct, KYBER1024_CIPHERTEXTBYTES);
    /* hash concatenation of pre-k and H(c) to k */
    kdf(ss, kr, 2u * KYBER_SYMBYTES);

    return 0;
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_crypto_kem_dec
*
* Description: Generates shared secret for given
*              cipher text and private key
*
* Arguments:   - uint8       *ss: pointer to output shared secret (of length KYBER_SSBYTES bytes)
*              - const uint8 *ct: pointer to input cipher text (of length KYBER1024_CIPHERTEXTBYTES bytes)
*              - const uint8 *sk: pointer to input private key (of length KYBER1024_SECRETKEYBYTES bytes)
*
* Returns 0.
*
* On failure, ss will contain a pseudo-random value.
***********************************************************************************************************************/
uint8 FsmSw_Kyber1024_crypto_kem_dec(uint8 *ss, const uint8 *ct, const uint8 *sk)
{
    uint32 i;
    uint8 fail;
    uint8 buf[2u * KYBER_SYMBYTES];
    /* Will contain key, coins */
    uint8 kr[2u * KYBER_SYMBYTES];
    uint8 cmp[KYBER1024_CIPHERTEXTBYTES];
    const uint8 *pk = &sk[KYBER1024_INDCPA_SECRETKEYBYTES];

    FsmSw_Kyber1024_indcpa_dec(buf, ct, sk);

    /* Multitarget countermeasure for coins + contributory KEM */
    for (i = 0; i < KYBER_SYMBYTES; i++)
    {
        buf[KYBER_SYMBYTES + i] = sk[KYBER1024_SECRETKEYBYTES - 2u * KYBER_SYMBYTES + i];
    }

    hash_g(kr, buf, 2u * KYBER_SYMBYTES);

    /* coins are in kr+KYBER_SYMBYTES */
    FsmSw_Kyber1024_indcpa_enc(cmp, buf, pk, &kr[KYBER_SYMBYTES]);

    fail = FsmSw_Kyber_verify(ct, cmp, KYBER1024_CIPHERTEXTBYTES);

    /* overwrite coins in kr with H(c) */
    hash_h(&kr[KYBER_SYMBYTES], ct, KYBER1024_CIPHERTEXTBYTES);

    /* Overwrite pre-k with z on re-encryption failure */
    FsmSw_Kyber_cmov(kr, &sk[KYBER1024_SECRETKEYBYTES - KYBER_SYMBYTES], KYBER_SYMBYTES, fail);

    /* hash concatenation of pre-k and H(c) to k */
    kdf(ss, kr, 2u * KYBER_SYMBYTES);

    return 0;
}
