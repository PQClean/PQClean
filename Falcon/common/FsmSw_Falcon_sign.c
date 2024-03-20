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
/* Falcon signature generation. */
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_CommonLib.h"
#include "FsmSw_Falcon_common.h"
#include "FsmSw_Falcon_fft.h"
#include "FsmSw_Falcon_rng.h"
#include "FsmSw_Falcon_sign.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
/* Compute degree N from logarithm 'logn'. */
#define MKN(logn)   ((uint32)1 << (logn))

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
/* Internal sampler engine. Exported for tests.
 * sampler_context wraps around a source of random numbers (PRNG) and the sigma_min value (nominally dependent on the
 * degree). sampler() takes as parameters:
 *   ctx      pointer to the sampler_context structure
 *   mu       center for the distribution
 *   isigma   inverse of the distribution standard deviation
 * It returns an integer sampled along the Gaussian distribution centered on mu and of standard deviation
 * sigma = 1/isigma.
 * gaussian0_sampler() takes as parameter a pointer to a PRNG, and returns an integer sampled along a half-Gaussian
 * with standard deviation sigma0 = 1.8205 (center is 0, returned value is nonnegative). */
typedef struct
{
    prng p;
    fpr sigma_min;
} sampler_context;

typedef sint32 (*samplerZ)(void *ctx, fpr mu, fpr sigma);

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/
static const uint32 dist[] = {10745844u, 3068844u, 3741698u, 5559083u,
                              1580863u, 8248194u, 2260429u, 13669192u, 2736639u, 708981u,
                              4421575u, 10046180u, 169348u, 7122675u, 4136815u, 30538u, 13063405u,
                              7650655u, 4132u, 14505003u, 7826148u, 417u, 16768101u, 11363290u,
                              31u, 8444042u, 8086568u, 1u, 12844466u, 265321u, 0u, 1232676u,
                              13644283u, 0u, 38047u, 9111839u, 0u, 870u, 6138264u, 0u, 14u,
                              12545723u, 0u, 0u, 3104126u, 0u, 0u, 28824u, 0u, 0u, 198u, 0u, 0u,
                              1u};
/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/
       uint32   ffLDL_treesize(uint32 logn);
static void     ffLDL_fft_inner(fpr *tree, fpr *g0, fpr *g1, uint32 logn, fpr *tmp);
static void     ffLDL_binary_normalize(fpr *tree, uint32 orig_logn, uint32 logn);
static void     smallints_to_fpr(fpr *r, const sint8 *t, uint32 logn);
static sint32   ber_exp(prng *p, fpr x, fpr ccs);
       sint32   sign_sampler(void *ctx, fpr mu, fpr isigma);
       sint32   gaussian0_sampler(prng *p);
static void     ffSampling_fft_dyntree(samplerZ samp, void *samp_ctx, fpr *t0, fpr *t1, fpr *g00, fpr *g01, fpr *g11,
                                       uint32 orig_logn, uint32 logn, fpr *tmp);
static void     ffSampling_fft(samplerZ samp, void *samp_ctx, fpr *z0, fpr *z1, const fpr *tree, const fpr *t0,
                               const fpr *t1, uint32 logn, fpr *tmp);
static sint32   do_sign_dyn(samplerZ samp, void *samp_ctx, sint16 *s2, const sint8 *f, const sint8 *g, const sint8 *F,
                            const sint8 *G, const uint16 *hm, uint32 logn, fpr *tmp);

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        ffLDL_treesize
*
* Description: Get the size of the LDL tree for an input with polynomials of size 2^logn. The size is expressed in the
*              number of elements.
*
* Arguments:   - uint32 logn:    t.b.d.
*
* Returns t.b.d.
*
***********************************************************************************************************************/
uint32 ffLDL_treesize(uint32 logn)
{
  /* For logn = 0 (polynomials are constant), the "tree" is a single element. Otherwise, the tree node has size 2^logn,
   * and has two child trees for size logn-1 each. Thus, treesize s() must fulfill these two relations:
   *   s(0) = 1
   *   s(logn) = (2^logn) + 2*s(logn-1) */
  return (logn + 1u) << logn;
}

/***********************************************************************************************************************
* Name:        ffLDL_fft_inner
*
* Description: Inner function for ffLDL_fft(). It expects the matrix to be both auto-adjoint and quasicyclic; also, it
*              uses the source operands as modifiable temporaries.
*
* Arguments:   - fpr      *tree: t.b.d.
*              - fpr      *g0:   t.b.d.
*              - fpr      *g1:   t.b.d.
*              - uint32    logn: t.b.d.
*              - fpr      *tmp:  tmp[] must have room for at least one polynomial.
*
***********************************************************************************************************************/
static void ffLDL_fft_inner(fpr *tree, fpr *g0, fpr *g1, uint32 logn, fpr *tmp)
{
  uint32 n, hn;

  n = MKN(logn);

  if (n == 1u)
  {
    tree[0] = g0[0];
    return;
  }

  hn = n >> 1;

  /* The LDL decomposition yields L (which is written in the tree) and the diagonal of D. Since d00 = g0, we just write
   * d11 into tmp. */
  FsmSw_Falcon_poly_LDLmv_fft(tmp, tree, g0, g1, g0, logn);

  /* Split d00 (currently in g0) and d11 (currently in tmp). We reuse g0 and g1 as temporary storage spaces:
   *   d00 splits into g1, g1+hn
   *   d11 splits into g0, g0+hn */
  FsmSw_Falcon_poly_split_fft(g1, &g1[hn], g0, logn);
  FsmSw_Falcon_poly_split_fft(g0, &g0[hn], tmp, logn);

  /* Each split result is the first row of a new auto-adjoint quasicyclic matrix for the next recursive step. */
  /* MISRA C 2012 rule 17.2 violation: [R] Functions shall not call themselves, either directly or indirectly:
   * Without in-depth knowledge, this violation cannot be resolved. */
  ffLDL_fft_inner(&tree[n], g1, &g1[hn], logn - 1u, tmp);
  ffLDL_fft_inner(&tree[n + ffLDL_treesize(logn - 1u)], g0, &g0[hn], logn - 1u, tmp);
}


