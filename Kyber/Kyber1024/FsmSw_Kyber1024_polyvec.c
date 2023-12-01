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
#include "FsmSw_Kyber1024_params.h"
#include "FsmSw_Kyber1024_poly.h"
#include "FsmSw_Kyber1024_polyvec.h"

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
* Name:        FsmSw_Kyber1024_polyvec_compress
*
* Description: Compress and serialize vector of polynomials
*
* Arguments:   - uint8             *r: pointer to output byte array
*              - const polyvec1024 *a: pointer to input vector of polynomials
***********************************************************************************************************************/
void FsmSw_Kyber1024_polyvec_compress(uint8 r[KYBER1024_POLYVECCOMPRESSEDBYTES], const polyvec1024 *a)
{
    uint8  i, k;
    uint16 j, t[8];

    for (i = 0; i < KYBER1024_K; i++)
    {
        for (j = 0; j < KYBER_N / 8u; j++)
        {
            for (k = 0; k < 8u; k++)
            {
                t[k]  = (uint16)(a->vec[i].coeffs[8u * j + k]);
                /* Shift to get the first bit */
                if ((t[k] >> 15u) != 0u)
                {
                    t[k] = t[k] + KYBER_Q;
                }
                t[k]  = (uint16)((((t[k] << 11u) + KYBER_Q / 2u) / KYBER_Q) & 0x7ffu);
            }

            r[ 0] = (uint8)( t[0] >>  0);
            r[ 1] = (uint8)((t[0] >>  8) | (t[1] << 3));
            r[ 2] = (uint8)((t[1] >>  5) | (t[2] << 6));
            r[ 3] = (uint8)( t[2] >>  2);
            r[ 4] = (uint8)((t[2] >> 10) | (t[3] << 1));
            r[ 5] = (uint8)((t[3] >>  7) | (t[4] << 4));
            r[ 6] = (uint8)((t[4] >>  4) | (t[5] << 7));
            r[ 7] = (uint8)( t[5] >>  1);
            r[ 8] = (uint8)((t[5] >>  9) | (t[6] << 2));
            r[ 9] = (uint8)((t[6] >>  6) | (t[7] << 5));
            r[10] = (uint8)( t[7] >>  3);
            r     = &(r[11]);
        }
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_polyvec_decompress
*
* Description: De-serialize and decompress vector of polynomials;
*              approximate inverse of FsmSw_Kyber1024_polyvec_compress
*
* Arguments:   - polyvec1024 *r:   pointer to output vector of polynomials
*              - const uint8 *a:   pointer to input byte array (of length KYBER1024_POLYVECCOMPRESSEDBYTES bytes)
***********************************************************************************************************************/
void FsmSw_Kyber1024_polyvec_decompress(polyvec1024 *r, const uint8 a[KYBER1024_POLYVECCOMPRESSEDBYTES])
{
    uint8  i, k;
    uint16 j, t[8];

    for (i = 0; i < KYBER1024_K; i++)
    {
        for (j = 0; j < KYBER_N / 8u; j++)
        {
            t[0] = ((uint16)a[0] >> 0) | ((uint16)a[ 1] << 8);
            t[1] = ((uint16)a[1] >> 3) | ((uint16)a[ 2] << 5);
            t[2] = ((uint16)a[2] >> 6) | ((uint16)a[ 3] << 2) | ((uint16)a[4] << 10);
            t[3] = ((uint16)a[4] >> 1) | ((uint16)a[ 5] << 7);
            t[4] = ((uint16)a[5] >> 4) | ((uint16)a[ 6] << 4);
            t[5] = ((uint16)a[6] >> 7) | ((uint16)a[ 7] << 1) | ((uint16)a[8] << 9);
            t[6] = ((uint16)a[8] >> 2) | ((uint16)a[ 9] << 6);
            t[7] = ((uint16)a[9] >> 5) | ((uint16)a[10] << 3);
            /* Set address from pointer a[10] to address a[11]  */
            a = &(a[11]);

            for (k = 0; k < 8u; k++)
            {
                r->vec[i].coeffs[8u * j + k] = (sint16)((uint16)(((t[k] & 0x7FFu) * KYBER_Q + 1024u) >> 11u));
            }
        }
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_polyvec_tobytes
*
* Description: Serialize vector of polynomials
*
* Arguments:   - uint8             *r: pointer to output byte array (of length KYBER1024_POLYVECBYTES bytes)
*              - const polyvec1024 *a: pointer to input vector of polynomials
***********************************************************************************************************************/
void FsmSw_Kyber1024_polyvec_tobytes(uint8 r[KYBER1024_POLYVECBYTES], const polyvec1024 *a)
{
    uint8 i;

    for (i = 0; i < KYBER1024_K; i++)
    {
        FsmSw_Kyber1024_poly_tobytes(&r[i * KYBER_POLYBYTES], &a->vec[i]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_polyvec_frombytes
*
* Description: De-serialize vector of polynomials;
*              inverse of FsmSw_Kyber1024_polyvec_tobytes
*
* Arguments:   - polyvec1024 *r: pointer to output byte array
*              - const uint8 *a: pointer to input vector of polynomials (of length KYBER1024_POLYVECBYTES bytes)
***********************************************************************************************************************/
void FsmSw_Kyber1024_polyvec_frombytes(polyvec1024 *r, const uint8 a[KYBER1024_POLYVECBYTES])
{
    uint8 i;

    for (i = 0; i < KYBER1024_K; i++)
    {
        FsmSw_Kyber1024_poly_frombytes(&r->vec[i], &a[i * KYBER_POLYBYTES]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_polyvec_ntt
*
* Description: Apply forward NTT to all elements of a vector of polynomials
*
* Arguments:   - polyvec1024 *r: pointer to in/output vector of polynomials
***********************************************************************************************************************/
void FsmSw_Kyber1024_polyvec_ntt(polyvec1024 *r)
{
    uint8 i;

    for (i = 0; i < KYBER1024_K; i++)
    {
        FsmSw_Kyber1024_poly_ntt(&r->vec[i]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_polyvec_invntt_tomont
*
* Description: Apply inverse NTT to all elements of a vector of polynomials
*              and multiply by Montgomery factor 2^16
*
* Arguments:   - polyvec1024 *r: pointer to in/output vector of polynomials
***********************************************************************************************************************/
void FsmSw_Kyber1024_polyvec_invntt_tomont(polyvec1024 *r)
{
    uint8 i;

    for (i = 0; i < KYBER1024_K; i++)
    {
        FsmSw_Kyber1024_poly_invntt_tomont(&r->vec[i]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_polyvec_basemul_acc_montgomery
*
* Description: Multiply elements of a and b in NTT domain, accumulate into r,
*              and multiply by 2^-16.
*
* Arguments: - poly1024          *r: pointer to output polynomial
*            - const polyvec1024 *a: pointer to first input vector of polynomials
*            - const polyvec1024 *b: pointer to second input vector of polynomials
***********************************************************************************************************************/
void FsmSw_Kyber1024_polyvec_basemul_acc_montgomery(poly1024 *r, const polyvec1024 *a, const polyvec1024 *b)
{
    uint8    i;
    poly1024 t;

    FsmSw_Kyber1024_poly_basemul_montgomery(r, &a->vec[0], &b->vec[0]);

    for (i = 1; i < KYBER1024_K; i++)
    {
        FsmSw_Kyber1024_poly_basemul_montgomery(&t, &a->vec[i], &b->vec[i]);
        FsmSw_Kyber1024_poly_add(r, r, &t);
    }

    FsmSw_Kyber1024_poly_reduce(r);
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_polyvec_reduce
*
* Description: Applies Barrett reduction to each coefficient
*              of each element of a vector of polynomials;
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments:   - polyvec1024 *r: pointer to input/output polynomial
***********************************************************************************************************************/
void FsmSw_Kyber1024_polyvec_reduce(polyvec1024 *r)
{
    uint8 i;

    for (i = 0; i < KYBER1024_K; i++)
    {
        FsmSw_Kyber1024_poly_reduce(&r->vec[i]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_polyvec_add
*
* Description: Add vectors of polynomials
*
* Arguments: - polyvec1024       *r: pointer to output vector of polynomials
*            - const polyvec1024 *a: pointer to first input vector of polynomials
*            - const polyvec1024 *b: pointer to second input vector of polynomials
***********************************************************************************************************************/
void FsmSw_Kyber1024_polyvec_add(polyvec1024 *r, const polyvec1024 *a, const polyvec1024 *b)
{
    uint8 i;

    for (i = 0; i < KYBER1024_K; i++)
    {
        FsmSw_Kyber1024_poly_add(&r->vec[i], &a->vec[i], &b->vec[i]);
    }
}
