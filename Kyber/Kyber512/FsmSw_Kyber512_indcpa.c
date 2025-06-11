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
#include "FsmSw_CommonLib.h"
#include "FsmSw_Kyber512_params.h"
#include "FsmSw_Kyber512_poly.h"
#include "FsmSw_Kyber512_polyvec.h"
#include "FsmSw_Kyber_ntt.h"
#include "FsmSw_Kyber_poly.h"
#include "FsmSw_Kyber_symmetric.h"
#include "FsmSw_Types.h"

#include "FsmSw_Kyber512_indcpa.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
/* polyspace +2 MISRA2012:2.2 [Justified:]"The function is used deeper in the code for crucial calculations 
if the defines change, so it's not dead code." */
#define GEN_MATRIX_NBLOCKS ((((12u * KYBER_N) / (8u * 4096u)) / KYBER_Q) + ((XOF_BLOCKBYTES) / XOF_BLOCKBYTES))

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
static void fsmsw_kyber512_PackPk(uint8 r[KYBER512_INDCPA_PUBLICKEYBYTES], const polyvec512 *pk,
                                  const uint8 seed[KYBER_SYMBYTES]);
static void fsmsw_kyber512_UnpackPk(polyvec512 *pk, uint8 seed[KYBER_SYMBYTES],
                                    const uint8 packedpk[KYBER512_INDCPA_PUBLICKEYBYTES]);
static void fsmsw_kyber512_PackSk(uint8 r[KYBER512_INDCPA_SECRETKEYBYTES], const polyvec512 *sk);
static void fsmsw_kyber512_UnpackSk(polyvec512 *sk, const uint8 packedsk[KYBER512_INDCPA_SECRETKEYBYTES]);
static void fsmsw_kyber512_PackCiphertext(uint8 r[KYBER512_INDCPA_BYTES], const polyvec512 *b, const poly *v);
static void fsmsw_kyber512_UnpackCiphertext(polyvec512 *b, poly *v, const uint8 c[KYBER512_INDCPA_BYTES]);
static uint16 fsmsw_kyber512_RejUniform(sint16 *r, uint16 len, const uint8 *buf, uint16 buflen);

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/

/***********************************************************************************************************************
* Name:        fsmsw_kyber512_PackPk
*
* Description: Serialize the public key as concatenation of the
*              serialized vector of polynomials pk
*              and the public seed used to generate the matrix A.
*
* Arguments:         uint8      *r:    pointer to the output serialized public key
*              const polyvec512 *pk:   pointer to the input public-key polyvec
*              const uint8      *seed: pointer to the input public seed
***********************************************************************************************************************/
static void fsmsw_kyber512_PackPk(uint8 r[KYBER512_INDCPA_PUBLICKEYBYTES], const polyvec512 *pk,
                                  const uint8 seed[KYBER_SYMBYTES])
{
  uint32 i = 0;

  FsmSw_Kyber512_Polyvec_ToBytes(r, pk);

  for (i = 0; i < KYBER_SYMBYTES; i++)
  {
    r[i + KYBER512_POLYVECBYTES] = seed[i];
  }
}

/***********************************************************************************************************************
* Name:        fsmsw_kyber512_UnpackPk
*
* Description: De-serialize public key from a byte array;
*              approximate inverse of fsmsw_kyber512_PackPk
*
* Arguments:   -       polyvec512 *pk:       pointer to output public-key polynomial vector
*              -       uint8      *seed:     pointer to output seed to generate matrix A
*              - const uint8      *packedpk: pointer to input serialized public key
***********************************************************************************************************************/
static void fsmsw_kyber512_UnpackPk(polyvec512 *pk, uint8 seed[KYBER_SYMBYTES],
                                    const uint8 packedpk[KYBER512_INDCPA_PUBLICKEYBYTES])
{
  uint32 i = 0;

  FsmSw_Kyber512_Polyvec_FromBytes(pk, packedpk);

  for (i = 0; i < KYBER_SYMBYTES; i++)
  {
    seed[i] = packedpk[i + KYBER512_POLYVECBYTES];
  }
}

/***********************************************************************************************************************
* Name:        fsmsw_kyber512_PackSk
*
* Description: Serialize the secret key
*
* Arguments:   -       uint8      *r:  pointer to output serialized secret key
*              - const polyvec512 *sk: pointer to input vector of polynomials (secret key)
***********************************************************************************************************************/
static void fsmsw_kyber512_PackSk(uint8 r[KYBER512_INDCPA_SECRETKEYBYTES], const polyvec512 *sk)
{
  FsmSw_Kyber512_Polyvec_ToBytes(r, sk);
}

