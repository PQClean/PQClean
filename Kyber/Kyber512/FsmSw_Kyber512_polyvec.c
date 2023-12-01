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

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "Platform_Types.h"
#include "FsmSw_Kyber512_params.h"
#include "FsmSw_Kyber512_poly.h"
#include "FsmSw_Kyber512_polyvec.h"

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
* Name:        FsmSw_Kyber512_polyvec_compress
*
* Description: Compress and serialize vector of polynomials
*
* Arguments:   -       uint8   *r: pointer to output byte array
*              - const polyvec *a: pointer to input vector of polynomials
***********************************************************************************************************************/
void FsmSw_Kyber512_polyvec_compress(uint8 r[KYBER512_POLYVECCOMPRESSEDBYTES], const polyvec512 *a)
{
    uint8  i, k;
    uint16 j, t[4];

    for (i = 0; i < KYBER512_K; i++)
    {
        for (j = 0; j < KYBER_N / 4u; j++)
        {
            for (k = 0; k < 4u; k++)
            {
                t[k]  = (uint16)(a->vec[i].coeffs[4u * j + k]);
                /* Shift to get the first bit */
                if ((t[k] >> 15u) != 0u)
                {
                    t[k] = t[k] + KYBER_Q;
                }
                t[k]  = (uint16)(((t[k] << 10u) + KYBER_Q / 2u) / KYBER_Q) & 0x3ffu;
            }

            r[0] = (uint8)(t[0] >> 0);
            r[1] = (uint8)((t[0] >> 8) | (t[1] << 2));
            r[2] = (uint8)((t[1] >> 6) | (t[2] << 4));
            r[3] = (uint8)((t[2] >> 4) | (t[3] << 6));
            r[4] = (uint8)(t[3] >> 2);
            r    = &(r[5]);
        }
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber512_polyvec_decompress
*
* Description: De-serialize and decompress vector of polynomials;
*              approximate inverse of FsmSw_Kyber512_polyvec_compress
*
* Arguments:   -       polyvec512 *r: pointer to output vector of polynomials
*              - const uint8      *a: pointer to input byte array of length KYBER512_POLYVECCOMPRESSEDBYTES
***********************************************************************************************************************/
void FsmSw_Kyber512_polyvec_decompress(polyvec512 *r, const uint8 a[KYBER512_POLYVECCOMPRESSEDBYTES])
{
    uint8  i, k;
    uint16 j, t[4];

    for (i = 0; i < KYBER512_K; i++)
    {
        for (j = 0; j < KYBER_N / 4u; j++)
        {
            t[0] = ((uint16)a[0] >> 0) | ((uint16)a[1] << 8);
            t[1] = ((uint16)a[1] >> 2) | ((uint16)a[2] << 6);
            t[2] = ((uint16)a[2] >> 4) | ((uint16)a[3] << 4);
            t[3] = ((uint16)a[3] >> 6) | ((uint16)a[4] << 2);
            /* Set address from pointer a[4] to address a[5] */
            a    = &(a[5]);

            for (k = 0; k < 4u; k++)
            {
                r->vec[i].coeffs[4u * j + k] = (sint16)((uint16)(((t[k] & 0x3FFu) * KYBER_Q + 512u) >> 10u));
            }
        }
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber512_polyvec_tobytes
*
* Description: Serialize vector of polynomials
*
* Arguments:   -       uint8      *r: pointer to output byte array  of length KYBER512_POLYVECBYTES
*              - const polyvec512 *a: pointer to input vector of polynomials
***********************************************************************************************************************/
void FsmSw_Kyber512_polyvec_tobytes(uint8 r[KYBER512_POLYVECBYTES], const polyvec512 *a)
{
    uint8 i;

    for (i = 0; i < KYBER512_K; i++)
    {
        FsmSw_Kyber512_poly_tobytes(&(r[i * KYBER_POLYBYTES]), &a->vec[i]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber512_polyvec_frombytes
*
* Description: De-serialize vector of polynomials;
*              inverse of FsmSw_Kyber512_polyvec_tobytes
*
* Arguments:   -       uint8      *r: pointer to output byte array
*              - const polyvec512 *a: pointer to input vector of polynomials of length KYBER512_POLYVECBYTES
***********************************************************************************************************************/
void FsmSw_Kyber512_polyvec_frombytes(polyvec512 *r, const uint8 a[KYBER512_POLYVECBYTES])
{
    uint8 i;

    for (i = 0; i < KYBER512_K; i++)
    {
        FsmSw_Kyber512_poly_frombytes(&r->vec[i], &a[i * KYBER_POLYBYTES]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber512_polyvec_ntt
*
* Description: Apply forward NTT to all elements of a vector of polynomials
*
* Arguments:   - polyvec512 *r: pointer to in/output vector of polynomials
***********************************************************************************************************************/
void FsmSw_Kyber512_polyvec_ntt(polyvec512 *r)
{
    uint8 i;

    for (i = 0; i < KYBER512_K; i++)
    {
        FsmSw_Kyber512_poly_ntt(&r->vec[i]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber512_polyvec_invntt_tomont
*
* Description: Apply inverse NTT to all elements of a vector of polynomials
*              and multiply by Montgomery factor 2^16
*
* Arguments:   - polyvec512 *r: pointer to in/output vector of polynomials
***********************************************************************************************************************/
void FsmSw_Kyber512_polyvec_invntt_tomont(polyvec512 *r)
{
    uint8 i;

    for (i = 0; i < KYBER512_K; i++)
    {
        FsmSw_Kyber512_poly_invntt_tomont(&r->vec[i]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber512_polyvec_basemul_acc_montgomery
*
* Description: Multiply elements of a and b in NTT domain, accumulate into r,
*              and multiply by 2^-16.
*
* Arguments: -       poly512    *r: pointer to output polynomial
*            - const polyvec512 *a: pointer to first input vector of polynomials
*            - const polyvec512 *b: pointer to second input vector of polynomials
***********************************************************************************************************************/
void FsmSw_Kyber512_polyvec_basemul_acc_montgomery(poly512 *r, const polyvec512 *a, const polyvec512 *b)
{
    uint8   i;
    poly512 t;

    FsmSw_Kyber512_poly_basemul_montgomery(r, &a->vec[0], &b->vec[0]);

    for (i = 1; i < KYBER512_K; i++)
    {
        FsmSw_Kyber512_poly_basemul_montgomery(&t, &a->vec[i], &b->vec[i]);
        FsmSw_Kyber512_poly_add(r, r, &t);
    }

    FsmSw_Kyber512_poly_reduce(r);
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber512_polyvec_reduce
*
* Description: Applies Barrett reduction to each coefficient
*              of each element of a vector of polynomials;
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments:   - polyvec512 *r: pointer to input/output polynomial
***********************************************************************************************************************/
void FsmSw_Kyber512_polyvec_reduce(polyvec512 *r)
{
    uint8 i;

    for (i = 0; i < KYBER512_K; i++)
    {
        FsmSw_Kyber512_poly_reduce(&r->vec[i]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber512_polyvec_add
*
* Description: Add vectors of polynomials
*
* Arguments: -       polyvec512 *r: pointer to output vector of polynomials
*            - const polyvec512 *a: pointer to first input vector of polynomials
*            - const polyvec512 *b: pointer to second input vector of polynomials
***********************************************************************************************************************/
void FsmSw_Kyber512_polyvec_add(polyvec512 *r, const polyvec512 *a, const polyvec512 *b)
{
    uint8 i;

    for (i = 0; i < KYBER512_K; i++)
    {
        FsmSw_Kyber512_poly_add(&r->vec[i], &a->vec[i], &b->vec[i]);
    }
}
