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
#include "FsmSw_Dilithium2_params.h"
#include "FsmSw_Dilithium2_rounding.h"
#include "FsmSw_Dilithium_ntt.h"
#include "FsmSw_Dilithium_reduce.h"
#include "FsmSw_Dilithium_symmetric.h"
#include "FsmSw_Types.h"

#include "FsmSw_Dilithium2_poly.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define POLY_UNIFORM_NBLOCKS        ((768u + STREAM128_BLOCKBYTES - 1u) / STREAM128_BLOCKBYTES)
#define POLY_UNIFORM_ETA_NBLOCKS    ((136u + STREAM256_BLOCKBYTES - 1u) / STREAM256_BLOCKBYTES)
#define POLY_UNIFORM_GAMMA1_NBLOCKS ((POLYZ_PACKEDBYTES_DILITHIUM2 + STREAM256_BLOCKBYTES - 1u) / STREAM256_BLOCKBYTES)

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
static uint32 fsmsw_dilithium2_RejEta(sint32 *a, uint32 len, const uint8 *buf, uint32 buflen);
static uint32 fsmsw_dilithium2_RejUniform(sint32 *a, uint32 len, const uint8 *buf, uint32 buflen);

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/
/***********************************************************************************************************************
 * Name:        fsmsw_dilithium2_RejEta
 *
 * Description: Sample uniformly random coefficients in [-ETA, ETA] by
 *              performing rejection sampling on array of random bytes.
 *
 * Arguments:   -       sint32 *a:      pointer to output array (allocated)
 *              -       uint32  len:    number of coefficients to be sampled
 *              - const uint8  *buf:    array of random bytes
 *              -       uint32  buflen: length of array of random bytes
 *
 * Returns number of sampled coefficients. Can be smaller than len if not enough random bytes were given.
 **********************************************************************************************************************/
static uint32 fsmsw_dilithium2_RejEta(sint32 *a, uint32 len, const uint8 *buf, uint32 buflen)
{
  uint32 ctr = 0;
  uint32 pos = 0;
  uint32 t0  = 0;
  uint32 t1  = 0;

  while ((ctr < len) && (pos < buflen))
  {
    t0 = (uint32)(buf[pos]) & 0x0Fu;
    t1 = (uint32)(buf[pos]) >> 4;
    pos++;

    if (t0 < 15u)
    {
      t0     = t0 - (uint32)(((205u * t0) >> 10) * 5u);
      a[ctr] = (sint32)((uint32)(2u - t0));
      ctr++;
    }

    if ((t1 < 15u) && (ctr < len))
    {
      t1     = t1 - (((205u * t1) >> 10) * 5u);
      a[ctr] = (sint32)((uint32)(2u - t1));
      ctr++;
    }
  }

  return ctr;
}

/***********************************************************************************************************************
 * Name:        fsmsw_dilithium2_RejUniform
 *
 * Description: Sample uniformly random coefficients in [0, Q-1] by
 *              performing rejection sampling on array of random bytes.
 *
 * Arguments:   -       sint32 *a:     pointer to output array (allocated)
 *              -       uint32 len:    number of coefficients to be sampled
 *              - const uint8 *buf:    array of random bytes
 *              -       uint32 buflen: length of array of random bytes
 *
 * Returns number of sampled coefficients. Can be smaller than len if not enough
 * random bytes were given.
 **********************************************************************************************************************/
static uint32 fsmsw_dilithium2_RejUniform(sint32 *a, uint32 len, const uint8 *buf, uint32 buflen)
{
  uint32 ctr, pos;
  uint32 t;

  ctr = 0;
  pos = 0;
  while ((ctr < len) && ((pos + 3u) <= buflen))
  {
    t = buf[pos];
    pos++;
    t |= (uint32)buf[pos] << 8;
    pos++;
    t |= (uint32)buf[pos] << 16;
    pos++;
    t &= 0x7FFFFFu;

    if (t < (uint32)Q_DILITHIUM)
    {
      a[ctr] = (sint32)t;
      ctr++;
    }
  }

  return ctr;
}

