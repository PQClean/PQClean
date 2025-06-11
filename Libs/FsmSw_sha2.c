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
***********************************************************************************************************************/
/* Based on the public domain implementation in crypto_hash/FsmSw_Sha512/ref/ from */
/* polyspace +1 MISRA2012:3.1 [Justified:]"The comment is a link and therefore contains a slash" */
/* http://bench.cr.yp.to/supercop.html */
/* by D. J. Bernstein */
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_sha2.h"
#include "FsmSw_CommonLib.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
/* polyspace +80 MISRA2012:D4.9 [Justified:]"No refactoring of macros, as converting to, for example, 
inline functions would not provide significant benefits." */
#define SHR(x, c)     ((x) >> (c))
#define ROTR_32(x, c) (((x) >> (c)) | ((x) << (32 - (c))))
#define ROTR_64(x, c) (((x) >> (c)) | ((x) << (64 - (c))))

#define Ch(x, y, z)  (((x) & (y)) ^ (~(x) & (z)))
#define Maj(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

#define Sigma0_32(x) (ROTR_32(x, 2) ^ ROTR_32(x, 13) ^ ROTR_32(x, 22))
#define Sigma1_32(x) (ROTR_32(x, 6) ^ ROTR_32(x, 11) ^ ROTR_32(x, 25))
#define sigma0_32(x) (ROTR_32(x, 7) ^ ROTR_32(x, 18) ^ SHR(x, 3))
#define sigma1_32(x) (ROTR_32(x, 17) ^ ROTR_32(x, 19) ^ SHR(x, 10))

#define Sigma0_64(x) (ROTR_64(x, 28) ^ ROTR_64(x, 34) ^ ROTR_64(x, 39))
#define Sigma1_64(x) (ROTR_64(x, 14) ^ ROTR_64(x, 18) ^ ROTR_64(x, 41))
#define sigma0_64(x) (ROTR_64(x, 1) ^ ROTR_64(x, 8) ^ SHR(x, 7))
#define sigma1_64(x) (ROTR_64(x, 19) ^ ROTR_64(x, 61) ^ SHR(x, 6))

#define M_32(w0, w14, w9, w1) w0 = sigma1_32(w14) + (w9) + sigma0_32(w1) + (w0);
#define M_64(w0, w14, w9, w1) w0 = sigma1_64(w14) + (w9) + sigma0_64(w1) + (w0);

#define EXPAND_32                                                                                                      \
  M_32(w0, w14, w9, w1)                                                                                                \
  M_32(w1, w15, w10, w2)                                                                                               \
  M_32(w2, w0, w11, w3)                                                                                                \
  M_32(w3, w1, w12, w4)                                                                                                \
  M_32(w4, w2, w13, w5)                                                                                                \
  M_32(w5, w3, w14, w6)                                                                                                \
  M_32(w6, w4, w15, w7)                                                                                                \
  M_32(w7, w5, w0, w8)                                                                                                 \
  M_32(w8, w6, w1, w9)                                                                                                 \
  M_32(w9, w7, w2, w10)                                                                                                \
  M_32(w10, w8, w3, w11)                                                                                               \
  M_32(w11, w9, w4, w12)                                                                                               \
  M_32(w12, w10, w5, w13)                                                                                              \
  M_32(w13, w11, w6, w14)                                                                                              \
  M_32(w14, w12, w7, w15)                                                                                              \
  M_32(w15, w13, w8, w0)

#define EXPAND_64                                                                                                      \
  M_64(w0, w14, w9, w1)                                                                                                \
  M_64(w1, w15, w10, w2)                                                                                               \
  M_64(w2, w0, w11, w3)                                                                                                \
  M_64(w3, w1, w12, w4)                                                                                                \
  M_64(w4, w2, w13, w5)                                                                                                \
  M_64(w5, w3, w14, w6)                                                                                                \
  M_64(w6, w4, w15, w7)                                                                                                \
  M_64(w7, w5, w0, w8)                                                                                                 \
  M_64(w8, w6, w1, w9)                                                                                                 \
  M_64(w9, w7, w2, w10)                                                                                                \
  M_64(w10, w8, w3, w11)                                                                                               \
  M_64(w11, w9, w4, w12)                                                                                               \
  M_64(w12, w10, w5, w13)                                                                                              \
  M_64(w13, w11, w6, w14)                                                                                              \
  M_64(w14, w12, w7, w15)                                                                                              \
  M_64(w15, w13, w8, w0)

#define F_32(w, k)                                                                                                     \
  T1 = h + Sigma1_32(e) + Ch(e, f, g) + (k) + (w);                                                                     \
  T2 = Sigma0_32(a) + Maj(a, b, c);                                                                                    \
  h  = g;                                                                                                              \
  g  = f;                                                                                                              \
  f  = e;                                                                                                              \
  e  = d + T1;                                                                                                         \
  d  = c;                                                                                                              \
  c  = b;                                                                                                              \
  b  = a;                                                                                                              \
  a  = T1 + T2;

#define F_64(w, k)                                                                                                     \
  T1 = h + Sigma1_64(e) + Ch(e, f, g) + (k) + (w);                                                                     \
  T2 = Sigma0_64(a) + Maj(a, b, c);                                                                                    \
  h  = g;                                                                                                              \
  g  = f;                                                                                                              \
  f  = e;                                                                                                              \
  e  = d + T1;                                                                                                         \
  d  = c;                                                                                                              \
  c  = b;                                                                                                              \
  b  = a;                                                                                                              \
  a  = T1 + T2;

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/
static const uint8 iv_224[32] = {0xc1, 0x05, 0x9e, 0xd8, 0x36, 0x7c, 0xd5, 0x07, 0x30, 0x70, 0xdd,
                                 0x17, 0xf7, 0x0e, 0x59, 0x39, 0xff, 0xc0, 0x0b, 0x31, 0x68, 0x58,
                                 0x15, 0x11, 0x64, 0xf9, 0x8f, 0xa7, 0xbe, 0xfa, 0x4f, 0xa4};

