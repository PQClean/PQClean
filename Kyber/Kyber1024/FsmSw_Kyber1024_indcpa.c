/***********************************************************************************************************************
*
*                                          IAV GmbH
*                          All rights reserved - Alle Rechte vorbehalten
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
#include "Platform_Types.h"
#include "FsmSw_CommonLib.h"
#include "FsmSw_Kyber_ntt.h"
#include "FsmSw_Kyber_symmetric.h"
#include "FsmSw_Kyber1024_params.h"
#include "FsmSw_Kyber1024_poly.h"
#include "FsmSw_Kyber1024_polyvec.h"
#include "FsmSw_Kyber1024_indcpa.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define GEN_MATRIX_NBLOCKS ((12u*KYBER_N/8u*4096u/KYBER_Q + XOF_BLOCKBYTES)/XOF_BLOCKBYTES)

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/
#define gen_a(A,B)  FsmSw_Kyber1024_gen_matrix(A,B,0)
#define gen_at(A,B) FsmSw_Kyber1024_gen_matrix(A,B,1)

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/
static void pack_pk(uint8 r[KYBER1024_INDCPA_PUBLICKEYBYTES], polyvec1024 *pk, const uint8 seed[KYBER_SYMBYTES]);
static void unpack_pk(polyvec1024 *pk, uint8 seed[KYBER_SYMBYTES],
                      const uint8 packedpk[KYBER1024_INDCPA_PUBLICKEYBYTES]);
static void pack_sk(uint8 r[KYBER1024_INDCPA_SECRETKEYBYTES], polyvec1024 *sk);
static void unpack_sk(polyvec1024 *sk, const uint8 packedsk[KYBER1024_INDCPA_SECRETKEYBYTES]);
static void pack_ciphertext(uint8 r[KYBER1024_INDCPA_BYTES], polyvec1024 *b, poly1024 *v);
static void unpack_ciphertext(polyvec1024 *b, poly1024 *v, const uint8 c[KYBER1024_INDCPA_BYTES]);
static uint16 rej_uniform(sint16 *r, uint16 len, const uint8 *buf, uint16 buflen);

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        pack_pk
*
* Description: Serialize the public key as concatenation of the
*              serialized vector of polynomials pk
*              and the public seed used to generate the matrix A.
*
* Arguments:         uint8       *r:    pointer to the output serialized public key
*                    polyvec1024 *pk:   pointer to the input public-key polyvec
*              const uint8       *seed: pointer to the input public seed
***********************************************************************************************************************/
static void pack_pk(uint8 r[KYBER1024_INDCPA_PUBLICKEYBYTES], polyvec1024 *pk, const uint8 seed[KYBER_SYMBYTES])
{
    uint32 i;

    FsmSw_Kyber1024_polyvec_tobytes(r, pk);

    for (i = 0; i < KYBER_SYMBYTES; i++)
    {
        r[i + KYBER1024_POLYVECBYTES] = seed[i];
    }
}

/***********************************************************************************************************************
* Name:        unpack_pk
*
* Description: De-serialize public key from a byte array
*              approximate inverse of pack_pk
*
* Arguments:   -       polyvec1024 *pk:       pointer to output public-key polynomial vector
*              -       uint8       *seed:     pointer to output seed to generate matrix A
*              - const uint8       *packedpk: pointer to input serialized public key
***********************************************************************************************************************/
static void unpack_pk(polyvec1024 *pk,
                      uint8 seed[KYBER_SYMBYTES],
                      const uint8 packedpk[KYBER1024_INDCPA_PUBLICKEYBYTES]) 
{
    uint32 i;

    FsmSw_Kyber1024_polyvec_frombytes(pk, packedpk);

    for (i = 0; i < KYBER_SYMBYTES; i++)
    {
        seed[i] = packedpk[i + KYBER1024_POLYVECBYTES];
    }
}

/***********************************************************************************************************************
* Name:        pack_sk
*
* Description: Serialize the secret key
*
* Arguments:   - uint8       *r:  pointer to output serialized secret key
*              - polyvec1024 *sk: pointer to input vector of polynomials (secret key)
***********************************************************************************************************************/
static void pack_sk(uint8 r[KYBER1024_INDCPA_SECRETKEYBYTES], polyvec1024 *sk) 
{
    FsmSw_Kyber1024_polyvec_tobytes(r, sk);
}