/***********************************************************************************************************************
* Name:        ffLDL_binary_normalize
*
* Description: Normalize an ffLDL tree: each leaf of value x is replaced with sigma / sqrt(x).
*
* Arguments:   - fpr    *tree:      t.b.d.
*              - uint32  orig_logn: t.b.d.
*              - uint32  logn:      t.b.d.
*
***********************************************************************************************************************/
static void ffLDL_binary_normalize(fpr *tree, uint32 orig_logn, uint32 logn)
{
  /* TODO: make an iterative version. */
  uint32 n;

  n = MKN(logn);
  if (n == 1u)
  {
    /* We actually store in the tree leaf the inverse of the value mandated by the specification:
     * this saves a division both here and in the sampler. */
    tree[0] = FsmSw_Falcon_fpr_mul(FsmSw_Falcon_fpr_sqrt(tree[0]), fpr_inv_sigma[orig_logn]);
  }
  else
  {
    /* MISRA C 2012 rule 17.2 violation: [R] Functions shall not call themselves, either directly or indirectly:
     * Without in-depth knowledge, this violation cannot be resolved. */
    ffLDL_binary_normalize(&tree[n], orig_logn, logn - 1u);
    ffLDL_binary_normalize(&tree[n + ffLDL_treesize(logn - 1u)], orig_logn, logn - 1u);
  }
}

/***********************************************************************************************************************
* Name:        smallints_to_fpr
*
* Description: Convert an integer polynomial (with small values) into the representation with complex numbers.
*
* Arguments:   -       fpr      *r:    t.b.d.
*              - const sint8    *t:    t.b.d.
*              -       uint32    logn: t.b.d.
*
***********************************************************************************************************************/
static void smallints_to_fpr(fpr *r, const sint8 *t, uint32 logn)
{
  uint32 n, u;

  n = MKN(logn);
  for (u = 0; u < n; u++)
  {
    r[u] = FsmSw_Falcon_fpr_of(t[u]);
  }
}


/***********************************************************************************************************************
* Name:        ber_exp
*
* Description: Sample a bit with probability exp(-x) for some x >= 0.
*
* Arguments:   - prng *p:   t.b.d.
*              - fpr   x:   t.b.d.
*              - fpr   ccs: t.b.d.
*
* Returns t.b.d.
*
***********************************************************************************************************************/
static sint32 ber_exp(prng *p, fpr x, fpr ccs)
{
  sint32 s, i;
  fpr r;
  uint32 sw, w;
  uint64 z;

  /* Reduce x modulo log(2): x = s*log(2) + r, with s an integer, and 0 <= r < log(2). Since x >= 0, we can use
   * FsmSw_Falcon_fpr_trunc(). */
  s = (sint32) FsmSw_Falcon_fpr_trunc(FsmSw_Falcon_fpr_mul(x, fpr_inv_log2));
  r = FsmSw_Falcon_fpr_sub(x, FsmSw_Falcon_fpr_mul(FsmSw_Falcon_fpr_of(s), fpr_log2));

  /* It may happen (quite rarely) that s >= 64; if sigma = 1.2 (the minimum value for sigma), r = 0 and b = 1, then we
   * get s >= 64 if the half-Gaussian produced a z >= 13, which happens with probability about 0.000000000230383991,
   * which is approximatively equal to 2^(-32). In any case, if s >= 64, then ber_exp will be non-zero with probability
   * less than 2^(-64), so we can simply saturate s at 63. */
  sw = (uint32) s;
  sw ^= (sw ^ 63u) & ((uint32)((sint32)((-1) * (sint32)((uint32)((63u - sw) >> 31)))));
  s = (sint32) sw;

  /* Compute exp(-r); we know that 0 <= r < log(2) at this point, so we can use FsmSw_Falcon_fpr_expm_p63(), which
   * yields a result scaled to 2^63. We scale it up to 2^64, then right-shift it by s bits because we really want
   * exp(-x) = 2^(-s)*exp(-r).
   * The "-1" operation makes sure that the value fits on 64 bits (i.e. if r = 0, we may get 2^64, and we prefer 2^64-1
   * in that case). The bias is negligible since FsmSw_Falcon_fpr_expm_p63() only computes with 51 bits of precision
   * or so. */
  z = ((FsmSw_Falcon_fpr_expm_p63(r, ccs) << 1) - 1u) >> (uint32)s;

  /* Sample a bit with probability exp(-x). Since x = s*log(2) + r, exp(-x) = 2^-s * exp(-r), we compare lazily exp(-x)
   * with the PRNG output to limit its consumption, the sign of the difference yields the expected result. */
  i = 64;
  do
  {
    i -= 8;
    w = FsmSw_Falcon_prng_get_u8(p) - (uint32)((uint64)(((uint64)(z >> (uint64)i)) & 0xFFu));
  }
  while ((0u == w) && (i > 0));

  return (sint32)((uint32)(w >> 31));
}