static const uint8 iv_256[32] = {0x6a, 0x09, 0xe6, 0x67, 0xbb, 0x67, 0xae, 0x85, 0x3c, 0x6e, 0xf3,
                                 0x72, 0xa5, 0x4f, 0xf5, 0x3a, 0x51, 0x0e, 0x52, 0x7f, 0x9b, 0x05,
                                 0x68, 0x8c, 0x1f, 0x83, 0xd9, 0xab, 0x5b, 0xe0, 0xcd, 0x19};

static const uint8 iv_384[64] = {0xcb, 0xbb, 0x9d, 0x5d, 0xc1, 0x05, 0x9e, 0xd8, 0x62, 0x9a, 0x29, 0x2a, 0x36,
                                 0x7c, 0xd5, 0x07, 0x91, 0x59, 0x01, 0x5a, 0x30, 0x70, 0xdd, 0x17, 0x15, 0x2f,
                                 0xec, 0xd8, 0xf7, 0x0e, 0x59, 0x39, 0x67, 0x33, 0x26, 0x67, 0xff, 0xc0, 0x0b,
                                 0x31, 0x8e, 0xb4, 0x4a, 0x87, 0x68, 0x58, 0x15, 0x11, 0xdb, 0x0c, 0x2e, 0x0d,
                                 0x64, 0xf9, 0x8f, 0xa7, 0x47, 0xb5, 0x48, 0x1d, 0xbe, 0xfa, 0x4f, 0xa4};

