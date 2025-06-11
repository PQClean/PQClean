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
/* Wrapper for implementing the PQClean API. */
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_CommonLib.h"
#include "FsmSw_Falcon_codec.h"
#include "FsmSw_Falcon_common.h"
#include "FsmSw_Falcon_fpr.h"
#include "FsmSw_Falcon_keygen.h"
#include "FsmSw_Falcon_sign.h"
#include "FsmSw_Falcon_vrfy.h"

#include "FsmSw_Falcon512_api.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define FSMSW_FALCON512_NONCELEN 40u

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
typedef struct
{
  uint8 b[72 * 512];
} tmp1_512_struct;

typedef struct
{
  uint8 b[FALCON_KEYGEN_TEMP_9];
} tmp2_512_struct;

typedef struct
{
  uint8 b[2 * 512];
} tmp3_512_struct;

typedef struct
{
  sint16 sig[512];
  uint16 hm[512];
} r_512_struct;
/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/
static r_512_struct r_512;
/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/
static sint32 fsmsw_falcon512_DoSign(uint8 *nonce, uint8 *sigbuf, uint32 *sigbuflen, const uint8 *m, uint32 mlen,
                                     const uint8 *sk);
static sint32 fsmsw_falcon512_DoVerify(const uint8 *nonce, const uint8 *sigbuf, uint32 sigbuflen, const uint8 *m,
                                       uint32 mlen, const uint8 *pk);
/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/
/***********************************************************************************************************************
 * Name:        fsmsw_falcon512_DoSign
 *
 * Description: If a signature could be computed but not encoded because it would exceed the output buffer size, then a
 *              new signature is computed. If the provided buffer size is too low, this could loop indefinitely, so the
 *              caller must provide a size that can accommodate signatures with a large enough probability.
 *
 * Arguments:   -       uint8  *nonce:     receives the nonce and must have length FSMSW_FALCON1024_NONCELEN bytes
 *              -       uint8  *sigbuf:    receives the signature value (without nonce or header byte)
 *              -       uint32 *sigbuflen: providing the maximum value length and receiving the actual value length
 *              - const uint8  *m:         t.b.d.
 *              -       uint32  mlen:      t.b.d.
 *              - const uint8  *sk:        t.b.d.
 *
 * Returns 0 on success, -1 on error.
 *
 **********************************************************************************************************************/
static sint32 fsmsw_falcon512_DoSign(uint8 *nonce, uint8 *sigbuf, uint32 *sigbuflen, const uint8 *m, uint32 mlen,
                                     const uint8 *sk)
{
  sint8 f[512]              = {0};
  sint8 g[512]              = {0};
  sint8 F[512]              = {0};
  sint8 G[512]              = {0};
  uint8 seed[48]            = {0};
  inner_shake256_context sc = {{0}};
  uint32 u                  = 0;
  uint32 v                  = 0;
  boolean bStopFunc         = FALSE;
  sint8 retVal              = -1;
  tmp1_512_struct tmp1_512;

  /* Decode the private key. */
  if (sk[0] != (0x50u + 9u))
  {
    bStopFunc = TRUE;
  }

  u = 1;
  v = FsmSw_Falcon_TrimI8Decode(f, 9, FsmSw_Falcon_max_small_fg_bits[9], &sk[u],
                                FSMSW_FALCON512_CRYPTO_SECRETKEYBYTES - u);

  if (v == 0u)
  {
    bStopFunc = TRUE;
  }

  u += v;
  v = FsmSw_Falcon_TrimI8Decode(g, 9, FsmSw_Falcon_max_small_fg_bits[9], &sk[u],
                                FSMSW_FALCON512_CRYPTO_SECRETKEYBYTES - u);

  if (v == 0u)
  {
    bStopFunc = TRUE;
  }

  u += v;

  v = FsmSw_Falcon_TrimI8Decode(F, 9, FsmSw_Falcon_max_big_FG_bits[9], &sk[u],
                                FSMSW_FALCON512_CRYPTO_SECRETKEYBYTES - u);

  if (v == 0u)
  {
    bStopFunc = TRUE;
  }

  u += v;

  if (u != FSMSW_FALCON512_CRYPTO_SECRETKEYBYTES)
  {
    bStopFunc = TRUE;
  }

  if (0 == FsmSw_Falcon_CompletePrivate(G, f, g, F, 9, tmp1_512.b))
  {
    bStopFunc = TRUE;
  }

  if (bStopFunc == FALSE)
  {
    /* Create a random nonce (40 bytes). */
    (void)FsmSw_CommonLib_RandomBytes(nonce, FSMSW_FALCON512_NONCELEN);

    /* Hash message nonce + message into a vector. */
    FsmSw_Fips202_Shake256_IncInit(&sc);
    FsmSw_Fips202_Shake256_IncAbsorb(&sc, nonce, FSMSW_FALCON512_NONCELEN);
    FsmSw_Fips202_Shake256_IncAbsorb(&sc, m, mlen);
    FsmSw_Fips202_Shake256_IncFinalize(&sc);
    FsmSw_Falcon_HashToPointCt(&sc, r_512.hm, 9, tmp1_512.b);

    /* Initialize a RNG. */
    (void)FsmSw_CommonLib_RandomBytes(seed, sizeof(seed));
    FsmSw_Fips202_Shake256_IncInit(&sc);
    FsmSw_Fips202_Shake256_IncAbsorb(&sc, seed, sizeof(seed));
    FsmSw_Fips202_Shake256_IncFinalize(&sc);

    /* Compute and return the signature. This loops until a signature value is found that fits in the provided
     * buffer. */
    for (uint32 i = 0; i < 0xFFFFFFFFu; i++)
    {
      FsmSw_Falcon_Sign_Dyn(r_512.sig, &sc, f, g, F, G, r_512.hm, 9, tmp1_512.b);
      v = FsmSw_Falcon_CompEncode(sigbuf, *sigbuflen, r_512.sig, 9);

      if (v != 0u)
      {
        *sigbuflen = v;
        retVal     = 0;
        break;
      }
    }
  }

  return retVal;
}