/***********************************************************************************************************************
* Name:        sign_sampler
*
* Description: The sampler produces a random integer that follows a discrete Gaussian distribution, centered on mu, and
*              with standard deviation sigma. The provided parameter isigma is equal to 1/sigma.
*              The value of sigma MUST lie between 1 and 2 (i.e. isigma lies between 0.5 and 1); in Falcon, sigma
*              should always be between 1.2 and 1.9.
*
* Arguments:   - void *ctx:    t.b.d.
*              - fpr   mu:     t.b.d.
*              - fpr   isigma: t.b.d.
*
* Returns t.b.d.
*
***********************************************************************************************************************/
sint32 sign_sampler(void *ctx, fpr mu, fpr isigma)
{
    sampler_context *spc;
    sint32 s;
    fpr r, dss, ccs;
    sint32 z0, z, b;
    fpr x;

    spc = ctx;

    /* Center is mu. We compute mu = s + r where s is an integer and 0 <= r < 1. */
    s = (sint32) FsmSw_Falcon_fpr_floor(mu);
    r = FsmSw_Falcon_fpr_sub(mu, FsmSw_Falcon_fpr_of(s));

    /* dss = 1/(2*sigma^2) = 0.5*(isigma^2). */
    dss = FsmSw_Falcon_fpr_half(FsmSw_Falcon_fpr_sqr(isigma));

    /* ccs = sigma_min / sigma = sigma_min * isigma. */
    ccs = FsmSw_Falcon_fpr_mul(isigma, spc->sigma_min);

    /* We now need to sample on center r. */
    for (uint32 i = 0; i < 0xFFFFFFFFu; i++)
    {
      /* Sample z for a Gaussian distribution. Then get a random bit b to turn the sampling into a bimodal
       * distribution: if b = 1, we use z+1, otherwise we use -z. We thus have two situations:
       * - b = 1: z >= 1 and sampled against a Gaussian centered on 1.
       * - b = 0: z <= 0 and sampled against a Gaussian centered on 0.
       */
      z0 = gaussian0_sampler(&spc->p);
      b = (sint32)((uint32)(FsmSw_Falcon_prng_get_u8(&spc->p) & 1u));
      z = b + ((sint32) ((uint32)(((uint32)b << 1) - 1u))) * z0;

      /* Rejection sampling. We want a Gaussian centered on r; but we sampled against a Gaussian centered on b (0 or 1).
       * But we know that z is always in the range where our sampling distribution is greater than the Gaussian
       * distribution, so rejection works.
       * We got z with distribution:
       *    G(z) = exp(-((z-b)^2)/(2*sigma0^2))
       * We target distribution:
       *    S(z) = exp(-((z-r)^2)/(2*sigma^2))
       * Rejection sampling works by keeping the value z with probability S(z)/G(z), and starting again otherwise.
       * This requires S(z) <= G(z), which is the case here. Thus, we simply need to keep our z with probability:
       *    P = exp(-x)
       * where:
       *    x = ((z-r)^2)/(2*sigma^2) - ((z-b)^2)/(2*sigma0^2)
       * Here, we scale up the Bernouilli distribution, which* makes rejection more probable, but makes rejection
       * rate sufficiently decorrelated from the Gaussian center and standard deviation that the whole sampler
       * can be said to be constant-time. */

      x = FsmSw_Falcon_fpr_mul(FsmSw_Falcon_fpr_sqr(FsmSw_Falcon_fpr_sub(FsmSw_Falcon_fpr_of(z), r)), dss);
      x = FsmSw_Falcon_fpr_sub(x, FsmSw_Falcon_fpr_mul(FsmSw_Falcon_fpr_of((sint64)((sint64)z0 * (sint64)z0)),
                                                                                          fpr_inv_2sqrsigma0));

      if (0 < ber_exp(&spc->p, x, ccs))
      {
        /* Rejection sampling was centered on r, but the actual center is mu = s + r. */
        return s + z;
      }
    }

    return 0;
}

/***********************************************************************************************************************
* Name:        gaussian0_sampler
*
* Description: Sample an integer value along a half-gaussian distribution centered on zero and standard deviation
*              1.8205, with a precision of 72 bits.
*
* Arguments:   - prng *p: t.b.d.
*
* Returns z
*
***********************************************************************************************************************/
sint32 gaussian0_sampler(prng *p)
{
    uint32 v0, v1, v2, hi;
    uint64 lo;
    uint32 u;
    sint32 z;

    /* Get a random 72-bit value, into three 24-bit limbs v0..v2. */
    lo = FsmSw_Falcon_prng_get_u64(p);
    hi = FsmSw_Falcon_prng_get_u8(p);
    v0 = (uint32) lo & 0xFFFFFFu;
    v1 = (uint32)((lo >> 24) & 0xFFFFFFu);
    v2 = (uint32)(lo >> 48) | (hi << 16);

    /* Sampled value is z, such that v0..v2 is lower than the first z elements of the table. */
    z = 0;

    for (u = 0; u < (sizeof dist) / sizeof(dist[0]); u += 3u)
    {
      uint32 w0, w1, w2, cc;

      w0 = dist[u + 2u];
      w1 = dist[u + 1u];
      w2 = dist[u + 0u];
      cc = (v0 - w0) >> 31;
      cc = (v1 - w1 - cc) >> 31;
      cc = (v2 - w2 - cc) >> 31;
      z += (sint32) cc;
    }

    return z;
}