static const uint8 iv_512[64] = {0x6a, 0x09, 0xe6, 0x67, 0xf3, 0xbc, 0xc9, 0x08, 0xbb, 0x67, 0xae, 0x85, 0x84,
                                 0xca, 0xa7, 0x3b, 0x3c, 0x6e, 0xf3, 0x72, 0xfe, 0x94, 0xf8, 0x2b, 0xa5, 0x4f,
                                 0xf5, 0x3a, 0x5f, 0x1d, 0x36, 0xf1, 0x51, 0x0e, 0x52, 0x7f, 0xad, 0xe6, 0x82,
                                 0xd1, 0x9b, 0x05, 0x68, 0x8c, 0x2b, 0x3e, 0x6c, 0x1f, 0x1f, 0x83, 0xd9, 0xab,
                                 0xfb, 0x41, 0xbd, 0x6b, 0x5b, 0xe0, 0xcd, 0x19, 0x13, 0x7e, 0x21, 0x79};

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/
static uint32 fsmsw_sha2_crypto_HashBlocksSha256(uint8 *statebytes, const uint8 *in, uint32 inlen);
static uint32 fsmsw_sha2_crypto_HashBlocksSha512(uint8 *statebytes, const uint8 *in, uint32 inlen);
static uint32 fsmsw_sha2_LoadBigendian32(const uint8 *x);
static uint64 fsmsw_sha2_LoadBigendian64(const uint8 *x);
static void fsmsw_sha2_StoreBigendian32(uint8 *x, uint64 u);
static void fsmsw_sha2_StoreBigendian64(uint8 *x, uint64 u);
static void fsmsw_sha224_IncInit(sha224ctx *state);
static void fsmsw_sha384_IncInit(sha384ctx *state);
static void fsmsw_sha224_IncFinalize(uint8 *out, sha224ctx *state, const uint8 *in, uint32 inlen);
static void fsmsw_sha384_IncFinalize(uint8 *out, sha384ctx *state, const uint8 *in, uint32 inlen);

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        fsmsw_sha2_crypto_HashBlocksSha256
*
* Description: t.b.d.
*
* Arguments:   -       uint8  *statebytes: t.b.d.
*              - const uint8  *in:         t.b.d.
*              -       uint32  inlen:      t.b.d.
*
***********************************************************************************************************************/
static uint32 fsmsw_sha2_crypto_HashBlocksSha256(uint8 *statebytes, const uint8 *in, uint32 inlen)
{
  uint32 state[8] = {0};
  uint32 a        = 0;
  uint32 b        = 0;
  uint32 c        = 0;
  uint32 d        = 0;
  uint32 e        = 0;
  uint32 f        = 0;
  uint32 g        = 0;
  uint32 h        = 0;
  uint32 T1       = 0;
  uint32 T2       = 0;

  /* in_temp and inlen_temp are used to avoid modifying the input. */
  const uint8 *in_temp = in;
  uint32 inlen_temp    = inlen;

  a        = fsmsw_sha2_LoadBigendian32(&statebytes[0]);
  state[0] = a;
  b        = fsmsw_sha2_LoadBigendian32(&statebytes[4]);
  state[1] = b;
  c        = fsmsw_sha2_LoadBigendian32(&statebytes[8]);
  state[2] = c;
  d        = fsmsw_sha2_LoadBigendian32(&statebytes[12]);
  state[3] = d;
  e        = fsmsw_sha2_LoadBigendian32(&statebytes[16]);
  state[4] = e;
  f        = fsmsw_sha2_LoadBigendian32(&statebytes[20]);
  state[5] = f;
  g        = fsmsw_sha2_LoadBigendian32(&statebytes[24]);
  state[6] = g;
  h        = fsmsw_sha2_LoadBigendian32(&statebytes[28]);
  state[7] = h;

  while (inlen_temp >= 64u)
  {
    uint32 w0  = fsmsw_sha2_LoadBigendian32(&in_temp[0]);
    uint32 w1  = fsmsw_sha2_LoadBigendian32(&in_temp[4]);
    uint32 w2  = fsmsw_sha2_LoadBigendian32(&in_temp[8]);
    uint32 w3  = fsmsw_sha2_LoadBigendian32(&in_temp[12]);
    uint32 w4  = fsmsw_sha2_LoadBigendian32(&in_temp[16]);
    uint32 w5  = fsmsw_sha2_LoadBigendian32(&in_temp[20]);
    uint32 w6  = fsmsw_sha2_LoadBigendian32(&in_temp[24]);
    uint32 w7  = fsmsw_sha2_LoadBigendian32(&in_temp[28]);
    uint32 w8  = fsmsw_sha2_LoadBigendian32(&in_temp[32]);
    uint32 w9  = fsmsw_sha2_LoadBigendian32(&in_temp[36]);
    uint32 w10 = fsmsw_sha2_LoadBigendian32(&in_temp[40]);
    uint32 w11 = fsmsw_sha2_LoadBigendian32(&in_temp[44]);
    uint32 w12 = fsmsw_sha2_LoadBigendian32(&in_temp[48]);
    uint32 w13 = fsmsw_sha2_LoadBigendian32(&in_temp[52]);
    uint32 w14 = fsmsw_sha2_LoadBigendian32(&in_temp[56]);
    uint32 w15 = fsmsw_sha2_LoadBigendian32(&in_temp[60]);

    F_32(w0, 0x428a2f98u)
    F_32(w1, 0x71374491u)
    F_32(w2, 0xb5c0fbcfu)
    F_32(w3, 0xe9b5dba5u)
    F_32(w4, 0x3956c25bu)
    F_32(w5, 0x59f111f1u)
    F_32(w6, 0x923f82a4u)
    F_32(w7, 0xab1c5ed5u)
    F_32(w8, 0xd807aa98u)
    F_32(w9, 0x12835b01u)
    F_32(w10, 0x243185beu)
    F_32(w11, 0x550c7dc3u)
    F_32(w12, 0x72be5d74u)
    F_32(w13, 0x80deb1feu)
    F_32(w14, 0x9bdc06a7u)
    F_32(w15, 0xc19bf174u)

    EXPAND_32

    F_32(w0, 0xe49b69c1u)
    F_32(w1, 0xefbe4786u)
    F_32(w2, 0x0fc19dc6u)
    F_32(w3, 0x240ca1ccu)
    F_32(w4, 0x2de92c6fu)
    F_32(w5, 0x4a7484aau)
    F_32(w6, 0x5cb0a9dcu)
    F_32(w7, 0x76f988dau)
    F_32(w8, 0x983e5152u)
    F_32(w9, 0xa831c66du)
    F_32(w10, 0xb00327c8u)
    F_32(w11, 0xbf597fc7u)
    F_32(w12, 0xc6e00bf3u)
    F_32(w13, 0xd5a79147u)
    F_32(w14, 0x06ca6351u)
    F_32(w15, 0x14292967u)

    EXPAND_32

    F_32(w0, 0x27b70a85u)
    F_32(w1, 0x2e1b2138u)
    F_32(w2, 0x4d2c6dfcu)
    F_32(w3, 0x53380d13u)
    F_32(w4, 0x650a7354u)
    F_32(w5, 0x766a0abbu)
    F_32(w6, 0x81c2c92eu)
    F_32(w7, 0x92722c85u)
    F_32(w8, 0xa2bfe8a1u)
    F_32(w9, 0xa81a664bu)
    F_32(w10, 0xc24b8b70u)
    F_32(w11, 0xc76c51a3u)
    F_32(w12, 0xd192e819u)
    F_32(w13, 0xd6990624u)
    F_32(w14, 0xf40e3585u)
    F_32(w15, 0x106aa070u)

    EXPAND_32

    F_32(w0, 0x19a4c116u)
    F_32(w1, 0x1e376c08u)
    F_32(w2, 0x2748774cu)
    F_32(w3, 0x34b0bcb5u)
    F_32(w4, 0x391c0cb3u)
    F_32(w5, 0x4ed8aa4au)
    F_32(w6, 0x5b9cca4fu)
    F_32(w7, 0x682e6ff3u)
    F_32(w8, 0x748f82eeu)
    F_32(w9, 0x78a5636fu)
    F_32(w10, 0x84c87814u)
    F_32(w11, 0x8cc70208u)
    F_32(w12, 0x90befffau)
    F_32(w13, 0xa4506cebu)
    F_32(w14, 0xbef9a3f7u)
    F_32(w15, 0xc67178f2u)

    a += state[0];
    b += state[1];
    c += state[2];
    d += state[3];
    e += state[4];
    f += state[5];
    g += state[6];
    h += state[7];

    state[0] = a;
    state[1] = b;
    state[2] = c;
    state[3] = d;
    state[4] = e;
    state[5] = f;
    state[6] = g;
    state[7] = h;

    in_temp    = &in_temp[64];
    inlen_temp = inlen_temp - 64u;
  }

  fsmsw_sha2_StoreBigendian32(&statebytes[0], state[0]);
  fsmsw_sha2_StoreBigendian32(&statebytes[4], state[1]);
  fsmsw_sha2_StoreBigendian32(&statebytes[8], state[2]);
  fsmsw_sha2_StoreBigendian32(&statebytes[12], state[3]);
  fsmsw_sha2_StoreBigendian32(&statebytes[16], state[4]);
  fsmsw_sha2_StoreBigendian32(&statebytes[20], state[5]);
  fsmsw_sha2_StoreBigendian32(&statebytes[24], state[6]);
  fsmsw_sha2_StoreBigendian32(&statebytes[28], state[7]);

  return inlen_temp;
}

