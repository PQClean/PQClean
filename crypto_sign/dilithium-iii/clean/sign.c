#include "sign.h"
#include "fips202.h"
#include "packing.h"
#include "params.h"
#include "poly.h"
#include "polyvec.h"
#include "randombytes.h"
#include <stdint.h>

/*************************************************
 * Name:        expand_mat
 *
 * Description: Implementation of ExpandA. Generates matrix A with uniformly
 *              random coefficients a_{i,j} by performing rejection
 *              sampling on the output stream of SHAKE128(rho|i|j).
 *
 * Arguments:   - polyvecl mat[K]: output matrix
 *              - const unsigned char rho[]: byte array containing seed rho
 **************************************************/
void expand_mat(polyvecl mat[K], const unsigned char rho[SEEDBYTES]) {
  unsigned int i, j;
  unsigned char inbuf[SEEDBYTES + 1];
  /* Don't change this to smaller values,
   * sampling later assumes sufficient SHAKE output!
   * Probability that we need more than 5 blocks: < 2^{-132}.
   * Probability that we need more than 6 blocks: < 2^{-546}. */
  unsigned char outbuf[5 * SHAKE128_RATE];

  for (i = 0; i < SEEDBYTES; ++i)
    inbuf[i] = rho[i];

  for (i = 0; i < K; ++i) {
    for (j = 0; j < L; ++j) {
      inbuf[SEEDBYTES] = i + (j << 4);
      shake128(outbuf, sizeof(outbuf), inbuf, SEEDBYTES + 1);
      poly_uniform(mat[i].vec + j, outbuf);
    }
  }
}

/*************************************************
 * Name:        challenge
 *
 * Description: Implementation of H. Samples polynomial with 60 nonzero
 *              coefficients in {-1,1} using the output stream of
 *              SHAKE256(mu|w1).
 *
 * Arguments:   - poly *c: pointer to output polynomial
 *              - const unsigned char mu[]: byte array containing mu
 *              - const polyveck *w1: pointer to vector w1
 **************************************************/
void challenge(poly *c, const unsigned char mu[CRHBYTES], const polyveck *w1) {
  unsigned int i, b, pos;
  unsigned char inbuf[CRHBYTES + K * POLW1_SIZE_PACKED];
  unsigned char outbuf[SHAKE256_RATE];
  uint64_t state[25], signs, mask;

  for (i = 0; i < CRHBYTES; ++i)
    inbuf[i] = mu[i];
  for (i = 0; i < K; ++i)
    polyw1_pack(inbuf + CRHBYTES + i * POLW1_SIZE_PACKED, w1->vec + i);

  shake256_absorb(state, inbuf, sizeof(inbuf));
  shake256_squeezeblocks(outbuf, 1, state);

  signs = 0;
  for (i = 0; i < 8; ++i)
    signs |= (uint64_t)outbuf[i] << 8 * i;

  pos = 8;
  mask = 1;

  for (i = 0; i < N; ++i)
    c->coeffs[i] = 0;

  for (i = 196; i < 256; ++i) {
    do {
      if (pos >= SHAKE256_RATE) {
        shake256_squeezeblocks(outbuf, 1, state);
        pos = 0;
      }

      b = outbuf[pos++];
    } while (b > i);

    c->coeffs[i] = c->coeffs[b];
    c->coeffs[b] = (signs & mask) ? Q - 1 : 1;
    mask <<= 1;
  }
}

/*************************************************
 * Name:        crypto_sign_keypair
 *
 * Description: Generates public and private key.
 *
 * Arguments:   - unsigned char *pk: pointer to output public key (allocated
 *                                   array of CRYPTO_PUBLICKEYBYTES bytes)
 *              - unsigned char *sk: pointer to output private key (allocated
 *                                   array of CRYPTO_SECRETKEYBYTES bytes)
 *
 * Returns 0 (success)
 **************************************************/