/***********************************************************************************************************************
* Name:        ffSampling_fft_dyntree
*
* Description: Perform Fast Fourier Sampling for target vector t. The Gram matrix is provided (G = [[g00, g01],
*              [adj(g01), g11]]). The sampled vector is written over (t0,t1). The Gram matrix is modified as well.
*
* Arguments:   - samplerZ  samp:      t.b.d.
*              - void     *samp_ctx:  t.b.d.
*              - fpr      *t0:        t.b.d.
*              - fpr      *t1:        t.b.d.
*              - fpr      *g00:       t.b.d.
*              - fpr      *g01:       t.b.d.
*              - fpr      *g11:       t.b.d.
*              - uint32  orig_logn: t.b.d.
*              - uint32  logn:      t.b.d.
*              - fpr      *tmp:       tmp[] must have room for at least nine polynomials.
*
***********************************************************************************************************************/
static void ffSampling_fft_dyntree(samplerZ samp, void *samp_ctx, fpr *t0, fpr *t1, fpr *g00, fpr *g01, fpr *g11,
                                   uint32 orig_logn, uint32 logn, fpr *tmp)
{
  uint32 n, hn;
  fpr *z0, *z1;

  /* Deepest level: the LDL tree leaf value is just g00 (the array has length only 1 at this point); we normalize it
   * with regards to sigma, then use it for sampling. */
  if(logn == 0u)
  {
    fpr leaf;

    leaf = g00[0];
    leaf = FsmSw_Falcon_fpr_mul(FsmSw_Falcon_fpr_sqrt(leaf), fpr_inv_sigma[orig_logn]);
    t0[0] = FsmSw_Falcon_fpr_of(samp(samp_ctx, t0[0], leaf));
    t1[0] = FsmSw_Falcon_fpr_of(samp(samp_ctx, t1[0], leaf));

    return;
  }

  n = (uint32) 1 << logn;
  hn = n >> 1;

  /* Decompose G into LDL. We only need d00 (identical to g00), d11, and l10; we do that in place. */
  FsmSw_Falcon_poly_LDL_fft(g00, g01, g11, logn);

  /* Split d00 and d11 and expand them into half-size quasi-cyclic Gram matrices. We also save l10 in tmp[]. */
  FsmSw_Falcon_poly_split_fft(tmp, &tmp[hn], g00, logn);
  FsmSw_CommonLib_memcpy(g00, tmp, n * sizeof *tmp);
  FsmSw_Falcon_poly_split_fft(tmp, &tmp[hn], g11, logn);
  FsmSw_CommonLib_memcpy(g11, tmp, n * sizeof *tmp);
  FsmSw_CommonLib_memcpy(tmp, g01, n * sizeof *g01);
  FsmSw_CommonLib_memcpy(g01, g00, hn * sizeof *g00);
  FsmSw_CommonLib_memcpy(&g01[hn], g11, hn * sizeof *g00);

  /* The half-size Gram matrices for the recursive LDL tree building are now:
   *   - left sub-tree: g00, g00+hn, g01
   *   - right sub-tree: g11, g11+hn, g01+hn
   * l10 is in tmp[]. */

  /* We split t1 and use the first recursive call on the two halves, using the right sub-tree. The result is merged
   * back into tmp + 2*n. */
  z1 = &tmp[n];
  FsmSw_Falcon_poly_split_fft(z1, &z1[hn], t1, logn);
  /* MISRA C 2012 rule 17.2 violation: [R] Functions shall not call themselves, either directly or indirectly:
   * Without in-depth knowledge, this violation cannot be resolved. */
  ffSampling_fft_dyntree(samp, samp_ctx, z1, &z1[hn], g11, &g11[hn], &g01[hn], orig_logn, logn - 1u, &z1[n]);
  FsmSw_Falcon_poly_merge_fft(&tmp[(n << 1)], z1, &z1[hn], logn);

  /* Compute tb0 = t0 + (t1 - z1) * l10. At that point, l10 is in tmp, t1 is unmodified, and z1 is in tmp + (n << 1).
   * The buffer in z1 is free.   *
   * In the end, z1 is written over t1, and tb0 is in t0. */
  FsmSw_CommonLib_memcpy(z1, t1, n * sizeof *t1);
  FsmSw_Falcon_poly_sub(z1, &tmp[(n << 1)], logn);
  FsmSw_CommonLib_memcpy(t1, &tmp[(n << 1)], n * sizeof *tmp);
  FsmSw_Falcon_poly_mul_fft(tmp, z1, logn);
  FsmSw_Falcon_poly_add(t0, tmp, logn);

  /* Second recursive invocation, on the split tb0 (currently in t0) and the left sub-tree. */
  z0 = tmp;
  FsmSw_Falcon_poly_split_fft(z0, &z0[hn], t0, logn);
  /* MISRA C 2012 rule 17.2 violation: [R] Functions shall not call themselves, either directly or indirectly:
   * Without in-depth knowledge, this violation cannot be resolved. */
  ffSampling_fft_dyntree(samp, samp_ctx, z0, &z0[hn], g00, &g00[hn], g01, orig_logn, logn - 1u, &z0[n]);
  FsmSw_Falcon_poly_merge_fft(t0, z0, &z0[hn], logn);
}