/***********************************************************************************************************************
* Name:        fsmsw_kyber512_UnpackSk
*
* Description: De-serialize the secret key; inverse of fsmsw_kyber512_PackSk
*
* Arguments:   -       polyvec512 *sk:       pointer to output vector of polynomials (secret key)
*              - const uint8      *packedsk: pointer to input serialized secret key
***********************************************************************************************************************/
static void fsmsw_kyber512_UnpackSk(polyvec512 *sk, const uint8 packedsk[KYBER512_INDCPA_SECRETKEYBYTES])
{
  FsmSw_Kyber512_Polyvec_FromBytes(sk, packedsk);
}

/***********************************************************************************************************************
* Name:        fsmsw_kyber512_PackCiphertext
*
* Description: Serialize the ciphertext as concatenation of the
*              compressed and serialized vector of polynomials b
*              and the compressed and serialized polynomial v
*
* Arguments:         uint8   *r: pointer to the output serialized ciphertext
*              const poly *b: pointer to the input vector of polynomials b
*              const poly *v: pointer to the input polynomial v
***********************************************************************************************************************/
static void fsmsw_kyber512_PackCiphertext(uint8 r[KYBER512_INDCPA_BYTES], const polyvec512 *b, const poly *v)
{
  FsmSw_Kyber512_Polyvec_Compress(r, b);
  FsmSw_Kyber512_Poly_Compress(&r[KYBER512_POLYVECCOMPRESSEDBYTES], v);
}

/***********************************************************************************************************************
* Name:        fsmsw_kyber512_UnpackCiphertext
*
* Description: De-serialize and decompress ciphertext from a byte array;
*              approximate inverse of fsmsw_kyber512_PackCiphertext
*
* Arguments:   -       polyvec512 *b: pointer to the output vector of polynomials b
*              -       poly    *v: pointer to the output polynomial v
*              - const uint8      *c: pointer to the input serialized ciphertext
***********************************************************************************************************************/
static void fsmsw_kyber512_UnpackCiphertext(polyvec512 *b, poly *v, const uint8 c[KYBER512_INDCPA_BYTES])
{
  FsmSw_Kyber512_Polyvec_Decompress(b, c);
  FsmSw_Kyber512_Poly_Decompress(v, &c[KYBER512_POLYVECCOMPRESSEDBYTES]);
}

/***********************************************************************************************************************
* Name:        fsmsw_kyber512_RejUniform
*
* Description: Run rejection sampling on uniform random bytes to generate
*              uniform random integers mod q
*
* Arguments:   -       int16_t *r:      pointer to output buffer
*              -       uint16   len:    requested number of 16-bit integers (uniform mod q)
*              - const uint8   *buf:    pointer to input buffer (assumed to be uniformly random bytes)
*              -       uint16   buflen: length of input buffer in bytes
*
* Returns number of sampled 16-bit integers (at most len)
***********************************************************************************************************************/
static uint16 fsmsw_kyber512_RejUniform(sint16 *r, uint16 len, const uint8 *buf, uint16 buflen)
{
  uint16 ctr  = 0;
  uint16 pos  = 0;
  uint16 val0 = 0;
  uint16 val1 = 0;

  while ((ctr < len) && ((pos + 3u) <= buflen))
  {
    val0 = (((uint16)buf[pos] >> 0u) | ((uint16)buf[pos + 1u] << 8u)) & 0xFFFu;
    val1 = (((uint16)buf[pos + 1u] >> 4u) | ((uint16)buf[pos + 2u] << 4u)) & 0xFFFu;
    pos  = pos + 3u;

    if (val0 < KYBER_Q)
    {
      r[ctr] = (sint16)val0;
      ctr++;
    }

    if ((ctr < len) && (val1 < KYBER_Q))
    {
      r[ctr] = (sint16)val1;
      ctr++;
    }
  }

  return ctr;
}