/***********************************************************************************************************************
 * Name:        fsmsw_falcon512_DoVerify
 *
 * Description: Verify a signature.
 *
 * Arguments:   -       uint8  *nonce:     The nonce has size FSMSW_FALCON1024_NONCELEN bytes
 *              -       uint8  *sigbuf:    contains the signature value
 *              -       uint32  sigbuflen: providing the maximum value length
 *              - const uint8  *m:         t.b.d.
 *              -       uint32  mlen:      t.b.d.
 *              - const uint8  *pk:        t.b.d.
 *
 * Returns 0 on success, -1 on error.
 *
 **********************************************************************************************************************/
static sint32 fsmsw_falcon512_DoVerify(const uint8 *nonce, const uint8 *sigbuf, uint32 sigbuflen, const uint8 *m,
                                       uint32 mlen, const uint8 *pk)
{
  uint16 h[512]             = {0};
  uint16 hm[512]            = {0};
  sint16 sig[512]           = {0};
  inner_shake256_context sc = {{0}};
  boolean bStopFunc         = FALSE;
  sint8 retVal              = 0;
  tmp3_512_struct tmp3_512  = {{0}};

  /* Decode public key. */
  if (pk[0] != (9u))
  {
    retVal    = -1;
    bStopFunc = TRUE;
  }

  if ((FALSE == bStopFunc) && (FsmSw_Falcon_ModqDecode(h, 9, &pk[1], FSMSW_FALCON512_CRYPTO_PUBLICKEYBYTES - 1u) !=
                               FSMSW_FALCON512_CRYPTO_PUBLICKEYBYTES - 1u))
  {
    retVal    = -1;
    bStopFunc = TRUE;
  }

  if (FALSE == bStopFunc)
  {
    FsmSw_Falcon_ToNttMonty(h, 9);

    /* Decode signature. */
    if (sigbuflen == 0u)
    {
      retVal    = -1;
      bStopFunc = TRUE;
    }

    if ((FALSE == bStopFunc) && (FsmSw_Falcon_CompDecode(sig, 9, sigbuf, sigbuflen) != sigbuflen))
    {
      retVal    = -1;
      bStopFunc = TRUE;
    }

    if (FALSE == bStopFunc)
    {
      /* Hash nonce + message into a vector. */
      FsmSw_Fips202_Shake256_IncInit(&sc);
      FsmSw_Fips202_Shake256_IncAbsorb(&sc, nonce, FSMSW_FALCON512_NONCELEN);
      FsmSw_Fips202_Shake256_IncAbsorb(&sc, m, mlen);
      FsmSw_Fips202_Shake256_IncFinalize(&sc);
      FsmSw_Falcon_HashToPointCt(&sc, hm, 9, tmp3_512.b);

      /* Verify signature. */
      if (0 == FsmSw_Falcon_VerifyRaw(hm, sig, h, 9, tmp3_512.b))
      {
        retVal = -1;
      }
    }
  }
  return retVal;
}
/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/* Encoding formats (nnnn = log of degree, 9 for Falcon-512, 10 for Falcon-1024)
 *
 *   private key:
 *      header byte: 0101nnnn
 *      private f  (6 or 5 bits by element, depending on degree)
 *      private g  (6 or 5 bits by element, depending on degree)
 *      private F  (8 bits by element)
 *
 *   public key:
 *      header byte: 0000nnnn
 *      public h   (14 bits by element)
 *
 *   signature:
 *      header byte: 0011nnnn
 *      nonce     40 bytes
 *      value     (12 bits by element)
 *
 *   message + signature:
 *      signature length   (2 bytes, big-endian)
 *      nonce              40 bytes
 *      message
 *      header byte:       0010nnnn
 *      value              (12 bits by element)
 *      (signature length is 1+len(value), not counting the nonce) */

