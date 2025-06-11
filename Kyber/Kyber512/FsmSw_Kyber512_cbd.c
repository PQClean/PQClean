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
#include "FsmSw_Kyber512_params.h"
#include "FsmSw_Kyber_CommonLib.h"
#include "FsmSw_Types.h"

#include "FsmSw_Kyber512_cbd.h"
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
static uint32 fsmsw_kyber512_Load24LittleEndian(const uint8 x[3]);
static void fsmsw_kyber512_Cbd3(poly *r, const uint8 buf[3u * KYBER_N / 4u]);

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        fsmsw_kyber512_Load24LittleEndian
*
* Description: load 3 bytes into a 32-bit integer
*              in little-endian order.
*              This function is only needed for Kyber-512
*
* Arguments:   - const uint8 *x: pointer to input byte array
*
* Returns 32-bit unsigned integer loaded from x (most significant byte is zero)
***********************************************************************************************************************/
static uint32 fsmsw_kyber512_Load24LittleEndian(const uint8 x[3])
{
  uint32 r = 0;

  r = (uint32)x[0];
  r |= (uint32)x[1] << 8;
  r |= (uint32)x[2] << 16;

  return r;
}

/***********************************************************************************************************************
* Name:        fsmsw_kyber512_Cbd3
*
* Description: Given an array of uniformly random bytes, compute
*              polynomial with coefficients distributed according to
*              a centered binomial distribution with parameter eta=3.
*              This function is only needed for Kyber-512
*
* Arguments:   -       poly *r:   pointer to output polynomial
*              - const uint8   *buf: pointer to input byte array
***********************************************************************************************************************/
static void fsmsw_kyber512_Cbd3(poly *r, const uint8 buf[3u * KYBER_N / 4u])
{
  uint8 i  = 0;
  uint8 j  = 0;
  uint32 t = 0;
  uint32 d = 0;
  sint16 a = 0;
  sint16 b = 0;

  for (i = 0; i < (KYBER_N / 4u); i++)
  {
    t = fsmsw_kyber512_Load24LittleEndian(&buf[3u * i]);
    d = t & 0x00249249u;
    d += (t >> 1u) & 0x00249249u;
    d += (t >> 2u) & 0x00249249u;

    for (j = 0; j < 4u; j++)
    {
      a                       = (sint16)((uint16)((uint16)((d >> ((6u * j)))) & 0x7u));
      b                       = (sint16)((uint16)((uint16)((d >> ((6u * j) + 3u))) & 0x7u));
      r->coeffs[(4u * i) + j] = a - b;
    }
  }
}

/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        FsmSw_Kyber512_Poly_Cbd_Eta1
*
* Description: Given an array of uniformly random bytes, compute
*              polynomial with coefficients distributed according to
*              a centered binomial distribution with parameter eta=3.
*              This function is only needed for Kyber-512
*
* Arguments:   -       poly *r:   pointer to output polynomial
*              - const uint8   *buf: pointer to input byte array
***********************************************************************************************************************/
void FsmSw_Kyber512_Poly_Cbd_Eta1(poly *r, const uint8 buf[KYBER512_ETA1 * KYBER_N / 4u])
{
  fsmsw_kyber512_Cbd3(r, buf);
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber512_Poly_Cbd_Eta2
*
* Description: Given an array of uniformly random bytes, compute
*              polynomial with coefficients distributed according to
*              a centered binomial distribution with parameter eta=2
*
* Arguments:   -       poly  *r:   pointer to output polynomial
*              - const uint8 *buf: pointer to input byte array
***********************************************************************************************************************/
void FsmSw_Kyber512_Poly_Cbd_Eta2(poly *r, const uint8 buf[KYBER512_ETA2 * KYBER_N / 4u])
{
  FsmSw_Kyber_Cbd2(r, buf);
}
