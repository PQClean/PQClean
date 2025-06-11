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
/* Let w = exp(i*pi/N); w is a primitive 2N-th root of 1. We define the values w_j = w^(2j+1) for all j from 0 to N-1:
*  these are the roots of X^N+1 in the field of complex numbers. A crucial property is that w_{N-1-j} = conj(w_j) =
*  1/w_j for all j. *
*  FFT representation of a polynomial f (taken modulo X^N+1) is the set of values f(w_j). Since f is real,
*  conj(f(w_j)) = f(conj(w_j)), thus f(w_{N-1-j}) = conj(f(w_j)). We thus store only half the values, for j = 0 to
*  N/2-1; the other half can be recomputed easily when (if) needed. A consequence is that FFT representation has the
*  same size as normal representation: N/2 complex numbers use N real numbers (each complex number is the combination
*  of a real and an imaginary part).
*  We use a specific ordering which makes computations easier. Let rev() be the bit-reversal function over log(N) bits.
*  For j in 0..N/2-1, we store the real and imaginary parts of f(w_j) in slots:
*     Re(f(w_j)) -> slot rev(j)/2
*     Im(f(w_j)) -> slot rev(j)/2+N/2
*  (Note that rev(j) is even for j < N/2.) */
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_sha2.h"

#include "FsmSw_Falcon_fft.h"
/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
/*  Rules for complex number macros:
 * --------------------------------
 * - Operand order is: destination, source1, source2...
 * - Each operand is a real and an imaginary part.
 * - All overlaps are allowed. */

/* polyspace +68 MISRA2012:D4.9 [Justified:]"No refactoring of macros, as converting to, for example, 
inline functions would not provide significant benefits." */

/* Addition of two complex numbers (d = a + b). */
#define FPC_ADD(d_re, d_im, a_re, a_im, b_re, b_im)                                                                    \
  {                                                                                                                    \
    fpr fpct_re, fpct_im;                                                                                              \
    fpct_re = FsmSw_Falcon_Fpr_Add(a_re, b_re);                                                                        \
    fpct_im = FsmSw_Falcon_Fpr_Add(a_im, b_im);                                                                        \
    (d_re)  = fpct_re;                                                                                                 \
    (d_im)  = fpct_im;                                                                                                 \
  }

/* Subtraction of two complex numbers (d = a - b). */
#define FPC_SUB(d_re, d_im, a_re, a_im, b_re, b_im)                                                                    \
  {                                                                                                                    \
    fpr fpct_re, fpct_im;                                                                                              \
    fpct_re = FsmSw_Falcon_Fpr_Sub(a_re, b_re);                                                                        \
    fpct_im = FsmSw_Falcon_Fpr_Sub(a_im, b_im);                                                                        \
    (d_re)  = fpct_re;                                                                                                 \
    (d_im)  = fpct_im;                                                                                                 \
  }

/* Multplication of two complex numbers (d = a * b). */
/* polyspace +2 MISRA2012:2.2 [Justified:]"Polyspace does not properly decompose the function; 
location of dead code unclear." */
#define FPC_MUL(d_re, d_im, a_re, a_im, b_re, b_im)                                                                    \
  {                                                                                                                    \
    fpr fpct_a_re, fpct_a_im;                                                                                          \
    fpr fpct_b_re, fpct_b_im;                                                                                          \
    fpr fpct_d_re, fpct_d_im;                                                                                          \
    fpct_a_re = (a_re);                                                                                                \
    fpct_a_im = (a_im);                                                                                                \
    fpct_b_re = (b_re);                                                                                                \
    fpct_b_im = (b_im);                                                                                                \
    fpct_d_re =                                                                                                        \
        FsmSw_Falcon_Fpr_Sub(FsmSw_Falcon_Fpr_Mul(fpct_a_re, fpct_b_re), FsmSw_Falcon_Fpr_Mul(fpct_a_im, fpct_b_im));  \
    fpct_d_im =                                                                                                        \
        FsmSw_Falcon_Fpr_Add(FsmSw_Falcon_Fpr_Mul(fpct_a_re, fpct_b_im), FsmSw_Falcon_Fpr_Mul(fpct_a_im, fpct_b_re));  \
    (d_re) = fpct_d_re;                                                                                                \
    (d_im) = fpct_d_im;                                                                                                \
  }