/***********************************************************************************************************************
 * Name:        FsmSw_Falcon512_Crypto_Sign_KeyPair
 *
 * Description: Generate a new key pair. Public key goes into pk[], private key in sk[]. Key sizes are exact (in bytes).
 *              public (pk):  FSMSW_FALCON512_CRYPTO_PUBLICKEYBYTES
 *              private (sk): FSMSW_FALCON512_CRYPTO_SECRETKEYBYTES
 *
 * Arguments:   - uint8 *pk: Key sizes is FSMSW_FALCON512_CRYPTO_PUBLICKEYBYTES
 *              - uint8 *sk: Key sizes is FSMSW_FALCON512_CRYPTO_SECRETKEYBYTES
 *
 * Returns 0 on success, -1 on error.
 *
 **********************************************************************************************************************/
sint32 FsmSw_Falcon512_Crypto_Sign_KeyPair(uint8 *pk, uint8 *sk)
{
  sint8 f[512]               = {0};
  sint8 g[512]               = {0};
  sint8 F[512]               = {0};
  uint16 h[512]              = {0};
  uint8 seed[48]             = {0};
  inner_shake256_context rng = {{0}};
  uint32 u                   = 0;
  uint32 v                   = 0;
  sint8 retVal               = 0;
  boolean bStopFunc          = FALSE;
  tmp2_512_struct tmp2_512   = {{0}};

  /* Generate key pair. */
  (void)FsmSw_CommonLib_RandomBytes(seed, sizeof(seed));
  FsmSw_Fips202_Shake256_IncInit(&rng);
  FsmSw_Fips202_Shake256_IncAbsorb(&rng, seed, sizeof(seed));
  FsmSw_Fips202_Shake256_IncFinalize(&rng);
  FsmSw_Falcon_Keygen(&rng, f, g, F, ((void *)0), h, 9, tmp2_512.b);

  /* Encode private key. */
  sk[0] = 0x50u + 9u;
  u     = 1;
  v     = FsmSw_Falcon_TrimI8Encode(&sk[u], FSMSW_FALCON512_CRYPTO_SECRETKEYBYTES - u, f, 9u,
                                    FsmSw_Falcon_max_small_fg_bits[9]);

  if (v == 0u)
  {
    retVal    = -1;
    bStopFunc = TRUE;
  }

  u += v;
  v = FsmSw_Falcon_TrimI8Encode(&sk[u], FSMSW_FALCON512_CRYPTO_SECRETKEYBYTES - u, g, 9u,
                                FsmSw_Falcon_max_small_fg_bits[9]);

  if (v == 0u)
  {
    retVal    = -1;
    bStopFunc = TRUE;
  }

  u += v;
  v = FsmSw_Falcon_TrimI8Encode(&sk[u], FSMSW_FALCON512_CRYPTO_SECRETKEYBYTES - u, F, 9u,
                                FsmSw_Falcon_max_big_FG_bits[9]);

  if (v == 0u)
  {
    retVal    = -1;
    bStopFunc = TRUE;
  }
  u += v;
  if (u != FSMSW_FALCON512_CRYPTO_SECRETKEYBYTES)
  {
    retVal    = -1;
    bStopFunc = TRUE;
  }

  if (FALSE == bStopFunc)
  {
    /* Encode public key. */
    pk[0] = 9u;
    v     = FsmSw_Falcon_ModqEncode(&pk[1], FSMSW_FALCON512_CRYPTO_PUBLICKEYBYTES - 1u, h, 9u);

    if (v != FSMSW_FALCON512_CRYPTO_PUBLICKEYBYTES - 1u)
    {
      retVal = -1;
    }
  }

  return retVal;
}

