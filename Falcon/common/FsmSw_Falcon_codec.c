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
/* Encoding/decoding of keys and signatures. */

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_CommonLib.h"

#include "FsmSw_Falcon_codec.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/
/* Key elements and signatures are polynomials with small integer coefficients. Here are some statistics gathered over
 * many generated key pairs (10000 or more for each degree):
 *
 *   log(n)     n   max(f,g)   std(f,g)   max(F,G)   std(F,G)
 *      1       2     129       56.31       143       60.02
 *      2       4     123       40.93       160       46.52
 *      3       8      97       28.97       159       38.01
 *      4      16     100       21.48       154       32.50
 *      5      32      71       15.41       151       29.36
 *      6      64      59       11.07       138       27.77
 *      7     128      39        7.91       144       27.00
 *      8     256      32        5.63       148       26.61
 *      9     512      22        4.00       137       26.46
 *     10    1024      15        2.84       146       26.41
 *
 * We want a compact storage format for private key, and, as part of key generation, we are allowed to reject some keys
 * which would otherwise be fine (this does not induce any noticeable vulnerability as long as we reject only a small
 * proportion of possible keys). Hence, we enforce at key generation time maximum values for the elements of f, g, F
 * and G, so that their encoding can be expressed in fixed-width values. Limits have been chosen so that generated keys
 * are almost always within bounds, thus not impacting neither security or performance.
 *
 * IMPORTANT: the code assumes that all coefficients of f, g, F and G ultimately fit in the -127..+127 range. Thus,
 * none of the elements of max_fg_bits[] and max_FG_bits[] shall be greater than 8. */

const uint8 FsmSw_Falcon_max_small_fg_bits[11] = {0, /* unused */ 8, 8, 8, 8, 8, 7, 7, 6, 6, 5};
const uint8 FsmSw_Falcon_max_big_FG_bits[11]   = {0, /* unused */ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8};

/* When generating a new key pair, we can always reject keys which feature an abnormally large coefficient. This can
 * also be done for signatures, albeit with some care: in case the signature process is used in a derandomized setup
 * (explicitly seeded with the message and private key), we have to follow the specification faithfully, and the
 * specification only enforces a limit on the L2 norm of the signature vector. The limit on the L2 norm implies that
 * the absolute value of a coefficient of the signature cannot be more than the following:
 *
 *   log(n)     n   max sig coeff (theoretical)
 *      1       2       412
 *      2       4       583
 *      3       8       824
 *      4      16      1166
 *      5      32      1649
 *      6      64      2332
 *      7     128      3299
 *      8     256      4665
 *      9     512      6598
 *     10    1024      9331
 *
 * However, the largest observed signature coefficients during our experiments was 1077 (in absolute value), hence we
 * can assume that, with overwhelming probability, signature coefficients will fit in -2047..2047, i.e. 12 bits. */