/* Division of complex numbers (d = a / b). */
#define FPC_DIV(d_re, d_im, a_re, a_im, b_re, b_im)                                                                    \
  {                                                                                                                    \
    fpr fpct_a_re, fpct_a_im;                                                                                          \
    fpr fpct_b_re, fpct_b_im;                                                                                          \
    fpr fpct_d_re, fpct_d_im;                                                                                          \
    fpr fpct_m;                                                                                                        \
    fpct_a_re = (a_re);                                                                                                \
    fpct_a_im = (a_im);                                                                                                \
    fpct_b_re = (b_re);                                                                                                \
    fpct_b_im = (b_im);                                                                                                \
    fpct_m    = FsmSw_Falcon_Fpr_Add(FsmSw_Falcon_Fpr_Sqr(fpct_b_re), FsmSw_Falcon_Fpr_Sqr(fpct_b_im));                \
    fpct_m    = FsmSw_Falcon_Fpr_Inv(fpct_m);                                                                          \
    fpct_b_re = FsmSw_Falcon_Fpr_Mul(fpct_b_re, fpct_m);                                                               \
    fpct_b_im = FsmSw_Falcon_Fpr_Mul(FsmSw_Falcon_Fpr_Neg(fpct_b_im), fpct_m);                                         \
    fpct_d_re =                                                                                                        \
        FsmSw_Falcon_Fpr_Sub(FsmSw_Falcon_Fpr_Mul(fpct_a_re, fpct_b_re), FsmSw_Falcon_Fpr_Mul(fpct_a_im, fpct_b_im));  \
    fpct_d_im =                                                                                                        \
        FsmSw_Falcon_Fpr_Add(FsmSw_Falcon_Fpr_Mul(fpct_a_re, fpct_b_im), FsmSw_Falcon_Fpr_Mul(fpct_a_im, fpct_b_re));  \
    (d_re) = fpct_d_re;                                                                                                \
    (d_im) = fpct_d_im;                                                                                                \
  }

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
* Name:        FsmSw_Falcon_FFT
*
* Description: Compute FFT in-place: the source array should contain a real polynomial (N coefficients); its storage
*              area is reused to store the FFT representation of that polynomial (N/2 complex numbers).
*              'logn' MUST lie between 1 and 10 (inclusive).
*
* Arguments:   - fpr    *f:    t.b.d.
*              - uint32  logn: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Falcon_FFT(fpr *f, uint32 logn)
{
  /* FFT algorithm in bit-reversal order uses the following iterative algorithm:
     *
     *   t = N
     *   for m = 1; m < N; m *= 2:
     *       ht = t/2
     *       for i1 = 0; i1 < m; i1 ++:
     *           j1 = i1 * t
     *           s = GM[m + i1]
     *           for j = j1; j < (j1 + ht); j ++:
     *               x = f[j]
     *               y = s * f[j + ht]
     *               f[j] = x + y
     *               f[j + ht] = x - y
     *       t = ht
     *
     * GM[k] contains w^rev(k) for primitive root w = exp(i*pi/N).
     *
     * In the description above, f[] is supposed to contain complex numbers. In our in-memory representation, the real
     * and imaginary parts of f[k] are in array slots k and k+N/2.
     * We only keep the first half of the complex numbers. We can see that after the first iteration, the first and
     * second halves of the array of complex numbers have separate lives, so we simply ignore the second part. */

  uint32 u  = 0;
  uint32 t  = 0;
  uint32 n  = 0;
  uint32 hn = 0;
  uint32 m  = 0;
  uint32 ht = 0;
  uint32 hm = 0;
  uint32 i1 = 0;
  uint32 j1 = 0;
  uint32 j  = 0;
  uint32 j2 = 0;
  fpr x_re  = 0;
  fpr x_im  = 0;
  fpr y_re  = 0;
  fpr y_im  = 0;

  /* First iteration: compute f[j] + i * f[j+N/2] for all j < N/2 (because GM[1] = w^rev(1) = w^(N/2) = i). In our
     * chosen representation, this is a no-op: everything is already where it should be.
     * Subsequent iterations are truncated to use only the first half of values. */
  n  = (uint32)1 << logn;
  hn = n >> 1;
  t  = hn;
  m  = 2;
  for (u = 1; u < logn; u++)
  {
    ht = t >> 1;
    hm = m >> 1;
    j1 = 0;

    for (i1 = 0; i1 < hm; i1++)
    {
      j2 = j1 + ht;
      fpr s_re, s_im;

      s_re = fpr_gm_tab[((m + i1) << 1)];
      s_im = fpr_gm_tab[((m + i1) << 1) + 1u];

      for (j = j1; j < j2; j++)
      {
        x_re = f[j];
        x_im = f[j + hn];
        y_re = f[j + ht];
        y_im = f[j + ht + hn];
        FPC_MUL(y_re, y_im, y_re, y_im, s_re, s_im);
        FPC_ADD(f[j], f[j + hn], x_re, x_im, y_re, y_im);
        FPC_SUB(f[j + ht], f[j + ht + hn], x_re, x_im, y_re, y_im);
      }

      j1 += t;
    }
    t = ht;

    m <<= 1;
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_IFFT
*
* Description: Compute the inverse FFT in-place: the source array should contain the FFT representation of a real
*              polynomial (N/2 elements); the resulting real polynomial (N coefficients of type 'fpr') is written over
*              the array.
*              'logn' MUST lie between 1 and 10 (inclusive).
*
* Arguments:   - fpr    *f:    t.b.d.
*              - uint32  logn: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Falcon_IFFT(fpr *f, uint32 logn)
{
  /* Inverse FFT algorithm in bit-reversal order uses the following iterative algorithm:
     *
     *   t = 1
     *   for m = N; m > 1; m /= 2:
     *       hm = m/2
     *       dt = t*2
     *       for i1 = 0; i1 < hm; i1 ++:
     *           j1 = i1 * dt
     *           s = iGM[hm + i1]
     *           for j = j1; j < (j1 + t); j ++:
     *               x = f[j]
     *               y = f[j + t]
     *               f[j] = x + y
     *               f[j + t] = s * (x - y)
     *       t = dt
     *   for i1 = 0; i1 < N; i1 ++:
     *       f[i1] = f[i1] / N
     *
     * iGM[k] contains (1/w)^rev(k) for primitive root w = exp(i*pi/N) (actually, iGM[k] = 1/GM[k] = conj(GM[k])).
     *
     * In the main loop (not counting the final division loop), in all iterations except the last, the first and second
     * half of f[] (as an array of complex numbers) are separate. In our chosen representation, we do not keep the
     * second half.
     *
     * The last iteration recombines the recomputed half with the implicit half, and should yield only real numbers
     * since the target polynomial is real; moreover, s = i at that step.
     * Thus, when considering x and y:
     *    y = conj(x) since the final f[j] must be real.
     *    Therefore, f[j] is filled with 2*Re(x), and f[j + t] is filled with 2*Im(x).
     * But we already have Re(x) and Im(x) in array slots j and j+t in our chosen representation. That last iteration
     * is thus a simple doubling of the values in all the array.
     *
     * We make the last iteration a no-op by tweaking the final division into a division by N/2, not N. */
  uint32 u  = 0;
  uint32 n  = 0;
  uint32 hn = 0;
  uint32 t  = 0;
  uint32 m  = 0;
  uint32 hm = 0;
  uint32 dt = 0;
  uint32 i1 = 0;
  uint32 j1 = 0;
  uint32 j  = 0;
  uint32 j2 = 0;
  fpr x_re  = 0;
  fpr x_im  = 0;
  fpr y_re  = 0;
  fpr y_im  = 0;
  fpr ni    = 0;

  n  = (uint32)1 << logn;
  t  = 1;
  m  = n;
  hn = n >> 1;

  for (u = logn; u > 1u; u--)
  {
    hm = m >> 1;
    dt = t << 1;

    i1 = 0;
    for (j1 = 0; j1 < hn; j1 += dt)
    {
      j2 = j1 + t;
      fpr s_re, s_im;

      s_re = fpr_gm_tab[((hm + i1) << 1)];
      s_im = FsmSw_Falcon_Fpr_Neg(fpr_gm_tab[((hm + i1) << 1) + 1u]);

      for (j = j1; j < j2; j++)
      {
        x_re = f[j];
        x_im = f[j + hn];
        y_re = f[j + t];
        y_im = f[j + t + hn];
        FPC_ADD(f[j], f[j + hn], x_re, x_im, y_re, y_im);
        FPC_SUB(x_re, x_im, x_re, x_im, y_re, y_im);
        FPC_MUL(f[j + t], f[j + t + hn], x_re, x_im, s_re, s_im);
      }

      i1++;
    }
    t = dt;
    m = hm;
  }

  /* Last iteration is a no-op, provided that we divide by N/2 instead of N. We need to make a special case for
     * logn = 0. */
  if (logn > 0u)
  {
    ni = fpr_p2_tab[logn];
    for (u = 0; u < n; u++)
    {
      f[u] = FsmSw_Falcon_Fpr_Mul(f[u], ni);
    }
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_Poly_Add
*
* Description: Add polynomial b to polynomial a. a and b MUST NOT overlap. This function works in both normal and FFT
*              representations.
*
* Arguments:   -       fpr    *a:    t.b.d.
*              - const fpr    *b:    t.b.d.
*              -       uint32  logn: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Falcon_Poly_Add(fpr *a, const fpr *b, uint32 logn)
{
  uint32 n = 0;
  uint32 u = 0;

  n = (uint32)1 << logn;
  for (u = 0; u < n; u++)
  {
    a[u] = FsmSw_Falcon_Fpr_Add(a[u], b[u]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_Poly_Sub
*
* Description: Subtract polynomial b from polynomial a. a and b MUST NOT overlap. This function works in both normal
*              and FFT representations.
*
* Arguments:   -       fpr    *a:    t.b.d.
*              - const fpr    *b:    t.b.d.
*              -       uint32  logn: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Falcon_Poly_Sub(fpr *a, const fpr *b, uint32 logn)
{
  uint32 n = 0;
  uint32 u = 0;

  n = (uint32)1 << logn;
  for (u = 0; u < n; u++)
  {
    a[u] = FsmSw_Falcon_Fpr_Sub(a[u], b[u]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_Poly_Neg
*
* Description: Negate polynomial a. This function works in both normal and FFT representations.
*
* Arguments:   - fpr    *a:    t.b.d.
*              - uint32  logn: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Falcon_Poly_Neg(fpr *a, uint32 logn)
{
  uint32 n = 0;
  uint32 u = 0;

  n = (uint32)1 << logn;
  for (u = 0; u < n; u++)
  {
    a[u] = FsmSw_Falcon_Fpr_Neg(a[u]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_Poly_AdjFFT
*
* Description: Compute adjoint of polynomial a. This function works only in FFT representation.
*
* Arguments:   - fpr    *a:    t.b.d.
*              - uint32  logn: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Falcon_Poly_AdjFFT(fpr *a, uint32 logn)
{
  uint32 n = 0;
  uint32 u = 0;

  n = (uint32)1 << logn;
  for (u = (n >> 1); u < n; u++)
  {
    a[u] = FsmSw_Falcon_Fpr_Neg(a[u]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_Poly_MulFFT
*
* Description: Multiply polynomial a with polynomial b. a and b MUST NOT overlap. This function works only in FFT
*              representation.
*
* Arguments:   -       fpr    *a:    t.b.d.
*              - const fpr    *b:    t.b.d.
*              -       uint32  logn: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Falcon_Poly_MulFFT(fpr *a, const fpr *b, uint32 logn)
{
  uint32 n  = 0;
  uint32 hn = 0;
  uint32 u  = 0;
  fpr a_re  = 0;
  fpr a_im  = 0;
  fpr b_re  = 0;
  fpr b_im  = 0;

  n  = (uint32)1 << logn;
  hn = n >> 1;
  for (u = 0; u < hn; u++)
  {
    a_re = a[u];
    a_im = a[u + hn];
    b_re = b[u];
    b_im = b[u + hn];
    FPC_MUL(a[u], a[u + hn], a_re, a_im, b_re, b_im);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_Poly_MuladjFFT
*
* Description: Multiply polynomial a with the adjoint of polynomial b. a and b MUST NOT overlap. This function works
*              only in FFT representation.
*
* Arguments:   -       fpr    *a:    t.b.d.
*              - const fpr    *b:    t.b.d.
*              -       uint32  logn: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Falcon_Poly_MuladjFFT(fpr *a, const fpr *b, uint32 logn)
{
  uint32 n  = 0;
  uint32 hn = 0;
  uint32 u  = 0;

  n  = (uint32)1 << logn;
  hn = n >> 1;
  for (u = 0; u < hn; u++)
  {
    fpr a_re, a_im, b_re, b_im;

    a_re = a[u];
    a_im = a[u + hn];
    b_re = b[u];
    b_im = FsmSw_Falcon_Fpr_Neg(b[u + hn]);
    FPC_MUL(a[u], a[u + hn], a_re, a_im, b_re, b_im);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_Poly_MulselfadjFFT
*
* Description: Multiply polynomial with its own adjoint. This function works only in FFT representation.
*
* Arguments:   - fpr    *a:    t.b.d.
*              - uint32  logn: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Falcon_Poly_MulselfadjFFT(fpr *a, uint32 logn)
{
  /* Since each coefficient is multiplied with its own conjugate, the result contains only real values. */
  uint32 n  = 0;
  uint32 hn = 0;
  uint32 u  = 0;
  fpr a_re  = 0;
  fpr a_im  = 0;

  n  = (uint32)1 << logn;
  hn = n >> 1;
  for (u = 0; u < hn; u++)
  {
    a_re      = a[u];
    a_im      = a[u + hn];
    a[u]      = FsmSw_Falcon_Fpr_Add(FsmSw_Falcon_Fpr_Sqr(a_re), FsmSw_Falcon_Fpr_Sqr(a_im));
    a[u + hn] = fpr_zero;
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_Poly_Mulconst
*
* Description: Multiply polynomial with a real constant. This function works in both normal and FFT representations.
*
* Arguments:   - fpr    *a:    t.b.d.
*              - fpr     x:    t.b.d.
*              - uint32  logn: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Falcon_Poly_Mulconst(fpr *a, fpr x, uint32 logn)
{
  uint32 n = 0;
  uint32 u = 0;

  n = (uint32)1 << logn;
  for (u = 0; u < n; u++)
  {
    a[u] = FsmSw_Falcon_Fpr_Mul(a[u], x);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_Poly_Invnorm2FFT
*
* Description: Given f and g (in FFT representation), compute 1/(f*adj(f)+g*adj(g)) (also in FFT representation). Since
*              the result is auto-adjoint, all its coordinates in FFT representation are real; as such, only the first
*              N/2 values of d[] are filled (the imaginary parts are skipped).
*              Array d MUST NOT overlap with either a or b.
*
* Arguments:   -       fpr    *d:    t.b.d.
*              - const fpr    *a:    t.b.d.
*              - const fpr    *b:    t.b.d.
*              -       uint32  logn: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Falcon_Poly_Invnorm2FFT(fpr *d, const fpr *a, const fpr *b, uint32 logn)
{
  uint32 n  = 0;
  uint32 hn = 0;
  uint32 u  = 0;
  fpr a_re  = 0;
  fpr a_im  = 0;
  fpr b_re  = 0;
  fpr b_im  = 0;

  n  = (uint32)1 << logn;
  hn = n >> 1;
  for (u = 0; u < hn; u++)
  {
    a_re = a[u];
    a_im = a[u + hn];
    b_re = b[u];
    b_im = b[u + hn];
    d[u] = FsmSw_Falcon_Fpr_Inv(
        FsmSw_Falcon_Fpr_Add(FsmSw_Falcon_Fpr_Add(FsmSw_Falcon_Fpr_Sqr(a_re), FsmSw_Falcon_Fpr_Sqr(a_im)),
                             FsmSw_Falcon_Fpr_Add(FsmSw_Falcon_Fpr_Sqr(b_re), FsmSw_Falcon_Fpr_Sqr(b_im))));
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_FFT
*
* Description: Given F, G, f and g (in FFT representation), compute F*adj(f)+G*adj(g) (also in FFT representation).
*              Destination d MUST NOT overlap with any of the source arrays.
*
* Arguments:   -       fpr    *d:    t.b.d.
*              - const fpr    *F:    t.b.d.
*              - const fpr    *G:    t.b.d.
*              - const fpr    *f:    t.b.d.
*              - const fpr    *g:    t.b.d.
*              -       uint32  logn: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Falcon_Poly_Add_MuladjFFT(fpr *d, const fpr *F, const fpr *G, const fpr *f, const fpr *g, uint32 logn)
{
  uint32 n             = 0;
  uint32 hn            = 0;
  uint32 u             = 0;
  fpr poly_F_real      = 0;
  fpr poly_F_imaginary = 0;
  fpr poly_G_real      = 0;
  fpr poly_G_imaginary = 0;
  fpr adj_f_real       = 0;
  fpr adj_f_imaginary  = 0;
  fpr adj_g_real       = 0;
  fpr adj_g_imaginary  = 0;
  fpr a_re             = 0;
  fpr a_im             = 0;
  fpr b_re             = 0;
  fpr b_im             = 0;

  n  = (uint32)1 << logn;
  hn = n >> 1;
  for (u = 0; u < hn; u++)
  {
    poly_F_real      = F[u];
    poly_F_imaginary = F[u + hn];
    poly_G_real      = G[u];
    poly_G_imaginary = G[u + hn];
    adj_f_real       = f[u];
    adj_f_imaginary  = f[u + hn];
    adj_g_real       = g[u];
    adj_g_imaginary  = g[u + hn];

    FPC_MUL(a_re, a_im, poly_F_real, poly_F_imaginary, adj_f_real, FsmSw_Falcon_Fpr_Neg(adj_f_imaginary));
    FPC_MUL(b_re, b_im, poly_G_real, poly_G_imaginary, adj_g_real, FsmSw_Falcon_Fpr_Neg(adj_g_imaginary));
    d[u]      = FsmSw_Falcon_Fpr_Add(a_re, b_re);
    d[u + hn] = FsmSw_Falcon_Fpr_Add(a_im, b_im);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_Poly_Mul_AutoadjFFT
*
* Description: Multiply polynomial a by polynomial b, where b is autoadjoint. Both a and b are in FFT representation.
*              Since b is autoadjoint, all its FFT coefficients are real, and the array b contains only N/2 elements.
*              a and b MUST NOT overlap.
*
* Arguments:   -       fpr    *a:    t.b.d.
*              - const fpr    *b:    t.b.d.
*              -       uint32  logn: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Falcon_Poly_Mul_AutoadjFFT(fpr *a, const fpr *b, uint32 logn)
{
  uint32 n  = 0;
  uint32 hn = 0;
  uint32 u  = 0;

  n  = (uint32)1 << logn;
  hn = n >> 1;
  for (u = 0; u < hn; u++)
  {
    a[u]      = FsmSw_Falcon_Fpr_Mul(a[u], b[u]);
    a[u + hn] = FsmSw_Falcon_Fpr_Mul(a[u + hn], b[u]);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_Poly_Div_AutoadjFFT
*
* Description: Divide polynomial a by polynomial b, where b is autoadjoint. Both a and b are in FFT representation.
*              Since b is autoadjoint, all its FFT coefficients are real, and the array b contains only N/2 elements.
*              a and b MUST NOT overlap.
*
* Arguments:   -       fpr    *a:    t.b.d.
*              - const fpr    *b:    t.b.d.
*              -       uint32  logn: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Falcon_Poly_Div_AutoadjFFT(fpr *a, const fpr *b, uint32 logn)
{
  uint32 n  = 0;
  uint32 hn = 0;
  uint32 u  = 0;

  n  = (uint32)1 << logn;
  hn = n >> 1;
  for (u = 0; u < hn; u++)
  {
    fpr ib;

    ib        = FsmSw_Falcon_Fpr_Inv(b[u]);
    a[u]      = FsmSw_Falcon_Fpr_Mul(a[u], ib);
    a[u + hn] = FsmSw_Falcon_Fpr_Mul(a[u + hn], ib);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_Poly_LdlFFT
*
* Description: Perform an LDL decomposition of an auto-adjoint matrix G, in FFT representation. On input, g00, g01 and
*              g11 are provided (where the matrix G = [[g00, g01], [adj(g01), g11]]). On output, the d00, l10 and d11
*              values are written in g00, g01 and g11, respectively (with D = [[d00, 0], [0, d11]] and L = [[1, 0],
*              [l10, 1]]). (In fact, d00 = g00, so the g00 operand is left unmodified.)
*
* Arguments:   - const fpr    *g00:  t.b.d.
*              -       fpr    *g01:  t.b.d.
*              -       fpr    *g11:  t.b.d.
*              -       uint32  logn: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Falcon_Poly_LdlFFT(const fpr *g00, fpr *g01, fpr *g11, uint32 logn)
{
  uint32 n   = 0;
  uint32 hn  = 0;
  uint32 u   = 0;
  fpr g00_re = 0;
  fpr g00_im = 0;
  fpr g01_re = 0;
  fpr g01_im = 0;
  fpr g11_re = 0;
  fpr g11_im = 0;
  fpr mu_re  = 0;
  fpr mu_im  = 0;

  n  = (uint32)1 << logn;
  hn = n >> 1;
  for (u = 0; u < hn; u++)
  {
    g00_re = g00[u];
    g00_im = g00[u + hn];
    g01_re = g01[u];
    g01_im = g01[u + hn];
    g11_re = g11[u];
    g11_im = g11[u + hn];
    FPC_DIV(mu_re, mu_im, g01_re, g01_im, g00_re, g00_im);
    FPC_MUL(g01_re, g01_im, mu_re, mu_im, g01_re, FsmSw_Falcon_Fpr_Neg(g01_im));
    FPC_SUB(g11[u], g11[u + hn], g11_re, g11_im, g01_re, g01_im);
    g01[u]      = mu_re;
    g01[u + hn] = FsmSw_Falcon_Fpr_Neg(mu_im);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_Poly_LdlMvFFT
*
* Description: Perform an LDL decomposition of an auto-adjoint matrix G, in FFT representation. This is identical to
*              poly_LDL_fft() except that g00, g01 and g11 are unmodified; the outputs d11 and l10 are written in two
*              other separate buffers provided as extra parameters.
*
* Arguments:   -       fpr    *d11:  t.b.d.
*              -       fpr    *l10:  t.b.d.
*              - const fpr    *g00:  t.b.d.
*              - const fpr    *g01:  t.b.d.
*              - const fpr    *g11:  t.b.d.
*              -       uint32  logn: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Falcon_Poly_LdlMvFFT(fpr *d11, fpr *l10, const fpr *g00, const fpr *g01, const fpr *g11, uint32 logn)
{
  uint32 n   = 0;
  uint32 hn  = 0;
  uint32 u   = 0;
  fpr g00_re = 0;
  fpr g00_im = 0;
  fpr g01_re = 0;
  fpr g01_im = 0;
  fpr g11_re = 0;
  fpr g11_im = 0;
  fpr mu_re  = 0;
  fpr mu_im  = 0;

  n  = (uint32)1 << logn;
  hn = n >> 1;
  for (u = 0; u < hn; u++)
  {
    g00_re = g00[u];
    g00_im = g00[u + hn];
    g01_re = g01[u];
    g01_im = g01[u + hn];
    g11_re = g11[u];
    g11_im = g11[u + hn];
    FPC_DIV(mu_re, mu_im, g01_re, g01_im, g00_re, g00_im);
    FPC_MUL(g01_re, g01_im, mu_re, mu_im, g01_re, FsmSw_Falcon_Fpr_Neg(g01_im));
    FPC_SUB(d11[u], d11[u + hn], g11_re, g11_im, g01_re, g01_im);
    l10[u]      = mu_re;
    l10[u + hn] = FsmSw_Falcon_Fpr_Neg(mu_im);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_Poly_SplitFFT
*
* Description: Apply "split" operation on a polynomial in FFT representation:
*              f = f0(x^2) + x*f1(x^2), for half-size polynomials f0 and f1 (polynomials modulo X^(N/2)+1).
*              f0, f1 and f MUST NOT overlap.
*
* Arguments:   -       fpr    *f0:   t.b.d.
*              -       fpr    *f1:   t.b.d.
*              - const fpr    *f:    t.b.d.
*              -       uint32  logn: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Falcon_Poly_SplitFFT(fpr *f0, fpr *f1, const fpr *f, uint32 logn)
{
  /* The FFT representation we use is in bit-reversed order (element i contains f(w^(rev(i))), where rev() is the
     * bit-reversal function over the ring degree. This changes indexes with regards to the Falcon specification. */
  uint32 n  = 0;
  uint32 hn = 0;
  uint32 qn = 0;
  uint32 u  = 0;
  fpr a_re  = 0;
  fpr a_im  = 0;
  fpr b_re  = 0;
  fpr b_im  = 0;
  fpr t_re  = 0;
  fpr t_im  = 0;

  n  = (uint32)1 << logn;
  hn = n >> 1;
  qn = hn >> 1;

  /* We process complex values by pairs. For logn = 1, there is only one complex value (the other one is the implicit
     * conjugate), so we add the two lines below because the loop will be skipped. */
  f0[0] = f[0];
  f1[0] = f[hn];

  for (u = 0; u < qn; u++)
  {
    a_re = f[(u << 1)];
    a_im = f[(u << 1) + hn];
    b_re = f[(u << 1) + 1u];
    b_im = f[(u << 1) + 1u + hn];

    FPC_ADD(t_re, t_im, a_re, a_im, b_re, b_im);
    f0[u]      = FsmSw_Falcon_Fpr_Half(t_re);
    f0[u + qn] = FsmSw_Falcon_Fpr_Half(t_im);

    FPC_SUB(t_re, t_im, a_re, a_im, b_re, b_im);
    FPC_MUL(t_re, t_im, t_re, t_im, fpr_gm_tab[((u + hn) << 1) + 0u],
            FsmSw_Falcon_Fpr_Neg(fpr_gm_tab[((u + hn) << 1) + 1u]));
    f1[u]      = FsmSw_Falcon_Fpr_Half(t_re);
    f1[u + qn] = FsmSw_Falcon_Fpr_Half(t_im);
  }
}

/***********************************************************************************************************************
* Name:        FsmSw_Falcon_Poly_MergeFFT
*
* Description: Apply "merge" operation on two polynomials in FFT representation:
*              Given f0 and f1, polynomials moduo X^(N/2)+1, this function computes f = f0(x^2) + x*f1(x^2), in FFT
*              representation modulo X^N+1. f MUST NOT overlap with either f0 or f1.
*
* Arguments:   -       fpr    *f:    t.b.d.
*              - const fpr    *f0:   t.b.d.
*              - const fpr    *f1:   t.b.d.
*              -       uint32  logn: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Falcon_Poly_MergeFFT(fpr *f, const fpr *f0, const fpr *f1, uint32 logn)
{
  uint32 n  = 0;
  uint32 hn = 0;
  uint32 qn = 0;
  uint32 u  = 0;
  fpr a_re  = 0;
  fpr a_im  = 0;
  fpr b_re  = 0;
  fpr b_im  = 0;
  fpr t_re  = 0;
  fpr t_im  = 0;

  n  = (uint32)1 << logn;
  hn = n >> 1;
  qn = hn >> 1;

  /* An extra copy to handle the special case logn = 1. */
  f[0]  = f0[0];
  f[hn] = f1[0];

  for (u = 0; u < qn; u++)
  {
    a_re = f0[u];
    a_im = f0[u + qn];
    FPC_MUL(b_re, b_im, f1[u], f1[u + qn], fpr_gm_tab[((u + hn) << 1) + 0u], fpr_gm_tab[((u + hn) << 1) + 1u]);
    FPC_ADD(t_re, t_im, a_re, a_im, b_re, b_im);
    f[(u << 1)]      = t_re;
    f[(u << 1) + hn] = t_im;
    FPC_SUB(t_re, t_im, a_re, a_im, b_re, b_im);
    f[(u << 1) + 1u]      = t_re;
    f[(u << 1) + 1u + hn] = t_im;
  }
}