/***********************************************************************************************************************
* Name:        unpack_sk
*
* Description: De-serialize the secret key; inverse of pack_sk
*
* Arguments:   -       polyvec1024 *sk:       pointer to output vector of polynomials (secret key)
*              - const uint8       *packedsk: pointer to input serialized secret key
***********************************************************************************************************************/
static void unpack_sk(polyvec1024 *sk, const uint8 packedsk[KYBER1024_INDCPA_SECRETKEYBYTES]) 
{
    FsmSw_Kyber1024_polyvec_frombytes(sk, packedsk);
}

/***********************************************************************************************************************
* Name:        pack_ciphertext
*
* Description: Serialize the ciphertext as concatenation of the
*              compressed and serialized vector of polynomials b
*              and the compressed and serialized polynomial v
*
* Arguments:   uint8    *r: pointer to the output serialized ciphertext
*              poly1024 *b: pointer to the input vector of polynomials b
*              poly1024 *v: pointer to the input polynomial v
***********************************************************************************************************************/
static void pack_ciphertext(uint8 r[KYBER1024_INDCPA_BYTES], polyvec1024 *b, poly1024 *v) 
{
    FsmSw_Kyber1024_polyvec_compress(r, b);
    FsmSw_Kyber1024_poly_compress(&r[KYBER1024_POLYVECCOMPRESSEDBYTES], v);
}

/***********************************************************************************************************************
* Name:        unpack_ciphertext
*
* Description: De-serialize and decompress ciphertext from a byte array;
*              approximate inverse of pack_ciphertext
*
* Arguments:   -       polyvec1024 *b: pointer to the output vector of polynomials b
*              -       poly1024    *v: pointer to the output polynomial v
*              - const uint8       *c: pointer to the input serialized ciphertext
***********************************************************************************************************************/
static void unpack_ciphertext(polyvec1024 *b, poly1024 *v, const uint8 c[KYBER1024_INDCPA_BYTES]) 
{
    FsmSw_Kyber1024_polyvec_decompress(b, c);
    FsmSw_Kyber1024_poly_decompress(v, &c[KYBER1024_POLYVECCOMPRESSEDBYTES]);
}

/***********************************************************************************************************************
* Name:        rej_uniform
*
* Description: Run rejection sampling on uniform random bytes to generate
*              uniform random integers mod q
*
* Arguments:   -       sint16 *r:      pointer to output buffer
*              -       uint16  len:    requested number of 16-bit integers (uniform mod q)
*              - const uint8  *buf:    pointer to input buffer (assumed to be uniformly random bytes)
*              -       uint16  buflen: length of input buffer in bytes
*
* Returns number of sampled 16-bit integers (at most len)
***********************************************************************************************************************/
static uint16 rej_uniform(sint16 *r, uint16 len, const uint8 *buf, uint16 buflen)
{
    uint16 ctr, pos;
    uint16 val0, val1;

    ctr = pos = 0;

    while (ctr < len && pos + 3u <= buflen)
    {
        val0 = (((uint16)buf[pos + 0u] >> 0u) | ((uint16)buf[pos + 1u] << 8u)) & 0xFFFu;
        val1 = (((uint16)buf[pos + 1u] >> 4u) | ((uint16)buf[pos + 2u] << 4u)) & 0xFFFu;
        pos  = pos + 3u;

        if (val0 < KYBER_Q)
        {
            r[ctr++] = (sint16)val0;
        }

        if (ctr < len && val1 < KYBER_Q)
        {
            r[ctr++] = (sint16)val1;
        }
    }

    return ctr;
}