/***********************************************************************************************************************
* Name:        ffSampling_fft
*
* Description: Perform Fast Fourier Sampling for target vector t and LDL tree T.
*
* Arguments:   -       samplerZ  samp:     t.b.d.
*              -       void     *samp_ctx: t.b.d.
*              -       fpr      *z0:       t.b.d.
*              -       fpr      *z1:       t.b.d.
*              - const fpr      *tree:     t.b.d.
*              - const fpr      *t0:       t.b.d.
*              - const fpr      *t1:       t.b.d.
*              -       uint32    logn:     t.b.d.
*              -       fpr      *tmp:      tmp[] must have size for at least two polynomials of size 2^logn.
*
***********************************************************************************************************************/
static void ffSampling_fft(samplerZ samp, void *samp_ctx, fpr *z0, fpr *z1, const fpr *tree, const fpr *t0,
                           const fpr *t1, uint32 logn, fpr *tmp)
{
  uint32 n, hn;
  const fpr *tree0, *tree1;

  /* When logn == 2, we inline the last two recursion levels. */
  if (logn == 2u)
  {
    fpr x0, x1, y0, y1, w0, w1, w2, w3, sigma;
    fpr a_re, a_im, b_re, b_im, c_re, c_im;

    tree0 = &tree[4];
    tree1 = &tree[8];

    /* We split t1 into w*, then do the recursive invocation, with output in w*. We finally merge back into z1. */
    a_re = t1[0];
    a_im = t1[2];
    b_re = t1[1];
    b_im = t1[3];
    c_re = FsmSw_Falcon_fpr_add(a_re, b_re);
    c_im = FsmSw_Falcon_fpr_add(a_im, b_im);
    w0 = FsmSw_Falcon_fpr_half(c_re);
    w1 = FsmSw_Falcon_fpr_half(c_im);
    c_re = FsmSw_Falcon_fpr_sub(a_re, b_re);
    c_im = FsmSw_Falcon_fpr_sub(a_im, b_im);
    w2 = FsmSw_Falcon_fpr_mul(FsmSw_Falcon_fpr_add(c_re, c_im), fpr_invsqrt8);
    w3 = FsmSw_Falcon_fpr_mul(FsmSw_Falcon_fpr_sub(c_im, c_re), fpr_invsqrt8);

    x0 = w2;
    x1 = w3;
    sigma = tree1[3];
    w2 = FsmSw_Falcon_fpr_of(samp(samp_ctx, x0, sigma));
    w3 = FsmSw_Falcon_fpr_of(samp(samp_ctx, x1, sigma));
    a_re = FsmSw_Falcon_fpr_sub(x0, w2);
    a_im = FsmSw_Falcon_fpr_sub(x1, w3);
    b_re = tree1[0];
    b_im = tree1[1];
    c_re = FsmSw_Falcon_fpr_sub(FsmSw_Falcon_fpr_mul(a_re, b_re), FsmSw_Falcon_fpr_mul(a_im, b_im));
    c_im = FsmSw_Falcon_fpr_add(FsmSw_Falcon_fpr_mul(a_re, b_im), FsmSw_Falcon_fpr_mul(a_im, b_re));
    x0 = FsmSw_Falcon_fpr_add(c_re, w0);
    x1 = FsmSw_Falcon_fpr_add(c_im, w1);
    sigma = tree1[2];
    w0 = FsmSw_Falcon_fpr_of(samp(samp_ctx, x0, sigma));
    w1 = FsmSw_Falcon_fpr_of(samp(samp_ctx, x1, sigma));

    a_re = w0;
    a_im = w1;
    b_re = w2;
    b_im = w3;
    c_re = FsmSw_Falcon_fpr_mul(FsmSw_Falcon_fpr_sub(b_re, b_im), fpr_invsqrt2);
    c_im = FsmSw_Falcon_fpr_mul(FsmSw_Falcon_fpr_add(b_re, b_im), fpr_invsqrt2);
    z1[0] = w0 = FsmSw_Falcon_fpr_add(a_re, c_re);
    z1[2] = w2 = FsmSw_Falcon_fpr_add(a_im, c_im);
    z1[1] = w1 = FsmSw_Falcon_fpr_sub(a_re, c_re);
    z1[3] = w3 = FsmSw_Falcon_fpr_sub(a_im, c_im);

    /* Compute tb0 = t0 + (t1 - z1) * L. Value tb0 ends up in w*. */
    w0 = FsmSw_Falcon_fpr_sub(t1[0], w0);
    w1 = FsmSw_Falcon_fpr_sub(t1[1], w1);
    w2 = FsmSw_Falcon_fpr_sub(t1[2], w2);
    w3 = FsmSw_Falcon_fpr_sub(t1[3], w3);

    a_re = w0;
    a_im = w2;
    b_re = tree[0];
    b_im = tree[2];
    w0 = FsmSw_Falcon_fpr_sub(FsmSw_Falcon_fpr_mul(a_re, b_re), FsmSw_Falcon_fpr_mul(a_im, b_im));
    w2 = FsmSw_Falcon_fpr_add(FsmSw_Falcon_fpr_mul(a_re, b_im), FsmSw_Falcon_fpr_mul(a_im, b_re));
    a_re = w1;
    a_im = w3;
    b_re = tree[1];
    b_im = tree[3];
    w1 = FsmSw_Falcon_fpr_sub(FsmSw_Falcon_fpr_mul(a_re, b_re), FsmSw_Falcon_fpr_mul(a_im, b_im));
    w3 = FsmSw_Falcon_fpr_add(FsmSw_Falcon_fpr_mul(a_re, b_im), FsmSw_Falcon_fpr_mul(a_im, b_re));

    w0 = FsmSw_Falcon_fpr_add(w0, t0[0]);
    w1 = FsmSw_Falcon_fpr_add(w1, t0[1]);
    w2 = FsmSw_Falcon_fpr_add(w2, t0[2]);
    w3 = FsmSw_Falcon_fpr_add(w3, t0[3]);

    /* Second recursive invocation.*/
    a_re = w0;
    a_im = w2;
    b_re = w1;
    b_im = w3;
    c_re = FsmSw_Falcon_fpr_add(a_re, b_re);
    c_im = FsmSw_Falcon_fpr_add(a_im, b_im);
    w0 = FsmSw_Falcon_fpr_half(c_re);
    w1 = FsmSw_Falcon_fpr_half(c_im);
    c_re = FsmSw_Falcon_fpr_sub(a_re, b_re);
    c_im = FsmSw_Falcon_fpr_sub(a_im, b_im);
    w2 = FsmSw_Falcon_fpr_mul(FsmSw_Falcon_fpr_add(c_re, c_im), fpr_invsqrt8);
    w3 = FsmSw_Falcon_fpr_mul(FsmSw_Falcon_fpr_sub(c_im, c_re), fpr_invsqrt8);

    x0 = w2;
    x1 = w3;
    sigma = tree0[3];
    w2 = y0 = FsmSw_Falcon_fpr_of(samp(samp_ctx, x0, sigma));
    w3 = y1 = FsmSw_Falcon_fpr_of(samp(samp_ctx, x1, sigma));
    a_re = FsmSw_Falcon_fpr_sub(x0, y0);
    a_im = FsmSw_Falcon_fpr_sub(x1, y1);
    b_re = tree0[0];
    b_im = tree0[1];
    c_re = FsmSw_Falcon_fpr_sub(FsmSw_Falcon_fpr_mul(a_re, b_re), FsmSw_Falcon_fpr_mul(a_im, b_im));
    c_im = FsmSw_Falcon_fpr_add(FsmSw_Falcon_fpr_mul(a_re, b_im), FsmSw_Falcon_fpr_mul(a_im, b_re));
    x0 = FsmSw_Falcon_fpr_add(c_re, w0);
    x1 = FsmSw_Falcon_fpr_add(c_im, w1);
    sigma = tree0[2];
    w0 = FsmSw_Falcon_fpr_of(samp(samp_ctx, x0, sigma));
    w1 = FsmSw_Falcon_fpr_of(samp(samp_ctx, x1, sigma));

    a_re = w0;
    a_im = w1;
    b_re = w2;
    b_im = w3;
    c_re = FsmSw_Falcon_fpr_mul(FsmSw_Falcon_fpr_sub(b_re, b_im), fpr_invsqrt2);
    c_im = FsmSw_Falcon_fpr_mul(FsmSw_Falcon_fpr_add(b_re, b_im), fpr_invsqrt2);
    z0[0] = FsmSw_Falcon_fpr_add(a_re, c_re);
    z0[2] = FsmSw_Falcon_fpr_add(a_im, c_im);
    z0[1] = FsmSw_Falcon_fpr_sub(a_re, c_re);
    z0[3] = FsmSw_Falcon_fpr_sub(a_im, c_im);

    return;
  }

  /* Case logn == 1 is reachable only when using Falcon-2 (the smallest size for which Falcon is mathematically
   * defined, but of course way too insecure to be of any use). */
  if (logn == 1u)
  {
    fpr x0, x1, y0, y1, sigma;
    fpr a_re, a_im, b_re, b_im, c_re, c_im;

    x0 = t1[0];
    x1 = t1[1];
    sigma = tree[3];
    z1[0] = y0 = FsmSw_Falcon_fpr_of(samp(samp_ctx, x0, sigma));
    z1[1] = y1 = FsmSw_Falcon_fpr_of(samp(samp_ctx, x1, sigma));
    a_re = FsmSw_Falcon_fpr_sub(x0, y0);
    a_im = FsmSw_Falcon_fpr_sub(x1, y1);
    b_re = tree[0];
    b_im = tree[1];
    c_re = FsmSw_Falcon_fpr_sub(FsmSw_Falcon_fpr_mul(a_re, b_re), FsmSw_Falcon_fpr_mul(a_im, b_im));
    c_im = FsmSw_Falcon_fpr_add(FsmSw_Falcon_fpr_mul(a_re, b_im), FsmSw_Falcon_fpr_mul(a_im, b_re));
    x0 = FsmSw_Falcon_fpr_add(c_re, t0[0]);
    x1 = FsmSw_Falcon_fpr_add(c_im, t0[1]);
    sigma = tree[2];
    z0[0] = FsmSw_Falcon_fpr_of(samp(samp_ctx, x0, sigma));
    z0[1] = FsmSw_Falcon_fpr_of(samp(samp_ctx, x1, sigma));

    return;
  }

  /* General recursive case (logn >= 3). */
  n = (uint32) 1 << logn;
  hn = n >> 1;
  tree0 = &tree[n];
  tree1 = &tree[n + ffLDL_treesize(logn - 1u)];

  /* We split t1 into z1 (reused as temporary storage), then do the recursive invocation, with output in tmp.
   * We finally merge back into z1. */
  FsmSw_Falcon_poly_split_fft(z1, &z1[hn], t1, logn);
  ffSampling_fft(samp, samp_ctx, tmp, &tmp[hn], tree1, z1, &z1[hn], logn - 1u, &tmp[n]);
  FsmSw_Falcon_poly_merge_fft(z1, tmp, &tmp[hn], logn);

  /* Compute tb0 = t0 + (t1 - z1) * L. Value tb0 ends up in tmp[]. */
  FsmSw_CommonLib_memcpy(tmp, t1, n * sizeof *t1);
  FsmSw_Falcon_poly_sub(tmp, z1, logn);
  FsmSw_Falcon_poly_mul_fft(tmp, tree, logn);
  FsmSw_Falcon_poly_add(tmp, t0, logn);

  /* Second recursive invocation. */
  FsmSw_Falcon_poly_split_fft(z0, &z0[hn], tmp, logn);
  /* MISRA C 2012 rule 17.2 violation: [R] Functions shall not call themselves, either directly or indirectly:
   * Without in-depth knowledge, this violation cannot be resolved. */
  ffSampling_fft(samp, samp_ctx, tmp, &tmp[hn], tree0, z0, &z0[hn], logn - 1u, &tmp[n]);
  FsmSw_Falcon_poly_merge_fft(z0, tmp, &tmp[hn], logn);
}

