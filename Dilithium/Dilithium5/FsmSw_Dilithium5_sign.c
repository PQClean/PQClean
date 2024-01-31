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
#include "FsmSw_Fips202.h"
#include "FsmSw_CommonLib.h"
#include "FsmSw_Dilithium5_packing.h"
#include "FsmSw_Dilithium5_params.h"
#include "FsmSw_Dilithium5_poly.h"
#include "FsmSw_Dilithium5_polyvec.h"
#include "FsmSw_Dilithium_symmetric.h"
#include "FsmSw_Dilithium5_sign.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

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
* Name:        FsmSw_Dilithium5_crypto_sign_keypair
*
* Description: Generates public and private key.
*
* Arguments:   - uint8 *pk: pointer to output public key
*                           (allocated array of FSMSW_DILITHIUM5_CRYPTO_PUBLICKEYBYTES bytes)
*              - uint8 *sk: pointer to output private key (allocated
*                           array of FSMSW_DILITHIUM5_CRYPTO_SECRETKEYBYTES bytes)
*
* Returns 0 (success)
***********************************************************************************************************************/
sint8 FsmSw_Dilithium5_crypto_sign_keypair(uint8 *pk, uint8 *sk)
{
    uint8 seedbuf[2u * SEEDBYTES_DILITHIUM + CRHBYTES_DILITHIUM];
    uint8 tr[TRBYTES_DILITHIUM];
    const uint8 *rho, *rhoprime, *key;
    polyvecl_D5 mat[K_DILITHIUM5];
    polyvecl_D5 s1, s1hat;
    polyveck_D5 s2, t1, t0;

    /* Get randomness for rho, rhoprime and key */
    (void)FsmSw_CommonLib_randombytes(seedbuf, SEEDBYTES_DILITHIUM);
    FsmSw_Fips202_shake256(seedbuf, 2u * SEEDBYTES_DILITHIUM + CRHBYTES_DILITHIUM, seedbuf, SEEDBYTES_DILITHIUM);
    rho = seedbuf;
    rhoprime = &rho[SEEDBYTES_DILITHIUM];
    key      = &rhoprime[CRHBYTES_DILITHIUM];

    /* Expand matrix */
    FsmSw_Dilithium5_polyvec_matrix_expand(mat, rho);

    /* Sample short vectors s1 and s2 */
    FsmSw_Dilithium5_polyvecl_uniform_eta(&s1, rhoprime, 0u);
    FsmSw_Dilithium5_polyveck_uniform_eta(&s2, rhoprime, L_DILITHIUM5);

    /* Matrix-vector multiplication */
    s1hat = s1;
    FsmSw_Dilithium5_polyvecl_ntt(&s1hat);
    FsmSw_Dilithium5_polyvec_matrix_pointwise_montgomery(&t1, mat, &s1hat);
    FsmSw_Dilithium5_polyveck_reduce(&t1);
    FsmSw_Dilithium5_polyveck_invntt_tomont(&t1);

    /* Add error vector s2 */
    FsmSw_Dilithium5_polyveck_add(&t1, &t1, &s2);

    /* Extract t1 and write public key */
    FsmSw_Dilithium5_polyveck_caddq(&t1);
    FsmSw_Dilithium5_polyveck_power2round(&t1, &t0, &t1);
    FsmSw_Dilithium5_pack_pk(pk, rho, &t1);

    /* Compute H(rho, t1) and write secret key */
    FsmSw_Fips202_shake256(tr, TRBYTES_DILITHIUM, pk, FSMSW_DILITHIUM5_CRYPTO_PUBLICKEYBYTES);
    FsmSw_Dilithium5_pack_sk(sk, rho, tr, key, &t0, &s1, &s2);

    return 0;
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_crypto_sign_signature
*
* Description: Computes signature.
*
* Arguments:   - uint8 *sig:     pointer to output signature (of length FSMSW_DILITHIUM5_CRYPTO_BYTES)
*              - uint32 *siglen: pointer to output length of signature
*              - uint8 *m:       pointer to message to be signed
*              - uint32 mlen:    length of message
*              - uint8 *sk:      pointer to bit-packed secret key
*
* Returns 0 (success)
***********************************************************************************************************************/
sint8 FsmSw_Dilithium5_crypto_sign_signature(uint8 *sig, uint32 *siglen, const uint8 *m, uint32 mlen, const uint8 *sk)
{
    uint32 n;
    uint8 seedbuf[2u * SEEDBYTES_DILITHIUM + TRBYTES_DILITHIUM + RNDBYTES_DILITHIUM + 2u * CRHBYTES_DILITHIUM];
    uint8 *rho, *tr, *key, *mu, *rhoprime, *rnd;
    uint16 nonce = 0;
    polyvecl_D5 mat[K_DILITHIUM5], s1, y, z;
    polyveck_D5 t0, s2, w1, w0, h;
    poly_D5 cp;
    shake256incctx state;
    boolean loop = TRUE;

    rho = seedbuf;
    tr  = &rho[SEEDBYTES_DILITHIUM];
    key = &tr[TRBYTES_DILITHIUM];
    rnd = &key[SEEDBYTES_DILITHIUM];
    mu  = &rnd[RNDBYTES_DILITHIUM];
    rhoprime = &mu[CRHBYTES_DILITHIUM];
    FsmSw_Dilithium5_unpack_sk(rho, tr, key, &t0, &s1, &s2, sk);

    /* Compute mu = CRH(tr, msg) */
    FsmSw_Fips202_shake256_inc_init(&state);
    FsmSw_Fips202_shake256_inc_absorb(&state, tr, TRBYTES_DILITHIUM);
    FsmSw_Fips202_shake256_inc_absorb(&state, m, mlen);
    FsmSw_Fips202_shake256_inc_finalize(&state);
    FsmSw_Fips202_shake256_inc_squeeze(mu, CRHBYTES_DILITHIUM, &state);

    for (n = 0; n < RNDBYTES_DILITHIUM; n++)
    {
        rnd[n] = 0;
    }
    FsmSw_Fips202_shake256(rhoprime, CRHBYTES_DILITHIUM, key,
                           SEEDBYTES_DILITHIUM + RNDBYTES_DILITHIUM + CRHBYTES_DILITHIUM);

    /* Expand matrix and transform vectors */
    FsmSw_Dilithium5_polyvec_matrix_expand(mat, rho);
    FsmSw_Dilithium5_polyvecl_ntt(&s1);
    FsmSw_Dilithium5_polyveck_ntt(&s2);
    FsmSw_Dilithium5_polyveck_ntt(&t0);

    while(TRUE == loop)
    {
        /* Sample intermediate vector y */
        FsmSw_Dilithium5_polyvecl_uniform_gamma1(&y, rhoprime, nonce++);

        /* Matrix-vector multiplication */
        z = y;
        FsmSw_Dilithium5_polyvecl_ntt(&z);
        FsmSw_Dilithium5_polyvec_matrix_pointwise_montgomery(&w1, mat, &z);
        FsmSw_Dilithium5_polyveck_reduce(&w1);
        FsmSw_Dilithium5_polyveck_invntt_tomont(&w1);

        /* Decompose w and call the random oracle */
        FsmSw_Dilithium5_polyveck_caddq(&w1);
        FsmSw_Dilithium5_polyveck_decompose(&w1, &w0, &w1);
        FsmSw_Dilithium5_polyveck_pack_w1(sig, &w1);

        FsmSw_Fips202_shake256_inc_init(&state);
        FsmSw_Fips202_shake256_inc_absorb(&state, mu, CRHBYTES_DILITHIUM);
        FsmSw_Fips202_shake256_inc_absorb(&state, sig, K_DILITHIUM5 * POLYW1_PACKEDBYTES_DILITHIUM5);
        FsmSw_Fips202_shake256_inc_finalize(&state);
        FsmSw_Fips202_shake256_inc_squeeze(sig, CTILDEBYTES_DILITHIUM5, &state);
        FsmSw_Dilithium5_poly_challenge(&cp, sig); /* uses only the first SEEDBYTES bytes of sig */
        FsmSw_Dilithium5_poly_ntt(&cp);

        /* Compute z, reject if it reveals secret */
        FsmSw_Dilithium5_polyvecl_pointwise_poly_montgomery(&z, &cp, &s1);
        FsmSw_Dilithium5_polyvecl_invntt_tomont(&z);
        FsmSw_Dilithium5_polyvecl_add(&z, &z, &y);
        FsmSw_Dilithium5_polyvecl_reduce(&z);
        if (0 < FsmSw_Dilithium5_polyvecl_chknorm(&z, (sint32)(GAMMA1_DILITHIUM5 - BETA_DILITHIUM5)))
        {
            continue;
        }

        /* Check that subtracting cs2 does not change high bits of w and low bits
         * do not reveal secret information */
        FsmSw_Dilithium5_polyveck_pointwise_poly_montgomery(&h, &cp, &s2);
        FsmSw_Dilithium5_polyveck_invntt_tomont(&h);
        FsmSw_Dilithium5_polyveck_sub(&w0, &w0, &h);
        FsmSw_Dilithium5_polyveck_reduce(&w0);
        if (0 < FsmSw_Dilithium5_polyveck_chknorm(&w0, (sint32)((uint32)((uint32)GAMMA2_DILITHIUM5 - BETA_DILITHIUM5))))
        {
            continue;
        }

        /* Compute hints for w1 */
        FsmSw_Dilithium5_polyveck_pointwise_poly_montgomery(&h, &cp, &t0);
        FsmSw_Dilithium5_polyveck_invntt_tomont(&h);
        FsmSw_Dilithium5_polyveck_reduce(&h);
        if (0 < FsmSw_Dilithium5_polyveck_chknorm(&h, (sint32)GAMMA2_DILITHIUM5))
        {
            continue;
        }

        FsmSw_Dilithium5_polyveck_add(&w0, &w0, &h);
        n = FsmSw_Dilithium5_polyveck_make_hint(&h, &w0, &w1);
        if (n > OMEGA_DILITHIUM5)
        {
            continue;
        }

        loop = FALSE;
    }


    /* Write signature */
    FsmSw_Dilithium5_pack_sig(sig, sig, &z, &h);
    *siglen = FSMSW_DILITHIUM5_CRYPTO_BYTES;
    return 0;
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_crypto_sign
*
* Description: Compute signed message.
*
* Arguments:   -       uint8 *sm:     pointer to output signed message (allocated
*                                     array with FSMSW_DILITHIUM5_CRYPTO_BYTES + mlen bytes), can be equal to m
*              -       uint32 *smlen: pointer to output length of signed message
*              - const uint8 *m:      pointer to message to be signed
*              -       uint32 mlen:   length of message
*              - const uint8 *sk:     pointer to bit-packed secret key
*
* Returns 0 (success)
***********************************************************************************************************************/
sint8 FsmSw_Dilithium5_crypto_sign(uint8 *sm, uint32 *smlen, const uint8 *m, uint32 mlen, const uint8 *sk)
{
    uint32 i;

    for (i = 0; i < mlen; ++i)
    {
        sm[FSMSW_DILITHIUM5_CRYPTO_BYTES + mlen - 1u - i] = m[mlen - 1u - i];
    }
    (void)FsmSw_Dilithium5_crypto_sign_signature(sm, smlen, &sm[FSMSW_DILITHIUM5_CRYPTO_BYTES], mlen, sk);
    *smlen += mlen;
    return 0;
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_crypto_sign_verify
*
* Description: Verifies signature.
*
* Arguments:   -       uint8 *m:      pointer to input signature
*              -       uint32 siglen: length of signature
*              - const uint8 *m:      pointer to message
*              -       uint32 mlen:   length of message
*              - const uint8 *pk:     pointer to bit-packed public key
*
* Returns 0 if signature could be verified correctly and -1 otherwise
***********************************************************************************************************************/
sint8 FsmSw_Dilithium5_crypto_sign_verify(const uint8 *sig, uint32 siglen, const uint8 *m, uint32 mlen, const uint8 *pk)
{
    uint16 i;
    uint8 buf[K_DILITHIUM5 * POLYW1_PACKEDBYTES_DILITHIUM5];
    uint8 rho[SEEDBYTES_DILITHIUM];
    uint8 mu[CRHBYTES_DILITHIUM];
    uint8 c[CTILDEBYTES_DILITHIUM5];
    uint8 c2[CTILDEBYTES_DILITHIUM5];
    poly_D5 cp;
    polyvecl_D5 mat[K_DILITHIUM5], z;
    polyveck_D5 t1, w1, h;
    shake256incctx state;

    if (siglen != FSMSW_DILITHIUM5_CRYPTO_BYTES)
    {
        return -1;
    }

    FsmSw_Dilithium5_unpack_pk(rho, &t1, pk);
    if (0 < FsmSw_Dilithium5_unpack_sig(c, &z, &h, sig))
    {
        return -1;
    }
    if (0 < FsmSw_Dilithium5_polyvecl_chknorm(&z, (sint32)(GAMMA1_DILITHIUM5 - BETA_DILITHIUM5)))
    {
        return -1;
    }

    /* Compute CRH(H(rho, t1), msg) */
    FsmSw_Fips202_shake256(mu, CRHBYTES_DILITHIUM, pk, FSMSW_DILITHIUM5_CRYPTO_PUBLICKEYBYTES);
    FsmSw_Fips202_shake256_inc_init(&state);
    FsmSw_Fips202_shake256_inc_absorb(&state, mu, CRHBYTES_DILITHIUM);
    FsmSw_Fips202_shake256_inc_absorb(&state, m, mlen);
    FsmSw_Fips202_shake256_inc_finalize(&state);
    FsmSw_Fips202_shake256_inc_squeeze(mu, CRHBYTES_DILITHIUM, &state);

    /* Matrix-vector multiplication; compute Az - c2^dt1 */
    FsmSw_Dilithium5_poly_challenge(&cp, c); /* uses only the first SEEDBYTES bytes of c */
    FsmSw_Dilithium5_polyvec_matrix_expand(mat, rho);

    FsmSw_Dilithium5_polyvecl_ntt(&z);
    FsmSw_Dilithium5_polyvec_matrix_pointwise_montgomery(&w1, mat, &z);

    FsmSw_Dilithium5_poly_ntt(&cp);
    FsmSw_Dilithium5_polyveck_shiftl(&t1);
    FsmSw_Dilithium5_polyveck_ntt(&t1);
    FsmSw_Dilithium5_polyveck_pointwise_poly_montgomery(&t1, &cp, &t1);

    FsmSw_Dilithium5_polyveck_sub(&w1, &w1, &t1);
    FsmSw_Dilithium5_polyveck_reduce(&w1);
    FsmSw_Dilithium5_polyveck_invntt_tomont(&w1);

    /* Reconstruct w1 */
    FsmSw_Dilithium5_polyveck_caddq(&w1);
    FsmSw_Dilithium5_polyveck_use_hint(&w1, &w1, &h);
    FsmSw_Dilithium5_polyveck_pack_w1(buf, &w1);

    /* Call random oracle and verify FsmSw_Dilithium5_challenge */
    FsmSw_Fips202_shake256_inc_init(&state);
    FsmSw_Fips202_shake256_inc_absorb(&state, mu, CRHBYTES_DILITHIUM);
    FsmSw_Fips202_shake256_inc_absorb(&state, buf, K_DILITHIUM5 * POLYW1_PACKEDBYTES_DILITHIUM5);
    FsmSw_Fips202_shake256_inc_finalize(&state);
    FsmSw_Fips202_shake256_inc_squeeze(c2, CTILDEBYTES_DILITHIUM5, &state);
    for (i = 0; i < CTILDEBYTES_DILITHIUM5; ++i)
    {
        if (c[i] != c2[i])
        {
            return -1;
        }
    }
    return 0;
}

/***********************************************************************************************************************
* Name:        FsmSw_Dilithium5_crypto_sign_open
*
* Description: Verify signed message.
*
* Arguments:   -       uint8  *m:     pointer to output message (allocated array with smlen bytes), can be equal to sm
*              -       uint32 *mlen:  pointer to output length of message
*              - const uint8  *sm:    pointer to signed message
*              -       uint32  smlen: length of signed message
*              - const uint8  *pk:    pointer to bit-packed public key
*
* Returns 0 if signed message could be verified correctly and -1 otherwise
***********************************************************************************************************************/
sint8 FsmSw_Dilithium5_crypto_sign_open(uint8 *m, uint32 *mlen, const uint8 *sm, uint32 smlen, const uint8 *pk)
{
    uint32 i;
    sint8  retVal = -1;

    if (smlen >= FSMSW_DILITHIUM5_CRYPTO_BYTES)
    {
        *mlen = smlen - FSMSW_DILITHIUM5_CRYPTO_BYTES;
        if (0 == FsmSw_Dilithium5_crypto_sign_verify(sm, FSMSW_DILITHIUM5_CRYPTO_BYTES,
                                                    &sm[FSMSW_DILITHIUM5_CRYPTO_BYTES], *mlen, pk))
        {
            /* All good, copy msg, return 0 */
            for (i = 0; i < *mlen; ++i)
            {
                m[i] = sm[FSMSW_DILITHIUM5_CRYPTO_BYTES + i];
            }
            retVal = 0;
        }
    }

    if((sint8)0u != retVal)
    {
        /* Signature verification failed */
        *mlen = (uint32)-1;
        for (i = 0; i < smlen; ++i)
        {
            m[i] = 0;
        }
    }

    return retVal;
}
