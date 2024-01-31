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

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_sha2.h"
#include "FsmSw_CommonLib.h"
#include "FsmSw_Sphincs_sha2_address.h"
#include "FsmSw_Sphincs_utils.h"
#include "FsmSw_SphincsSha2_128sSimple_hash.h"
#include "FsmSw_SphincsSha2_128sSimple_params.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/
#define SPX_SHAX_OUTPUT_BYTES  FSMSW_SPHINCS_SHA256_OUTPUT_BYTES
#define SPX_SHAX_BLOCK_BYTES   FSMSW_SPHINCS_SHA256_BLOCK_BYTES
#define shaX_inc_init          FsmSw_sha256_inc_init
#define shaX_inc_blocks        FsmSw_sha256_inc_blocks
#define shaX_inc_finalize      FsmSw_sha256_inc_finalize
#define shaX                   FsmSw_sha256
#define mgf1_X                 FsmSw_SphincsSha2_128sSimple_mgf1_256
#define shaXstate              sha256ctx

#define SPX_TREE_BITS     (FSMSW_SPHINCSSHA2_128SSIMPLE_TREE_HEIGHT * (FSMSW_SPHINCSSHA2_128SSIMPLE_D - 1u))
#define SPX_TREE_BYTES   ((SPX_TREE_BITS + 7u) / 8u)
#define SPX_LEAF_BITS      FSMSW_SPHINCSSHA2_128SSIMPLE_TREE_HEIGHT
#define SPX_LEAF_BYTES   ((SPX_LEAF_BITS + 7u) / 8u)
#define SPX_DGST_BYTES    (FSMSW_SPHINCSSHA2_128SSIMPLE_FORS_MSG_BYTES + SPX_TREE_BYTES + SPX_LEAF_BYTES)


    /* Round to nearest multiple of SPX_SHAX_BLOCK_BYTES */
