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
#include "FsmSw_Kyber_CommonLib.h"
#include "FsmSw_Kyber_ntt.h"
#include "FsmSw_Kyber_reduce.h"
#include "FsmSw_Kyber_symmetric.h"
#include "FsmSw_Types.h"

#include "FsmSw_Kyber_poly.h"
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
* Name:        FsmSw_Kyber_Poly_ToBytes
*
* Description: Serialization of a polynomial
*
* Arguments:   -       uint8   *r: pointer to output byte array (needs space for KYBER_POLYBYTES bytes)
*              - const poly *a: pointer to input polynomial
***********************************************************************************************************************/
void FsmSw_Kyber_Poly_ToBytes(uint8 r[KYBER_POLYBYTES], const poly *a)
{
  uint16 i  = 0;
  uint16 t0 = 0;
  uint16 t1 = 0;

  for (i = 0; i < (KYBER_N / 2u); i++)
  {
    /* map to positive standard representatives */
    t0 = (uint16)(a->coeffs[2u * i]);

    /* Shift to get the first bit */
    if ((t0 >> 15u) != 0u)
    {
      t0 = t0 + KYBER_Q;
    }

    t1 = (uint16)(a->coeffs[(2u * i) + 1u]);
    /* Shift to get the first bit */
    if ((t1 >> 15u) != 0u)
    {
      t1 = t1 + KYBER_Q;
    }

    r[3u * i]        = (uint8)(t0 >> 0);
    r[(3u * i) + 1u] = (uint8)((t0 >> 8u) | (t1 << 4u));
    r[(3u * i) + 2u] = (uint8)(t1 >> 4u);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber_Poly_FromBytes
*
* Description: De-serialization of a polynomial;
*              inverse of FsmSw_Kyber_Poly_ToBytes
*
* Arguments:   -       poly *r: pointer to output polynomial
*              - const uint8   *a: pointer to input byte array (of KYBER_POLYBYTES bytes)
***********************************************************************************************************************/
void FsmSw_Kyber_Poly_FromBytes(poly *r, const uint8 a[KYBER_POLYBYTES])
{
  uint16 i = 0;

  for (i = 0; i < (KYBER_N / 2u); i++)
  {
    r->coeffs[2u * i] =
        (sint16)((uint16)((((((uint16)a[3u * i]) >> 0u) | (((uint16)a[((3u * i) + 1u)]) << 8u))) & 0xFFFu));

    r->coeffs[(2u * i) + 1u] =
        (sint16)((uint16)((((((uint16)a[(3u * i) + 1u]) >> 4u) | (((uint16)a[(3u * i) + 2u]) << 4u))) & 0xFFFu));
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber_Poly_Ntt
*
* Description: Computes negacyclic number-theoretic transform (NTT) of
*              a polynomial in place;
*              inputs assumed to be in normal order, output in bitreversed order
*
* Arguments:   - uint16 *r: pointer to in/output polynomial
***********************************************************************************************************************/
void FsmSw_Kyber_Poly_Ntt(poly *r)
{
  FsmSw_Kyber_Ntt(r->coeffs);
  FsmSw_Kyber_Poly_Reduce(r);
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber_Poly_InvnttTomont
*
* Description: Computes inverse of negacyclic number-theoretic transform (NTT)
*              of a polynomial in place;
*              inputs assumed to be in bitreversed order, output in normal order
*
* Arguments:   - uint16 *a: pointer to in/output polynomial
***********************************************************************************************************************/
void FsmSw_Kyber_Poly_InvnttTomont(poly *r)
{
  FsmSw_Kyber_Invntt(r->coeffs);
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber_Poly_BasemulMontgomery
*
* Description: Multiplication of two polynomials in NTT domain
*
* Arguments:   -       poly *r: pointer to output polynomial
*              - const poly *a: pointer to first input polynomial
*              - const poly *b: pointer to second input polynomial
***********************************************************************************************************************/
void FsmSw_Kyber_Poly_BasemulMontgomery(poly *r, const poly *a, const poly *b)
{
  uint16 i = 0;

  for (i = 0; i < (KYBER_N / 4u); i++)
  {
    FsmSw_Kyber_Basemul(&r->coeffs[4u * i], &a->coeffs[4u * i], &b->coeffs[4u * i], FsmSw_Kyber_zetas[64u + i]);

    FsmSw_Kyber_Basemul(&r->coeffs[(4u * i) + 2u], &a->coeffs[(4u * i) + 2u], &b->coeffs[(4u * i) + 2u],
                        -FsmSw_Kyber_zetas[64u + i]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber_Poly_Tomont
*
* Description: Inplace conversion of all coefficients of a polynomial
*              from normal domain to Montgomery domain
*
* Arguments:   - poly *r: pointer to input/output polynomial
***********************************************************************************************************************/
void FsmSw_Kyber_Poly_Tomont(poly *r)
{
  uint16 i       = 0;
  const sint16 f = (sint16)((1ULL << 32u) % KYBER_Q);

  for (i = 0; i < KYBER_N; i++)
  {
    r->coeffs[i] = FsmSw_Kyber_MontgomeryReduce((sint32)r->coeffs[i] * (sint32)f);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber_Poly_Reduce
*
* Description: Applies Barrett reduction to all coefficients of a polynomial
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments:   - poly *r: pointer to input/output polynomial
***********************************************************************************************************************/
void FsmSw_Kyber_Poly_Reduce(poly *r)
{
  uint16 i = 0;

  for (i = 0; i < KYBER_N; i++)
  {
    r->coeffs[i] = FsmSw_Kyber_BarrettReduce(r->coeffs[i]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber_Poly_Add
*
* Description: Add two polynomials; no modular reduction is performed
*
* Arguments: -       poly *r: pointer to output polynomial
*            - const poly *a: pointer to first input polynomial
*            - const poly *b: pointer to second input polynomial
***********************************************************************************************************************/
void FsmSw_Kyber_Poly_Add(poly *r, const poly *a, const poly *b)
{
  uint16 i = 0;

  for (i = 0; i < KYBER_N; i++)
  {
    r->coeffs[i] = a->coeffs[i] + b->coeffs[i];
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber_Poly_Sub
*
* Description: Subtract two polynomials; no modular reduction is performed
*
* Arguments: -       poly *r: pointer to output polynomial
*            - const poly *a: pointer to first input polynomial
*            - const poly *b: pointer to second input polynomial
***********************************************************************************************************************/
void FsmSw_Kyber_Poly_Sub(poly *r, const poly *a, const poly *b)
{
  uint16 i = 0;

  for (i = 0; i < KYBER_N; i++)
  {
    r->coeffs[i] = a->coeffs[i] - b->coeffs[i];
  }
}
