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
#include "FsmSw_Dilithium2_params.h"
#include "FsmSw_Dilithium2_poly.h"
#include "FsmSw_Dilithium2_polyvec.h"
#include "FsmSw_Types.h"

#include "FsmSw_Dilithium2_packing.h"
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
* Name:        FsmSw_Dilithium2_Pack_Pk
*
* Description: Bit-pack public key pk = (rho, t1).
*
* Arguments:   -       uint8        pk[]:  output byte array
*              - const uint8        rho[]: byte array containing rho
*              - const polyveck_D2 *t1:    pointer to vector t1
***********************************************************************************************************************/
void FsmSw_Dilithium2_Pack_Pk(uint8 pk[FSMSW_DILITHIUM2_CRYPTO_PUBLICKEYBYTES], const uint8 rho[SEEDBYTES_DILITHIUM],
                              const polyveck_D2 *t1)
{
  uint16 i = 0;
  /* pk_temp is used to avoid modifying the input. */
  uint8 *pk_temp = pk;

  for (i = 0; i < SEEDBYTES_DILITHIUM; ++i)
  {
    pk_temp[i] = rho[i];
  }
  pk_temp = &pk_temp[SEEDBYTES_DILITHIUM];

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_T1Pack(&pk_temp[i * POLYT1_PACKEDBYTES_DILITHIUM], &t1->vec[i]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium2_Unpack_Pk
*
* Description: Unpack public key pk = (rho, t1).
*
* Arguments:   -       uint8        rho[]: output byte array for rho
*              -       polyveck_D2 *t1:    pointer to output vector t1
*              - const uint8        pk[]:  byte array containing bit-packed pk
***********************************************************************************************************************/
void FsmSw_Dilithium2_Unpack_Pk(uint8 rho[SEEDBYTES_DILITHIUM], polyveck_D2 *t1,
                                const uint8 pk[FSMSW_DILITHIUM2_CRYPTO_PUBLICKEYBYTES])
{
  uint16 i = 0;
  /* pk_temp is used to avoid modifying the input. */
  const uint8 *pk_temp = pk;

  for (i = 0; i < SEEDBYTES_DILITHIUM; ++i)
  {
    rho[i] = pk_temp[i];
  }
  pk_temp = &pk_temp[SEEDBYTES_DILITHIUM];

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_T1Unpack(&t1->vec[i], &pk_temp[i * POLYT1_PACKEDBYTES_DILITHIUM]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium2_Pack_Sk
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
void FsmSw_Dilithium2_Pack_Sk(uint8 sk[FSMSW_DILITHIUM2_CRYPTO_SECRETKEYBYTES], const uint8 rho[SEEDBYTES_DILITHIUM],
                              const uint8 tr[TRBYTES_DILITHIUM], const uint8 key[SEEDBYTES_DILITHIUM],
                              const polyveck_D2 *t0, const polyvecl_D2 *s1, const polyveck_D2 *s2)
{
  uint16 i = 0;
  /* sk_temp is used to avoid modifying the input. */
  uint8 *sk_temp = sk;

  for (i = 0; i < SEEDBYTES_DILITHIUM; ++i)
  {
    sk_temp[i] = rho[i];
  }
  sk_temp = &sk_temp[SEEDBYTES_DILITHIUM];

  for (i = 0; i < SEEDBYTES_DILITHIUM; ++i)
  {
    sk_temp[i] = key[i];
  }
  sk_temp = &sk_temp[SEEDBYTES_DILITHIUM];

  for (i = 0; i < TRBYTES_DILITHIUM; ++i)
  {
    sk_temp[i] = tr[i];
  }
  sk_temp = &sk_temp[TRBYTES_DILITHIUM];

  for (i = 0; i < L_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Polyeta_EtaPack(&sk_temp[i * POLYETA_PACKEDBYTES_DILITHIUM2], &s1->vec[i]);
  }
  sk_temp = &sk_temp[L_DILITHIUM2 * POLYETA_PACKEDBYTES_DILITHIUM2];

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Polyeta_EtaPack(&sk_temp[i * POLYETA_PACKEDBYTES_DILITHIUM2], &s2->vec[i]);
  }
  sk_temp = &sk_temp[K_DILITHIUM2 * POLYETA_PACKEDBYTES_DILITHIUM2];

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_T0Pack(&sk_temp[i * POLYT0_PACKEDBYTES_DILITHIUM], &t0->vec[i]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium2_Unpack_Sk
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
void FsmSw_Dilithium2_Unpack_Sk(uint8 rho[SEEDBYTES_DILITHIUM], uint8 tr[TRBYTES_DILITHIUM],
                                uint8 key[SEEDBYTES_DILITHIUM], polyveck_D2 *t0, polyvecl_D2 *s1, polyveck_D2 *s2,
                                const uint8 sk[FSMSW_DILITHIUM2_CRYPTO_SECRETKEYBYTES])
{
  uint16 i = 0;
  /* pk_temp is used to avoid modifying the input. */
  const uint8 *sk_temp = sk;

  for (i = 0; i < SEEDBYTES_DILITHIUM; ++i)
  {
    rho[i] = sk_temp[i];
  }
  sk_temp = &sk_temp[SEEDBYTES_DILITHIUM];

  for (i = 0; i < SEEDBYTES_DILITHIUM; ++i)
  {
    key[i] = sk_temp[i];
  }
  sk_temp = &sk_temp[SEEDBYTES_DILITHIUM];

  for (i = 0; i < TRBYTES_DILITHIUM; ++i)
  {
    tr[i] = sk_temp[i];
  }
  sk_temp = &sk_temp[TRBYTES_DILITHIUM];

  for (i = 0; i < L_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Polyeta_EtaUnpack(&s1->vec[i], &sk_temp[i * POLYETA_PACKEDBYTES_DILITHIUM2]);
  }
  sk_temp = &sk_temp[L_DILITHIUM2 * POLYETA_PACKEDBYTES_DILITHIUM2];

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Polyeta_EtaUnpack(&s2->vec[i], &sk_temp[i * POLYETA_PACKEDBYTES_DILITHIUM2]);
  }
  sk_temp = &sk_temp[K_DILITHIUM2 * POLYETA_PACKEDBYTES_DILITHIUM2];

  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_T0Unpack(&t0->vec[i], &sk_temp[i * POLYT0_PACKEDBYTES_DILITHIUM]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium2_Pack_Sig
*
* Description: Bit-pack signature sig = (c, z, h).
*
* Arguments:   -       uint8        sig[]: output byte array
*              - const uint8       *c:     pointer to FsmSw_Dilithium2_Challenge hash length SEEDBYTES_DILITHIUM
*              - const polyvecl_D2 *z:     pointer to vector z
*              - const polyveck_D2 *h:     pointer to hint vector h
***********************************************************************************************************************/
void FsmSw_Dilithium2_Pack_Sig(uint8 sig[FSMSW_DILITHIUM2_CRYPTO_BYTES], const uint8 c[CTILDEBYTES_DILITHIUM2],
                               const polyvecl_D2 *z, const polyveck_D2 *h)
{
  uint16 i = 0;
  uint16 j = 0;
  uint16 k = 0;
  /* sig_temp is used to avoid modifying the input. */
  uint8 *sig_temp = sig;

  for (i = 0; i < CTILDEBYTES_DILITHIUM2; ++i)
  {
    sig_temp[i] = c[i];
  }
  sig_temp = &sig_temp[CTILDEBYTES_DILITHIUM2];

  for (i = 0; i < L_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_ZPack(&sig_temp[i * POLYZ_PACKEDBYTES_DILITHIUM2], &z->vec[i]);
  }
  sig_temp = &sig_temp[L_DILITHIUM2 * POLYZ_PACKEDBYTES_DILITHIUM2];

  /* Encode h */
  for (i = 0; i < (OMEGA_DILITHIUM2 + K_DILITHIUM2); ++i)
  {
    sig_temp[i] = 0;
  }

  k = 0;
  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    for (j = 0; j < N_DILITHIUM; ++j)
    {
      if (h->vec[i].coeffs[j] != 0)
      {
        sig_temp[k] = (uint8)j;
        k++;
      }
    }

    sig_temp[OMEGA_DILITHIUM2 + i] = (uint8)k;
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium2_Unpack_Sig
*
* Description: Unpack signature sig = (c, z, h).
*
* Arguments:   -       uint8       *c:     pointer to output FsmSw_Dilithium2_Challenge hash
*              -       polyvecl_D2 *z:     pointer to output vector z
*              -       polyveck_D2 *h:     pointer to output hint vector h
*              - const uint8        sig[]: byte array containing bit-packed signature
*
* Returns 1 in case of malformed signature; otherwise 0.
***********************************************************************************************************************/
sint8 FsmSw_Dilithium2_Unpack_Sig(uint8 c[CTILDEBYTES_DILITHIUM2], polyvecl_D2 *z, polyveck_D2 *h,
                                  const uint8 sig[FSMSW_DILITHIUM2_CRYPTO_BYTES])
{
  uint16 i     = 0;
  uint16 j     = 0;
  uint16 k     = 0;
  sint8 retVal = 0;
  /* sig_temp is used to avoid modifying the input. */
  const uint8 *sig_temp = sig;

  for (i = 0; i < CTILDEBYTES_DILITHIUM2; ++i)
  {
    c[i] = sig_temp[i];
  }
  sig_temp = &sig_temp[CTILDEBYTES_DILITHIUM2];

  for (i = 0; i < L_DILITHIUM2; ++i)
  {
    FsmSw_Dilithium2_Poly_ZUnpack(&z->vec[i], &sig_temp[i * POLYZ_PACKEDBYTES_DILITHIUM2]);
  }
  sig_temp = &sig_temp[L_DILITHIUM2 * POLYZ_PACKEDBYTES_DILITHIUM2];

  /* Decode h */
  k = 0;
  for (i = 0; i < K_DILITHIUM2; ++i)
  {
    for (j = 0; j < N_DILITHIUM; ++j)
    {
      h->vec[i].coeffs[j] = 0;
    }

    if ((sig_temp[OMEGA_DILITHIUM2 + i] < k) || (sig_temp[OMEGA_DILITHIUM2 + i] > OMEGA_DILITHIUM2))
    {
      retVal = 1;
    }

    for (j = k; j < sig_temp[OMEGA_DILITHIUM2 + i]; ++j)
    {
      /* Coefficients are ordered for strong unforgeability */
      if ((j > k) && (sig_temp[j] <= sig_temp[j - 1u]))
      {
        retVal = 1;
      }
      h->vec[i].coeffs[sig_temp[j]] = 1;
    }

    k = sig_temp[OMEGA_DILITHIUM2 + i];
  }

  /* Extra indices are zero for strong unforgeability */
  for (j = k; j < OMEGA_DILITHIUM2; ++j)
  {
    if (0u < sig_temp[j])
    {
      retVal = 1;
    }
  }

  return retVal;
}
