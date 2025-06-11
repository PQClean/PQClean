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
#include "FsmSw_Kyber768_cbd.h"
#include "FsmSw_Kyber768_params.h"
#include "FsmSw_Kyber_ntt.h"
#include "FsmSw_Kyber_poly.h"
#include "FsmSw_Kyber_reduce.h"
#include "FsmSw_Kyber_symmetric.h"
#include "FsmSw_Types.h"

#include "FsmSw_Kyber768_poly.h"
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
* Name:        FsmSw_Kyber768_Poly_Compress
*
* Description: Compression and subsequent serialization of a polynomial
*
* Arguments:   -       uint8 *r: pointer to output byte array (of length KYBER768_POLYCOMPRESSEDBYTES bytes)
*              - const poly  *a: pointer to input polynomial
***********************************************************************************************************************/
void FsmSw_Kyber768_Poly_Compress(uint8 r[KYBER768_POLYCOMPRESSEDBYTES], const poly *a)
{
  uint16 i   = 0;
  sint16 u   = 0;
  uint8 j    = 0;
  uint8 t[8] = {0};

  /* r_temp is used to avoid modifying the input. */
  uint8 *r_temp = r;

  for (i = 0; i < (KYBER_N / 8u); i++)
  {
    for (j = 0; j < 8u; j++)
    {
      /* map to positive standard representatives */
      u    = a->coeffs[(8u * i) + j];
      u    = (sint16)(u + (sint16)((uint16)(((uint32)u >> 15u) & KYBER_Q)));
      t[j] = (uint8)(((((uint32)u << 4u) + KYBER_Q / 2u) / KYBER_Q) & 15u);
    }

    r_temp[0] = t[0] | (t[1] << 4);
    r_temp[1] = t[2] | (t[3] << 4);
    r_temp[2] = t[4] | (t[5] << 4);
    r_temp[3] = t[6] | (t[7] << 4);
    r_temp    = &(r_temp[4]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber768_Poly_Decompress
*
* Description: De-serialization and subsequent decompression of a polynomial;
*              approximate inverse of FsmSw_Kyber768_Poly_Compress
*
* Arguments:   -       poly *r: pointer to output polynomial
*              - const uint8   *a: pointer to input byte array (of length KYBER768_POLYCOMPRESSEDBYTES bytes)
***********************************************************************************************************************/
void FsmSw_Kyber768_Poly_Decompress(poly *r, const uint8 a[KYBER768_POLYCOMPRESSEDBYTES])
{
  uint16 i     = 0;
  uint16 temp1 = 0;
  uint8 temp2  = 0;

  /* a_temp is used to avoid modifying the input. */
  const uint8 *a_temp = a;

  for (i = 0; i < (KYBER_N / 2u); i++)
  {
    temp1             = (uint16)a_temp[0] & 15u;
    r->coeffs[2u * i] = (sint16)(uint16)((((uint32)temp1 * KYBER_Q) + 8u) >> 4u);

    temp2                    = a_temp[0] >> 4u;
    r->coeffs[(2u * i) + 1u] = (sint16)(uint16)((((uint32)temp2 * KYBER_Q) + 8u) >> 4u);

    a_temp = &(a_temp[1]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber768_Poly_FromMsg
*
* Description: Convert 32-byte message to polynomial
*
* Arguments:   -       poly *r:   pointer to output polynomial
*              - const uint8   *msg: pointer to input message
***********************************************************************************************************************/
void FsmSw_Kyber768_Poly_FromMsg(poly *r, const uint8 msg[KYBER768_INDCPA_MSGBYTES])
{
  uint8 j     = 0;
  uint16 i    = 0;
  sint16 mask = 0;

  for (i = 0; i < (KYBER_N / 8u); i++)
  {
    for (j = 0; j < 8u; j++)
    {
      mask                    = -(sint16)((uint16)(((uint32)msg[i] >> j) & 1u));
      r->coeffs[(8u * i) + j] = (sint16)((uint16)((uint16)mask & ((uint16)((KYBER_Q + 1u) / 2u))));
    }
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber768_Poly_ToMsg
*
* Description: Convert polynomial to 32-byte message
*
* Arguments:   -       uint8   *msg: pointer to output message
*              - const poly *a:   pointer to input polynomial
***********************************************************************************************************************/
void FsmSw_Kyber768_Poly_ToMsg(uint8 msg[KYBER768_INDCPA_MSGBYTES], const poly *a)
{
  uint8 j  = 0;
  uint16 i = 0;
  uint16 t = 0;

  for (i = 0; i < (KYBER_N / 8u); i++)
  {
    msg[i] = 0;
    for (j = 0; j < 8u; j++)
    {
      t = (uint16)(a->coeffs[(8u * i) + j]);
      /* Shift to get the first bit */
      if ((t >> 15u) != 0u)
      {
        t = t + KYBER_Q;
      }

      t      = (((t << 1u) + (KYBER_Q / 2u)) / KYBER_Q) & 1u;
      msg[i] = (uint8)((uint16)msg[i] | (t << j));
    }
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber768_Poly_GetNoiseEta1
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER768_ETA1
*
* Arguments:   -       poly *r:     pointer to output polynomial
*              - const uint8   *seed:  pointer to input seed (of length KYBER_SYMBYTES bytes)
*              -       uint8    nonce: one-byte input nonce
***********************************************************************************************************************/
void FsmSw_Kyber768_Poly_GetNoiseEta1(poly *r, const uint8 seed[KYBER_SYMBYTES], uint8 nonce)
{
  uint8 buf[KYBER768_ETA1 * KYBER_N / 4u] = {0};

  FsmSw_Kyber_Shake256_Prf(buf, sizeof(buf), seed, nonce);
  FsmSw_Kyber768_Poly_Cbd_Eta1(r, buf);
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber768_Poly_GetNoiseEta2
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER768_ETA2
*
* Arguments:   -       poly *r:     pointer to output polynomial
*              - const uint8   *seed:  pointer to input seed (of length KYBER_SYMBYTES bytes)
*              -       uint8    nonce: one-byte input nonce
***********************************************************************************************************************/
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_Kyber768_Poly_GetNoiseEta2(poly *r, const uint8 seed[KYBER_SYMBYTES], uint8 nonce)
{
  uint8 buf[KYBER768_ETA2 * KYBER_N / 4u] = {0};

  FsmSw_Kyber_Shake256_Prf(buf, sizeof(buf), seed, nonce);
  FsmSw_Kyber768_Poly_Cbd_Eta2(r, buf);
}