/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        FsmSw_Kyber512_Indcpa_GenMatrix
*
* Description: Deterministically generate matrix A (or the transpose of A)
*              from a seed. Entries of the matrix are polynomials that look
*              uniformly random. Performs rejection sampling on output of
*              a XOF
*
* Arguments:   -       polyvec512 *a:          pointer to ouptput matrix A
*              - const uint8      *seed:       pointer to input seed
*              -       uint8       transposed: boolean deciding whether A or A^T is generated
***********************************************************************************************************************/
/* polyspace +1 MISRA2012:8.7 [Justified:]"Not static for benchmarking */
void FsmSw_Kyber512_Indcpa_GenMatrix(polyvec512 *a, const uint8 seed[KYBER_SYMBYTES], uint8 transposed)
{
  uint8 i       = 0;
  uint8 j       = 0;
  uint16 buflen = 0;
  uint16 off    = 0;
  uint16 ctr    = 0;
  /* polyspace +2 MISRA2012:2.2 [Justified:]"Calculation is important if defines should change 
    and therefore not dead code" */
  uint8 buf[((GEN_MATRIX_NBLOCKS) * (XOF_BLOCKBYTES)) + 2u] = {0};
  xof_state state;

  for (i = 0; i < KYBER512_K; i++)
  {
    for (j = 0; j < KYBER512_K; j++)
    {
      if (transposed > 0u)
      {
        FsmSw_Kyber_Shake128_Absorb(&state, seed, i, j);
      }
      else
      {
        FsmSw_Kyber_Shake128_Absorb(&state, seed, j, i);
      }

      FsmSw_Fips202_Shake128_SqueezeBlocks(buf, GEN_MATRIX_NBLOCKS, &state);
      /* polyspace +2 MISRA2012:2.2 [Justified:]"Calculation is important if defines should change 
            and therefore not dead code" */
      buflen = ((GEN_MATRIX_NBLOCKS) * (XOF_BLOCKBYTES));
      ctr    = fsmsw_kyber512_RejUniform(a[i].vec[j].coeffs, KYBER_N, buf, buflen);

      while (ctr < KYBER_N)
      {
        off = buflen % 3u;

        FsmSw_Fips202_Shake128_SqueezeBlocks(&buf[off], 1u, &state);
        buflen = off + XOF_BLOCKBYTES;
        ctr    = ctr + (fsmsw_kyber512_RejUniform(&(a[i].vec[j].coeffs[ctr]), KYBER_N - ctr, buf, buflen));
      }
    }
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber512_Indcpa_KeyPair
*
* Description: Generates public and private key for the CPA-secure
*              public-key encryption scheme underlying Kyber
*
* Arguments:   - uint8 *pk: pointer to output public key (of length KYBER512_INDCPA_PUBLICKEYBYTES bytes)
*              - uint8 *sk: pointer to output private key (of length KYBER512_INDCPA_SECRETKEYBYTES bytes)
***********************************************************************************************************************/
void FsmSw_Kyber512_Indcpa_KeyPair(uint8 pk[KYBER512_INDCPA_PUBLICKEYBYTES], uint8 sk[KYBER512_INDCPA_SECRETKEYBYTES])
{
  uint8 i                        = 0;
  uint8 buf[2u * KYBER_SYMBYTES] = {0};
  const uint8 *publicseed        = buf;
  const uint8 *noiseseed         = &buf[KYBER_SYMBYTES];
  uint8 nonce                    = 0;

  polyvec512 a[KYBER512_K], e, pkpv, skpv;

  (void)FsmSw_CommonLib_RandomBytes(buf, KYBER_SYMBYTES);
  FsmSw_Fips202_Sha3_512(buf, buf, KYBER_SYMBYTES);

  FsmSw_Kyber512_Indcpa_GenMatrix(a, publicseed, 0);

  for (i = 0; i < KYBER512_K; i++)
  {
    FsmSw_Kyber512_Poly_GetNoiseEta1(&skpv.vec[i], noiseseed, nonce);
    nonce++;
  }

  for (i = 0; i < KYBER512_K; i++)
  {
    FsmSw_Kyber512_Poly_GetNoiseEta1(&e.vec[i], noiseseed, nonce);
    nonce++;
  }

  FsmSw_Kyber512_Polyvec_Ntt(&skpv);
  FsmSw_Kyber512_Polyvec_Ntt(&e);

  // matrix-vector multiplication
  for (i = 0; i < KYBER512_K; i++)
  {
    FsmSw_Kyber512_Polyvec_BasemulAccMontgomery(&pkpv.vec[i], &a[i], &skpv);
    FsmSw_Kyber_Poly_Tomont(&pkpv.vec[i]);
  }

  FsmSw_Kyber512_Polyvec_Add(&pkpv, &pkpv, &e);
  FsmSw_Kyber512_Polyvec_Reduce(&pkpv);

  fsmsw_kyber512_PackSk(sk, &skpv);
  fsmsw_kyber512_PackPk(pk, &pkpv, publicseed);
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber512_Indcpa_Enc
*
* Description: Encryption function of the CPA-secure
*              public-key encryption scheme underlying Kyber.
*
* Arguments:   -       uint8 *c:     pointer to output ciphertext (of length KYBER512_INDCPA_BYTES bytes)
*              - const uint8 *m:     pointer to input message (of length KYBER512_INDCPA_MSGBYTES bytes)
*              - const uint8 *pk:    pointer to input public key (of length KYBER512_INDCPA_PUBLICKEYBYTES bytes)
*              - const uint8 *coins: pointer to input random coins used as seed
*                                    (of length KYBER_SYMBYTES bytes) to deterministically
*                                    generate all randomness
***********************************************************************************************************************/
void FsmSw_Kyber512_Indcpa_Enc(uint8 c[KYBER512_INDCPA_BYTES], const uint8 m[KYBER512_INDCPA_MSGBYTES],
                               const uint8 pk[KYBER512_INDCPA_PUBLICKEYBYTES], const uint8 coins[KYBER_SYMBYTES])
{
  uint8 i                    = 0;
  uint8 seed[KYBER_SYMBYTES] = {0};
  uint8 nonce                = 0;
  polyvec512 sp              = {{{{0}}}};
  polyvec512 pkpv            = {{{{0}}}};
  polyvec512 ep              = {{{{0}}}};
  polyvec512 at[KYBER512_K]  = {{{{{0}}}}};
  polyvec512 b               = {{{{0}}}};
  poly v                     = {{0}};
  poly k                     = {{0}};
  poly epp                   = {{0}};

  fsmsw_kyber512_UnpackPk(&pkpv, seed, pk);
  FsmSw_Kyber512_Poly_FromMsg(&k, m);
  FsmSw_Kyber512_Indcpa_GenMatrix(at, seed, 1);

  for (i = 0; i < KYBER512_K; i++)
  {
    FsmSw_Kyber512_Poly_GetNoiseEta1(&(sp.vec[i]), coins, nonce);
    nonce++;
  }

  for (i = 0; i < KYBER512_K; i++)
  {
    FsmSw_Kyber512_Poly_GetNoiseEta2(&(ep.vec[i]), coins, nonce);
    nonce++;
  }

  FsmSw_Kyber512_Poly_GetNoiseEta2(&epp, coins, nonce);

  FsmSw_Kyber512_Polyvec_Ntt(&sp);

  // matrix-vector multiplication
  for (i = 0; i < KYBER512_K; i++)
  {
    FsmSw_Kyber512_Polyvec_BasemulAccMontgomery(&b.vec[i], &at[i], &sp);
  }

  FsmSw_Kyber512_Polyvec_BasemulAccMontgomery(&v, &pkpv, &sp);

  FsmSw_Kyber512_Polyvec_InvnttTomont(&b);
  FsmSw_Kyber_Poly_InvnttTomont(&v);

  FsmSw_Kyber512_Polyvec_Add(&b, &b, &ep);
  FsmSw_Kyber_Poly_Add(&v, &v, &epp);
  FsmSw_Kyber_Poly_Add(&v, &v, &k);
  FsmSw_Kyber512_Polyvec_Reduce(&b);
  FsmSw_Kyber_Poly_Reduce(&v);

  fsmsw_kyber512_PackCiphertext(c, &b, &v);
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber512_Indcpa_Dec
*
* Description: Decryption function of the CPA-secure
*              public-key encryption scheme underlying Kyber.
*
* Arguments:   -       uint8 *m:  pointer to output decrypted message (of length KYBER512_INDCPA_MSGBYTES bytes)
*              - const uint8 *c:  pointer to input ciphertext (of length KYBER512_INDCPA_BYTES bytes)
*              - const uint8 *sk: pointer to input secret key (of length KYBER512_INDCPA_SECRETKEYBYTES bytes)
***********************************************************************************************************************/
void FsmSw_Kyber512_Indcpa_Dec(uint8 m[KYBER512_INDCPA_MSGBYTES], const uint8 c[KYBER512_INDCPA_BYTES],
                               const uint8 sk[KYBER512_INDCPA_SECRETKEYBYTES])
{
  polyvec512 b    = {{{{0}}}};
  polyvec512 skpv = {{{{0}}}};
  poly v          = {{0}};
  poly mp         = {{0}};

  fsmsw_kyber512_UnpackCiphertext(&b, &v, c);
  fsmsw_kyber512_UnpackSk(&skpv, sk);

  FsmSw_Kyber512_Polyvec_Ntt(&b);
  FsmSw_Kyber512_Polyvec_BasemulAccMontgomery(&mp, &skpv, &b);
  FsmSw_Kyber_Poly_InvnttTomont(&mp);

  FsmSw_Kyber_Poly_Sub(&mp, &v, &mp);
  FsmSw_Kyber_Poly_Reduce(&mp);

  FsmSw_Kyber512_Poly_ToMsg(m, &mp);
}