/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_gen_matrix
*
* Description: Deterministically generate matrix A (or the transpose of A)
*              from a seed. Entries of the matrix are polynomials that look
*              uniformly random. Performs rejection sampling on output of
*              a XOF
*
* Arguments:   -       polyvec1024 *a:          pointer to ouptput matrix A
*              - const uint8       *seed:       pointer to input seed
*              -       uint8        transposed: boolean deciding whether A or A^T is generated
***********************************************************************************************************************/
// Not static for benchmarking
void FsmSw_Kyber1024_gen_matrix(polyvec1024 *a, const uint8 seed[KYBER_SYMBYTES], uint8 transposed)
{
    uint8  i, j;
    uint16 buflen, off, ctr, k;
    uint8  buf[GEN_MATRIX_NBLOCKS * XOF_BLOCKBYTES + 2u];
    xof_state state;

    for (i = 0; i < KYBER1024_K; i++) 
    {
        for (j = 0; j < KYBER1024_K; j++) 
        {
            if (transposed > 0u)
            {
                xof_absorb(&state, seed, i, j);
            }
            else
            {
                xof_absorb(&state, seed, j, i);
            }

            xof_squeezeblocks(buf, GEN_MATRIX_NBLOCKS, &state);
            buflen = GEN_MATRIX_NBLOCKS * XOF_BLOCKBYTES;
            ctr = rej_uniform(a[i].vec[j].coeffs, KYBER_N, buf, buflen);

            while (ctr < KYBER_N)
            {
                off = buflen % 3u;
                for (k = 0; k < off; k++)
                {
                    buf[k] = buf[buflen - off + k];
                }
                xof_squeezeblocks(&buf[off], 1u, &state);
                buflen = off + XOF_BLOCKBYTES;
                ctr = ctr + (rej_uniform(&(a[i].vec[j].coeffs[ctr]), KYBER_N - ctr, buf, buflen));
            }
        }
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_indcpa_keypair
*
* Description: Generates public and private key for the CPA-secure
*              public-key encryption scheme underlying Kyber
*
* Arguments:   - uint8 *pk: pointer to output public key (of length KYBER1024_INDCPA_PUBLICKEYBYTES bytes)
*              - uint8 *sk: pointer to output private key  (of length KYBER1024_INDCPA_SECRETKEYBYTES bytes)
***********************************************************************************************************************/
void FsmSw_Kyber1024_indcpa_keypair(uint8 pk[KYBER1024_INDCPA_PUBLICKEYBYTES],
                                    uint8 sk[KYBER1024_INDCPA_SECRETKEYBYTES])
{
    uint8 i;
    uint8 buf[2u * KYBER_SYMBYTES];
    const uint8 *publicseed = buf;
    const uint8 *noiseseed = &buf[KYBER_SYMBYTES];
    uint8 nonce = 0;

    polyvec1024 a[KYBER1024_K], e, pkpv, skpv;

    (void)FsmSw_CommonLib_randombytes(buf, KYBER_SYMBYTES);
    hash_g(buf, buf, KYBER_SYMBYTES);

    gen_a(a, publicseed);

    for (i = 0; i < KYBER1024_K; i++)
    {
        FsmSw_Kyber1024_poly_getnoise_eta1(&skpv.vec[i], noiseseed, nonce++);
    }

    for (i = 0; i < KYBER1024_K; i++)
    {
        FsmSw_Kyber1024_poly_getnoise_eta1(&e.vec[i], noiseseed, nonce++);
    }

    FsmSw_Kyber1024_polyvec_ntt(&skpv);
    FsmSw_Kyber1024_polyvec_ntt(&e);

    // matrix-vector multiplication
    for (i = 0; i < KYBER1024_K; i++) 
    {
        FsmSw_Kyber1024_polyvec_basemul_acc_montgomery(&pkpv.vec[i], &a[i], &skpv);
        FsmSw_Kyber1024_poly_tomont(&pkpv.vec[i]);
    }

    FsmSw_Kyber1024_polyvec_add(&pkpv, &pkpv, &e);
    FsmSw_Kyber1024_polyvec_reduce(&pkpv);

    pack_sk(sk, &skpv);
    pack_pk(pk, &pkpv, publicseed);
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_indcpa_enc
*
* Description: Encryption function of the CPA-secure
*              public-key encryption scheme underlying Kyber.
*
* Arguments:   -       uint8 *c:     pointer to output ciphertext (of length KYBER1024_INDCPA_BYTES bytes)
*              - const uint8 *m:     pointer to input message (of length KYBER1024_INDCPA_MSGBYTES bytes)
*              - const uint8 *pk:    pointer to input public key (of length KYBER1024_INDCPA_PUBLICKEYBYTES)
*              - const uint8 *coins: pointer to input random coins used as seed
*                                    (of length KYBER_SYMBYTES) to deterministically
*                                    generate all randomness
***********************************************************************************************************************/
void FsmSw_Kyber1024_indcpa_enc(      uint8 c[KYBER1024_INDCPA_BYTES],
                                const uint8 m[KYBER1024_INDCPA_MSGBYTES],
                                const uint8 pk[KYBER1024_INDCPA_PUBLICKEYBYTES],
                                const uint8 coins[KYBER_SYMBYTES])
{
    uint8 i;
    uint8 seed[KYBER_SYMBYTES];
    uint8 nonce = 0;
    polyvec1024 sp, pkpv, ep, at[KYBER1024_K], b;
    poly1024    v, k, epp;

    unpack_pk(&pkpv, seed, pk);
    FsmSw_Kyber1024_poly_frommsg(&k, m);
    gen_at(at, seed);

    for (i = 0; i < KYBER1024_K; i++)
    {
        FsmSw_Kyber1024_poly_getnoise_eta1(&(sp.vec[i]), coins, nonce++);
    }

    for (i = 0; i < KYBER1024_K; i++)
    {
        FsmSw_Kyber1024_poly_getnoise_eta2(&(ep.vec[i]), coins, nonce++);
    }

    FsmSw_Kyber1024_poly_getnoise_eta2(&epp, coins, nonce);

    FsmSw_Kyber1024_polyvec_ntt(&sp);

    // matrix-vector multiplication
    for (i = 0; i < KYBER1024_K; i++)
    {
        FsmSw_Kyber1024_polyvec_basemul_acc_montgomery(&b.vec[i], &at[i], &sp);
    }

    FsmSw_Kyber1024_polyvec_basemul_acc_montgomery(&v, &pkpv, &sp);

    FsmSw_Kyber1024_polyvec_invntt_tomont(&b);
    FsmSw_Kyber1024_poly_invntt_tomont(&v);

    FsmSw_Kyber1024_polyvec_add(&b, &b, &ep);
    FsmSw_Kyber1024_poly_add(&v, &v, &epp);
    FsmSw_Kyber1024_poly_add(&v, &v, &k);
    FsmSw_Kyber1024_polyvec_reduce(&b);
    FsmSw_Kyber1024_poly_reduce(&v);

    pack_ciphertext(c, &b, &v);
}

/***********************************************************************************************************************
* Name:        FsmSw_Kyber1024_indcpa_dec
*
* Description: Decryption function of the CPA-secure
*              public-key encryption scheme underlying Kyber.
*
* Arguments:   -       uint8 *m:  pointer to output decrypted message (of length KYBER1024_INDCPA_MSGBYTES bytes)
*              - const uint8 *c:  pointer to input ciphertext (of length KYBER1024_INDCPA_BYTES bytes)
*              - const uint8 *sk: pointer to input secret key (of length KYBER1024_INDCPA_SECRETKEYBYTES bytes)
***********************************************************************************************************************/
void FsmSw_Kyber1024_indcpa_dec(      uint8 m[KYBER1024_INDCPA_MSGBYTES],
                                const uint8 c[KYBER1024_INDCPA_BYTES],
                                const uint8 sk[KYBER1024_INDCPA_SECRETKEYBYTES])
{
    polyvec1024 b, skpv;
    poly1024    v, mp;

    unpack_ciphertext(&b, &v, c);
    unpack_sk(&skpv, sk);

    FsmSw_Kyber1024_polyvec_ntt(&b);
    FsmSw_Kyber1024_polyvec_basemul_acc_montgomery(&mp, &skpv, &b);
    FsmSw_Kyber1024_poly_invntt_tomont(&mp);

    FsmSw_Kyber1024_poly_sub(&mp, &v, &mp);
    FsmSw_Kyber1024_poly_reduce(&mp);

    FsmSw_Kyber1024_poly_tomsg(m, &mp);
}