int crypto_sign_keypair(unsigned char *pk, unsigned char *sk) {
  unsigned int i;
  unsigned char seedbuf[3 * SEEDBYTES];
  unsigned char tr[CRHBYTES];
  unsigned char *rho, *rhoprime, *key;
  uint16_t nonce = 0;
  polyvecl mat[K];
  polyvecl s1, s1hat;
  polyveck s2, t, t1, t0;

  /* Expand 32 bytes of randomness into rho, rhoprime and key */
  randombytes(seedbuf, SEEDBYTES);
  shake256(seedbuf, 3 * SEEDBYTES, seedbuf, SEEDBYTES);
  rho = seedbuf;
  rhoprime = rho + SEEDBYTES;
  key = rho + 2 * SEEDBYTES;

  /* Expand matrix */
  expand_mat(mat, rho);

  /* Sample short vectors s1 and s2 */
  for (i = 0; i < L; ++i)
    poly_uniform_eta(s1.vec + i, rhoprime, nonce++);
  for (i = 0; i < K; ++i)
    poly_uniform_eta(s2.vec + i, rhoprime, nonce++);

  /* Matrix-vector multiplication */
  s1hat = s1;
  polyvecl_ntt(&s1hat);
  for (i = 0; i < K; ++i) {
    polyvecl_pointwise_acc_invmontgomery(t.vec + i, mat + i, &s1hat);
    poly_reduce(t.vec + i);
    poly_invntt_montgomery(t.vec + i);
  }

  /* Add noise vector s2 */
  polyveck_add(&t, &t, &s2);

  /* Extract t1 and write public key */
  polyveck_freeze(&t);
  polyveck_power2round(&t1, &t0, &t);
  pack_pk(pk, rho, &t1);

  /* Compute CRH(rho, t1) and write secret key */
  shake256(tr, CRHBYTES, pk, CRYPTO_PUBLICKEYBYTES);
  pack_sk(sk, rho, key, tr, &s1, &s2, &t0);

  return 0;
}

/*************************************************
 * Name:        crypto_sign
 *
 * Description: Compute signed message.
 *
 * Arguments:   - unsigned char *sm: pointer to output signed message (allocated
 *                                   array with CRYPTO_BYTES + mlen bytes),
 *                                   can be equal to m
 *              - unsigned long long *smlen: pointer to output length of signed
 *                                           message
 *              - const unsigned char *m: pointer to message to be signed
 *              - unsigned long long mlen: length of message
 *              - const unsigned char *sk: pointer to bit-packed secret key
 *
 * Returns 0 (success)
 **************************************************/
int crypto_sign(unsigned char *sm, unsigned long long *smlen,
                const unsigned char *m, unsigned long long mlen,
                const unsigned char *sk) {
  unsigned long long i, j;
  unsigned int n;
  unsigned char
      seedbuf[2 * SEEDBYTES + CRHBYTES]; // TODO: nonce in seedbuf (2x)
  unsigned char tr[CRHBYTES];
  unsigned char *rho, *key, *mu;
  uint16_t nonce = 0;
  poly c, chat;
  polyvecl mat[K], s1, y, yhat, z;
  polyveck s2, t0, w, w1;
  polyveck h, wcs2, wcs20, ct0, tmp;

  rho = seedbuf;
  key = seedbuf + SEEDBYTES;
  mu = seedbuf + 2 * SEEDBYTES;
  unpack_sk(rho, key, tr, &s1, &s2, &t0, sk);

  /* Copy tr and message into the sm buffer,
   * backwards since m and sm can be equal in SUPERCOP API */
  for (i = 1; i <= mlen; ++i)
    sm[CRYPTO_BYTES + mlen - i] = m[mlen - i];
  for (i = 0; i < CRHBYTES; ++i)
    sm[CRYPTO_BYTES - CRHBYTES + i] = tr[i];

  /* Compute CRH(tr, msg) */
  shake256(mu, CRHBYTES, sm + CRYPTO_BYTES - CRHBYTES, CRHBYTES + mlen);

  /* Expand matrix and transform vectors */
  expand_mat(mat, rho);
  polyvecl_ntt(&s1);
  polyveck_ntt(&s2);
  polyveck_ntt(&t0);

rej:
  /* Sample intermediate vector y */
  for (i = 0; i < L; ++i)
    poly_uniform_gamma1m1(y.vec + i, key, nonce++);

  /* Matrix-vector multiplication */
  yhat = y;
  polyvecl_ntt(&yhat);
  for (i = 0; i < K; ++i) {
    polyvecl_pointwise_acc_invmontgomery(w.vec + i, mat + i, &yhat);
    poly_reduce(w.vec + i);
    poly_invntt_montgomery(w.vec + i);
  }

  /* Decompose w and call the random oracle */
  polyveck_csubq(&w);
  polyveck_decompose(&w1, &tmp, &w);
  challenge(&c, mu, &w1);

  /* Compute z, reject if it reveals secret */
  chat = c;
  poly_ntt(&chat);
  for (i = 0; i < L; ++i) {
    poly_pointwise_invmontgomery(z.vec + i, &chat, s1.vec + i);
    poly_invntt_montgomery(z.vec + i);
  }
  polyvecl_add(&z, &z, &y);
  polyvecl_freeze(&z);
  if (polyvecl_chknorm(&z, GAMMA1 - BETA))
    goto rej;

  /* Compute w - cs2, reject if w1 can not be computed from it */
  for (i = 0; i < K; ++i) {
    poly_pointwise_invmontgomery(wcs2.vec + i, &chat, s2.vec + i);
    poly_invntt_montgomery(wcs2.vec + i);
  }
  polyveck_sub(&wcs2, &w, &wcs2);
  polyveck_freeze(&wcs2);
  polyveck_decompose(&tmp, &wcs20, &wcs2);
  polyveck_csubq(&wcs20);
  if (polyveck_chknorm(&wcs20, GAMMA2 - BETA))
    goto rej;

  for (i = 0; i < K; ++i)
    for (j = 0; j < N; ++j)
      if (tmp.vec[i].coeffs[j] != w1.vec[i].coeffs[j])
        goto rej;

  /* Compute hints for w1 */
  for (i = 0; i < K; ++i) {
    poly_pointwise_invmontgomery(ct0.vec + i, &chat, t0.vec + i);
    poly_invntt_montgomery(ct0.vec + i);
  }

  polyveck_csubq(&ct0);
  if (polyveck_chknorm(&ct0, GAMMA2))
    goto rej;

  polyveck_add(&tmp, &wcs2, &ct0);
  polyveck_csubq(&tmp);
  n = polyveck_make_hint(&h, &wcs2, &tmp);
  if (n > OMEGA)
    goto rej;

  /* Write signature */
  pack_sig(sm, &z, &h, &c);

  *smlen = mlen + CRYPTO_BYTES;
  return 0;
}

