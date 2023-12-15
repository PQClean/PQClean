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
#include "FsmSw_Dilithium2_packing.h"
#include "FsmSw_Dilithium2_params.h"
#include "FsmSw_Dilithium2_poly.h"
#include "FsmSw_Dilithium2_polyvec.h"

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
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium2_pack_pk
*
* Description: Bit-pack public key pk = (rho, t1).
*
* Arguments:   -       uint8        pk[]:  output byte array
*              - const uint8        rho[]: byte array containing rho
*              - const polyveck_D2 *t1:    pointer to vector t1
***********************************************************************************************************************/
void FsmSw_Dilithium2_pack_pk(uint8 pk[FSMSW_DILITHIUM2_CRYPTO_PUBLICKEYBYTES],
                              const uint8 rho[SEEDBYTES_DILITHIUM],
                              const polyveck_D2 *t1)
{
    uint16 i;

    for (i = 0; i < SEEDBYTES_DILITHIUM; ++i)
    {
        pk[i] = rho[i];
    }
    pk = &pk[SEEDBYTES_DILITHIUM];

    for (i = 0; i < K_DILITHIUM2; ++i)
    {
        FsmSw_Dilithium2_polyt1_pack(&pk[i * POLYT1_PACKEDBYTES_DILITHIUM], &t1->vec[i]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium2_unpack_pk
*
* Description: Unpack public key pk = (rho, t1).
*
* Arguments:   -       uint8        rho[]: output byte array for rho
*              -       polyveck_D2 *t1:    pointer to output vector t1
*              - const uint8        pk[]:  byte array containing bit-packed pk
***********************************************************************************************************************/
void FsmSw_Dilithium2_unpack_pk(uint8 rho[SEEDBYTES_DILITHIUM],
                                polyveck_D2 *t1,
                                const uint8 pk[FSMSW_DILITHIUM2_CRYPTO_PUBLICKEYBYTES])
{
    uint16 i;

    for (i = 0; i < SEEDBYTES_DILITHIUM; ++i)
    {
        rho[i] = pk[i];
    }
    pk = &pk[SEEDBYTES_DILITHIUM];

    for (i = 0; i < K_DILITHIUM2; ++i)
    {
        FsmSw_Dilithium2_polyt1_unpack(&t1->vec[i], &pk[i * POLYT1_PACKEDBYTES_DILITHIUM]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium2_pack_sk
*
* Description: Bit-pack secret key sk = (rho, tr, key, t0, s1, s2).
*
* Arguments:   -       uint8        sk[]:  output byte array
*              - const uint8        rho[]: byte array containing rho
*              - const uint8        tr[]:  byte array containing tr
*              - const uint8        key[]: byte array containing key
*              - const polyveck_D2 *t0:    pointer to vector t0
*              - const polyvecl_D2 *s1:    pointer to vector s1
*              - const polyveck_D2 *s2:    pointer to vector s2
***********************************************************************************************************************/
void FsmSw_Dilithium2_pack_sk(uint8 sk[FSMSW_DILITHIUM2_CRYPTO_SECRETKEYBYTES],
                              const uint8 rho[SEEDBYTES_DILITHIUM],
                              const uint8 tr[TRBYTES_DILITHIUM],
                              const uint8 key[SEEDBYTES_DILITHIUM],
                              const polyveck_D2 *t0,
                              const polyvecl_D2 *s1,
                              const polyveck_D2 *s2)
{
    uint16 i;

    for (i = 0; i < SEEDBYTES_DILITHIUM; ++i)
    {
        sk[i] = rho[i];
    }
    sk = &sk[SEEDBYTES_DILITHIUM];

    for (i = 0; i < SEEDBYTES_DILITHIUM; ++i)
    {
        sk[i] = key[i];
    }
    sk = &sk[SEEDBYTES_DILITHIUM];

    for (i = 0; i < TRBYTES_DILITHIUM; ++i)
    {
        sk[i] = tr[i];
    }
    sk = &sk[TRBYTES_DILITHIUM];

    for (i = 0; i < L_DILITHIUM2; ++i)
    {
        FsmSw_Dilithium2_polyeta_pack(&sk[i * POLYETA_PACKEDBYTES_DILITHIUM2], &s1->vec[i]);
    }
    sk = &sk[L_DILITHIUM2 * POLYETA_PACKEDBYTES_DILITHIUM2];

    for (i = 0; i < K_DILITHIUM2; ++i)
    {
        FsmSw_Dilithium2_polyeta_pack(&sk[i * POLYETA_PACKEDBYTES_DILITHIUM2], &s2->vec[i]);
    }
    sk = &sk[K_DILITHIUM2 * POLYETA_PACKEDBYTES_DILITHIUM2];

    for (i = 0; i < K_DILITHIUM2; ++i)
    {
        FsmSw_Dilithium2_polyt0_pack(&sk[i * POLYT0_PACKEDBYTES_DILITHIUM], &t0->vec[i]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium2_unpack_sk
*
* Description: Unpack secret key sk = (rho, tr, key, t0, s1, s2).
*
* Arguments:   -       uint8        rho[]: output byte array for rho
*              -       uint8        tr[]:  output byte array for tr
*              -       uint8        key[]: output byte array for key
*              -       polyveck_D2 *t0:    pointer to output vector t0
*              -       polyvecl_D2 *s1:    pointer to output vector s1
*              -       polyveck_D2 *s2:    pointer to output vector s2
*              - const uint8        sk[]:  byte array containing bit-packed sk
***********************************************************************************************************************/
void FsmSw_Dilithium2_unpack_sk(uint8 rho[SEEDBYTES_DILITHIUM],
                                uint8 tr[TRBYTES_DILITHIUM],
                                uint8 key[SEEDBYTES_DILITHIUM],
                                polyveck_D2 *t0,
                                polyvecl_D2 *s1,
                                polyveck_D2 *s2,
                                const uint8 sk[FSMSW_DILITHIUM2_CRYPTO_SECRETKEYBYTES])
{
    uint16 i;

    for (i = 0; i < SEEDBYTES_DILITHIUM; ++i)
    {
        rho[i] = sk[i];
    }
    sk = &sk[SEEDBYTES_DILITHIUM];

    for (i = 0; i < SEEDBYTES_DILITHIUM; ++i)
    {
        key[i] = sk[i];
    }
    sk = &sk[SEEDBYTES_DILITHIUM];

    for (i = 0; i < TRBYTES_DILITHIUM; ++i)
    {
        tr[i] = sk[i];
    }
    sk = &sk[TRBYTES_DILITHIUM];

    for (i = 0; i < L_DILITHIUM2; ++i)
    {
        FsmSw_Dilithium2_polyeta_unpack(&s1->vec[i], &sk[i * POLYETA_PACKEDBYTES_DILITHIUM2]);
    }
    sk = &sk[L_DILITHIUM2 * POLYETA_PACKEDBYTES_DILITHIUM2];

    for (i = 0; i < K_DILITHIUM2; ++i)
    {
        FsmSw_Dilithium2_polyeta_unpack(&s2->vec[i], &sk[i * POLYETA_PACKEDBYTES_DILITHIUM2]);
    }
    sk = &sk[K_DILITHIUM2 * POLYETA_PACKEDBYTES_DILITHIUM2];

    for (i = 0; i < K_DILITHIUM2; ++i)
    {
        FsmSw_Dilithium2_polyt0_unpack(&t0->vec[i], &sk[i * POLYT0_PACKEDBYTES_DILITHIUM]);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium2_pack_sig
*
* Description: Bit-pack signature sig = (c, z, h).
*
* Arguments:   -       uint8        sig[]: output byte array
*              - const uint8       *c:     pointer to FsmSw_Dilithium2_challenge hash length SEEDBYTES_DILITHIUM
*              - const polyvecl_D2 *z:     pointer to vector z
*              - const polyveck_D2 *h:     pointer to hint vector h
***********************************************************************************************************************/
void FsmSw_Dilithium2_pack_sig(uint8 sig[FSMSW_DILITHIUM2_CRYPTO_BYTES],
                               const uint8 c[CTILDEBYTES_DILITHIUM2],
                               const polyvecl_D2 *z,
                               const polyveck_D2 *h)
{
    uint16 i, j, k;

    for (i = 0; i < CTILDEBYTES_DILITHIUM2; ++i)
    {
        sig[i] = c[i];
    }
    sig = &sig[CTILDEBYTES_DILITHIUM2];

    for (i = 0; i < L_DILITHIUM2; ++i)
    {
        FsmSw_Dilithium2_polyz_pack(&sig[i * POLYZ_PACKEDBYTES_DILITHIUM2], &z->vec[i]);
    }
    sig = &sig[L_DILITHIUM2 * POLYZ_PACKEDBYTES_DILITHIUM2];

    /* Encode h */
    for (i = 0; i < OMEGA_DILITHIUM2 + K_DILITHIUM2; ++i)
    {
        sig[i] = 0;
    }

    k = 0;
    for (i = 0; i < K_DILITHIUM2; ++i)
    {
        for (j = 0; j < N_DILITHIUM; ++j)
        {
            if (h->vec[i].coeffs[j] != 0)
            {
                sig[k++] = (uint8) j;
            }
        }

        sig[OMEGA_DILITHIUM2 + i] = (uint8) k;
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium2_unpack_sig
*
* Description: Unpack signature sig = (c, z, h).
*
* Arguments:   -       uint8       *c:     pointer to output FsmSw_Dilithium2_challenge hash
*              -       polyvecl_D2 *z:     pointer to output vector z
*              -       polyveck_D2 *h:     pointer to output hint vector h
*              - const uint8        sig[]: byte array containing bit-packed signature
*
* Returns 1 in case of malformed signature; otherwise 0.
***********************************************************************************************************************/
sint8 FsmSw_Dilithium2_unpack_sig(uint8 c[CTILDEBYTES_DILITHIUM2],
                                polyvecl_D2 *z,
                                polyveck_D2 *h,
                                const uint8 sig[FSMSW_DILITHIUM2_CRYPTO_BYTES])
{
    uint16 i, j, k;

    for (i = 0; i < CTILDEBYTES_DILITHIUM2; ++i)
    {
        c[i] = sig[i];
    }
    sig = &sig[CTILDEBYTES_DILITHIUM2];

    for (i = 0; i < L_DILITHIUM2; ++i)
    {
        FsmSw_Dilithium2_polyz_unpack(&z->vec[i], &sig[i * POLYZ_PACKEDBYTES_DILITHIUM2]);
    }
    sig = &sig[L_DILITHIUM2 * POLYZ_PACKEDBYTES_DILITHIUM2];

    /* Decode h */
    k = 0;
    for (i = 0; i < K_DILITHIUM2; ++i)
    {
        for (j = 0; j < N_DILITHIUM; ++j)
        {
            h->vec[i].coeffs[j] = 0;
        }

        if (sig[OMEGA_DILITHIUM2 + i] < k || sig[OMEGA_DILITHIUM2 + i] > OMEGA_DILITHIUM2)
        {
            return 1;
        }

        for (j = k; j < sig[OMEGA_DILITHIUM2 + i]; ++j)
        {
            /* Coefficients are ordered for strong unforgeability */
            if (j > k && sig[j] <= sig[j - 1u])
            {
                return 1;
            }
            h->vec[i].coeffs[sig[j]] = 1;
        }

        k = sig[OMEGA_DILITHIUM2 + i];
    }

    /* Extra indices are zero for strong unforgeability */
    for (j = k; j < OMEGA_DILITHIUM2; ++j)
    {
        if (0u < sig[j])
        {
            return 1;
        }
    }

    return 0;
}