#define SPX_INBLOCKS            (uint32)((uint32)((uint32)(((FSMSW_SPHINCSSHA2_128SSIMPLE_N + \
                                                             FSMSW_SPHINCSSHA2_128SSIMPLE_PK_BYTES +\
                                                             SPX_SHAX_BLOCK_BYTES - 1u)& \
                                 (uint32)((sint32)((-1) * (sint32)SPX_SHAX_BLOCK_BYTES)))) / \
                                                                  SPX_SHAX_BLOCK_BYTES))
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
* Name:        FsmSw_SphincsSha2_128sSimple_mgf1_256
*
* Description: mgf1 function based on the SHA-256 hash function. Note that inlen should be sufficiently small that it
*              still allows for an array to be allocated on the stack. Typically 'in' is merely a seed.
*              Outputs outlen number of bytes
*
* Arguments:   -       uint8  *out:    t.b.d.
*              -       uint32  outlen: t.b.d.
*              - const uint8  *in:     t.b.d.
*              -       uint32  inlen:  t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsSha2_128sSimple_mgf1_256(uint8 *out, uint32 outlen, const uint8 *in, uint32 inlen)
{
    uint8 inbuf[FSMSW_SPHINCSSHA2_128SSIMPLE_MGF1_256_BUF_LEN + 4u];
    uint8 outbuf[FSMSW_SPHINCS_SHA256_OUTPUT_BYTES];
    uint32 i;

    FsmSw_CommonLib_memcpy(inbuf, in, inlen);

    /* While we can fit in at least another full block of SHA256 output.. */
    for (i = 0; (i + 1u)*FSMSW_SPHINCS_SHA256_OUTPUT_BYTES <= outlen; i++)
    {
        FsmSw_Sphincs_u32_to_bytes(&inbuf[inlen], i);
        FsmSw_sha256(out, inbuf, inlen + 4u);
        out = &out[FSMSW_SPHINCS_SHA256_OUTPUT_BYTES];
    }
    /* Until we cannot anymore, and we fill the remainder. */
    if (outlen > i * FSMSW_SPHINCS_SHA256_OUTPUT_BYTES)
    {
        FsmSw_Sphincs_u32_to_bytes(&inbuf[inlen], i);
        FsmSw_sha256(outbuf, inbuf, inlen + 4u);
        FsmSw_CommonLib_memcpy(out, outbuf, outlen - i * FSMSW_SPHINCS_SHA256_OUTPUT_BYTES);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_128sSimple_mgf1_512
*
* Description: mgf1 function based on the SHA-512 hash function
*
* Arguments:   -       uint8  *out:    t.b.d.
*              -       uint32  outlen: t.b.d.
*              - const uint8  *in:     t.b.d.
*              -       uint32  inlen:  t.b.d.
*
* Note:        This function is currently not used.
***********************************************************************************************************************/
void FsmSw_SphincsSha2_128sSimple_mgf1_512(uint8 *out, uint32 outlen, const uint8 *in, uint32 inlen)
{
    uint8 inbuf[FSMSW_SPHINCSSHA2_128SSIMPLE_MGF1_512_BUF_LEN + 4u];
    uint8 outbuf[FSMSW_SPHINCS_SHA512_OUTPUT_BYTES];
    uint32 i;

    FsmSw_CommonLib_memcpy(inbuf, in, inlen);

    /* While we can fit in at least another full block of SHA512 output.. */
    for (i = 0; (i + 1u)*FSMSW_SPHINCS_SHA512_OUTPUT_BYTES <= outlen; i++)
    {
        FsmSw_Sphincs_u32_to_bytes(&inbuf[inlen], i);
        FsmSw_sha512(out, inbuf, inlen + 4u);
        out = &out[FSMSW_SPHINCS_SHA512_OUTPUT_BYTES];
    }
    /* Until we cannot anymore, and we fill the remainder. */
    if (outlen > i * FSMSW_SPHINCS_SHA512_OUTPUT_BYTES)
    {
        FsmSw_Sphincs_u32_to_bytes(&inbuf[inlen], i);
        FsmSw_sha512(outbuf, inbuf, inlen + 4u);
        FsmSw_CommonLib_memcpy(out, outbuf, outlen - i * FSMSW_SPHINCS_SHA512_OUTPUT_BYTES);
    }
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_128sSimple_prf_addr
*
* Description:  Computes PRF(pk_seed, sk_seed, addr).
*
* Arguments:   -       uint8                 *out:     t.b.d.
*              - const sphincs_sha2_128s_ctx *ctx:     t.b.d.
*              - const uint32                 addr[8]: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsSha2_128sSimple_prf_addr(uint8 *out, const sphincs_sha2_128s_ctx *ctx, const uint32 addr[8])
{
    sha256ctx sha2_state;
    uint8 buf[SPX_SHA256_ADDR_BYTES + FSMSW_SPHINCSSHA2_128SSIMPLE_N];
    uint8 outbuf[FSMSW_SPHINCS_SHA256_OUTPUT_BYTES];

    /* Retrieve precomputed state containing pub_seed */
    FsmSw_sha256_inc_ctx_clone(&sha2_state, &ctx->state_seeded);

    /* Remainder: ADDR^c ‖ SK.seed */
    FsmSw_CommonLib_memcpy(buf, addr, SPX_SHA256_ADDR_BYTES);
    FsmSw_CommonLib_memcpy(&buf[SPX_SHA256_ADDR_BYTES], ctx->sk_seed, FSMSW_SPHINCSSHA2_128SSIMPLE_N);

    FsmSw_sha256_inc_finalize(outbuf, &sha2_state, buf, SPX_SHA256_ADDR_BYTES + FSMSW_SPHINCSSHA2_128SSIMPLE_N);

    FsmSw_CommonLib_memcpy(out, outbuf, FSMSW_SPHINCSSHA2_128SSIMPLE_N);
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_128sSimple_gen_message_random
*
* Description: Computes the message-dependent randomness R, using a secret seed as a key for HMAC, and an optional
*              randomization value prefixed to the message. This requires m to have at least SPX_SHAX_BLOCK_BYTES +
*              FSMSW_SPHINCSSHA2_128SSIMPLE_N space available in front of the pointer, i.e. before the message to use
*              for the prefix. This is necessary to prevent having to move the message around (and allocate memory
*              for it).
*
* Arguments:   -       uint8                 *R:       t.b.d.
*              - const uint8                 *sk_prf:  t.b.d.
*              - const uint8                 *optrand: t.b.d.
*              - const uint8                 *m:       t.b.d.
*              -       uint32                 mlen:    t.b.d.
*              - const sphincs_sha2_128s_ctx *ctx:     t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsSha2_128sSimple_gen_message_random(uint8 *R, const uint8 *sk_prf, const uint8 *optrand,
                                                     const uint8 *m, uint32 mlen, const sphincs_sha2_128s_ctx *ctx)
{
    (void)ctx;

    uint8 buf[SPX_SHAX_BLOCK_BYTES + SPX_SHAX_OUTPUT_BYTES];
    shaXstate state;
    uint8 i;

    /* This implements HMAC-SHA */
    for (i = 0; i < FSMSW_SPHINCSSHA2_128SSIMPLE_N; i++)
    {
        buf[i] = 0x36u ^ sk_prf[i];
    }
    FsmSw_CommonLib_memset(&buf[FSMSW_SPHINCSSHA2_128SSIMPLE_N], 0x36,
                           SPX_SHAX_BLOCK_BYTES - FSMSW_SPHINCSSHA2_128SSIMPLE_N);

    shaX_inc_init(&state);
    shaX_inc_blocks(&state, buf, 1);

    FsmSw_CommonLib_memcpy(buf, optrand, FSMSW_SPHINCSSHA2_128SSIMPLE_N);

    /* If optrand + message cannot fill up an entire block */
    if (FSMSW_SPHINCSSHA2_128SSIMPLE_N + mlen < SPX_SHAX_BLOCK_BYTES)
    {
        FsmSw_CommonLib_memcpy(&buf[FSMSW_SPHINCSSHA2_128SSIMPLE_N], m, mlen);
        shaX_inc_finalize(&buf[SPX_SHAX_BLOCK_BYTES], &state,
                          buf, mlen + FSMSW_SPHINCSSHA2_128SSIMPLE_N);
    }
    /* Otherwise first fill a block, so that finalize only uses the message */
    else
    {
        FsmSw_CommonLib_memcpy(&buf[FSMSW_SPHINCSSHA2_128SSIMPLE_N], m,
                               SPX_SHAX_BLOCK_BYTES - FSMSW_SPHINCSSHA2_128SSIMPLE_N);
        shaX_inc_blocks(&state, buf, 1);

        m = &m[SPX_SHAX_BLOCK_BYTES - FSMSW_SPHINCSSHA2_128SSIMPLE_N];
        mlen -= SPX_SHAX_BLOCK_BYTES - FSMSW_SPHINCSSHA2_128SSIMPLE_N;
        shaX_inc_finalize(&buf[SPX_SHAX_BLOCK_BYTES], &state, m, mlen);
    }

    for (i = 0; i < FSMSW_SPHINCSSHA2_128SSIMPLE_N; i++)
    {
        buf[i] = 0x5cu ^ sk_prf[i];
    }
    FsmSw_CommonLib_memset(&buf[FSMSW_SPHINCSSHA2_128SSIMPLE_N], 0x5c,
                           SPX_SHAX_BLOCK_BYTES - FSMSW_SPHINCSSHA2_128SSIMPLE_N);

    shaX(buf, buf, SPX_SHAX_BLOCK_BYTES + SPX_SHAX_OUTPUT_BYTES);
    FsmSw_CommonLib_memcpy(R, buf, FSMSW_SPHINCSSHA2_128SSIMPLE_N);
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_128sSimple_hash_message
*
* Description: Computes the message hash using R, the public key, and the message. Outputs the message digest and the
*              index of the leaf. The index is split in the tree index and the leaf index, for convenient copying to
*              an address.
*
* Arguments:   -       uint8                 *digest:   t.b.d.
*              -       uint64                *tree:     t.b.d.
*              -       uint32                *leaf_idx: t.b.d.
*              - const uint8                 *R:        t.b.d.
*              - const uint8                 *pk:       t.b.d.
*              - const uint8                 *m:        t.b.d.
*              -       uint32                 mlen:     t.b.d.
*              - const sphincs_sha2_128s_ctx *ctx:      t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsSha2_128sSimple_hash_message(uint8 *digest, uint64 *tree, uint32 *leaf_idx, const uint8 *R,
                                               const uint8 *pk, const uint8 *m, uint32 mlen,
                                               const sphincs_sha2_128s_ctx *ctx)
{
    (void)ctx;
    uint8 seed[2u * FSMSW_SPHINCSSHA2_128SSIMPLE_N + SPX_SHAX_OUTPUT_BYTES];

    uint8 inbuf[SPX_INBLOCKS * SPX_SHAX_BLOCK_BYTES];

    uint8 buf[SPX_DGST_BYTES];
    uint8 *bufp = buf;
    shaXstate state;

    shaX_inc_init(&state);

    /* seed: SHA-X(R ‖ PK.seed ‖ PK.root ‖ M) */
    FsmSw_CommonLib_memcpy(inbuf, R, FSMSW_SPHINCSSHA2_128SSIMPLE_N);
    FsmSw_CommonLib_memcpy(&inbuf[FSMSW_SPHINCSSHA2_128SSIMPLE_N], pk, FSMSW_SPHINCSSHA2_128SSIMPLE_PK_BYTES);

    /* If R + pk + message cannot fill up an entire block */
    if (FSMSW_SPHINCSSHA2_128SSIMPLE_N + FSMSW_SPHINCSSHA2_128SSIMPLE_PK_BYTES +
                                         mlen < SPX_INBLOCKS * SPX_SHAX_BLOCK_BYTES)
    {
        FsmSw_CommonLib_memcpy(&inbuf[FSMSW_SPHINCSSHA2_128SSIMPLE_N + FSMSW_SPHINCSSHA2_128SSIMPLE_PK_BYTES], m, mlen);
        shaX_inc_finalize(&seed[2u * FSMSW_SPHINCSSHA2_128SSIMPLE_N], &state, inbuf,
                           FSMSW_SPHINCSSHA2_128SSIMPLE_N + FSMSW_SPHINCSSHA2_128SSIMPLE_PK_BYTES + mlen);
    }
    /* Otherwise first fill a block, so that finalize only uses the message */
    else
    {
        FsmSw_CommonLib_memcpy(&inbuf[FSMSW_SPHINCSSHA2_128SSIMPLE_N + FSMSW_SPHINCSSHA2_128SSIMPLE_PK_BYTES], m,
                                SPX_INBLOCKS * SPX_SHAX_BLOCK_BYTES - FSMSW_SPHINCSSHA2_128SSIMPLE_N -
                                FSMSW_SPHINCSSHA2_128SSIMPLE_PK_BYTES);
        shaX_inc_blocks(&state, inbuf, SPX_INBLOCKS);

        m = &m[SPX_INBLOCKS * SPX_SHAX_BLOCK_BYTES - FSMSW_SPHINCSSHA2_128SSIMPLE_N -
                                                     FSMSW_SPHINCSSHA2_128SSIMPLE_PK_BYTES];
        mlen -= SPX_INBLOCKS * SPX_SHAX_BLOCK_BYTES - FSMSW_SPHINCSSHA2_128SSIMPLE_N -
                                                      FSMSW_SPHINCSSHA2_128SSIMPLE_PK_BYTES;
        shaX_inc_finalize(&seed[2u * FSMSW_SPHINCSSHA2_128SSIMPLE_N], &state, m, (uint32)mlen);
    }

    /* H_msg: MGF1-SHA-X(R ‖ PK.seed ‖ seed) */
    FsmSw_CommonLib_memcpy(seed, R, FSMSW_SPHINCSSHA2_128SSIMPLE_N);
    FsmSw_CommonLib_memcpy(&seed[FSMSW_SPHINCSSHA2_128SSIMPLE_N], pk, FSMSW_SPHINCSSHA2_128SSIMPLE_N);

    /* By doing this in two steps, we prevent hashing the message twice;
       otherwise each iteration in MGF1 would hash the message again. */
    mgf1_X(bufp, SPX_DGST_BYTES, seed, 2u * FSMSW_SPHINCSSHA2_128SSIMPLE_N + SPX_SHAX_OUTPUT_BYTES);

    FsmSw_CommonLib_memcpy(digest, bufp, FSMSW_SPHINCSSHA2_128SSIMPLE_FORS_MSG_BYTES);
    bufp = &bufp[FSMSW_SPHINCSSHA2_128SSIMPLE_FORS_MSG_BYTES];

    *tree = FsmSw_Sphincs_bytes_to_ull(bufp, SPX_TREE_BYTES);
    *tree &= (~(uint64)0) >> (64u - SPX_TREE_BITS);
    bufp = &bufp[SPX_TREE_BYTES];

    *leaf_idx = (uint32)FsmSw_Sphincs_bytes_to_ull(bufp, SPX_LEAF_BYTES);
    *leaf_idx &= (~(uint32)0) >> (32u - SPX_LEAF_BITS);
}