/***********************************************************************************************************************
* Name:        fsmsw_sha2_crypto_HashBlocksSha512
*
* Description: t.b.d.
*
* Arguments:   -       uint8  *statebytes: t.b.d.
*              - const uint8  *in:         t.b.d.
*              -       uint32  inlen:      t.b.d.
*
***********************************************************************************************************************/
/* polyspace +2 CODE-METRICS:STMT [Justified:]"[Value: 926]More instructions are necessary 
because the MISRA 17.8 warnings require temporary variables to be fixed." */
static uint32 fsmsw_sha2_crypto_HashBlocksSha512(uint8 *statebytes, const uint8 *in, uint32 inlen)
{
  uint64 state[8];
  uint64 a;
  uint64 b;
  uint64 c;
  uint64 d;
  uint64 e;
  uint64 f;
  uint64 g;
  uint64 h;
  uint64 T1;
  uint64 T2;

  /* in_temp and inlen_temp are used to avoid modifying the input. */
  const uint8 *in_temp = in;
  uint32 inlen_temp    = inlen;

  a        = fsmsw_sha2_LoadBigendian64(&statebytes[0]);
  state[0] = a;
  b        = fsmsw_sha2_LoadBigendian64(&statebytes[8]);
  state[1] = b;
  c        = fsmsw_sha2_LoadBigendian64(&statebytes[16]);
  state[2] = c;
  d        = fsmsw_sha2_LoadBigendian64(&statebytes[24]);
  state[3] = d;
  e        = fsmsw_sha2_LoadBigendian64(&statebytes[32]);
  state[4] = e;
  f        = fsmsw_sha2_LoadBigendian64(&statebytes[40]);
  state[5] = f;
  g        = fsmsw_sha2_LoadBigendian64(&statebytes[48]);
  state[6] = g;
  h        = fsmsw_sha2_LoadBigendian64(&statebytes[56]);
  state[7] = h;

  while (inlen_temp >= 128u)
  {
    uint64 w0  = fsmsw_sha2_LoadBigendian64(&in_temp[0]);
    uint64 w1  = fsmsw_sha2_LoadBigendian64(&in_temp[8]);
    uint64 w2  = fsmsw_sha2_LoadBigendian64(&in_temp[16]);
    uint64 w3  = fsmsw_sha2_LoadBigendian64(&in_temp[24]);
    uint64 w4  = fsmsw_sha2_LoadBigendian64(&in_temp[32]);
    uint64 w5  = fsmsw_sha2_LoadBigendian64(&in_temp[40]);
    uint64 w6  = fsmsw_sha2_LoadBigendian64(&in_temp[48]);
    uint64 w7  = fsmsw_sha2_LoadBigendian64(&in_temp[56]);
    uint64 w8  = fsmsw_sha2_LoadBigendian64(&in_temp[64]);
    uint64 w9  = fsmsw_sha2_LoadBigendian64(&in_temp[72]);
    uint64 w10 = fsmsw_sha2_LoadBigendian64(&in_temp[80]);
    uint64 w11 = fsmsw_sha2_LoadBigendian64(&in_temp[88]);
    uint64 w12 = fsmsw_sha2_LoadBigendian64(&in_temp[96]);
    uint64 w13 = fsmsw_sha2_LoadBigendian64(&in_temp[104]);
    uint64 w14 = fsmsw_sha2_LoadBigendian64(&in_temp[112]);
    uint64 w15 = fsmsw_sha2_LoadBigendian64(&in_temp[120]);

    F_64(w0, 0x428a2f98d728ae22ULL)
    F_64(w1, 0x7137449123ef65cdULL)
    F_64(w2, 0xb5c0fbcfec4d3b2fULL)
    F_64(w3, 0xe9b5dba58189dbbcULL)
    F_64(w4, 0x3956c25bf348b538ULL)
    F_64(w5, 0x59f111f1b605d019ULL)
    F_64(w6, 0x923f82a4af194f9bULL)
    F_64(w7, 0xab1c5ed5da6d8118ULL)
    F_64(w8, 0xd807aa98a3030242ULL)
    F_64(w9, 0x12835b0145706fbeULL)
    F_64(w10, 0x243185be4ee4b28cULL)
    F_64(w11, 0x550c7dc3d5ffb4e2ULL)
    F_64(w12, 0x72be5d74f27b896fULL)
    F_64(w13, 0x80deb1fe3b1696b1ULL)
    F_64(w14, 0x9bdc06a725c71235ULL)
    F_64(w15, 0xc19bf174cf692694ULL)

    EXPAND_64

    F_64(w0, 0xe49b69c19ef14ad2ULL)
    F_64(w1, 0xefbe4786384f25e3ULL)
    F_64(w2, 0x0fc19dc68b8cd5b5ULL)
    F_64(w3, 0x240ca1cc77ac9c65ULL)
    F_64(w4, 0x2de92c6f592b0275ULL)
    F_64(w5, 0x4a7484aa6ea6e483ULL)
    F_64(w6, 0x5cb0a9dcbd41fbd4ULL)
    F_64(w7, 0x76f988da831153b5ULL)
    F_64(w8, 0x983e5152ee66dfabULL)
    F_64(w9, 0xa831c66d2db43210ULL)
    F_64(w10, 0xb00327c898fb213fULL)
    F_64(w11, 0xbf597fc7beef0ee4ULL)
    F_64(w12, 0xc6e00bf33da88fc2ULL)
    F_64(w13, 0xd5a79147930aa725ULL)
    F_64(w14, 0x06ca6351e003826fULL)
    F_64(w15, 0x142929670a0e6e70ULL)

    EXPAND_64

    F_64(w0, 0x27b70a8546d22ffcULL)
    F_64(w1, 0x2e1b21385c26c926ULL)
    F_64(w2, 0x4d2c6dfc5ac42aedULL)
    F_64(w3, 0x53380d139d95b3dfULL)
    F_64(w4, 0x650a73548baf63deULL)
    F_64(w5, 0x766a0abb3c77b2a8ULL)
    F_64(w6, 0x81c2c92e47edaee6ULL)
    F_64(w7, 0x92722c851482353bULL)
    F_64(w8, 0xa2bfe8a14cf10364ULL)
    F_64(w9, 0xa81a664bbc423001ULL)
    F_64(w10, 0xc24b8b70d0f89791ULL)
    F_64(w11, 0xc76c51a30654be30ULL)
    F_64(w12, 0xd192e819d6ef5218ULL)
    F_64(w13, 0xd69906245565a910ULL)
    F_64(w14, 0xf40e35855771202aULL)
    F_64(w15, 0x106aa07032bbd1b8ULL)

    EXPAND_64

    F_64(w0, 0x19a4c116b8d2d0c8ULL)
    F_64(w1, 0x1e376c085141ab53ULL)
    F_64(w2, 0x2748774cdf8eeb99ULL)
    F_64(w3, 0x34b0bcb5e19b48a8ULL)
    F_64(w4, 0x391c0cb3c5c95a63ULL)
    F_64(w5, 0x4ed8aa4ae3418acbULL)
    F_64(w6, 0x5b9cca4f7763e373ULL)
    F_64(w7, 0x682e6ff3d6b2b8a3ULL)
    F_64(w8, 0x748f82ee5defb2fcULL)
    F_64(w9, 0x78a5636f43172f60ULL)
    F_64(w10, 0x84c87814a1f0ab72ULL)
    F_64(w11, 0x8cc702081a6439ecULL)
    F_64(w12, 0x90befffa23631e28ULL)
    F_64(w13, 0xa4506cebde82bde9ULL)
    F_64(w14, 0xbef9a3f7b2c67915ULL)
    F_64(w15, 0xc67178f2e372532bULL)

    EXPAND_64

    F_64(w0, 0xca273eceea26619cULL)
    F_64(w1, 0xd186b8c721c0c207ULL)
    F_64(w2, 0xeada7dd6cde0eb1eULL)
    F_64(w3, 0xf57d4f7fee6ed178ULL)
    F_64(w4, 0x06f067aa72176fbaULL)
    F_64(w5, 0x0a637dc5a2c898a6ULL)
    F_64(w6, 0x113f9804bef90daeULL)
    F_64(w7, 0x1b710b35131c471bULL)
    F_64(w8, 0x28db77f523047d84ULL)
    F_64(w9, 0x32caab7b40c72493ULL)
    F_64(w10, 0x3c9ebe0a15c9bebcULL)
    F_64(w11, 0x431d67c49c100d4cULL)
    F_64(w12, 0x4cc5d4becb3e42b6ULL)
    F_64(w13, 0x597f299cfc657e2aULL)
    F_64(w14, 0x5fcb6fab3ad6faecULL)
    F_64(w15, 0x6c44198c4a475817ULL)

    a += state[0];
    b += state[1];
    c += state[2];
    d += state[3];
    e += state[4];
    f += state[5];
    g += state[6];
    h += state[7];

    state[0] = a;
    state[1] = b;
    state[2] = c;
    state[3] = d;
    state[4] = e;
    state[5] = f;
    state[6] = g;
    state[7] = h;

    in_temp    = &in_temp[128];
    inlen_temp = inlen_temp - 128u;
  }

  fsmsw_sha2_StoreBigendian64(&statebytes[0], state[0]);
  fsmsw_sha2_StoreBigendian64(&statebytes[8], state[1]);
  fsmsw_sha2_StoreBigendian64(&statebytes[16], state[2]);
  fsmsw_sha2_StoreBigendian64(&statebytes[24], state[3]);
  fsmsw_sha2_StoreBigendian64(&statebytes[32], state[4]);
  fsmsw_sha2_StoreBigendian64(&statebytes[40], state[5]);
  fsmsw_sha2_StoreBigendian64(&statebytes[48], state[6]);
  fsmsw_sha2_StoreBigendian64(&statebytes[56], state[7]);

  return inlen_temp;
}