/***********************************************************************************************************************
* Name:        do_sign_dyn
*
* Description: Compute a signature: the signature contains two vectors, s1 and s2. The s1 vector is not returned. The
*              squared norm of (s1,s2) is computed, and if it is short enough, then s2 is returned into the s2[] buffer,
*              and 1 is returned; otherwise, s2[] is untouched and 0 is returned; the caller should then try again.
*
* Arguments:   -       samplerZ  samp:     t.b.d.
*              -       void     *samp_ctx: t.b.d.
*              -       sint16   *s2:       t.b.d.
*              - const sint8    *f:        t.b.d.
*              - const sint8    *g:        t.b.d.
*              - const sint8    *F:        t.b.d.
*              - const sint8    *G:        t.b.d.
*              - const uint16   *hm:       t.b.d.
*              -       uint32  logn:     t.b.d.
*              -       fpr      *tmp:      tmp[] must have room for at least nine polynomials.
*
* Returns 0 or 1
*
***********************************************************************************************************************/
static sint32 do_sign_dyn(samplerZ samp, void *samp_ctx, sint16 *s2, const sint8 *f, const sint8 *g, const sint8 *F,
                       const sint8 *G, const uint16 *hm, uint32 logn, fpr *tmp)
{
  uint32 n, u;
  fpr *t0, *t1, *tx, *ty;
  fpr *b00, *b01, *b10, *b11, *g00, *g01, *g11;
  fpr ni;
  uint32 sqn, ng;
  sint16 *s1tmp, *s2tmp;
  sint32 z;

  n = MKN(logn);

  /* Lattice basis is B = [[g, -f], [G, -F]]. We convert it to FFT. */
  b00 = tmp;
  b01 = &b00[n];
  b10 = &b01[n];
  b11 = &b10[n];
  smallints_to_fpr(b01, f, logn);
  smallints_to_fpr(b00, g, logn);
  smallints_to_fpr(b11, F, logn);
  smallints_to_fpr(b10, G, logn);
  FsmSw_Falcon_FFT(b01, logn);
  FsmSw_Falcon_FFT(b00, logn);
  FsmSw_Falcon_FFT(b11, logn);
  FsmSw_Falcon_FFT(b10, logn);
  FsmSw_Falcon_poly_neg(b01, logn);
  FsmSw_Falcon_poly_neg(b11, logn);

  /* Compute the Gram matrix G = B·B*. Formulas are:
   *   g00 = b00*adj(b00) + b01*adj(b01)
   *   g01 = b00*adj(b10) + b01*adj(b11)
   *   g10 = b10*adj(b00) + b11*adj(b01)
   *   g11 = b10*adj(b10) + b11*adj(b11)
   *
   * For historical reasons, this implementation uses g00, g01 and g11 (upper triangle). g10 is not kept since it is
   * equal to adj(g01).
   *
   * We _replace_ the matrix B with the Gram matrix, but we must keep b01 and b11 for computing the target vector. */
  t0 = &b11[n];
  t1 = &t0[n];

  FsmSw_CommonLib_memcpy(t0, b01, n * sizeof *b01);
  FsmSw_Falcon_poly_mulselfadj_fft(t0, logn);    /* t0 <- b01*adj(b01) */

  FsmSw_CommonLib_memcpy(t1, b00, n * sizeof *b00);
  FsmSw_Falcon_poly_muladj_fft(t1, b10, logn);   /* t1 <- b00*adj(b10) */
  FsmSw_Falcon_poly_mulselfadj_fft(b00, logn);   /* b00 <- b00*adj(b00) */
  FsmSw_Falcon_poly_add(b00, t0, logn);      /* b00 <- g00 */
  FsmSw_CommonLib_memcpy(t0, b01, n * sizeof *b01);
  FsmSw_Falcon_poly_muladj_fft(b01, b11, logn);  /* b01 <- b01*adj(b11) */
  FsmSw_Falcon_poly_add(b01, t1, logn);      /* b01 <- g01 */

  FsmSw_Falcon_poly_mulselfadj_fft(b10, logn);   /* b10 <- b10*adj(b10) */
  FsmSw_CommonLib_memcpy(t1, b11, n * sizeof *b11);
  FsmSw_Falcon_poly_mulselfadj_fft(t1, logn);    /* t1 <- b11*adj(b11) */
  FsmSw_Falcon_poly_add(b10, t1, logn);      /* b10 <- g11 */

  /* We rename variables to make things clearer. The three elements of the Gram matrix uses the first 3*n slots of
   * tmp[], followed by b11 and b01 (in that order). */
  g00 = b00;
  g01 = b01;
  g11 = b10;
  b01 = t0;
  t0 = &b01[n];
  t1 = &t0[n];

  /* Memory layout at that point: g00 g01 g11 b11 b01 t0 t1 */

  /* Set the target vector to [hm, 0] (hm is the hashed message). */
  for (u = 0; u < n; u++)
  {
    t0[u] = FsmSw_Falcon_fpr_of((sint64)hm[u]);
    /* This is implicit.
     * t1[u] = fpr_zero; */
  }

  /* Apply the lattice basis to obtain the real target vector (after normalization with regards to modulus). */
  FsmSw_Falcon_FFT(t0, logn);
  ni = fpr_inverse_of_q;
  FsmSw_CommonLib_memcpy(t1, t0, n * sizeof *t0);
  FsmSw_Falcon_poly_mul_fft(t1, b01, logn);
  FsmSw_Falcon_poly_mulconst(t1, FsmSw_Falcon_fpr_neg(ni), logn);
  FsmSw_Falcon_poly_mul_fft(t0, b11, logn);
  FsmSw_Falcon_poly_mulconst(t0, ni, logn);

  /* b01 and b11 can be discarded, so we move back (t0,t1). Memory layout is now: g00 g01 g11 t0 t1 */
  FsmSw_CommonLib_memcpy(b11, t0, n * 2u * sizeof *t0);
  t0 = &g11[n];
  t1 = &t0[n];

  /* Apply sampling; result is written over (t0,t1). */
  ffSampling_fft_dyntree(samp, samp_ctx, t0, t1, g00, g01, g11, logn, logn, &t1[n]);

  /* We arrange the layout back to: b00 b01 b10 b11 t0 t1   *
   * We did not conserve the matrix basis, so we must recompute it now. */
  b00 = tmp;
  b01 = &b00[n];
  b10 = &b01[n];
  b11 = &b10[n];
  FsmSw_CommonLib_memmove(&b11[n], t0, n * 2u * sizeof *t0);
  t0 = &b11[n];
  t1 = &t0[n];
  smallints_to_fpr(b01, f, logn);
  smallints_to_fpr(b00, g, logn);
  smallints_to_fpr(b11, F, logn);
  smallints_to_fpr(b10, G, logn);
  FsmSw_Falcon_FFT(b01, logn);
  FsmSw_Falcon_FFT(b00, logn);
  FsmSw_Falcon_FFT(b11, logn);
  FsmSw_Falcon_FFT(b10, logn);
  FsmSw_Falcon_poly_neg(b01, logn);
  FsmSw_Falcon_poly_neg(b11, logn);
  tx = &t1[n];
  ty = &tx[n];

  /* Get the lattice point corresponding to that tiny vector. */
  FsmSw_CommonLib_memcpy(tx, t0, n * sizeof *t0);
  FsmSw_CommonLib_memcpy(ty, t1, n * sizeof *t1);
  FsmSw_Falcon_poly_mul_fft(tx, b00, logn);
  FsmSw_Falcon_poly_mul_fft(ty, b10, logn);
  FsmSw_Falcon_poly_add(tx, ty, logn);
  FsmSw_CommonLib_memcpy(ty, t0, n * sizeof *t0);
  FsmSw_Falcon_poly_mul_fft(ty, b01, logn);

  FsmSw_CommonLib_memcpy(t0, tx, n * sizeof *tx);
  FsmSw_Falcon_poly_mul_fft(t1, b11, logn);
  FsmSw_Falcon_poly_add(t1, ty, logn);
  FsmSw_Falcon_iFFT(t0, logn);
  FsmSw_Falcon_iFFT(t1, logn);

  s1tmp = (sint16 *)((void*)tx);
  sqn = 0;
  ng = 0;

  for (u = 0; u < n; u++)
  {
    z = (sint32) hm[u] - (sint32) FsmSw_Falcon_fpr_rint(t0[u]);
    sqn += (uint32)((uint32)z * (uint32)z);
    ng |= sqn;
    s1tmp[u] = (sint16) z;
  }

  sqn |= (uint32)((sint32)((-1) * (sint32)((uint32)(ng >> 31))));

  /* With "normal" degrees (e.g. 512 or 1024), it is very improbable that the computed vector is not short enough;
   * however, it may happen in practice for the very reduced versions (e.g. degree 16 or below). In that case, the
   * caller will loop, and we must not write anything into s2[] because s2[] may overlap with the hashed message hm[]
   * and we need hm[] for the next iteration. */
  s2tmp = (sint16 *)((void*)tmp);

  for (u = 0; u < n; u++)
  {
    s2tmp[u] = (sint16) - FsmSw_Falcon_fpr_rint(t1[u]);
  }

  if (0 < FsmSw_Falcon_is_short_half(sqn, s2tmp, logn))
  {
    FsmSw_CommonLib_memcpy(s2, s2tmp, n * sizeof *s2);
    FsmSw_CommonLib_memcpy(tmp, s1tmp, n * sizeof *s1tmp);

    return 1;
  }

  return 0;
}