/***********************************************************************************************************************
 * Name:        FsmSw_Falcon512_Crypto_Sign_Signature
 *
 * Description: Compute a signature on a provided message (m, mlen), with a given private key (sk). Signature is written
 *              in sig[], with length written into *siglen. Signature length is variable; maximum signature length
 *              (in bytes) is FSMSW_FALCON512_CRYPTO_BYTES. sig[], m[] and sk[] may overlap each other arbitrarily.
 *
 * Arguments:   -       uint8  *sig:    t.b.d.
 *              -       uint32 *siglen: t.b.d.
 *              - const uint8  *m:      t.b.d.
 *              -       uint32  mlen:   t.b.d.
 *              - const uint8  *sk:     t.b.d.
 *
 * Returns 0 on success, -1 on error.
 *
 **********************************************************************************************************************/
sint32 FsmSw_Falcon512_Crypto_Sign_Signature(uint8 *sig, uint32 *siglen, const uint8 *m, uint32 mlen, const uint8 *sk)
{
  /*The FSMSW_FALCON512_CRYPTO_BYTES constant is used for the signed message object (as produced by crypto_sign())
   * and includes a two-byte length value, so we take care here to only generate signatures that are two bytes shorter
   * than the maximum. This is done to ensure that crypto_sign() and crypto_sign_signature() produce the exact same
   * signature value, if used on the same message, with the same private key, and using the same output from
   * FsmSw_CommonLib_RandomBytes() (this is for reproducibility of tests). */
  uint32 vlen       = 0;
  sint8 retVal      = 0;
  boolean bStopFunc = FALSE;

  vlen = FSMSW_FALCON512_CRYPTO_BYTES - FSMSW_FALCON512_NONCELEN - 3u;

  if (fsmsw_falcon512_DoSign(&sig[1], &sig[1u + FSMSW_FALCON512_NONCELEN], &vlen, m, mlen, sk) < 0)
  {
    retVal    = -1;
    bStopFunc = TRUE;
  }

  if (FALSE == bStopFunc)
  {
    sig[0]  = 0x30 + 9;
    *siglen = 1u + FSMSW_FALCON512_NONCELEN + vlen;
  }

  return retVal;
}

/***********************************************************************************************************************
 * Name:        FsmSw_Falcon512_Crypto_Sign_Verify
 *
 * Description: Verify a signature (sig, siglen) on a message (m, mlen) with a given public key (pk).
 *              sig[], m[] and sk[] may overlap each other arbitrarily.
 *
 * Arguments:   - const uint8  *sig:    t.b.d.
 *              -       uint32 *siglen: t.b.d.
 *              - const uint8  *m:      t.b.d.
 *              -       uint32  mlen:   t.b.d.
 *              - const uint8  *pk:     t.b.d.
 *
 * Returns 0 on success, -1 on error.
 *
 **********************************************************************************************************************/
sint32 FsmSw_Falcon512_Crypto_Sign_Verify(const uint8 *sig, uint32 siglen, const uint8 *m, uint32 mlen, const uint8 *pk)
{
  sint32 retVal = fsmsw_falcon512_DoVerify(&sig[1], &sig[1u + FSMSW_FALCON512_NONCELEN],
                                           siglen - 1u - FSMSW_FALCON512_NONCELEN, m, mlen, pk);

  if (siglen < 1u + FSMSW_FALCON512_NONCELEN)
  {
    retVal = -1;
  }

  if (sig[0] != (0x30u + 9u))
  {
    retVal = -1;
  }

  return retVal;
}

/***********************************************************************************************************************
 * Name:        FsmSw_Falcon512_Crypto_Sign
 *
 * Description: Compute a signature on a message and pack the signature and message into a single object, written into
 *              sm[]. The length of that output is written in *smlen; that length may be larger than the message length
 *              (mlen) by up to FSMSW_FALCON512_CRYPTO_BYTES. sm[] and m[] may overlap each other arbitrarily; however,
 *              sm[] shall not overlap with sk[].
 *
 * Arguments:   - const uint8  *sig:    t.b.d.
 *              -       uint32 *siglen: t.b.d.
 *              - const uint8  *m:      t.b.d.
 *              -       uint32  mlen:   t.b.d.
 *              - const uint8  *pk:     t.b.d.
 *
 * Returns 0 on success, -1 on error.
 *
 **********************************************************************************************************************/