/***********************************************************************************************************************
* Name:        fsmsw_sha2_LoadBigendian32
*
* Description: t.b.d.
*
* Arguments:   - uint8  *x: t.b.d.
*
***********************************************************************************************************************/
static uint32 fsmsw_sha2_LoadBigendian32(const uint8 *x)
{
  return (uint32)(x[3]) | (((uint32)(x[2])) << 8) | (((uint32)(x[1])) << 16) | (((uint32)(x[0])) << 24);
}

/***********************************************************************************************************************
* Name:        fsmsw_sha2_LoadBigendian64
*
* Description: t.b.d.
*
* Arguments:   - const uint8  *x: t.b.d.
*
***********************************************************************************************************************/
static uint64 fsmsw_sha2_LoadBigendian64(const uint8 *x)
{
  return (uint64)(x[7]) | (((uint64)(x[6])) << 8) | (((uint64)(x[5])) << 16) | (((uint64)(x[4])) << 24) |
         (((uint64)(x[3])) << 32) | (((uint64)(x[2])) << 40) | (((uint64)(x[1])) << 48) | (((uint64)(x[0])) << 56);
}

/***********************************************************************************************************************
* Name:        fsmsw_sha2_StoreBigendian32
*
* Description: t.b.d.
*
* Arguments:   - uint8  *x: t.b.d.
*              - uint64  u: t.b.d.
*
***********************************************************************************************************************/
static void fsmsw_sha2_StoreBigendian32(uint8 *x, uint64 u)
{
  /* u_temp is used to avoid modifying the input. */
  uint64 u_temp = u;

  x[3] = (uint8)u_temp;
  u_temp >>= 8;
  x[2] = (uint8)u_temp;
  u_temp >>= 8;
  x[1] = (uint8)u_temp;
  u_temp >>= 8;
  x[0] = (uint8)u_temp;
}

