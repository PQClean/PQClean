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
/* Support functions for signatures (hash-to-point, norm). */
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_CommonLib.h"

#include "FsmSw_Falcon_common.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/
static const uint16 overtab[] = {0, /* unused */
                                 65, 67, 71, 77, 86, 100, 122, 154, 205, 287};

/* Acceptance bound for the (squared) l2-norm of the signature depends on the degree. This array is indexed by logn
 * (1 to 10). These bounds are _inclusive_ (they are equal to floor(beta^2)). */
static const uint32 l2bound[] = {0, /* unused */
                                 101498,  208714,  428865,   892039,   1852696,
                                 3842630, 7959734, 16468416, 34034726, 70265242};

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
* Name:        FsmSw_Falcon_HashToPointVartime
*
* Description: From a SHAKE256 context (must be already flipped), produce a new point. This is the non-constant-time
*              version, which may leak enough information to serve as a stop condition on a brute force attack on the
*              hashed message (provided that the nonce value is known).
*
* Arguments:   - inner_shake256_context *sc:   t.b.d.
*              - uint16                 *x:    t.b.d.
*              - uint32                  logn: t.b.d.
*
* Note:        This function is currently not used.
*
***********************************************************************************************************************/
void FsmSw_Falcon_HashToPointVartime(inner_shake256_context *sc, uint16 *x, uint32 logn)
{
  /* This is the straightforward per-the-spec implementation. It is not constant-time, thus it might reveal
     * information on the plaintext (at least, enough to check the plaintext against a list of potential plaintexts)
     * in a scenario where the attacker does not have access to the signature value or to the public key, but knows
     * the nonce (without knowledge of the nonce, the hashed output cannot be matched against potential plaintexts). */
  uint32 n = 0;
  /* x_temp is used to avoid modifying the input. */
  uint16 *x_temp = x;

  n = (uint32)1 << logn;

  while (n > 0u)
  {
    uint8 buf[2];
    uint32 w;

    /* polyspace +2 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality. 
        Ensured proper alignment and validity." */
    FsmSw_Fips202_Shake256_IncSqueeze((void *)buf, sizeof(buf), sc);
    w = ((uint32)buf[0] << 8) | (uint32)buf[1];

    if (w < 61445u)
    {
      while (w >= 12289u)
      {
        w -= 12289u;
      }
      *x_temp = (uint16)w;
      x_temp++;
      n--;
    }
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_HashToPointCt
*
* Description: From a SHAKE256 context (must be already flipped), produce a new point. The temporary buffer (tmp) must #
*              have room for 2*2^logn bytes. This function is constant-time but is typically more expensive than
*              FsmSw_Falcon_HashToPointVartime(). tmp[] must have 16-bit alignment.
*
* Arguments:   - inner_shake256_context *sc:   t.b.d.
*              - uint16                 *x:    t.b.d.
*              - uint32                  logn: t.b.d.
*              - uint8                  *tmp:  t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Falcon_HashToPointCt(inner_shake256_context *sc, uint16 *x, uint32 logn, uint8 *tmp)
{
  /* Each 16-bit sample is a value in 0..65535. The value is kept if it falls in 0..61444 (because 61445 = 5*12289)
     * and rejected otherwise; thus, each sample has probability about 0.93758 of being selected. We want to oversample
     * enough to be sure that we will have enough values with probability at least 1 - 2^(-256). Depending on degree N,
     * this leads to the following required oversampling:
     *
     *   logn     n  oversampling
     *     1      2     65
     *     2      4     67
     *     3      8     71
     *     4     16     77
     *     5     32     86
     *     6     64    100
     *     7    128    122
     *     8    256    154
     *     9    512    205
     *    10   1024    287
     *
     * If logn >= 7, then the provided temporary buffer is large enough. Otherwise, we use a stack buffer of 63 entries
     * (i.e. 126 bytes) for the values that do not fit in tmp[]. */

  uint32 n       = 0;
  uint32 n2      = 0;
  uint32 u       = 0;
  uint32 m       = 0;
  uint32 p       = 0;
  uint32 over    = 0;
  uint16 *tt1    = (uint16 *)NULL_PTR;
  uint16 tt2[63] = {0};
  uint32 temp    = 0;

  /* We first generate m 16-bit value. Values 0..n-1 go to x[]. Values n..2*n-1 go to tt1[]. Values 2*n and later
     * go to tt2[]. We also reduce modulo q the values; rejected values are set to 0xFFFF. */
  n    = (uint32)1u << logn;
  n2   = n << 1;
  over = overtab[logn];
  m    = n + over;
  /* polyspace +2 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
  tt1 = (uint16 *)((void *)tmp);

  for (u = 0; u < m; u++)
  {
    uint8 buf[2];
    uint32 w, wr;

    FsmSw_Fips202_Shake256_IncSqueeze(buf, sizeof(buf), sc);
    w  = ((uint32)buf[0] << 8) | (uint32)buf[1];
    wr = w - ((uint32)24578 & (((w - 24578u) >> 31) - 1u));
    wr = wr - ((uint32)24578 & (((wr - 24578u) >> 31) - 1u));
    wr = wr - ((uint32)12289 & (((wr - 12289u) >> 31) - 1u));
    wr |= ((w - 61445u) >> 31) - 1u;

    if (u < n)
    {
      x[u] = (uint16)wr;
    }
    else if (u < n2)
    {
      tt1[u - n] = (uint16)wr;
    }
    else
    {
      tt2[u - n2] = (uint16)wr;
    }
  }

  /* Now we must "squeeze out" the invalid values. We do this in a logarithmic sequence of passes; each pass computes
     * where a value should go, and moves it down by 'p' slots if necessary, where 'p' uses an increasing powers-of-two
     * scale. It can be shown that in all cases where the loop decides that a value has to be moved down by p slots,
     * the destination slot is "free" (i.e. contains an invalid value). */
  for (p = 1; p <= over; p <<= 1)
  {
    uint32 v;
    /* In the loop below:
         *   - v contains the index of the final destination of the value; it is recomputed dynamically based on
         *     whether values are valid or not.
         *   - u is the index of the value we consider ("source"); its address is s.
         *   - The loop may swap the value with the one at index u-p. The address of the swap destination is d. */
    v = 0;

    for (u = 0; u < m; u++)
    {
      uint16 *s, *d;
      uint32 j, sv, dv, mk;

      if (u < n)
      {
        s = &x[u];
      }
      else if (u < n2)
      {
        s = &tt1[u - n];
      }
      else
      {
        s = &tt2[u - n2];
      }
      sv = *s;

      /* The value in sv should ultimately go to address v, i.e. jump back by u-v slots. */
      j = u - v;

      /* We increment v for the next iteration, but only if the source value is valid. The mask 'mk' is -1 if
             * the value is valid, 0 otherwise, so we _subtract_ mk. */
      mk = (sv >> 15) - 1U;
      v -= mk;

      /* In this loop we consider jumps by p slots; if u < p then there is nothing more to do. */
      if (u < p)
      {
        continue;
      }

      /* Destination for the swap: value at address u-p. */
      if ((u - p) < n)
      {
        d = &x[u - p];
      }
      else if ((u - p) < n2)
      {
        d = &tt1[(u - p) - n];
      }
      else
      {
        d = &tt2[(u - p) - n2];
      }
      dv = *d;

      /* The swap should be performed only if the source is valid AND the jump j has its 'p' bit set. */
      temp = ((j & p) + 0x1FFu) >> 9;
      mk &= ((uint32)((sint32)((-1) * (sint32)(temp))));

      *s = (uint16)(sv ^ (mk & (sv ^ dv)));
      *d = (uint16)(dv ^ (mk & (sv ^ dv)));
    }
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_IsShort
*
* Description: Tell whether a given vector (2N coordinates, in two halves) is acceptable as a signature. This compares
*              the appropriate norm of the vector with the acceptance bound.
*
* Arguments:   - const sint16 *s1:   t.b.d.
*              - const sint16 *s2:   t.b.d.
*              -       uint32  logn: t.b.d.
*
* Returns 1 on success (vector is short enough to be acceptable), 0 otherwise.
*
***********************************************************************************************************************/
sint32 FsmSw_Falcon_IsShort(const sint16 *s1, const sint16 *s2, uint32 logn)
{
  /* We use the l2-norm. Code below uses only 32-bit operations to compute the square of the norm with saturation to
     * 2^32-1 if the value exceeds 2^31-1. */
  uint32 n      = 0;
  uint32 u      = 0;
  uint32 s      = 0;
  uint32 ng     = 0;
  sint32 z      = 0;
  sint32 retVal = 0;

  n  = (uint32)1 << logn;
  s  = 0;
  ng = 0;

  for (u = 0; u < n; u++)
  {
    z = s1[u];
    s += (uint32)((sint32)(z * z));
    ng |= s;
    z = s2[u];
    s += (uint32)((sint32)(z * z));
    ng |= s;
  }

  s |= (uint32)((sint32)((-1) * (sint32)((uint32)(ng >> 31))));

  if (s <= l2bound[logn])
  {
    retVal = 1;
  }

  return retVal;
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_IsShortHalf
*
* Description: Tell whether a given vector (2N coordinates, in two halves) is acceptable as a signature. Instead of the
*              first half s1, this function receives the "saturated squared norm" of s1, i.e. the sum of the squares of
*              the coordinates of s1 (saturated at 2^32-1 if the sum exceeds 2^31-1).
*
* Arguments:   -       uint32  sqn:  t.b.d.
*              - const sint16 *s2:   t.b.d.
*              -       uint32  logn: t.b.d.
*
* Returns 1 on success (vector is short enough to be acceptable), 0 otherwise.
*
***********************************************************************************************************************/
sint32 FsmSw_Falcon_IsShortHalf(uint32 sqn, const sint16 *s2, uint32 logn)
{
  uint32 n  = 0;
  uint32 u  = 0;
  uint32 ng = 0;
  sint32 z  = 0;
  /* sqn_temp is used to avoid modifying the input. */
  uint32 sqn_temp = sqn;
  sint32 retVal   = 0;

  n  = (uint32)1 << logn;
  ng = (uint32)((sint32)((-1) * (sint32)((uint32)(sqn_temp >> 31))));

  for (u = 0; u < n; u++)
  {
    z = s2[u];
    sqn_temp += (uint32)((sint32)(z * z));
    ng |= sqn_temp;
  }

  sqn_temp |= (uint32)((sint32)((-1) * (sint32)((uint32)(ng >> 31))));
  ;

  if (sqn_temp <= l2bound[logn])
  {
    retVal = 1;
  }

  return retVal;
}