/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        FsmSw_Falcon_sign_dyn
*
* Description: Compute a signature over the provided hashed message (hm); the signature value is one short vector. This
*              function uses a raw key and dynamically recompute the B0 matrix and LDL tree; this saves RAM since there
*              is no needed for an expanded key, but increases the signature cost.
*              The sig[] and hm[] buffers may overlap. On successful output, the start of the tmp[] buffer contains the
*              s1 vector (as sint16 elements).
*              The minimal size (in bytes) of tmp[] is 72*2^logn bytes. tmp[] must have 64-bit alignment.
*              This function uses floating-point rounding (see set_fpu_cw()).
*
* Arguments:   -       sint16                 *sig:  t.b.d.
*              -       inner_shake256_context *rng:  t.b.d.
*              - const sint8                  *f:    t.b.d.
*              - const sint8                  *g:    t.b.d.
*              - const sint8                  *F:    t.b.d.
*              - const sint8                  *G:    t.b.d.
*              - const sint16                 *hm:   t.b.d.
*              -       uint32                  logn: t.b.d.
*              -       uint8                  *tmp:  t.b.d.
*
***********************************************************************************************************************/
void FsmSw_Falcon_sign_dyn(sint16 *sig, inner_shake256_context *rng, const sint8 *f, const sint8 *g, const sint8 *F,
                           const sint8 *G, const uint16 *hm, uint32 logn, uint8 *tmp)
{
  fpr *ftmp;

  ftmp = (fpr*) ((void*)tmp);

  for (uint32 i = 0; i < 0xFFFFFFFFu; i++)
  {
    /* Signature produces short vectors s1 and s2. The signature is acceptable only if the aggregate vector s1,s2 is
     * short; we must use the same bound as the verifier.
     * If the signature is acceptable, then we return only s2 (the verifier recomputes s1 from s2, the hashed message,
     * and the public key). */
    sampler_context spc;
    samplerZ samp;
    void *samp_ctx;

    /* Normal sampling. We use a fast PRNG seeded from our SHAKE context ('rng'). */
    spc.sigma_min = fpr_sigma_min[logn];
    FsmSw_Falcon_prng_init(&spc.p, rng);
    samp = sign_sampler;
    samp_ctx = &spc;

    /* Do the actual signature. */
    if (0 < do_sign_dyn(samp, samp_ctx, sig, f, g, F, G, hm, logn, ftmp))
    {
      break;
    }
  }
}