/***********************************************************************************************************************
* Name:        fsmsw_sha2_StoreBigendian64
*
* Description: t.b.d.
*
* Arguments:   - uint8  *x: t.b.d.
*              - uint64  u: t.b.d.
*
***********************************************************************************************************************/
static void fsmsw_sha2_StoreBigendian64(uint8 *x, uint64 u)
{
  /* u_temp is used to avoid modifying the input. */
  uint64 u_temp = u;

  x[7] = (uint8)u_temp;
  u_temp >>= 8;
  x[6] = (uint8)u_temp;
  u_temp >>= 8;
  x[5] = (uint8)u_temp;
  u_temp >>= 8;
  x[4] = (uint8)u_temp;
  u_temp >>= 8;
  x[3] = (uint8)u_temp;
  u_temp >>= 8;
  x[2] = (uint8)u_temp;
  u_temp >>= 8;
  x[1] = (uint8)u_temp;
  u_temp >>= 8;
  x[0] = (uint8)u_temp;
}

/***********************************************************************************************************************
* Name:        fsmsw_sha224_IncInit
*
* Description: Initialize the incremental hashing API. Can't be called multiple times.
*
* Arguments:   - sha224ctx *state: t.b.d.
*
***********************************************************************************************************************/
static void fsmsw_sha224_IncInit(sha224ctx *state)
{
  for (uint32 i = 0; i < 32u; ++i)
  {
    state->ctx[i] = iv_224[i];
  }
  for (uint32 i = 32; i < 40u; ++i)
  {
    state->ctx[i] = 0;
  }
}

/***********************************************************************************************************************
* Name:        fsmsw_sha384_IncInit
*
* Description: Initialize the incremental hashing API.
*
* Arguments:   - sha384ctx *state: t.b.d.
*
***********************************************************************************************************************/
static void fsmsw_sha384_IncInit(sha384ctx *state)
{
  for (uint32 i = 0; i < 64u; ++i)
  {
    state->ctx[i] = iv_384[i];
  }
  for (uint32 i = 64; i < 72u; ++i)
  {
    state->ctx[i] = 0;
  }
}