const uint8 FsmSw_Falcon_max_sig_bits[11] = {0, /* unused */ 10, 11, 11, 12, 12, 12, 12, 12, 12, 12};

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
* Name:        FsmSw_Falcon_ModqEncode
*
* Description: Encoding functions take as parameters an output buffer (out) with a given maximum length (max_out_len);
*              returned value is the actual number of bytes which have been written. If the output buffer is not large
*              enough, then 0 is returned (some bytes may have been written to the buffer). If 'out' is NULL, then
*              'max_out_len' is ignored; instead, the function computes and returns the actual required output length
*              (in bytes).
*
* Arguments:   -       void   *out:         t.b.d.
*              -       uint32  max_out_len: t.b.d.
*              - const uint16 *x:           t.b.d.
*              -       uint32  logn:        t.b.d.
*
* Returns out_len.
*
***********************************************************************************************************************/
uint32 FsmSw_Falcon_ModqEncode(void *out, uint32 max_out_len, const uint16 *x, uint32 logn)
{
  uint32 n          = 0;
  uint32 u          = 0;
  uint32 out_len    = 0;
  uint8 *buf        = (uint8 *)NULL_PTR;
  uint32 acc        = 0;
  sint32 acc_len    = 0;
  boolean bStopFunc = FALSE;

  n = (uint32)1 << logn;

  for (u = 0; u < n; u++)
  {
    if (x[u] >= 12289u)
    {
      out_len   = 0;
      bStopFunc = TRUE;
    }
  }

  if (bStopFunc == FALSE)
  {
    out_len = ((n * 14u) + 7u) >> 3;

    if (out == ((void *)0))
    {
      bStopFunc = TRUE;
    }
  }

  if (bStopFunc == FALSE)
  {
    if (out_len > max_out_len)
    {
      out_len = 0;
    }

    /* polyspace +2 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
    buf     = out;
    acc     = 0;
    acc_len = 0;

    for (u = 0; u < n; u++)
    {
      acc = (acc << 14) | x[u];
      acc_len += 14;

      while (acc_len >= 8)
      {
        acc_len -= 8;
        *buf = (uint8)(acc >> (uint32)acc_len);
        buf++;
      }
    }

    if (acc_len > 0)
    {
      *buf = (uint8)(acc << (8u - (uint32)acc_len));
    }
  }
  return out_len;
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_ModqDecode
*
* Description: Decoding functions take as parameters an input buffer (in) with its maximum length (max_in_len);
*              returned value is the actual number of bytes that have been read from the buffer. If the provided length
*              is too short, then 0 is returned.
*
* Arguments:   -       sint16 *x:          t.b.d.
*              -       uint32  logn:       t.b.d.
*              - const void   *in:         t.b.d.
*              -       uint32  max_in_len: t.b.d.
*
* Returns out_len.
*
***********************************************************************************************************************/
uint32 FsmSw_Falcon_ModqDecode(uint16 *x, uint32 logn, const void *in, uint32 max_in_len)
{
  uint32 n          = 0;
  uint32 in_len     = 0;
  uint32 u          = 0;
  const uint8 *buf  = (uint8 *)NULL_PTR;
  uint32 acc        = 0;
  sint32 acc_len    = 0;
  uint32 w          = 0;
  boolean bStopFunc = FALSE;

  n      = (uint32)1 << logn;
  in_len = ((n * 14u) + 7u) >> 3;

  if (in_len > max_in_len)
  {
    in_len    = 0;
    bStopFunc = TRUE;
  }
  if (bStopFunc == FALSE)
  {
    buf     = in;
    acc     = 0;
    acc_len = 0;
    u       = 0;

    while (u < n)
    {
      acc = (acc << 8) | (*buf);
      buf++;
      acc_len += 8;

      if (acc_len >= 14)
      {
        acc_len -= 14;
        w = (acc >> (uint32)acc_len) & 0x3FFFu;

        if (w >= 12289u)
        {
          in_len = 0;
          break;
        }

        x[u] = (uint16)w;
        u++;
        ;
      }
    }

    if ((acc & (((uint32)1u << (uint32)acc_len) - 1u)) != 0u)
    {
      in_len = 0;
    }
  }
  return in_len;
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_TrimI8Encode
*
* Description: Encoding functions take as parameters an output buffer (out) with a given maximum length (max_out_len);
*              returned value is the actual number of bytes which have been written. If the output buffer is not large
*              enough, then 0 is returned (some bytes may have been written to the buffer). If 'out' is NULL, then
*              'max_out_len' is ignored; instead, the function computes and returns the actual required output length
*              (in bytes).
*
* Arguments:   -       void   *out:         t.b.d.
*              -       uint32  max_out_len: t.b.d.
*              - const sint8  *x:           t.b.d.
*              -       uint32  logn:        t.b.d.
*              -       uint32  bits:        t.b.d.
*
* Returns out_len.
*
***********************************************************************************************************************/
/* polyspace +2 CODE-METRICS:LEVEL [Justified:]"[Value: 5]Additional call levels are required 
to resolve a MISRA 15 warning." */
uint32 FsmSw_Falcon_TrimI8Encode(void *out, uint32 max_out_len, const sint8 *x, uint32 logn, uint32 bits)
{
  uint32 n          = 0;
  uint32 u          = 0;
  uint32 out_len    = 0;
  sint32 minv       = 0;
  sint32 maxv       = 0;
  uint8 *buf        = (uint8 *)NULL_PTR;
  uint32 acc        = 0;
  uint32 mask       = 0;
  uint32 acc_len    = 0;
  uint32 temp1      = 0;
  boolean bStopFunc = FALSE;

  n     = (uint32)1 << logn;
  temp1 = ((uint32)1u << (bits - 1u)) - 1u;
  maxv  = (sint32)temp1;
  minv  = -maxv;

  for (u = 0; u < n; u++)
  {
    if ((x[u] < minv) || (x[u] > maxv))
    {
      out_len   = 0;
      bStopFunc = TRUE;
    }
  }

  if (bStopFunc == FALSE)
  {
    out_len = ((n * bits) + 7u) >> 3;

    if (out == ((void *)0))
    {
      /* end of the funtion, return out_len */
    }

    else if (out_len > max_out_len)
    {
      out_len = 0;
    }
    else
    {
      /* polyspace +2 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality. 
    		Ensured proper alignment and validity." */
      buf     = out;
      acc     = 0;
      acc_len = 0;
      mask    = ((uint32)1u << bits) - 1u;

      for (u = 0; u < n; u++)
      {
        acc = (acc << bits) | ((uint8)x[u] & mask);
        acc_len += bits;

        while (acc_len >= 8u)
        {
          acc_len -= 8u;
          *buf = (uint8)(acc >> acc_len);
          buf++;
        }
      }

      if (acc_len > 0u)
      {
        *buf = (uint8)(acc << (8u - acc_len));
      }
    }
  }
  return out_len;
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_TrimI8Decode
*
* Description: Decoding functions take as parameters an input buffer (in) with its maximum length (max_in_len);
*              returned value is the actual number of bytes that have been read from the buffer. If the provided length
*              is too short, then 0 is returned.
*
* Arguments:   -       sint8  *x:          t.b.d.
*              -       uint32  logn:       t.b.d.
*              -       uint32  bits:       t.b.d.
*              - const void   *in:         t.b.d.
*              -       uint32  max_in_len: t.b.d.
*
* Returns out_len.
*
***********************************************************************************************************************/
uint32 FsmSw_Falcon_TrimI8Decode(sint8 *x, uint32 logn, uint32 bits, const void *in, uint32 max_in_len)
{
  uint32 n          = 0;
  uint32 in_len     = 0;
  const uint8 *buf  = (uint8 *)NULL_PTR;
  uint32 u          = 0;
  uint32 acc        = 0;
  uint32 mask1      = 0;
  uint32 mask2      = 0;
  uint32 acc_len    = 0;
  uint32 w          = 0;
  boolean bStopFunc = FALSE;

  n      = (uint32)1 << logn;
  in_len = ((n * bits) + 7u) >> 3;

  if (in_len > max_in_len)
  {
    in_len    = 0;
    bStopFunc = TRUE;
  }

  if (bStopFunc == FALSE)
  {
    buf     = in;
    u       = 0;
    acc     = 0;
    acc_len = 0;
    mask1   = ((uint32)1u << bits) - 1u;
    mask2   = (uint32)1u << (bits - 1u);

    while (u < n)
    {
      acc = (acc << 8) | *buf;
      buf++;
      acc_len += 8u;

      while ((acc_len >= bits) && (u < n))
      {
        acc_len -= bits;
        w = (acc >> acc_len) & mask1;
        w |= (uint32)((sint32)((-1) * (sint32)((uint32)(w & mask2))));

        if (w == (uint32)((sint32)((-1) * (sint32)mask2)))
        {
          /* The -2^(bits-1) value is forbidden. */
          in_len    = 0;
          bStopFunc = TRUE;
          break;
        }

        x[u] = (sint8)((sint32)w);
        u++;
      }
    }

    if (((acc & (((uint32)1 << acc_len) - 1u)) != 0u) && (bStopFunc == FALSE))
    {
      /* Extra bits in the last byte must be zero. */
      in_len = 0;
    }
  }
  return in_len;
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_CompEncode
*
* Description: Encoding functions take as parameters an output buffer (out) with a given maximum length (max_out_len);
*              returned value is the actual number of bytes which have been written. If the output buffer is not large
*              enough, then 0 is returned (some bytes may have been written to the buffer). If 'out' is NULL, then
*              'max_out_len' is ignored; instead, the function computes and returns the actual required output length
*              (in bytes).
*
* Arguments:   -       void   *out:         t.b.d.
*              -       uint32  max_out_len: t.b.d.
*              - const sint16  *x:          t.b.d.
*              -       uint32  logn:        t.b.d.
*
* Returns out_len.
*
***********************************************************************************************************************/
/* polyspace +2 CODE-METRICS:LEVEL [Justified:]"[Value: 5]Additional call levels are required 
to resolve a MISRA 15 warning." */
uint32 FsmSw_Falcon_CompEncode(void *out, uint32 max_out_len, const sint16 *x, uint32 logn)
{
  uint8 *buf        = (uint8 *)NULL_PTR;
  uint32 n          = 0;
  uint32 u          = 0;
  uint32 v          = 0;
  uint32 acc        = 0;
  uint32 acc_len    = 0;
  boolean bStopFunc = FALSE;

  n = (uint32)1 << logn;
  /* polyspace +2 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  buf = out;

  /* Make sure that all values are within the -2047..+2047 range. */
  for (u = 0; u < n; u++)
  {
    if ((x[u] < -2047) || (x[u] > +2047))
    {
      v         = 0;
      bStopFunc = TRUE;
    }
  }
  if (bStopFunc == FALSE)
  {
    acc     = 0;
    acc_len = 0;
    v       = 0;

    for (u = 0; u < n; u++)
    {
      sint32 t;
      uint32 w;

      /* Get sign and absolute value of next integer; push the sign bit. */
      acc <<= 1;
      t = x[u];

      if (t < 0)
      {
        t = -t;
        acc |= 1u;
      }

      w = (uint32)t;

      /* Push the low 7 bits of the absolute value. */
      acc <<= 7;
      acc |= w & 127u;
      w >>= 7;

      /* We pushed exactly 8 bits. */
      acc_len += 8u;

      /* Push as many zeros as necessary, then a one. Since the absolute value is at most 2047, w can only range up
        * to 15 at this point, thus we will add at most 16 bits here. With the 8 bits above and possibly up to 7 bits
        * from previous iterations, we may go up to 31 bits, which will fit in the accumulator, which is an uint32. */
      acc <<= (w + 1u);
      acc |= 1u;
      acc_len += w + 1u;

      /* Produce all full bytes. */
      while (acc_len >= 8u)
      {
        acc_len -= 8u;
        if (buf != ((void *)0))
        {
          if (v >= max_out_len)
          {
            v         = 0;
            bStopFunc = TRUE;
            break;
          }
          buf[v] = (uint8)(acc >> acc_len);
        }
        v++;
      }
    }

    /* Flush remaining bits (if any). */
    if ((acc_len > 0u) && (bStopFunc == FALSE))
    {
      if (buf != ((void *)0))
      {
        if (v >= max_out_len)
        {
          v         = 0;
          bStopFunc = TRUE;
        }
        buf[v] = (uint8)(acc << (8u - acc_len));
      }
      if (bStopFunc == FALSE)
      {
        v++;
      }
    }
  }
  return v;
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_CompDecode
*
* Description: Decoding functions take as parameters an input buffer (in) with its maximum length (max_in_len);
*              returned value is the actual number of bytes that have been read from the buffer. If the provided length
*              is too short, then 0 is returned.
*
* Arguments:   -       sint16 *x:           t.b.d.
*              -       uint32  logn:        t.b.d.
*              - const void   *in:           t.b.d.
*              -       uint32  max_in_len: t.b.d.
*
* Returns out_len.
*
***********************************************************************************************************************/
/* polyspace +3 MISRA2012:15.5 [Justified:]"Multiple return points enhance readability and efficiency 
by allowing early exits on error conditions. Using a single return variable (retVal) 
was evaluated but didn't work in this context." */
uint32 FsmSw_Falcon_CompDecode(sint16 *x, uint32 logn, const void *in, uint32 max_in_len)
{
  const uint8 *buf = (uint8 *)NULL_PTR;
  uint32 n         = 0;
  uint32 u         = 0;
  uint32 v         = 0;
  uint32 acc       = 0;
  uint32 acc_len   = 0;
  uint32 b         = 0;
  uint32 s         = 0;
  uint32 m         = 0;

  n       = (uint32)1 << logn;
  buf     = in;
  acc     = 0;
  acc_len = 0;
  v       = 0;
  for (u = 0; u < n; u++)
  {
    /* Get next eight bits: sign and low seven bits of the absolute value. */
    if (v >= max_in_len)
    {
      return 0;
    }
    acc = (acc << 8) | (uint32)buf[v];
    v++;
    b = acc >> acc_len;
    s = b & 128u;
    m = b & 127u;

    /* Get next bits until a 1 is reached. */
    for (;;)
    {
      if (acc_len == 0u)
      {
        if (v >= max_in_len)
        {
          return 0;
        }
        acc = (acc << 8) | (uint32)buf[v];
        v++;
        acc_len = 8;
      }
      acc_len--;

      if (((acc >> acc_len) & 1u) != 0u)
      {
        break;
      }

      m += 128u;

      if (m > 2047u)
      {
        return 0;
      }
    }

    /* "-0" is forbidden. */
    if ((0u != s) && (m == 0u))
    {
      return 0;
    }

    if (s > 0u)
    {
      x[u] = (sint16)((-1) * (sint32)m);
    }
    else
    {
      x[u] = (sint16)m;
    }
  }

  /* Unused bits in the last byte must be zero. */
  if ((acc & (((uint32)1u << acc_len) - 1u)) != 0u)
  {
    return 0;
  }

  return v;
}
