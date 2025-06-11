/***********************************************************************************************************************
 *
 *                                          IAV GmbH
 *
 **********************************************************************************************************************/
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
#include "FsmSw_CommonLib.h"
#include "FsmSw_Dilithium5_packing.h"
#include "FsmSw_Dilithium5_params.h"
#include "FsmSw_Dilithium5_poly.h"
#include "FsmSw_Dilithium5_polyvec.h"
#include "FsmSw_Dilithium_symmetric.h"
#include "FsmSw_Fips202.h"

#include "FsmSw_Dilithium5_sign.h"
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
 * Name:        FsmSw_Dilithium5_Crypto_Sign_KeyPair
 *
 * Description: Generates public and private key.
 *
 * Arguments:   - uint8 *pk: pointer to output public key
 *                           (allocated array of FSMSW_DILITHIUM5_CRYPTO_PUBLICKEYBYTES bytes)
 *              - uint8 *sk: pointer to output private key (allocated
 *                           array of FSMSW_DILITHIUM5_CRYPTO_SECRETKEYBYTES bytes)
 *
 * Returns 0 (success)
 **********************************************************************************************************************/
sint8 FsmSw_Dilithium5_Crypto_Sign_KeyPair(uint8 *pk, uint8 *sk)
{
  uint8 seedbuf[(2u * SEEDBYTES_DILITHIUM) + CRHBYTES_DILITHIUM] = {0};
  uint8 tr[TRBYTES_DILITHIUM]                                    = {0};
  const uint8 *rho                                               = (uint8 *)NULL_PTR;
  const uint8 *rhoprime                                          = (uint8 *)NULL_PTR;
  const uint8 *key                                               = (uint8 *)NULL_PTR;
  polyvecl_D5 mat[K_DILITHIUM5]                                  = {{{{{0}}}}};
  polyvecl_D5 s1                                                 = {{{{0}}}};
  polyvecl_D5 s1hat                                              = {{{{0}}}};
  polyveck_D5 s2                                                 = {{{{0}}}};
  polyveck_D5 t1                                                 = {{{{0}}}};
  polyveck_D5 t0                                                 = {{{{0}}}};

  /* Get randomness for rho, rhoprime and key */
  (void)FsmSw_CommonLib_RandomBytes(seedbuf, SEEDBYTES_DILITHIUM);
  FsmSw_Fips202_Shake256(seedbuf, (2u * SEEDBYTES_DILITHIUM) + CRHBYTES_DILITHIUM, seedbuf, SEEDBYTES_DILITHIUM);
  rho      = seedbuf;
  rhoprime = &rho[SEEDBYTES_DILITHIUM];
  key      = &rhoprime[CRHBYTES_DILITHIUM];

  /* Expand matrix */
  FsmSw_Dilithium5_Polyvec_Matrix_Expand(mat, rho);

  /* Sample short vectors s1 and s2 */
  FsmSw_Dilithium5_Polyvecl_Uniform_Eta(&s1, rhoprime, 0u);
  FsmSw_Dilithium5_Polyveck_Uniform_Eta(&s2, rhoprime, L_DILITHIUM5);

  /* Matrix-vector multiplication */
  s1hat = s1;
  FsmSw_Dilithium5_Polyvecl_Ntt(&s1hat);
  FsmSw_Dilithium5_Polyvec_Matrix_Pointwise_Montgomery(&t1, mat, &s1hat);
  FsmSw_Dilithium5_Polyveck_Reduce(&t1);
  FsmSw_Dilithium5_Polyveck_Invntt_Tomont(&t1);

  /* Add error vector s2 */
  FsmSw_Dilithium5_Polyveck_Add(&t1, &t1, &s2);

  /* Extract t1 and write public key */
  FsmSw_Dilithium5_Polyveck_CAddQ(&t1);
  FsmSw_Dilithium5_Polyveck_Power2Round(&t1, &t0, &t1);
  FsmSw_Dilithium5_PackPk(pk, rho, &t1);

  /* Compute H(rho, t1) and write secret key */
  FsmSw_Fips202_Shake256(tr, TRBYTES_DILITHIUM, pk, FSMSW_DILITHIUM5_CRYPTO_PUBLICKEYBYTES);
  FsmSw_Dilithium5_PackSk(sk, rho, tr, key, &t0, &s1, &s2);

  return 0;
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium5_Crypto_Sign_Signature
 *
 * Description: Computes signature.
 *
 * Arguments:   - uint8 *sig:     pointer to output signature (of length FSMSW_DILITHIUM5_CRYPTO_BYTES)
 *              - uint32 *siglen: pointer to output length of signature
 *              - uint8 *m:       pointer to message to be signed
 *              - uint32 mlen:    length of message
 *              - uint8 *sk:      pointer to bit-packed secret key
 *
 * Returns 0 (success)
 **********************************************************************************************************************/
/* polyspace +2 MISRA2012:8.7 [Justified:]"This is an interface function
designed for use by other systems that aim to integrate the Dilithium." */
sint8 FsmSw_Dilithium5_Crypto_Sign_Signature(uint8 *sig, uint32 *siglen, const uint8 *m, uint32 mlen, const uint8 *sk)
{
  uint32 n                                                                                                       = 0;
  uint8 seedbuf[(2u * SEEDBYTES_DILITHIUM) + TRBYTES_DILITHIUM + RNDBYTES_DILITHIUM + (2u * CRHBYTES_DILITHIUM)] = {0};
  uint8 *rho                    = (uint8 *)NULL_PTR;
  uint8 *tr                     = (uint8 *)NULL_PTR;
  uint8 *key                    = (uint8 *)NULL_PTR;
  uint8 *mu                     = (uint8 *)NULL_PTR;
  uint8 *rhoprime               = (uint8 *)NULL_PTR;
  uint8 *rnd                    = (uint8 *)NULL_PTR;
  uint16 nonce                  = 0;
  polyvecl_D5 mat[K_DILITHIUM5] = {{{{{0}}}}};
  polyvecl_D5 s1                = {{{{0}}}};
  polyvecl_D5 y                 = {{{{0}}}};
  polyvecl_D5 z                 = {{{{0}}}};
  polyveck_D5 t0                = {{{{0}}}};
  polyveck_D5 s2                = {{{{0}}}};
  polyveck_D5 w1                = {{{{0}}}};
  polyveck_D5 w0                = {{{{0}}}};
  polyveck_D5 h                 = {{{{0}}}};
  poly_D5 cp                    = {{0}};
  shake256incctx state          = {{0}};
  boolean loop                  = TRUE;

  rho      = seedbuf;
  tr       = &rho[SEEDBYTES_DILITHIUM];
  key      = &tr[TRBYTES_DILITHIUM];
  rnd      = &key[SEEDBYTES_DILITHIUM];
  mu       = &rnd[RNDBYTES_DILITHIUM];
  rhoprime = &mu[CRHBYTES_DILITHIUM];
  FsmSw_Dilithium5_UnpackSk(rho, tr, key, &t0, &s1, &s2, sk);

  /* Compute mu = CRH(tr, msg) */
  FsmSw_Fips202_Shake256_IncInit(&state);
  FsmSw_Fips202_Shake256_IncAbsorb(&state, tr, TRBYTES_DILITHIUM);
  FsmSw_Fips202_Shake256_IncAbsorb(&state, m, mlen);
  FsmSw_Fips202_Shake256_IncFinalize(&state);
  FsmSw_Fips202_Shake256_IncSqueeze(mu, CRHBYTES_DILITHIUM, &state);

  for (n = 0; n < RNDBYTES_DILITHIUM; n++)
  {
    rnd[n] = 0;
  }
  FsmSw_Fips202_Shake256(rhoprime, CRHBYTES_DILITHIUM, key,
                         SEEDBYTES_DILITHIUM + RNDBYTES_DILITHIUM + CRHBYTES_DILITHIUM);

  /* Expand matrix and transform vectors */
  FsmSw_Dilithium5_Polyvec_Matrix_Expand(mat, rho);
  FsmSw_Dilithium5_Polyvecl_Ntt(&s1);
  FsmSw_Dilithium5_Polyveck_Ntt(&s2);
  FsmSw_Dilithium5_Polyveck_Ntt(&t0);

  while (TRUE == loop)
  {
    /* Sample intermediate vector y */
    FsmSw_Dilithium5_Polyvecl_Uniform_Gamma1(&y, rhoprime, nonce);
    nonce++;

    /* Matrix-vector multiplication */
    z = y;
    FsmSw_Dilithium5_Polyvecl_Ntt(&z);
    FsmSw_Dilithium5_Polyvec_Matrix_Pointwise_Montgomery(&w1, mat, &z);
    FsmSw_Dilithium5_Polyveck_Reduce(&w1);
    FsmSw_Dilithium5_Polyveck_Invntt_Tomont(&w1);

    /* Decompose w and call the random oracle */
    FsmSw_Dilithium5_Polyveck_CAddQ(&w1);
    FsmSw_Dilithium5_Polyveck_Decompose(&w1, &w0, &w1);
    FsmSw_Dilithium5_Polyveck_Pack_W1(sig, &w1);

    FsmSw_Fips202_Shake256_IncInit(&state);
    FsmSw_Fips202_Shake256_IncAbsorb(&state, mu, CRHBYTES_DILITHIUM);
    FsmSw_Fips202_Shake256_IncAbsorb(&state, sig, K_DILITHIUM5 * POLYW1_PACKEDBYTES_DILITHIUM5);
    FsmSw_Fips202_Shake256_IncFinalize(&state);
    FsmSw_Fips202_Shake256_IncSqueeze(sig, CTILDEBYTES_DILITHIUM5, &state);
    FsmSw_Dilithium5_Poly_Challenge(&cp, sig); /* uses only the first SEEDBYTES bytes of sig */
    FsmSw_Dilithium5_Poly_Ntt(&cp);

    /* Compute z, reject if it reveals secret */
    FsmSw_Dilithium5_Polyvecl_Pointwise_Poly_Montgomery(&z, &cp, &s1);
    FsmSw_Dilithium5_Polyvecl_Invntt_Tomont(&z);
    FsmSw_Dilithium5_Polyvecl_Add(&z, &z, &y);
    FsmSw_Dilithium5_Polyvecl_Reduce(&z);
    if (0 < FsmSw_Dilithium5_Polyvecl_Chknorm(&z, (sint32)(GAMMA1_DILITHIUM5 - BETA_DILITHIUM5)))
    {
      continue;
    }

    /* Check that subtracting cs2 does not change high bits of w and low bits
         * do not reveal secret information */
    FsmSw_Dilithium5_Polyveck_Pointwise_Poly_Montgomery(&h, &cp, &s2);
    FsmSw_Dilithium5_Polyveck_Invntt_Tomont(&h);
    FsmSw_Dilithium5_Polyveck_Sub(&w0, &w0, &h);
    FsmSw_Dilithium5_Polyveck_Reduce(&w0);
    if (0 < FsmSw_Dilithium5_Polyveck_Chknorm(&w0, (sint32)((uint32)((uint32)GAMMA2_DILITHIUM5 - BETA_DILITHIUM5))))
    {
      continue;
    }

    /* Compute hints for w1 */
    FsmSw_Dilithium5_Polyveck_Pointwise_Poly_Montgomery(&h, &cp, &t0);
    FsmSw_Dilithium5_Polyveck_Invntt_Tomont(&h);
    FsmSw_Dilithium5_Polyveck_Reduce(&h);
    if (0 < FsmSw_Dilithium5_Polyveck_Chknorm(&h, (sint32)GAMMA2_DILITHIUM5))
    {
      continue;
    }

    FsmSw_Dilithium5_Polyveck_Add(&w0, &w0, &h);
    n = FsmSw_Dilithium5_Polyveck_Make_Hint(&h, &w0, &w1);
    if (n > OMEGA_DILITHIUM5)
    {
      continue;
    }

    loop = FALSE;
  }

  /* Write signature */
  FsmSw_Dilithium5_PackSig(sig, sig, &z, &h);
  *siglen = FSMSW_DILITHIUM5_CRYPTO_BYTES;
  return 0;
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium5_Crypto_Sign
 *
 * Description: Compute signed message.
 *
 * Arguments:   -       uint8 *sm:     pointer to output signed message (allocated
 *                                     array with FSMSW_DILITHIUM5_CRYPTO_BYTES + mlen bytes), can be equal to m
 *              -       uint32 *smlen: pointer to output length of signed message
 *              - const uint8 *m:      pointer to message to be signed
 *              -       uint32 mlen:   length of message
 *              - const uint8 *sk:     pointer to bit-packed secret key
 *
 * Returns 0 (success)
 **********************************************************************************************************************/
sint8 FsmSw_Dilithium5_Crypto_Sign(uint8 *sm, uint32 *smlen, const uint8 *m, uint32 mlen, const uint8 *sk)
{
  uint32 i = 0;

  for (i = 0; i < mlen; ++i)
  {
    sm[FSMSW_DILITHIUM5_CRYPTO_BYTES + mlen - 1u - i] = m[mlen - 1u - i];
  }
  (void)FsmSw_Dilithium5_Crypto_Sign_Signature(sm, smlen, &sm[FSMSW_DILITHIUM5_CRYPTO_BYTES], mlen, sk);
  *smlen += mlen;
  return 0;
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium5_Crypto_Sign_Verify
 *
 * Description: Verifies signature.
 *
 * Arguments:   -       uint8 *m:      pointer to input signature
 *              -       uint32 siglen: length of signature
 *              - const uint8 *m:      pointer to message
 *              -       uint32 mlen:   length of message
 *              - const uint8 *pk:     pointer to bit-packed public key
 *
 * Returns 0 if signature could be verified correctly and -1 otherwise
 **********************************************************************************************************************/
/* polyspace +2 MISRA2012:8.7 [Justified:]"This is an interface function
designed for use by other systems that aim to integrate the Dilithium." */
sint8 FsmSw_Dilithium5_Crypto_Sign_Verify(const uint8 *sig, uint32 siglen, const uint8 *m, uint32 mlen, const uint8 *pk)
{
  uint16 i                                                = 0;
  uint8 buf[K_DILITHIUM5 * POLYW1_PACKEDBYTES_DILITHIUM5] = {0};
  uint8 rho[SEEDBYTES_DILITHIUM]                          = {0};
  uint8 mu[CRHBYTES_DILITHIUM]                            = {0};
  uint8 c[CTILDEBYTES_DILITHIUM5]                         = {0};
  uint8 c2[CTILDEBYTES_DILITHIUM5]                        = {0};
  poly_D5 cp                                              = {{0}};
  polyvecl_D5 mat[K_DILITHIUM5]                           = {{{{{0}}}}};
  polyvecl_D5 z                                           = {{{{0}}}};
  polyveck_D5 t1                                          = {{{{0}}}};
  polyveck_D5 w1                                          = {{{{0}}}};
  polyveck_D5 h                                           = {{{{0}}}};
  shake256incctx state                                    = {{0}};
  sint8 retVal                                            = 0;

  if (siglen != FSMSW_DILITHIUM5_CRYPTO_BYTES)
  {
    retVal = -1;
  }

  FsmSw_Dilithium5_UnpackPk(rho, &t1, pk);
  if (0 < FsmSw_Dilithium5_UnpackSig(c, &z, &h, sig))
  {
    retVal = -1;
  }
  if (0 < FsmSw_Dilithium5_Polyvecl_Chknorm(&z, (sint32)(GAMMA1_DILITHIUM5 - BETA_DILITHIUM5)))
  {
    retVal = -1;
  }

  /* Compute CRH(H(rho, t1), msg) */
  FsmSw_Fips202_Shake256(mu, CRHBYTES_DILITHIUM, pk, FSMSW_DILITHIUM5_CRYPTO_PUBLICKEYBYTES);
  FsmSw_Fips202_Shake256_IncInit(&state);
  FsmSw_Fips202_Shake256_IncAbsorb(&state, mu, CRHBYTES_DILITHIUM);
  FsmSw_Fips202_Shake256_IncAbsorb(&state, m, mlen);
  FsmSw_Fips202_Shake256_IncFinalize(&state);
  FsmSw_Fips202_Shake256_IncSqueeze(mu, CRHBYTES_DILITHIUM, &state);

  /* Matrix-vector multiplication; compute Az - c2^dt1 */
  FsmSw_Dilithium5_Poly_Challenge(&cp, c); /* uses only the first SEEDBYTES bytes of c */
  FsmSw_Dilithium5_Polyvec_Matrix_Expand(mat, rho);

  FsmSw_Dilithium5_Polyvecl_Ntt(&z);
  FsmSw_Dilithium5_Polyvec_Matrix_Pointwise_Montgomery(&w1, mat, &z);

  FsmSw_Dilithium5_Poly_Ntt(&cp);
  FsmSw_Dilithium5_Polyveck_Shiftl(&t1);
  FsmSw_Dilithium5_Polyveck_Ntt(&t1);
  FsmSw_Dilithium5_Polyveck_Pointwise_Poly_Montgomery(&t1, &cp, &t1);

  FsmSw_Dilithium5_Polyveck_Sub(&w1, &w1, &t1);
  FsmSw_Dilithium5_Polyveck_Reduce(&w1);
  FsmSw_Dilithium5_Polyveck_Invntt_Tomont(&w1);

  /* Reconstruct w1 */
  FsmSw_Dilithium5_Polyveck_CAddQ(&w1);
  FsmSw_Dilithium5_Polyveck_Use_Hint(&w1, &w1, &h);
  FsmSw_Dilithium5_Polyveck_Pack_W1(buf, &w1);

  /* Call random oracle and verify FsmSw_Dilithium5_Challenge */
  FsmSw_Fips202_Shake256_IncInit(&state);
  FsmSw_Fips202_Shake256_IncAbsorb(&state, mu, CRHBYTES_DILITHIUM);
  FsmSw_Fips202_Shake256_IncAbsorb(&state, buf, K_DILITHIUM5 * POLYW1_PACKEDBYTES_DILITHIUM5);
  FsmSw_Fips202_Shake256_IncFinalize(&state);
  FsmSw_Fips202_Shake256_IncSqueeze(c2, CTILDEBYTES_DILITHIUM5, &state);
  for (i = 0; i < CTILDEBYTES_DILITHIUM5; ++i)
  {
    if (c[i] != c2[i])
    {
      retVal = -1;
    }
  }
  return retVal;
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium5_Crypto_Sign_Open
 *
 * Description: Verify signed message.
 *
 * Arguments:   -       uint8  *m:     pointer to output message (allocated array with smlen bytes), can be equal to sm
 *              -       uint32 *mlen:  pointer to output length of message
 *              - const uint8  *sm:    pointer to signed message
 *              -       uint32  smlen: length of signed message
 *              - const uint8  *pk:    pointer to bit-packed public key
 *
 * Returns 0 if signed message could be verified correctly and -1 otherwise
 **********************************************************************************************************************/
sint8 FsmSw_Dilithium5_Crypto_Sign_Open(uint8 *m, uint32 *mlen, const uint8 *sm, uint32 smlen, const uint8 *pk)
{
  uint32 i     = 0;
  sint8 retVal = -1;

  if (smlen >= FSMSW_DILITHIUM5_CRYPTO_BYTES)
  {
    *mlen = smlen - FSMSW_DILITHIUM5_CRYPTO_BYTES;
    if (0 == FsmSw_Dilithium5_Crypto_Sign_Verify(sm, FSMSW_DILITHIUM5_CRYPTO_BYTES, &sm[FSMSW_DILITHIUM5_CRYPTO_BYTES],
                                                 *mlen, pk))
    {
      /* All good, copy msg, return 0 */
      for (i = 0; i < *mlen; ++i)
      {
        m[i] = sm[FSMSW_DILITHIUM5_CRYPTO_BYTES + i];
      }
      retVal = 0;
    }
  }

  if ((sint8)0u != retVal)
  {
    /* Signature verification failed */
    *mlen = UINT32_MAXVAL;
    for (i = 0; i < smlen; ++i)
    {
      m[i] = 0;
    }
  }

  return retVal;
}
