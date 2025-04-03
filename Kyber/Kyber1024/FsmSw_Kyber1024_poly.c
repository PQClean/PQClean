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
#include "FsmSw_Kyber_ntt.h"
#include "FsmSw_Kyber_reduce.h"
#include "FsmSw_Kyber_symmetric.h"
#include "FsmSw_Kyber1024_params.h"
#include "FsmSw_Kyber1024_cbd.h"
#include "FsmSw_Kyber1024_poly.h"

#include "FsmSw_Types.h"

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
* Name:        FsmSw_Kyber1024_poly_compress
*
* Description: Compression and subsequent serialization of a polynomial
*
* Arguments:   -       uint8    *r: pointer to output byte array (of length KYBER1024_POLYCOMPRESSEDBYTES bytes)
*              - const poly1024 *a: pointer to input polynomial
***********************************************************************************************************************/
void FsmSw_Kyber1024_poly_compress(uint8 r[KYBER1024_POLYCOMPRESSEDBYTES], const poly1024 *a)
{
    uint16 i = 0;
    sint16 u = 0;
    uint8  j = 0;
    uint8  t[8] = {0};

    /* r_temp is used to avoid modifying the input. */
    uint8 *r_temp = r;

    for (i = 0; i < (KYBER_N / 8u); i++)
    {
        for (j = 0; j < 8u; j++)
        {
            /* map to positive standard representatives */
            u = a->coeffs[(8u * i) + j];
            u = (sint16)(u + (sint16)((uint16)(((uint32)u >> 15u) & KYBER_Q)));
            t[j] = (uint8)(((((uint32)u << 5u) + KYBER_Q / 2u) / KYBER_Q) & 31u);
        }

        r_temp[0] = (t[0] >> 0) | (t[1] << 5);
        r_temp[1] = (t[1] >> 3) | (t[2] << 2) | (t[3] << 7);
        r_temp[2] = (t[3] >> 1) | (t[4] << 4);
        r_temp[3] = (t[4] >> 4) | (t[5] << 1) | (t[6] << 6);
        r_temp[4] = (t[6] >> 2) | (t[7] << 3);
        r_temp = &(r_temp[5]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_poly_decompress
*
* Description: De-serialization and subsequent decompression of a polynomial;
*              approximate inverse of FsmSw_Kyber1024_poly_compress
*
* Arguments:   - poly1024    *r: pointer to output polynomial
*              - const uint8 *a: pointer to input byte array (of length KYBER1024_POLYCOMPRESSEDBYTES bytes)
***********************************************************************************************************************/
void FsmSw_Kyber1024_poly_decompress(poly1024 *r, const uint8 a[KYBER1024_POLYCOMPRESSEDBYTES])
{
    uint16 i = 0;
    uint8  j = 0;
    uint8  t[8] = {0};

    /* a_temp is used to avoid modifying the input. */
    const uint8 *a_temp = a;

    for (i = 0; i < (KYBER_N / 8u); i++)
    {
        t[0] = (a_temp[0] >> 0);
        t[1] = (a_temp[0] >> 5) | (a_temp[1] << 3);
        t[2] = (a_temp[1] >> 2);
        t[3] = (a_temp[1] >> 7) | (a_temp[2] << 1);
        t[4] = (a_temp[2] >> 4) | (a_temp[3] << 4);
        t[5] = (a_temp[3] >> 1);
        t[6] = (a_temp[3] >> 6) | (a_temp[4] << 2);
        t[7] = (a_temp[4] >> 3);
        a_temp = &(a_temp[5]);

        for (j = 0; j < 8u; j++)
        {
            r->coeffs[(8u * i) + j] = (sint16)((uint16)(((uint32)(((uint32)t[j] & 31u) * KYBER_Q) + 16u) >> 5u));
        }
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_poly_tobytes
*
* Description: Serialization of a polynomial
*
* Arguments:   - uint8          *r: pointer to output byte array (of length KYBER_POLYBYTES bytes)
*              - const poly1024 *a: pointer to input polynomial
***********************************************************************************************************************/
void FsmSw_Kyber1024_poly_tobytes(uint8 r[KYBER_POLYBYTES], const poly1024 *a)
{
    uint16 i = 0;
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

        r[3u * i] = (uint8)(t0 >> 0);
        r[(3u * i) + 1u] = (uint8)((t0 >> 8u) | (t1 << 4u));
        r[(3u * i) + 2u] = (uint8)(t1 >> 4u);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_poly_frombytes
*
* Description: De-serialization of a polynomial;
*              inverse of FsmSw_Kyber1024_poly_tobytes
*
* Arguments:   - poly1024    *r: pointer to output polynomial
*              - const uint8 *a: pointer to input byte array (of length KYBER_POLYBYTES bytes)
***********************************************************************************************************************/
void FsmSw_Kyber1024_poly_frombytes(poly1024 *r, const uint8 a[KYBER_POLYBYTES])
{
    uint16 i = 0;

    for (i = 0; i < (KYBER_N / 2u); i++)
    {
        r->coeffs[2u * i]      = (sint16) ((uint16)( (((
                                                         ((uint16)a[3u * i]) >> 0u) |
                                                         (((uint16)a[(3u * i) + 1u]) << 8u))
                                                     ) & 0xFFFu
                                                   )
                                          );

        r->coeffs[(2u * i) + 1u] = (sint16) ((uint16)( (((
                                                         ((uint16)a[(3u * i) + 1u]) >> 4u) |
                                                         (((uint16)a[(3u * i) + 2u]) << 4u))
                                                     ) & 0xFFFu
                                                   )
                                          );
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_poly_frommsg
*
* Description: Convert 32-byte message to polynomial
*
* Arguments:   - poly1024    *r:   pointer to output polynomial
*              - const uint8 *msg: pointer to input message (of length KYBER1024_INDCPA_MSGBYTES bytes)
***********************************************************************************************************************/
void FsmSw_Kyber1024_poly_frommsg(poly1024 *r, const uint8 msg[KYBER1024_INDCPA_MSGBYTES])
{
    uint8  j = 0;
    uint16 i = 0;
    sint16 mask = 0;

    for (i = 0; i < (KYBER_N / 8u); i++)
    {
        for (j = 0; j < 8u; j++)
        {
            mask = -(sint16)((uint16)(((uint32)msg[i] >> j) & 1u));
            r->coeffs[(8u * i) + j] = (sint16)((uint16)((uint16)mask & ((uint16)((KYBER_Q + 1u) / 2u))));
        }
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_poly_tomsg
*
* Description: Convert polynomial to 32-byte message
*
* Arguments:   - uint8          *msg: pointer to output message (of length KYBER1024_INDCPA_MSGBYTES bytes)
*              - const poly1024 *a:   pointer to input polynomial
***********************************************************************************************************************/
void FsmSw_Kyber1024_poly_tomsg(uint8 msg[KYBER1024_INDCPA_MSGBYTES], const poly1024 *a)
{
    uint8  j = 0;
    uint16 i = 0;
    uint16 t = 0;

    for (i = 0; i < (KYBER_N / 8u); i++)
    {
        msg[i] = 0;
        for (j = 0; j < 8u; j++)
        {
            t  = (uint16)(a->coeffs[(8u * i) + j]);
            /* Shift to get the first bit */
            if ((t >> 15u) != 0u)
            {
                t = t + KYBER_Q;
            }

            t  = (((t << 1u) + (KYBER_Q / 2u)) / KYBER_Q) & 1u;
            msg[i] = (uint8)((uint16)msg[i] | (t << j));
        }
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_poly_getnoise_eta1
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER1024_ETA1
*
* Arguments:   - poly1024    *r:     pointer to output polynomial
*              - const uint8 *seed:  pointer to input seed (of length KYBER_SYMBYTES bytes)
*              - uint8        nonce: one-byte input nonce
***********************************************************************************************************************/
void FsmSw_Kyber1024_poly_getnoise_eta1(poly1024 *r, const uint8 seed[KYBER_SYMBYTES], uint8 nonce)
{
    uint8 buf[KYBER1024_ETA1 * KYBER_N / 4u] = {0};

    FsmSw_Kyber_shake256_prf(buf, sizeof(buf), seed, nonce);
    FsmSw_Kyber1024_poly_cbd_eta1(r, buf);
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_poly_getnoise_eta2
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER1024_ETA2
*
* Arguments:   - poly1024    *r:     pointer to output polynomial
*              - const uint8 *seed:  pointer to input seed (of length KYBER_SYMBYTES bytes)
*              - uint8        nonce: one-byte input nonce
***********************************************************************************************************************/
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_Kyber1024_poly_getnoise_eta2(poly1024 *r, const uint8 seed[KYBER_SYMBYTES], uint8 nonce)
{
    uint8 buf[KYBER1024_ETA2 * KYBER_N / 4u] = {0};

    FsmSw_Kyber_shake256_prf(buf, sizeof(buf), seed, nonce);
    FsmSw_Kyber1024_poly_cbd_eta2(r, buf);
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_poly_ntt
*
* Description: Computes negacyclic number-theoretic transform (NTT) of
*              a polynomial in place;
*              inputs assumed to be in normal order, output in bitreversed order
*
* Arguments:   - poly1024 *r: pointer to in/output polynomial
***********************************************************************************************************************/
void FsmSw_Kyber1024_poly_ntt(poly1024 *r)
{
    FsmSw_Kyber_ntt(r->coeffs);
    FsmSw_Kyber1024_poly_reduce(r);
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_poly_invntt_tomont
*
* Description: Computes inverse of negacyclic number-theoretic transform (NTT)
*              of a polynomial in place;
*              inputs assumed to be in bitreversed order, output in normal order
*
* Arguments:   - poly1024 *a: pointer to in/output polynomial
***********************************************************************************************************************/
void FsmSw_Kyber1024_poly_invntt_tomont(poly1024 *r)
{
    FsmSw_Kyber_invntt(r->coeffs);
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_poly_basemul_montgomery
*
* Description: Multiplication of two polynomials in NTT domain
*
* Arguments:   - poly1024       *r: pointer to output polynomial
*              - const poly1024 *a: pointer to first input polynomial
*              - const poly1024 *b: pointer to second input polynomial
***********************************************************************************************************************/
void FsmSw_Kyber1024_poly_basemul_montgomery(poly1024 *r, const poly1024 *a, const poly1024 *b)
{
    uint16 i = 0;

    for (i = 0; i < (KYBER_N / 4u); i++)
    {
        FsmSw_Kyber_basemul(&r->coeffs[4u * i], &a->coeffs[4u * i],
                            &b->coeffs[4u * i], FsmSw_Kyber_zetas[64u + i]);

        FsmSw_Kyber_basemul(&r->coeffs[(4u * i) + 2u], &a->coeffs[(4u * i) + 2u],
                            &b->coeffs[(4u * i) + 2u], -FsmSw_Kyber_zetas[64u + i]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_poly_tomont
*
* Description: Inplace conversion of all coefficients of a polynomial
*              from normal domain to Montgomery domain
*
* Arguments:   - poly1024 *r: pointer to input/output polynomial
***********************************************************************************************************************/
void FsmSw_Kyber1024_poly_tomont(poly1024 *r)
{
    uint16 i = 0;
    const sint16 f = (sint16) ((1ULL << 32u) % KYBER_Q);

    for (i = 0; i < KYBER_N; i++)
    {
        r->coeffs[i] = FsmSw_Kyber_montgomery_reduce((sint32)r->coeffs[i] * (sint32)f);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_poly_reduce
*
* Description: Applies Barrett reduction to all coefficients of a polynomial
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments:   - poly1024 *r: pointer to input/output polynomial
***********************************************************************************************************************/
void FsmSw_Kyber1024_poly_reduce(poly1024 *r)
{
    uint16 i = 0;

    for (i = 0; i < KYBER_N; i++)
    {
        r->coeffs[i] = FsmSw_Kyber_barrett_reduce(r->coeffs[i]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_poly_add
*
* Description: Add two polynomials; no modular reduction is performed
*
* Arguments: - poly1024       *r: pointer to output polynomial
*            - const poly1024 *a: pointer to first input polynomial
*            - const poly1024 *b: pointer to second input polynomial
***********************************************************************************************************************/
void FsmSw_Kyber1024_poly_add(poly1024 *r, const poly1024 *a, const poly1024 *b)
{
    uint16 i = 0;

    for (i = 0; i < KYBER_N; i++)
    {
        r->coeffs[i] = a->coeffs[i] + b->coeffs[i];
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_poly_sub
*
* Description: Subtract two polynomials; no modular reduction is performed
*
* Arguments: - poly1024       *r: pointer to output polynomial
*            - const poly1024 *a: pointer to first input polynomial
*            - const poly1024 *b: pointer to second input polynomial
***********************************************************************************************************************/
void FsmSw_Kyber1024_poly_sub(poly1024 *r, const poly1024 *a, const poly1024 *b)
{
    uint16 i = 0;

    for (i = 0; i < KYBER_N; i++)
    {
        r->coeffs[i] = a->coeffs[i] - b->coeffs[i];
    }
}