sint32 FsmSw_Falcon512_Crypto_Sign(uint8 *sm, uint32 *smlen, const uint8 *m, uint32 mlen, const uint8 *sk)
{
  uint8 *pm         = (uint8 *)NULL_PTR;
  uint8 *sigbuf     = (uint8 *)NULL_PTR;
  uint32 sigbuflen  = 0;
  sint8 retVal      = 0;
  boolean bStopFunc = FALSE;

  /* Move the message to its final location; this is a FsmSw_CommonLib_MemMove() so it handles overlaps properly. */
  FsmSw_CommonLib_MemMove(&sm[2u + FSMSW_FALCON512_NONCELEN], m, mlen);
  pm        = &sm[2u + FSMSW_FALCON512_NONCELEN];
  sigbuf    = &pm[1u + mlen];
  sigbuflen = FSMSW_FALCON512_CRYPTO_BYTES - FSMSW_FALCON512_NONCELEN - 3u;

  if (fsmsw_falcon512_DoSign(&sm[2], sigbuf, &sigbuflen, pm, mlen, sk) < 0)
  {
    retVal    = -1;
    bStopFunc = TRUE;
  }

  if (bStopFunc == FALSE)
  {
    pm[mlen] = 0x20 + 9;
    sigbuflen++;
    sm[0]  = (uint8)(sigbuflen >> 8);
    sm[1]  = (uint8)sigbuflen;
    *smlen = mlen + 2u + FSMSW_FALCON512_NONCELEN + sigbuflen;
  }

  return retVal;
}

/***********************************************************************************************************************
 * Name:        FsmSw_Falcon512_Crypto_Sign_Open
 *
 * Description: Open a signed message object (sm, smlen) and verify the signature; on success, the message itself is
 *              written into m[] and its length into *mlen. The message is shorter than the signed message object,
 *              but the size difference depends on the signature value; the difference may range up to
 *              FSMSW_FALCON512_CRYPTO_BYTES.
 *              m[], sm[] and pk[] may overlap each other arbitrarily.
 *
 * Arguments:   -       uint8  *m:    t.b.d.
 *              -       uint32 *mlen:   t.b.d.
 *              - const uint8  *sm:      t.b.d.
 *              -       uint32 *smlen: t.b.d.
 *              - const uint8  *pk:     t.b.d.
 *
 * Returns 0 on success, -1 on error.
 *
 **********************************************************************************************************************/
sint32 FsmSw_Falcon512_Crypto_Sign_Open(uint8 *m, uint32 *mlen, const uint8 *sm, uint32 smlen, const uint8 *pk)
{
  const uint8 *sigbuf = (uint8 *)NULL_PTR;
  uint32 pmlen        = 0;
  uint32 sigbuflen    = 0;
  sint32 retVal       = 0;

  if (smlen < (3u + FSMSW_FALCON512_NONCELEN))
  {
    retVal = -1;
  }

  sigbuflen = ((uint32)sm[0] << 8) | (uint32)sm[1];

  if ((sigbuflen < 2u) || (sigbuflen > (smlen - FSMSW_FALCON512_NONCELEN - 2u)))
  {
    retVal = -1;
  }

  sigbuflen--;
  pmlen = smlen - FSMSW_FALCON512_NONCELEN - 3u - sigbuflen;

  if (sm[2u + FSMSW_FALCON512_NONCELEN + pmlen] != (0x20u + 9u))
  {
    retVal = -1;
  }
  sigbuf = &sm[2u + FSMSW_FALCON512_NONCELEN + pmlen + 1u];

  /* The 2-byte length header and the one-byte signature header have been verified. Nonce is at sm+2, followed by the
   * message itself. Message length is in pmlen. sigbuf/sigbuflen point to the signature value (excluding the header
   * byte). */
  if (fsmsw_falcon512_DoVerify(&sm[2u], sigbuf, sigbuflen, &sm[2u + FSMSW_FALCON512_NONCELEN], pmlen, pk) < 0)
  {
    retVal = -1;
  }

  if (retVal != -1)
  {
    /* Signature is correct, we just have to copy/move the message to its final destination. The
     * FsmSw_CommonLib_MemMove() properly handles overlaps. */
    FsmSw_CommonLib_MemMove(m, &sm[2u + FSMSW_FALCON512_NONCELEN], pmlen);
    *mlen = pmlen;
  }
  return retVal;
}