/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_Reduce
 *
 * Description: Inplace reduction of all coefficients of polynomial to representative in [-6283009,6283007].
 *
 * Arguments:   - poly_D2 *a: pointer to input/output polynomial
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Poly_Reduce(poly_D2 *a)
{
  uint16 i;

  for (i = 0; i < N_DILITHIUM; ++i)
  {
    a->coeffs[i] = FsmSw_Dilithium_Reduce32(a->coeffs[i]);
  }
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_CAddQ
 *
 * Description: For all coefficients of in/out polynomial add Q if coefficient is negative.
 *
 * Arguments:   - poly_D2 *a: pointer to input/output polynomial
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Poly_CAddQ(poly_D2 *a)
{
  uint16 i;

  for (i = 0; i < N_DILITHIUM; ++i)
  {
    a->coeffs[i] = FsmSw_Dilithium_CAddQ(a->coeffs[i]);
  }
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_Add
 *
 * Description: Add polynomials. No modular reduction is performed.
 *
 * Arguments:   -       poly_D2 *c: pointer to output polynomial
 *              - const poly_D2 *a: pointer to first summand
 *              - const poly_D2 *b: pointer to second summand
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Poly_Add(poly_D2 *c, const poly_D2 *a, const poly_D2 *b)
{
  uint16 i;

  for (i = 0; i < N_DILITHIUM; ++i)
  {
    c->coeffs[i] = a->coeffs[i] + b->coeffs[i];
  }
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_Sub
 *
 * Description: Subtract polynomials. No modular reduction is performed.
 *
 * Arguments:   -       poly_D2 *c: pointer to output polynomial
 *              - const poly_D2 *a: pointer to first input polynomial
 *              - const poly_D2 *b: pointer to second input polynomial to be subtraced from first input polynomial
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Poly_Sub(poly_D2 *c, const poly_D2 *a, const poly_D2 *b)
{
  uint16 i;

  for (i = 0; i < N_DILITHIUM; ++i)
  {
    c->coeffs[i] = a->coeffs[i] - b->coeffs[i];
  }
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_Shiftl
 *
 * Description: Multiply polynomial by 2^D without modular reduction.
 *              Assumes input coefficients to be less than 2^{31-D} in absolute value.
 *
 * Arguments:   - poly_D2 *a: pointer to input/output polynomial
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Poly_Shiftl(poly_D2 *a)
{
  uint16 i;

  for (i = 0; i < N_DILITHIUM; ++i)
  {
    a->coeffs[i] = (sint32)((uint32)((uint32)(a->coeffs[i]) << D_DILITHIUM));
  }
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_Ntt
 *
 * Description: Inplace forward NTT. Coefficients can grow by 8*Q in absolute value.
 *
 * Arguments:   - poly_D2 *a: pointer to input/output polynomial
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Poly_Ntt(poly_D2 *a)
{
  FsmSw_Dilithium_Ntt(a->coeffs);
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_InvnttTomont
 *
 * Description: Inplace inverse NTT and multiplication by 2^{32}. Input coefficients need to be less than Q in absolute
 *              value and output coefficients are again bounded by Q.
 *
 * Arguments:   - poly_D2 *a: pointer to input/output polynomial
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Poly_InvnttTomont(poly_D2 *a)
{
  FsmSw_Dilithium_InvnttTomont(a->coeffs);
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_PointwiseMontgomery
 *
 * Description: Pointwise multiplication of polynomials in NTT domain representation and multiplication of
 *              resulting polynomial by 2^{-32}.
 *
 * Arguments:   -       poly_D2 *c: pointer to output polynomial
 *              - const poly_D2 *a: pointer to first input polynomial
 *              - const poly_D2 *b: pointer to second input polynomial
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Poly_PointwiseMontgomery(poly_D2 *c, const poly_D2 *a, const poly_D2 *b)
{
  uint16 i;

  for (i = 0; i < N_DILITHIUM; ++i)
  {
    c->coeffs[i] = FsmSw_Dilithium_MontgomeryReduce((sint64)a->coeffs[i] * b->coeffs[i]);
  }
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_Power2Round
 *
 * Description: For all coefficients c of the input polynomial, compute c0, c1 such that c mod Q = c1*2^D + c0
 *              with -2^{D-1} < c0 <= 2^{D-1}. Assumes coefficients to be standard representatives.
 *
 * Arguments:   -       poly_D2 *a1: pointer to output polynomial with coefficients c1
 *              -       poly_D2 *a0: pointer to output polynomial with coefficients c0
 *              - const poly_D2 *a:  pointer to input polynomial
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Poly_Power2Round(poly_D2 *a1, poly_D2 *a0, const poly_D2 *a)
{
  uint16 i;

  for (i = 0; i < N_DILITHIUM; ++i)
  {
    a1->coeffs[i] = FsmSw_Dilithium2_Power2Round(&a0->coeffs[i], a->coeffs[i]);
  }
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_Decompose
 *
 * Description: For all coefficients c of the input polynomial, compute high and low bits c0, c1 such c mod
 *              Q = c1*ALPHA + c0 with -ALPHA/2 < c0 <= ALPHA/2 except c1 = (Q-1)/ALPHA where we set c1 = 0 and
 *              -ALPHA/2 <= c0 = c mod Q - Q < 0. Assumes coefficients to be standard representatives.
 *
 * Arguments:   -       poly_D2 *a1: pointer to output polynomial with coefficients c1
 *              -       poly_D2 *a0: pointer to output polynomial with coefficients c0
 *              - const poly_D2 *a:  pointer to input polynomial
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Poly_Decompose(poly_D2 *a1, poly_D2 *a0, const poly_D2 *a)
{
  uint16 i;

  for (i = 0; i < N_DILITHIUM; ++i)
  {
    a1->coeffs[i] = FsmSw_Dilithium2_Decompose(&a0->coeffs[i], a->coeffs[i]);
  }
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_MakeHint
 *
 * Description: Compute hint polynomial. The coefficients of which indicate whether the low bits of the corresponding
 *              coefficient of the input polynomial overflow into the high bits.
 *
 * Arguments:   -       poly_D2 *h:  pointer to output hint polynomial
 *              - const poly_D2 *a0: pointer to low part of input polynomial
 *              - const poly_D2 *a1: pointer to high part of input polynomial
 *
 * Returns number of 1 bits.
 **********************************************************************************************************************/
uint32 FsmSw_Dilithium2_Poly_MakeHint(poly_D2 *h, const poly_D2 *a0, const poly_D2 *a1)
{
  uint16 i;
  uint32 s = 0;

  for (i = 0; i < N_DILITHIUM; ++i)
  {
    h->coeffs[i] = (sint32)FsmSw_Dilithium2_MakeHint(a0->coeffs[i], a1->coeffs[i]);
    s            = s + (uint32)(h->coeffs[i]);
  }

  return s;
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_UseHint
 *
 * Description: Use hint polynomial to correct the high bits of a polynomial.
 *
 * Arguments:   -       poly_D2 *b: pointer to output polynomial with corrected high bits
 *              - const poly_D2 *a: pointer to input polynomial
 *              - const poly_D2 *h: pointer to input hint polynomial
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Poly_UseHint(poly_D2 *b, const poly_D2 *a, const poly_D2 *h)
{
  uint16 i;

  for (i = 0; i < N_DILITHIUM; ++i)
  {
    b->coeffs[i] = FsmSw_Dilithium2_UseHint(a->coeffs[i], (uint32)h->coeffs[i]);
  }
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_Chknorm
 *
 * Description: Check infinity norm of polynomial against given bound.
 *              Assumes input coefficients were reduced by FsmSw_Dilithium2_reduce32().
 *
 * Arguments:   - const poly_D2 *a: pointer to polynomial
 *              -       sint32 B:   norm bound
 *
 * Returns 0 if norm is strictly smaller than B <= (Q-1)/8 and 1 otherwise.
 **********************************************************************************************************************/
sint8 FsmSw_Dilithium2_Poly_Chknorm(const poly_D2 *a, sint32 B)
{
  uint16 i;
  sint32 t;

  sint8 retVal = 0;

  if (B > (Q_DILITHIUM - 1) / 8)
  {
    retVal = 1;
  }

  /* It is ok to leak which coefficient violates the bound since
       the probability for each coefficient is independent of secret
       data but we must not leak the sign of the centralized representative. */
  for (i = 0; i < N_DILITHIUM; ++i)
  {
    /* Absolute value */
    t = (sint32)((uint32)((uint64)a->coeffs[i] >> 31));
    t = (sint32)((uint32)((uint32)a->coeffs[i] - ((uint32)t & (2u * (uint32)a->coeffs[i]))));

    if (t >= B)
    {
      retVal = 1;
    }
  }

  return retVal;
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_Uniform
 *
 * Description: Sample polynomial with uniformly random coefficients in [0,Q-1] by performing rejection sampling on the
 *              output stream of SHAKE256(seed|nonce)
 *
 * Arguments:   -       poly_D2 *a:      pointer to output polynomial
 *              - const uint8    seed[]: byte array with seed of length SEEDBYTES_DILITHIUM
 *              -       uint16   nonce:  2-byte nonce
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Poly_Uniform(poly_D2 *a, const uint8 seed[SEEDBYTES_DILITHIUM], uint16 nonce)
{
  uint32 ctr, off;
  uint32 buflen = POLY_UNIFORM_NBLOCKS * STREAM128_BLOCKBYTES;
  uint8 buf[(POLY_UNIFORM_NBLOCKS * STREAM128_BLOCKBYTES) + 2u];
  FsmSw_Dilithium_stream128_state state;

  FsmSw_Dilithium_Shake128_StreamInit(&state, seed, nonce);
  FsmSw_Fips202_Shake128_IncSqueeze(buf, POLY_UNIFORM_NBLOCKS * SHAKE128_RATE, &state);

  ctr = fsmsw_dilithium2_RejUniform(a->coeffs, N_DILITHIUM, buf, buflen);

  while (ctr < N_DILITHIUM)
  {
    off = buflen % 3u;
    /* "As part of resolving the MISRA 2 warnings, a for-loop was removed here. 
        If it is needed in the future, it must be reinserted." */

    FsmSw_Fips202_Shake128_IncSqueeze(&buf[off], SHAKE128_RATE, &state);
    buflen = (uint32)(STREAM128_BLOCKBYTES + off);
    ctr += fsmsw_dilithium2_RejUniform(&a->coeffs[ctr], N_DILITHIUM - ctr, buf, buflen);
  }
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_UniformEta
 *
 * Description: Sample polynomial with uniformly random coefficients in [-ETA,ETA] by performing rejection sampling
 *              on the output stream from SHAKE256(seed|nonce)
 *
 * Arguments:   -       poly_D2 *a:      pointer to output polynomial
 *              - const uint8    seed[]: byte array with seed of length CRHBYTES_DILITHIUM
 *              -       uint16   nonce:  2-byte nonce
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Poly_UniformEta(poly_D2 *a, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce)
{
  uint32 ctr;
  /* polyspace +1 MISRA2012:2.2 [Justified:]"Calculation of buflen is important for generic implementation" */
  uint32 buflen = POLY_UNIFORM_ETA_NBLOCKS * STREAM256_BLOCKBYTES;
  /* polyspace +1 MISRA2012:2.2 [Justified:]"Calculation of buffer length is important for generic implementation" */
  uint8 buf[POLY_UNIFORM_ETA_NBLOCKS * STREAM256_BLOCKBYTES];
  FsmSw_Dilithium_stream256_state state;

  FsmSw_Dilithium_Shake256_StreamInit(&state, seed, nonce);

  /* polyspace +6 MISRA2012:2.2 [Justified:]"Original funktion call
                                              (stream256_squeezeblocks(buf, POLY_UNIFORM_ETA_NBLOCKS, &state);) has a
                                              MISRA finding. Therefore, it is not the macro that is used, but the
                                              function call. The calculation of parameter two in function
                                              FsmSw_Fips202_Shake256_IncSqueeze is important regarding rounding error
                                              in the interger division" */
  FsmSw_Fips202_Shake256_IncSqueeze(buf, (((136u + 136u - 1u) / 136u)) * (136u), &state);

  ctr = fsmsw_dilithium2_RejEta(a->coeffs, N_DILITHIUM, buf, buflen);

  while (ctr < N_DILITHIUM)
  {
    FsmSw_Fips202_Shake256_IncSqueeze(buf, SHAKE256_RATE, &state);
    ctr += fsmsw_dilithium2_RejEta(&(a->coeffs[ctr]), N_DILITHIUM - ctr, buf, STREAM256_BLOCKBYTES);
  }
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_Uniform_Gamma1
 *
 * Description: Sample polynomial with uniformly random coefficients in [-(GAMMA1 - 1), GAMMA1] by unpacking
 *              output stream of SHAKE256(seed|nonce)
 *
 * Arguments:   -       poly_D2 *a:      pointer to output polynomial
 *              - const uint8    seed[]: byte array with seed of length CRHBYTES_DILITHIUM
 *              -       uint16   nonce:  16-bit nonce
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Poly_UniformGamma1(poly_D2 *a, const uint8 seed[CRHBYTES_DILITHIUM], uint16 nonce)
{
  uint8 buf[POLY_UNIFORM_GAMMA1_NBLOCKS * STREAM256_BLOCKBYTES];
  FsmSw_Dilithium_stream256_state state;

  FsmSw_Dilithium_Shake256_StreamInit(&state, seed, nonce);
  FsmSw_Fips202_Shake256_IncSqueeze(buf, POLY_UNIFORM_GAMMA1_NBLOCKS * SHAKE256_RATE, &state);
  FsmSw_Dilithium2_Poly_ZUnpack(a, buf);
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Challenge
 *
 * Description: Implementation of H. Samples polynomial with TAU nonzero coefficients in {-1,1} using the output stream
 *              of SHAKE256(seed).
 *
 * Arguments:   -       poly_D2 *c:      pointer to output polynomial
 *              - const uint8    seed[]: byte array containing seed of length SEEDBYTES_DILITHIUM
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Poly_Challenge(poly_D2 *c, const uint8 seed[SEEDBYTES_DILITHIUM])
{
  uint32 i, b, pos;
  uint64 signs;
  uint8 buf[SHAKE256_RATE];
  shake256incctx state;

  FsmSw_Fips202_Shake256_IncInit(&state);
  FsmSw_Fips202_Shake256_IncAbsorb(&state, seed, SEEDBYTES_DILITHIUM);
  FsmSw_Fips202_Shake256_IncFinalize(&state);
  FsmSw_Fips202_Shake256_IncSqueeze(buf, sizeof(buf), &state);

  signs = 0;
  for (i = 0; i < 8u; ++i)
  {
    signs |= (uint64)buf[i] << (8u * i);
  }

  pos = 8;

  for (i = 0; i < N_DILITHIUM; ++i)
  {
    c->coeffs[i] = 0;
  }

  for (i = N_DILITHIUM - TAU_DILITHIUM2; i < N_DILITHIUM; ++i)
  {
    do
    {
      if (pos >= SHAKE256_RATE)
      {
        FsmSw_Fips202_Shake256_IncSqueeze(buf, sizeof(buf), &state);
        pos = 0;
      }

      b = buf[pos];
      pos++;
    } while (b > i);

    c->coeffs[i] = c->coeffs[b];
    c->coeffs[b] = (sint32)((uint32)((uint64)(1u - (2u * (signs & 1u)))));
    signs >>= 1;
  }
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Polyeta_EtaPack
 *
 * Description: Bit-pack polynomial with coefficients in [-ETA,ETA].
 *
 * Arguments:   -       uint8   *r: pointer to output byte array with at least POLYETA_PACKEDBYTES bytes
 *              - const poly_D2 *a: pointer to input polynomial
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Polyeta_EtaPack(uint8 *r, const poly_D2 *a)
{
  uint16 i;
  uint8 t[8];

  for (i = 0; i < (N_DILITHIUM / 8u); ++i)
  {
    t[0] = (uint8)(ETA_DILITHIUM2 - (uint8)a->coeffs[8u * i]);
    t[1] = (uint8)(ETA_DILITHIUM2 - (uint8)a->coeffs[(8u * i) + 1u]);
    t[2] = (uint8)(ETA_DILITHIUM2 - (uint8)a->coeffs[(8u * i) + 2u]);
    t[3] = (uint8)(ETA_DILITHIUM2 - (uint8)a->coeffs[(8u * i) + 3u]);
    t[4] = (uint8)(ETA_DILITHIUM2 - (uint8)a->coeffs[(8u * i) + 4u]);
    t[5] = (uint8)(ETA_DILITHIUM2 - (uint8)a->coeffs[(8u * i) + 5u]);
    t[6] = (uint8)(ETA_DILITHIUM2 - (uint8)a->coeffs[(8u * i) + 6u]);
    t[7] = (uint8)(ETA_DILITHIUM2 - (uint8)a->coeffs[(8u * i) + 7u]);

    r[3u * i]        = (t[0] >> 0) | (t[1] << 3) | (t[2] << 6);
    r[(3u * i) + 1u] = (t[2] >> 2) | (t[3] << 1) | (t[4] << 4) | (t[5] << 7);
    r[(3u * i) + 2u] = (t[5] >> 1) | (t[6] << 2) | (t[7] << 5);
  }
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Polyeta_EtaUnpack
 *
 * Description: Unpack polynomial with coefficients in [-ETA,ETA].
 *
 * Arguments:   -       poly_D2 *r: pointer to output polynomial
 *              - const uint8   *a: byte array with bit-packed polynomial
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Polyeta_EtaUnpack(poly_D2 *r, const uint8 *a)
{
  uint16 i;

  for (i = 0; i < (N_DILITHIUM / 8u); ++i)
  {
    r->coeffs[8u * i]        = (sint32)((uint32)((uint32)((uint32)(a[3u * i]) >> 0) & 7u));
    r->coeffs[(8u * i) + 1u] = (sint32)((uint32)((uint32)((uint32)(a[3u * i]) >> 3) & 7u));
    r->coeffs[(8u * i) + 2u] =
        (sint32)((uint32)((((uint32)(a[3u * i]) >> 6) | ((uint32)(a[(3u * i) + 1u]) << 2u)) & 7u));
    r->coeffs[(8u * i) + 3u] = (sint32)((uint32)((uint32)((uint32)(a[(3u * i) + 1u]) >> 1) & 7u));
    r->coeffs[(8u * i) + 4u] = (sint32)((uint32)((uint32)((uint32)(a[(3u * i) + 1u]) >> 4) & 7u));
    r->coeffs[(8u * i) + 5u] =
        (sint32)((uint32)((((uint32)(a[(3u * i) + 1u]) >> 7) | ((uint32)(a[(3u * i) + 2u]) << 1u)) & 7u));
    r->coeffs[(8u * i) + 6u] = (sint32)((uint32)((uint32)((uint32)(a[(3u * i) + 2u]) >> 2) & 7u));
    r->coeffs[(8u * i) + 7u] = (sint32)((uint32)((uint32)((uint32)(a[(3u * i) + 2u]) >> 5) & 7u));

    r->coeffs[8u * i]        = (sint32)ETA_DILITHIUM2 - r->coeffs[8u * i];
    r->coeffs[(8u * i) + 1u] = (sint32)ETA_DILITHIUM2 - r->coeffs[(8u * i) + 1u];
    r->coeffs[(8u * i) + 2u] = (sint32)ETA_DILITHIUM2 - r->coeffs[(8u * i) + 2u];
    r->coeffs[(8u * i) + 3u] = (sint32)ETA_DILITHIUM2 - r->coeffs[(8u * i) + 3u];
    r->coeffs[(8u * i) + 4u] = (sint32)ETA_DILITHIUM2 - r->coeffs[(8u * i) + 4u];
    r->coeffs[(8u * i) + 5u] = (sint32)ETA_DILITHIUM2 - r->coeffs[(8u * i) + 5u];
    r->coeffs[(8u * i) + 6u] = (sint32)ETA_DILITHIUM2 - r->coeffs[(8u * i) + 6u];
    r->coeffs[(8u * i) + 7u] = (sint32)ETA_DILITHIUM2 - r->coeffs[(8u * i) + 7u];
  }
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_T1Pack
 *
 * Description: Bit-pack polynomial t1 with coefficients fitting in 10 bits.
 *              Input coefficients are assumed to be standard representatives.
 *
 * Arguments:   -       uint8   *r: pointer to output byte array with at least POLYT1_PACKEDBYTES bytes
 *              - const poly_D2 *a: pointer to input polynomial
 ***********************************************************************************************************************/
void FsmSw_Dilithium2_Poly_T1Pack(uint8 *r, const poly_D2 *a)
{
  uint16 i;

  for (i = 0; i < (N_DILITHIUM / 4u); ++i)
  {
    r[5u * i]        = (uint8)((uint16)a->coeffs[4u * i] >> 0);
    r[(5u * i) + 1u] = (uint8)((uint16)(((uint16)a->coeffs[4u * i] >> 8) | ((uint16)a->coeffs[(4u * i) + 1u] << 2u)));
    r[(5u * i) + 2u] =
        (uint8)((uint16)(((uint16)a->coeffs[(4u * i) + 1u] >> 6) | ((uint16)a->coeffs[(4u * i) + 2u] << 4u)));
    r[(5u * i) + 3u] =
        (uint8)((uint16)(((uint16)a->coeffs[(4u * i) + 2u] >> 4) | ((uint16)a->coeffs[(4u * i) + 3u] << 6u)));
    r[(5u * i) + 4u] = (uint8)((uint16)a->coeffs[(4u * i) + 3u] >> 2);
  }
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_T1Unpack
 *
 * Description: Unpack polynomial t1 with 10-bit coefficients.
 *              Output coefficients are standard representatives.
 *
 * Arguments:   -       poly_D2 *r: pointer to output polynomial
 *              - const uint8   *a: byte array with bit-packed polynomial
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Poly_T1Unpack(poly_D2 *r, const uint8 *a)
{
  uint16 i;

  for (i = 0; i < (N_DILITHIUM / 4u); ++i)
  {
    r->coeffs[4u * i] =
        (sint32)((uint32)(((uint32)(((uint32)a[5u * i] >> 0) | ((uint32)a[(5u * i) + 1u] << 8u))) & 0x3FFu));
    r->coeffs[(4u * i) + 1u] =
        (sint32)((uint32)(((uint32)(((uint32)a[(5u * i) + 1u] >> 2) | ((uint32)a[(5u * i) + 2u] << 6u))) & 0x3FFu));
    r->coeffs[(4u * i) + 2u] =
        (sint32)((uint32)(((uint32)(((uint32)a[(5u * i) + 2u] >> 4) | ((uint32)a[(5u * i) + 3u] << 4u))) & 0x3FFu));
    r->coeffs[(4u * i) + 3u] =
        (sint32)((uint32)(((uint32)(((uint32)a[(5u * i) + 3u] >> 6) | ((uint32)a[(5u * i) + 4u] << 2u))) & 0x3FFu));
  }
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_T0Pack
 *
 * Description: Bit-pack polynomial t0 with coefficients in ]-2^{D-1}, 2^{D-1}].
 *
 * Arguments:   -       uint8   *r: pointer to output byte array with at least POLYT0_PACKEDBYTES_DILITHIUM bytes
 *              - const poly_D2 *a: pointer to input polynomial
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Poly_T0Pack(uint8 *r, const poly_D2 *a)
{
  uint16 i;
  uint32 t[8];

  for (i = 0; i < (N_DILITHIUM / 8u); ++i)
  {
    t[0] = ((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)a->coeffs[8u * i]);
    t[1] = ((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)a->coeffs[(8u * i) + 1u]);
    t[2] = ((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)a->coeffs[(8u * i) + 2u]);
    t[3] = ((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)a->coeffs[(8u * i) + 3u]);
    t[4] = ((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)a->coeffs[(8u * i) + 4u]);
    t[5] = ((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)a->coeffs[(8u * i) + 5u]);
    t[6] = ((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)a->coeffs[(8u * i) + 6u]);
    t[7] = ((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)a->coeffs[(8u * i) + 7u]);

    r[13u * i]        = (uint8)t[0];
    r[(13u * i) + 1u] = (uint8)(t[0] >> 8);
    r[(13u * i) + 1u] |= (uint8)(t[1] << 5);
    r[(13u * i) + 2u] = (uint8)(t[1] >> 3);
    r[(13u * i) + 3u] = (uint8)(t[1] >> 11);
    r[(13u * i) + 3u] |= (uint8)(t[2] << 2);
    r[(13u * i) + 4u] = (uint8)(t[2] >> 6);
    r[(13u * i) + 4u] |= (uint8)(t[3] << 7);
    r[(13u * i) + 5u] = (uint8)(t[3] >> 1);
    r[(13u * i) + 6u] = (uint8)(t[3] >> 9);
    r[(13u * i) + 6u] |= (uint8)(t[4] << 4);
    r[(13u * i) + 7u] = (uint8)(t[4] >> 4);
    r[(13u * i) + 8u] = (uint8)(t[4] >> 12);
    r[(13u * i) + 8u] |= (uint8)(t[5] << 1);
    r[(13u * i) + 9u] = (uint8)(t[5] >> 7);
    r[(13u * i) + 9u] |= (uint8)(t[6] << 6);
    r[(13u * i) + 10u] = (uint8)(t[6] >> 2);
    r[(13u * i) + 11u] = (uint8)(t[6] >> 10);
    r[(13u * i) + 11u] |= (uint8)(t[7] << 3);
    r[(13u * i) + 12u] = (uint8)(t[7] >> 5);
  }
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_T0Unpack
 *
 * Description: Unpack polynomial t0 with coefficients in ]-2^{D-1}, 2^{D-1}].
 *
 * Arguments:   -       poly_D2 *r: pointer to output polynomial
 *              - const uint8   *a: byte array with bit-packed polynomial
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Poly_T0Unpack(poly_D2 *r, const uint8 *a)
{
  uint16 i;

  for (i = 0; i < (N_DILITHIUM / 8u); ++i)
  {
    r->coeffs[8u * i] = (sint32)(a[13u * i]);
    r->coeffs[8u * i] = (sint32)((uint32)(((uint32)r->coeffs[8u * i]) | ((uint32)a[(13u * i) + 1u] << 8u)));
    r->coeffs[8u * i] = (sint32)((uint32)(((uint32)r->coeffs[8u * i]) & 0x1FFFu));

    r->coeffs[(8u * i) + 1u] = (sint32)((uint32)((uint32)a[(13u * i) + 1u] >> 5));
    r->coeffs[(8u * i) + 1u] =
        (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 1u]) | ((uint32)a[(13u * i) + 2u] << 3u)));
    r->coeffs[(8u * i) + 1u] =
        (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 1u]) | ((uint32)a[(13u * i) + 3u] << 11u)));
    r->coeffs[(8u * i) + 1u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 1u]) & 0x1FFFu));

    r->coeffs[(8u * i) + 2u] = (sint32)((uint32)((uint32)a[(13u * i) + 3u] >> 2));
    r->coeffs[(8u * i) + 2u] =
        (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 2u]) | ((uint32)a[(13u * i) + 4u] << 6u)));
    r->coeffs[(8u * i) + 2u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 2u]) & 0x1FFFu));

    r->coeffs[(8u * i) + 3u] = (sint32)((uint32)((uint32)a[(13u * i) + 4u] >> 7));
    r->coeffs[(8u * i) + 3u] =
        (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 3u]) | ((uint32)a[(13u * i) + 5u] << 1u)));
    r->coeffs[(8u * i) + 3u] =
        (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 3u]) | ((uint32)a[(13u * i) + 6u] << 9u)));
    r->coeffs[(8u * i) + 3u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 3u]) & 0x1FFFu));

    r->coeffs[(8u * i) + 4u] = (sint32)((uint32)((uint32)a[(13u * i) + 6u] >> 4));
    r->coeffs[(8u * i) + 4u] =
        (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 4u]) | ((uint32)a[(13u * i) + 7u] << 4u)));
    r->coeffs[(8u * i) + 4u] =
        (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 4u]) | ((uint32)a[(13u * i) + 8u] << 12u)));
    r->coeffs[(8u * i) + 4u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 4u]) & 0x1FFFu));

    r->coeffs[(8u * i) + 5u] = (sint32)((uint32)((uint32)a[(13u * i) + 8u] >> 1));
    r->coeffs[(8u * i) + 5u] =
        (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 5u]) | ((uint32)a[(13u * i) + 9u] << 7u)));
    r->coeffs[(8u * i) + 5u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 5u]) & 0x1FFFu));

    r->coeffs[(8u * i) + 6u] = (sint32)((uint32)((uint32)a[(13u * i) + 9u] >> 6));
    r->coeffs[(8u * i) + 6u] =
        (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 6u]) | ((uint32)a[(13u * i) + 10u] << 2u)));
    r->coeffs[(8u * i) + 6u] =
        (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 6u]) | ((uint32)a[(13u * i) + 11u] << 10u)));
    r->coeffs[(8u * i) + 6u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 6u]) & 0x1FFFu));

    r->coeffs[(8u * i) + 7u] = (sint32)((uint32)((uint32)a[(13u * i) + 11u] >> 3));
    r->coeffs[(8u * i) + 7u] =
        (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 7u]) | ((uint32)a[(13u * i) + 12u] << 5u)));
    r->coeffs[(8u * i) + 7u] = (sint32)((uint32)(((uint32)r->coeffs[(8u * i) + 7u]) & 0x1FFFu));

    r->coeffs[8u * i] = (sint32)((uint32)(((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)r->coeffs[8u * i])));
    r->coeffs[(8u * i) + 1u] =
        (sint32)((uint32)(((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)r->coeffs[(8u * i) + 1u])));
    r->coeffs[(8u * i) + 2u] =
        (sint32)((uint32)(((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)r->coeffs[(8u * i) + 2u])));
    r->coeffs[(8u * i) + 3u] =
        (sint32)((uint32)(((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)r->coeffs[(8u * i) + 3u])));
    r->coeffs[(8u * i) + 4u] =
        (sint32)((uint32)(((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)r->coeffs[(8u * i) + 4u])));
    r->coeffs[(8u * i) + 5u] =
        (sint32)((uint32)(((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)r->coeffs[(8u * i) + 5u])));
    r->coeffs[(8u * i) + 6u] =
        (sint32)((uint32)(((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)r->coeffs[(8u * i) + 6u])));
    r->coeffs[(8u * i) + 7u] =
        (sint32)((uint32)(((uint32)1u << (D_DILITHIUM - 1u)) - ((uint32)r->coeffs[(8u * i) + 7u])));
  }
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_ZPack
 *
 * Description: Bit-pack polynomial with coefficients in [-(GAMMA1 - 1), GAMMA1].
 *
 * Arguments:   -       uint8   *r: pointer to output byte array with at least POLYZ_PACKEDBYTES_DILITHIUM2 bytes
 *              - const poly_D2 *a: pointer to input polynomial
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Poly_ZPack(uint8 *r, const poly_D2 *a)
{
  uint16 i;
  uint32 t[4];

  for (i = 0; i < (N_DILITHIUM / 4u); ++i)
  {
    t[0] = GAMMA1_DILITHIUM2 - (uint32)a->coeffs[4u * i];
    t[1] = GAMMA1_DILITHIUM2 - (uint32)a->coeffs[(4u * i) + 1u];
    t[2] = GAMMA1_DILITHIUM2 - (uint32)a->coeffs[(4u * i) + 2u];
    t[3] = GAMMA1_DILITHIUM2 - (uint32)a->coeffs[(4u * i) + 3u];

    r[9u * i]        = (uint8)t[0];
    r[(9u * i) + 1u] = (uint8)(t[0] >> 8);
    r[(9u * i) + 2u] = (uint8)(t[0] >> 16);
    r[(9u * i) + 2u] |= (uint8)(t[1] << 2);
    r[(9u * i) + 3u] = (uint8)(t[1] >> 6);
    r[(9u * i) + 4u] = (uint8)(t[1] >> 14);
    r[(9u * i) + 4u] |= (uint8)(t[2] << 4);
    r[(9u * i) + 5u] = (uint8)(t[2] >> 4);
    r[(9u * i) + 6u] = (uint8)(t[2] >> 12);
    r[(9u * i) + 6u] |= (uint8)(t[3] << 6);
    r[(9u * i) + 7u] = (uint8)(t[3] >> 2);
    r[(9u * i) + 8u] = (uint8)(t[3] >> 10);
  }
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_ZUnpack
 *
 * Description: Unpack polynomial z with coefficients in [-(GAMMA1 - 1), GAMMA1].
 *
 * Arguments:   -       poly_D2 *r: pointer to output polynomial
 *              - const uint8   *a: byte array with bit-packed polynomial
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Poly_ZUnpack(poly_D2 *r, const uint8 *a)
{
  uint16 i;

  for (i = 0; i < (N_DILITHIUM / 4u); ++i)
  {
    r->coeffs[4u * i] = (sint32)a[9u * i];
    r->coeffs[4u * i] = (sint32)((uint32)((uint32)r->coeffs[4u * i] | ((uint32)a[(9u * i) + 1u] << 8u)));
    r->coeffs[4u * i] = (sint32)((uint32)((uint32)r->coeffs[4u * i] | ((uint32)a[(9u * i) + 2u] << 16u)));
    r->coeffs[4u * i] = (sint32)((uint32)((uint32)r->coeffs[4u * i] & 0x3FFFFu));

    r->coeffs[(4u * i) + 1u] = (sint32)((uint32)((uint32)a[(9u * i) + 2u] >> 2));
    r->coeffs[(4u * i) + 1u] = (sint32)((uint32)((uint32)r->coeffs[(4u * i) + 1u] | ((uint32)a[(9u * i) + 3u] << 6u)));
    r->coeffs[(4u * i) + 1u] = (sint32)((uint32)((uint32)r->coeffs[(4u * i) + 1u] | ((uint32)a[(9u * i) + 4u] << 14u)));
    r->coeffs[(4u * i) + 1u] = (sint32)((uint32)((uint32)r->coeffs[(4u * i) + 1u] & 0x3FFFFu));

    r->coeffs[(4u * i) + 2u] = (sint32)((uint32)((uint32)a[(9u * i) + 4u] >> 4));
    r->coeffs[(4u * i) + 2u] = (sint32)((uint32)((uint32)r->coeffs[(4u * i) + 2u] | ((uint32)a[(9u * i) + 5u] << 4u)));
    r->coeffs[(4u * i) + 2u] = (sint32)((uint32)((uint32)r->coeffs[(4u * i) + 2u] | ((uint32)a[(9u * i) + 6u] << 12u)));
    r->coeffs[(4u * i) + 2u] = (sint32)((uint32)((uint32)r->coeffs[(4u * i) + 2u] & 0x3FFFFu));

    r->coeffs[(4u * i) + 3u] = (sint32)((uint32)((uint32)a[(9u * i) + 6u] >> 6));
    r->coeffs[(4u * i) + 3u] = (sint32)((uint32)((uint32)r->coeffs[(4u * i) + 3u] | ((uint32)a[(9u * i) + 7u] << 2u)));
    r->coeffs[(4u * i) + 3u] = (sint32)((uint32)((uint32)r->coeffs[(4u * i) + 3u] | ((uint32)a[(9u * i) + 8u] << 10u)));
    r->coeffs[(4u * i) + 3u] = (sint32)((uint32)((uint32)r->coeffs[(4u * i) + 3u] & 0x3FFFFu));

    r->coeffs[4u * i]        = (sint32)((uint32)(GAMMA1_DILITHIUM2 - (uint32)r->coeffs[4u * i]));
    r->coeffs[(4u * i) + 1u] = (sint32)((uint32)(GAMMA1_DILITHIUM2 - (uint32)r->coeffs[(4u * i) + 1u]));
    r->coeffs[(4u * i) + 2u] = (sint32)((uint32)(GAMMA1_DILITHIUM2 - (uint32)r->coeffs[(4u * i) + 2u]));
    r->coeffs[(4u * i) + 3u] = (sint32)((uint32)(GAMMA1_DILITHIUM2 - (uint32)r->coeffs[(4u * i) + 3u]));
  }
}

/***********************************************************************************************************************
 * Name:        FsmSw_Dilithium2_Poly_W1Pack
 *
 * Description: Bit-pack polynomial w1 with coefficients in [0,15] or [0,43].
 *              Input coefficients are assumed to be standard representatives.
 *
 * Arguments:   -       uint8   *r: pointer to output byte array with at least POLYW1_PACKEDBYTES bytes
 *              - const poly_D2 *a: pointer to input polynomial
 **********************************************************************************************************************/
void FsmSw_Dilithium2_Poly_W1Pack(uint8 *r, const poly_D2 *a)
{
  uint16 i;

  for (i = 0; i < (N_DILITHIUM / 4u); ++i)
  {
    r[3u * i]        = (uint8)((uint32)a->coeffs[4u * i]);
    r[3u * i]        = (uint8)((uint32)((uint32)r[3u * i] | ((uint32)a->coeffs[(4u * i) + 1u] << 6u)));
    r[(3u * i) + 1u] = (uint8)((uint32)a->coeffs[(4u * i) + 1u] >> 2);
    r[(3u * i) + 1u] = (uint8)((uint32)((uint32)r[(3u * i) + 1u] | ((uint32)a->coeffs[(4u * i) + 2u] << 4u)));
    r[(3u * i) + 2u] = (uint8)((uint32)a->coeffs[(4u * i) + 2u] >> 4);
    r[(3u * i) + 2u] = (uint8)((uint32)((uint32)r[(3u * i) + 2u] | ((uint32)a->coeffs[(4u * i) + 3u] << 2u)));
  }
}