/***********************************************************************************************************************
* Name:        fsmsw_sha224_IncFinalize
*
* Description: Finalize and obtain the digest. If applicable, this function will free the memory associated with
*              the sha224ctx.
*
* Arguments:   -       uint8     *out:   t.b.d.
*              -       sha224ctx *state: t.b.d.
*              - const uint8     *in:    t.b.d.
*              -       uint32     inlen: t.b.d.
*
***********************************************************************************************************************/
static void fsmsw_sha224_IncFinalize(uint8 *out, sha224ctx *state, const uint8 *in, uint32 inlen)
{
  uint8 tmp[32] = {0};

  /* polyspace +2 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  FsmSw_Sha256_IncFinalize(tmp, (sha256ctx *)((void *)state), in, inlen);

  for (uint32 i = 0; i < 28u; ++i)
  {
    out[i] = tmp[i];
  }
}

/***********************************************************************************************************************
* Name:        fsmsw_sha384_IncFinalize
*
* Description: Finalize and obtain the digest. If applicable, this function will free the memory associated with the
*              sha384ctx.
*
* Arguments:   -       uint8     *out:   t.b.d.
*              -       sha384ctx *state: t.b.d.
*              - const uint8     *in:    t.b.d.
*              -       uint32     inlen: t.b.d.
*
***********************************************************************************************************************/
static void fsmsw_sha384_IncFinalize(uint8 *out, sha384ctx *state, const uint8 *in, uint32 inlen)
{
  uint8 tmp[64] = {0};

  /* polyspace +2 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  FsmSw_Sha512_IncFinalize(tmp, (sha512ctx *)((void *)state), in, inlen);

  for (uint32 i = 0; i < 48u; ++i)
  {
    out[i] = tmp[i];
  }
}
/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        FsmSw_Sha256_IncInit
*
* Description: Initialize the incremental hashing API.
*
* Arguments:   - sha256ctx *state: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Sha256_IncInit(sha256ctx *state)
{
  for (uint32 i = 0; i < 32u; ++i)
  {
    state->ctx[i] = iv_256[i];
  }
  for (uint32 i = 32; i < 40u; ++i)
  {
    state->ctx[i] = 0;
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Sha512_IncInit
*
* Description: Initialize the incremental hashing API.
*
* Arguments:   - sha512ctx *state: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Sha512_IncInit(sha512ctx *state)
{
  for (uint32 i = 0; i < 64u; ++i)
  {
    state->ctx[i] = iv_512[i];
  }
  for (uint32 i = 64; i < 72u; ++i)
  {
    state->ctx[i] = 0;
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Sha224_IncCtxClone
*
* Description: Copy the hashing state
*
* Arguments:   -       sha224ctx *stateout: t.b.d.
*              - const sha224ctx *statein
*
***********************************************************************************************************************/
void FsmSw_Sha224_IncCtxClone(sha224ctx *stateout, const sha224ctx *statein)
{
  FsmSw_CommonLib_MemCpy(stateout->ctx, statein->ctx, PQC_SHA256CTX_BYTES);
}

/***********************************************************************************************************************
* Name:        FsmSw_Sha256_IncCtxClone
*
* Description: Copy the hashing state.
*
* Arguments:   -       sha256ctx *stateout: t.b.d.
*              - const sha256ctx *statein:  t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Sha256_IncCtxClone(sha256ctx *stateout, const sha256ctx *statein)
{
  FsmSw_CommonLib_MemCpy(stateout->ctx, statein->ctx, PQC_SHA256CTX_BYTES);
}

/***********************************************************************************************************************
* Name:        FsmSw_Sha384_IncCtxClone
*
* Description: Copy the hashing state.
*
* Arguments:   -       sha384ctx *stateout: t.b.d.
*              - const sha384ctx *statein:  t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Sha384_IncCtxClone(sha384ctx *stateout, const sha384ctx *statein)
{
  FsmSw_CommonLib_MemCpy(stateout->ctx, statein->ctx, PQC_SHA512CTX_BYTES);
}

/***********************************************************************************************************************
* Name:        FsmSw_Sha512_IncCtxClone
*
* Description: Copy the hashing state.
*
* Arguments:   -       sha512ctx *stateout: t.b.d.
*              - const sha512ctx *statein:  t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Sha512_IncCtxClone(sha512ctx *stateout, const sha512ctx *statein)
{
  FsmSw_CommonLib_MemCpy(stateout->ctx, statein->ctx, PQC_SHA512CTX_BYTES);
}

/***********************************************************************************************************************
* Name:        FsmSw_Sha256_IncBlocks
*
* Description: Absorb blocks.
*
* Arguments:   -       sha256ctx *state:    t.b.d.
*              - const uint8     *in:       t.b.d.
*              -       uint32     inblocks: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Sha256_IncBlocks(sha256ctx *state, const uint8 *in, uint32 inblocks)
{
  uint64 bytes = fsmsw_sha2_LoadBigendian64(&state->ctx[32]);

  (void)fsmsw_sha2_crypto_HashBlocksSha256(state->ctx, in, (uint32)(64u * inblocks));
  bytes = bytes + (uint64)((uint32)(64u * inblocks));

  fsmsw_sha2_StoreBigendian64(&state->ctx[32], bytes);
}

/***********************************************************************************************************************
* Name:        FsmSw_Sha224_IncBlocks
*
* Description: Absorb blocks
*
* Arguments:   -       sha224ctx *state:    t.b.d.
*              - const uint8     *in:       t.b.d.
*              -       uint32     inblocks: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Sha224_IncBlocks(sha224ctx *state, const uint8 *in, uint32 inblocks)
{
  /* polyspace +2 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  FsmSw_Sha256_IncBlocks((sha256ctx *)((void *)state), in, inblocks);
}

/***********************************************************************************************************************
* Name:        FsmSw_Sha512_IncBlocks
*
* Description: Absorb blocks.
*
* Arguments:   -       sha512ctx *state:    t.b.d.
*              - const uint8     *in:       t.b.d.
*              -       uint32     inblocks: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Sha512_IncBlocks(sha512ctx *state, const uint8 *in, uint32 inblocks)
{
  uint64 bytes = fsmsw_sha2_LoadBigendian64(&state->ctx[64]);

  (void)fsmsw_sha2_crypto_HashBlocksSha512(state->ctx, in, (uint32)(128u * inblocks));
  bytes = bytes + (uint64)((uint32)(128u * inblocks));

  fsmsw_sha2_StoreBigendian64(&state->ctx[64], bytes);
}

/***********************************************************************************************************************
* Name:        FsmSw_Sha384_IncBlocks
*
* Description: Absorb blocks.
*
* Arguments:   -       sha384ctx *state:    t.b.d.
*              - const uint8     *in:       t.b.d.
*              -       uint32     inblocks: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Sha384_IncBlocks(sha384ctx *state, const uint8 *in, uint32 inblocks)
{
  /* polyspace +2 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  FsmSw_Sha512_IncBlocks((sha512ctx *)((void *)state), in, inblocks);
}

/***********************************************************************************************************************
* Name:        FsmSw_Sha256_IncFinalize
*
* Description: Finalize and obtain the digest. If applicable, this function will free the memory associated with the
*              sha256ctx.
*
* Arguments:   -       uint8     *out:   t.b.d.
*              -       sha256ctx *state: t.b.d.
*              - const uint8     *in:    t.b.d.
*              -       uint32     inlen: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Sha256_IncFinalize(uint8 *out, sha256ctx *state, const uint8 *in, uint32 inlen)
{
  uint8 padded[128] = {0};
  uint32 inlen_temp = 0;
  uint64 bytes      = fsmsw_sha2_LoadBigendian64(&state->ctx[32]) + inlen;

  /* in_temp and inlen_temp_input are used to avoid modifying the input. */
  const uint8 *in_temp    = in;
  uint32 inlen_temp_input = inlen;

  (void)fsmsw_sha2_crypto_HashBlocksSha256(state->ctx, in_temp, inlen_temp_input);
  inlen_temp = inlen_temp_input & 63u;
  in_temp    = &in_temp[(inlen_temp_input - inlen_temp)];

  inlen_temp_input = inlen_temp;

  for (uint32 i = 0; i < inlen_temp_input; ++i)
  {
    padded[i] = in_temp[i];
  }
  padded[inlen_temp_input] = 0x80;

  if (inlen_temp_input < 56u)
  {
    for (uint32 i = inlen_temp_input + 1u; i < 56u; ++i)
    {
      padded[i] = 0;
    }
    padded[56] = (uint8)(bytes >> 53);
    padded[57] = (uint8)(bytes >> 45);
    padded[58] = (uint8)(bytes >> 37);
    padded[59] = (uint8)(bytes >> 29);
    padded[60] = (uint8)(bytes >> 21);
    padded[61] = (uint8)(bytes >> 13);
    padded[62] = (uint8)(bytes >> 5);
    padded[63] = (uint8)(bytes << 3);
    (void)fsmsw_sha2_crypto_HashBlocksSha256(state->ctx, padded, 64);
  }
  else
  {
    for (uint32 i = inlen_temp_input + 1u; i < 120u; ++i)
    {
      padded[i] = 0;
    }
    padded[120] = (uint8)(bytes >> 53);
    padded[121] = (uint8)(bytes >> 45);
    padded[122] = (uint8)(bytes >> 37);
    padded[123] = (uint8)(bytes >> 29);
    padded[124] = (uint8)(bytes >> 21);
    padded[125] = (uint8)(bytes >> 13);
    padded[126] = (uint8)(bytes >> 5);
    padded[127] = (uint8)(bytes << 3);
    (void)fsmsw_sha2_crypto_HashBlocksSha256(state->ctx, padded, 128);
  }

  for (uint32 i = 0; i < 32u; ++i)
  {
    out[i] = state->ctx[i];
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Sha512_IncFinalize
*
* Description: Finalize and obtain the digest. If applicable, this function will free the memory associated with the
*              sha512ctx.
*
* Arguments:   -       uint8     *out:   t.b.d.
*              -       sha512ctx *state: t.b.d.
*              - const uint8     *in:    t.b.d.
*              -       uint32     inlen: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Sha512_IncFinalize(uint8 *out, sha512ctx *state, const uint8 *in, uint32 inlen)
{
  uint8 padded[256] = {0};
  uint32 inlen_temp = 0;
  uint64 bytes      = fsmsw_sha2_LoadBigendian64(&state->ctx[64]) + inlen;

  /* in_temp and inlen_temp_input are used to avoid modifying the input. */
  const uint8 *in_temp    = in;
  uint32 inlen_temp_input = inlen;

  (void)fsmsw_sha2_crypto_HashBlocksSha512(state->ctx, in_temp, inlen_temp_input);
  inlen_temp = inlen_temp_input & 127u;
  in_temp    = &in_temp[(inlen_temp_input - inlen_temp)];

  inlen_temp_input = inlen_temp;

  for (uint32 i = 0; i < inlen_temp_input; ++i)
  {
    padded[i] = in_temp[i];
  }
  padded[inlen_temp_input] = 0x80;

  if (inlen_temp_input < 112u)
  {
    for (uint32 i = inlen_temp_input + 1u; i < 119u; ++i)
    {
      padded[i] = 0;
    }
    padded[119] = (uint8)(bytes >> 61);
    padded[120] = (uint8)(bytes >> 53);
    padded[121] = (uint8)(bytes >> 45);
    padded[122] = (uint8)(bytes >> 37);
    padded[123] = (uint8)(bytes >> 29);
    padded[124] = (uint8)(bytes >> 21);
    padded[125] = (uint8)(bytes >> 13);
    padded[126] = (uint8)(bytes >> 5);
    padded[127] = (uint8)(bytes << 3);
    (void)fsmsw_sha2_crypto_HashBlocksSha512(state->ctx, padded, 128);
  }
  else
  {
    for (uint32 i = inlen_temp_input + 1u; i < 247u; ++i)
    {
      padded[i] = 0;
    }
    padded[247] = (uint8)(bytes >> 61);
    padded[248] = (uint8)(bytes >> 53);
    padded[249] = (uint8)(bytes >> 45);
    padded[250] = (uint8)(bytes >> 37);
    padded[251] = (uint8)(bytes >> 29);
    padded[252] = (uint8)(bytes >> 21);
    padded[253] = (uint8)(bytes >> 13);
    padded[254] = (uint8)(bytes >> 5);
    padded[255] = (uint8)(bytes << 3);
    (void)fsmsw_sha2_crypto_HashBlocksSha512(state->ctx, padded, 256);
  }

  for (uint32 i = 0; i < 64u; ++i)
  {
    out[i] = state->ctx[i];
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Sha224
*
* Description: All-in-one FsmSw_Sha224 function.
*
* Arguments:   -       uint8  *out:   t.b.d.
*              - const uint8  *in:    t.b.d.
*              -       uint32  inlen: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Sha224(uint8 *out, const uint8 *in, uint32 inlen)
{
  sha224ctx state = {{0}};

  fsmsw_sha224_IncInit(&state);
  fsmsw_sha224_IncFinalize(out, &state, in, inlen);
}

/***********************************************************************************************************************
* Name:        FsmSw_Sha256
*
* Description: All-in-one FsmSw_Sha256 function.
*
* Arguments:   -       uint8  *out:   t.b.d.
*              - const uint8  *in:    t.b.d.
*              -       uint32  inlen: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Sha256(uint8 *out, const uint8 *in, uint32 inlen)
{
  sha256ctx state = {{0}};

  FsmSw_Sha256_IncInit(&state);
  FsmSw_Sha256_IncFinalize(out, &state, in, inlen);
}

/***********************************************************************************************************************
* Name:        FsmSw_Sha384
*
* Description: All-in-one FsmSw_Sha384 function.
*
* Arguments:   -       uint8  *out:   t.b.d.
*              - const uint8  *in:    t.b.d.
*              -       uint32  inlen: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Sha384(uint8 *out, const uint8 *in, uint32 inlen)
{
  sha384ctx state = {{0}};

  fsmsw_sha384_IncInit(&state);
  fsmsw_sha384_IncFinalize(out, &state, in, inlen);
}

/***********************************************************************************************************************
* Name:        FsmSw_Sha512
*
* Description: All-in-one FsmSw_Sha512 function.
*
* Arguments:   -       uint8  *out:   t.b.d.
*              - const uint8  *in:    t.b.d.
*              -       uint32  inlen: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Sha512(uint8 *out, const uint8 *in, uint32 inlen)
{
  sha512ctx state = {{0}};

  FsmSw_Sha512_IncInit(&state);
  FsmSw_Sha512_IncFinalize(out, &state, in, inlen);
}