/*************************************************
 * Name:        crypto_sign_open
 *
 * Description: Verify signed message.
 *
 * Arguments:   - unsigned char *m: pointer to output message (allocated
 *                                  array with smlen bytes), can be equal to sm
 *              - unsigned long long *mlen: pointer to output length of message
 *              - const unsigned char *sm: pointer to signed message
 *              - unsigned long long smlen: length of signed message
 *              - const unsigned char *sk: pointer to bit-packed public key
 *
 * Returns 0 if signed message could be verified correctly and -1 otherwise
 **************************************************/
int crypto_sign_open(unsigned char *m, unsigned long long *mlen,
                     const unsigned char *sm, unsigned long long smlen,
                     const unsigned char *pk) {
  unsigned long long i;
  unsigned char rho[SEEDBYTES];
  unsigned char mu[CRHBYTES];
  poly c, chat, cp;
  polyvecl mat[K], z;
  polyveck t1, w1, h, tmp1, tmp2;

  if (smlen < CRYPTO_BYTES)
    goto badsig;

  *mlen = smlen - CRYPTO_BYTES;

  unpack_pk(rho, &t1, pk);
  if (unpack_sig(&z, &h, &c, sm))
    goto badsig;
  if (polyvecl_chknorm(&z, GAMMA1 - BETA))
    goto badsig;

  /* Compute CRH(CRH(rho, t1), msg) using m as "playground" buffer */
  if (sm != m)
    for (i = 0; i < *mlen; ++i)
      m[CRYPTO_BYTES + i] = sm[CRYPTO_BYTES + i];

  shake256(m + CRYPTO_BYTES - CRHBYTES, CRHBYTES, pk, CRYPTO_PUBLICKEYBYTES);
  shake256(mu, CRHBYTES, m + CRYPTO_BYTES - CRHBYTES, CRHBYTES + *mlen);

  /* Matrix-vector multiplication; compute Az - c2^dt1 */
  expand_mat(mat, rho);
  polyvecl_ntt(&z);
  for (i = 0; i < K; ++i)
    polyvecl_pointwise_acc_invmontgomery(tmp1.vec + i, mat + i, &z);

  chat = c;
  poly_ntt(&chat);
  polyveck_shiftl(&t1, D);
  polyveck_ntt(&t1);
  for (i = 0; i < K; ++i)
    poly_pointwise_invmontgomery(tmp2.vec + i, &chat, t1.vec + i);

  polyveck_sub(&tmp1, &tmp1, &tmp2);
  polyveck_reduce(&tmp1);
  polyveck_invntt_montgomery(&tmp1);

  /* Reconstruct w1 */
  polyveck_csubq(&tmp1);
  polyveck_use_hint(&w1, &tmp1, &h);

  /* Call random oracle and verify challenge */
  challenge(&cp, mu, &w1);
  for (i = 0; i < N; ++i)
    if (c.coeffs[i] != cp.coeffs[i])
      goto badsig;

  /* All good, copy msg, return 0 */
  for (i = 0; i < *mlen; ++i)
    m[i] = sm[CRYPTO_BYTES + i];

  return 0;

/* Signature verification failed */
badsig:
  *mlen = (unsigned long long)-1;
  for (i = 0; i < smlen; ++i)
    m[i] = 0;

  return -1;
}
