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
#include "FsmSw_Kyber768_params.h"
#include "FsmSw_Kyber768_cbd.h"
#include "FsmSw_Kyber768_poly.h"

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
* Name:        FsmSw_Kyber768_poly_compress
*
* Description: Compression and subsequent serialization of a polynomial
*
* Arguments:   -       uint8 *r: pointer to output byte array (of length KYBER768_POLYCOMPRESSEDBYTES bytes)
*              - const poly  *a: pointer to input polynomial
***********************************************************************************************************************/
void FsmSw_Kyber768_poly_compress(uint8 r[KYBER768_POLYCOMPRESSEDBYTES], const poly768 *a)
{
    uint16 i;
    sint16 u;
    uint8  j, t[8];

    /* r_temp is used to avoid modifying the input. */
    uint8 *r_temp = r;

    for (i = 0; i < (KYBER_N / 8u); i++)
    {
        for (j = 0; j < 8u; j++)
        {
            /* map to positive standard representatives */
            u  = a->coeffs[(8u * i) + j];
            u = (sint16)(u + (sint16)((uint16)(((uint32)u >> 15u) & KYBER_Q)));
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
* Name:        FsmSw_Kyber768_poly_decompress
*
* Description: De-serialization and subsequent decompression of a polynomial;
*              approximate inverse of FsmSw_Kyber768_poly_compress
*
* Arguments:   -       poly768 *r: pointer to output polynomial
*              - const uint8   *a: pointer to input byte array (of length KYBER768_POLYCOMPRESSEDBYTES bytes)
***********************************************************************************************************************/
void FsmSw_Kyber768_poly_decompress(poly768 *r, const uint8 a[KYBER768_POLYCOMPRESSEDBYTES])
{
    uint16 i;
    uint16 temp1;
    uint8  temp2;

    /* a_temp is used to avoid modifying the input. */
    const uint8 *a_temp = a;

    for (i = 0; i < (KYBER_N / 2u); i++)
    {
        temp1 = (uint16)a_temp[0] & 15u;
        r->coeffs[2u * i] = (sint16)(uint16)((((uint32)temp1 * KYBER_Q) + 8u) >> 4u);

        temp2 = a_temp[0] >> 4u;
        r->coeffs[(2u * i) + 1u] = (sint16)(uint16)((((uint32)temp2 * KYBER_Q) + 8u) >> 4u);

        a_temp = &(a_temp[1]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber768_poly_tobytes
*
* Description: Serialization of a polynomial
*
* Arguments:   -       uint8   *r: pointer to output byte array (of length KYBER_POLYBYTES bytes)
*              - const poly768 *a: pointer to input polynomial
***********************************************************************************************************************/
void FsmSw_Kyber768_poly_tobytes(uint8 r[KYBER_POLYBYTES], const poly768 *a)
{
    uint16 i, t0, t1;

    for (i = 0; i < (KYBER_N / 2u); i++)
    {
        /* map to positive standard representatives */
        t0  = (uint16)(a->coeffs[2u * i]);

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
* Name:        FsmSw_Kyber768_poly_frombytes
*
* Description: De-serialization of a polynomial;
*              inverse of FsmSw_Kyber768_poly_tobytes
*
* Arguments:   -       poly768 *r: pointer to output polynomial
*              - const uint8   *a: pointer to input byte array (of length KYBER_POLYBYTES bytes)
***********************************************************************************************************************/
void FsmSw_Kyber768_poly_frombytes(poly768 *r, const uint8 a[KYBER_POLYBYTES])
{
    uint16 i;

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
* Name:        FsmSw_Kyber768_poly_frommsg
*
* Description: Convert 32-byte message to polynomial
*
* Arguments:   -       poly768 *r:   pointer to output polynomial
*              - const uint8   *msg: pointer to input message
***********************************************************************************************************************/
void FsmSw_Kyber768_poly_frommsg(poly768 *r, const uint8 msg[KYBER768_INDCPA_MSGBYTES])
{
    uint8  j;
    uint16 i;
    sint16 mask;

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
* Name:        FsmSw_Kyber768_poly_tomsg
*
* Description: Convert polynomial to 32-byte message
*
* Arguments:   -       uint8   *msg: pointer to output message
*              - const poly768 *a:   pointer to input polynomial
***********************************************************************************************************************/
void FsmSw_Kyber768_poly_tomsg(uint8 msg[KYBER768_INDCPA_MSGBYTES], const poly768 *a)
{
    uint8  j;
    uint16 i, t;

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
* Name:        FsmSw_Kyber768_poly_getnoise_eta1
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER768_ETA1
*
* Arguments:   -       poly768 *r:     pointer to output polynomial
*              - const uint8   *seed:  pointer to input seed (of length KYBER_SYMBYTES bytes)
*              -       uint8    nonce: one-byte input nonce
***********************************************************************************************************************/
void FsmSw_Kyber768_poly_getnoise_eta1(poly768 *r, const uint8 seed[KYBER_SYMBYTES], uint8 nonce)
{
    uint8 buf[KYBER768_ETA1 * KYBER_N / 4u];

    FsmSw_Kyber_shake256_prf(buf, sizeof(buf), seed, nonce);
    FsmSw_Kyber768_poly_cbd_eta1(r, buf);
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber768_poly_getnoise_eta2
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER768_ETA2
*
* Arguments:   -       poly768 *r:     pointer to output polynomial
*              - const uint8   *seed:  pointer to input seed (of length KYBER_SYMBYTES bytes)
*              -       uint8    nonce: one-byte input nonce
***********************************************************************************************************************/
void FsmSw_Kyber768_poly_getnoise_eta2(poly768 *r, const uint8 seed[KYBER_SYMBYTES], uint8 nonce)
{
    uint8 buf[KYBER768_ETA2 * KYBER_N / 4u];

    FsmSw_Kyber_shake256_prf(buf, sizeof(buf), seed, nonce);
    FsmSw_Kyber768_poly_cbd_eta2(r, buf);
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber768_poly_ntt
*
* Description: Computes negacyclic number-theoretic transform (NTT) of
*              a polynomial in place;
*              inputs assumed to be in normal order, output in bitreversed order
*
* Arguments:   - uint16 *r: pointer to in/output polynomial
***********************************************************************************************************************/
void FsmSw_Kyber768_poly_ntt(poly768 *r)
{
    FsmSw_Kyber_ntt(r->coeffs);
    FsmSw_Kyber768_poly_reduce(r);
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber768_poly_invntt_tomont
*
* Description: Computes inverse of negacyclic number-theoretic transform (NTT)
*              of a polynomial in place;
*              inputs assumed to be in bitreversed order, output in normal order
*
* Arguments:   - uint16 *a: pointer to in/output polynomial
***********************************************************************************************************************/
void FsmSw_Kyber768_poly_invntt_tomont(poly768 *r)
{
    FsmSw_Kyber_invntt(r->coeffs);
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber768_poly_basemul_montgomery
*
* Description: Multiplication of two polynomials in NTT domain
*
* Arguments:   -       poly768 *r: pointer to output polynomial
*              - const poly768 *a: pointer to first input polynomial
*              - const poly768 *b: pointer to second input polynomial
***********************************************************************************************************************/
void FsmSw_Kyber768_poly_basemul_montgomery(poly768 *r, const poly768 *a, const poly768 *b)
{
    uint16 i;

    for (i = 0; i < (KYBER_N / 4u); i++)
    {
        FsmSw_Kyber_basemul(&r->coeffs[4u * i], &a->coeffs[4u * i],
                            &b->coeffs[4u * i], FsmSw_Kyber_zetas[64u + i]);

        FsmSw_Kyber_basemul(&r->coeffs[(4u * i) + 2u], &a->coeffs[(4u * i) + 2u],
                            &b->coeffs[(4u * i) + 2u], -FsmSw_Kyber_zetas[64u + i]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber768_poly_tomont
*
* Description: Inplace conversion of all coefficients of a polynomial
*              from normal domain to Montgomery domain
*
* Arguments:   - poly768 *r: pointer to input/output polynomial
***********************************************************************************************************************/
void FsmSw_Kyber768_poly_tomont(poly768 *r)
{
    uint16 i;
    const sint16 f = (sint16)((1ULL << 32u) % KYBER_Q);

    for (i = 0; i < KYBER_N; i++)
    {
        r->coeffs[i] = FsmSw_Kyber_montgomery_reduce((sint32)r->coeffs[i] * (sint32)f);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber768_poly_reduce
*
* Description: Applies Barrett reduction to all coefficients of a polynomial
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments:   - poly768 *r: pointer to input/output polynomial
***********************************************************************************************************************/
void FsmSw_Kyber768_poly_reduce(poly768 *r)
{
    uint16 i;

    for (i = 0; i < KYBER_N; i++)
    {
        r->coeffs[i] = FsmSw_Kyber_barrett_reduce(r->coeffs[i]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber768_poly_add
*
* Description: Add two polynomials; no modular reduction is performed
*
* Arguments: -       poly768 *r: pointer to output polynomial
*            - const poly768 *a: pointer to first input polynomial
*            - const poly768 *b: pointer to second input polynomial
***********************************************************************************************************************/
void FsmSw_Kyber768_poly_add(poly768 *r, const poly768 *a, const poly768 *b)
{
    uint16 i;

    for (i = 0; i < KYBER_N; i++)
    {
        r->coeffs[i] = a->coeffs[i] + b->coeffs[i];
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber768_poly_sub
*
* Description: Subtract two polynomials; no modular reduction is performed
*
* Arguments: -       poly768 *r: pointer to output polynomial
*            - const poly768 *a: pointer to first input polynomial
*            - const poly768 *b: pointer to second input polynomial
***********************************************************************************************************************/
void FsmSw_Kyber768_poly_sub(poly768 *r, const poly768 *a, const poly768 *b)
{
    uint16 i;

    for (i = 0; i < KYBER_N; i++)
    {
        r->coeffs[i] = a->coeffs[i] - b->coeffs[i];
    }
}
