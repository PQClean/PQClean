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
// TODO clarify address expectations, and make them more uniform.
// TODO i.e. do we expect types to be set already?
// TODO and do we expect modifications or copies?
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_CommonLib.h"
#include "FsmSw_Sphincs_sha2_address.h"
#include "FsmSw_Sphincs_utils.h"
#include "FsmSw_SphincsSha2_192sSimple_hash.h"
#include "FsmSw_SphincsSha2_192sSimple_params.h"
#include "FsmSw_SphincsSha2_192sSimple_thash.h"
#include "FsmSw_SphincsSha2_192sSimple_utils.h"
#include "FsmSw_SphincsSha2_192sSimple_utilsx1.h"
#include "FsmSw_SphincsSha2_192sSimple_wotsx1.h"
#include "FsmSw_SphincsSha2_192sSimple_wots.h"

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
static void fsmsw_sphincssha2_192ssimple_GenChain(uint8 *out, const uint8 *in, uint32 start, uint32 steps, 
                                                   const sphincs_sha2_192s_ctx *ctx, uint32 addr[8]);
static void fsmsw_sphincssha2_192ssimple_BaseW(uint32 *output, sint32 out_len, const uint8 *input);
static void fsmsw_sphincssha2_192ssimple_WotsChecksum(uint32 *csum_base_w, const uint32 *msg_base_w);

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        fsmsw_sphincssha2_192ssimple_GenChain
*
* Description: Computes the chaining function. out and in have to be n-byte arrays. Interprets in as start-th value of
*              the chain. addr has to contain the address of the chain.
*
* Arguments:   -       uint8                 *out:     t.b.d.
*              - const uint8                 *in:      t.b.d.
*              -       uint32                 start:   t.b.d.
*              -       uint32                 steps:   t.b.d.
*              - const sphincs_sha2_192s_ctx *ctx:     t.b.d.
*              -       uint32                 addr[8]: t.b.d.
*
***********************************************************************************************************************/
static void fsmsw_sphincssha2_192ssimple_GenChain(uint8 *out, const uint8 *in, uint32 start, uint32 steps, 
                                                   const sphincs_sha2_192s_ctx *ctx, uint32 addr[8])
{
    uint32 i;

    /* Initialize out with the value at position 'start'. */
    FsmSw_CommonLib_memcpy(out, in, FSMSW_SPHINCSSHA2_192SSIMPLE_N);

    /* Iterate 'steps' calls to the hash function. */
    for (i = start; (i < (start + steps)) && (i < FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_W); i++)
    {
        FsmSw_SphincsSha2_set_hash_addr(addr, i);
        FsmSw_SphincsSha2_192sSimple_thash(out, out, 1, ctx, addr);
    }
}

/***********************************************************************************************************************
* Name:        fsmsw_sphincssha2_192ssimple_BaseW
*
* Description: base_w algorithm as described in draft. Interprets an array of bytes as integers in base w. This only
*              works when log_w is a divisor of 8.
*
* Arguments:   -       uint32 *output:  t.b.d.
*              -       sint32 *out_len: t.b.d.
*              - const uint8  *input:   t.b.d.
*
***********************************************************************************************************************/
static void fsmsw_sphincssha2_192ssimple_BaseW(uint32 *output, sint32 out_len, const uint8 *input)
{
    sint32 in = 0;
    sint32 out = 0;
    uint8  total = 0;
    sint32 bits = 0;
    sint32 consumed;

    for (consumed = 0; consumed < out_len; consumed++)
    {
        if (bits == 0)
        {
            total = input[in];
            in++;
            bits += 8;
        }
        bits = bits - (sint32)FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_LOGW;
        output[out] = ((uint32)total >> (uint32)bits) & (FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_W - 1u);
        out++;
    }
}

/***********************************************************************************************************************
* Name:        fsmsw_sphincssha2_192ssimple_WotsChecksum
*
* Description: Computes the WOTS+ checksum over a message (in base_w).
*
* Arguments:   -       uint32 *csum_base_w: t.b.d.
*              - const uint32 *msg_base_w:  t.b.d.
*
***********************************************************************************************************************/
static void fsmsw_sphincssha2_192ssimple_WotsChecksum(uint32 *csum_base_w, const uint32 *msg_base_w)
{
    uint32 csum = 0;
    uint8 csum_bytes[((FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_LEN2 * FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_LOGW) + 7u) / 8u];
    uint32 i;

    /* Compute checksum. */
    for (i = 0; i < FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_LEN1; i++)
    {
        csum += FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_W - 1u - msg_base_w[i];
    }

    /* Convert checksum to base_w. Make sure expected empty zero bits are the least significant bits. */
    csum = csum << ((8u - ((FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_LEN2 * FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_LOGW) % 8u))
                     % 8u);
    FsmSw_Sphincs_ull_to_bytes(csum_bytes, sizeof(csum_bytes), csum);
    fsmsw_sphincssha2_192ssimple_BaseW(csum_base_w, (sint32)FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_LEN2, csum_bytes);
}

/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_192sSimple_chain_lengths
*
* Description: Takes a message and derives the matching chain lengths.
*
* Arguments:   -       uint8 *lengths: t.b.d.
*              - const uint8 *msg:     t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsSha2_192sSimple_chain_lengths(uint32 *lengths, const uint8 *msg)
{
    fsmsw_sphincssha2_192ssimple_BaseW(lengths, (sint32)FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_LEN1, msg);
    fsmsw_sphincssha2_192ssimple_WotsChecksum(&lengths[FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_LEN1], lengths);
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsSha2_192sSimple_wots_pk_from_sig
*
* Description: Takes a WOTS signature and an n-byte message, computes a WOTS public key.
*              Writes the computed public key to 'pk'.
*
* Arguments:   -       uint8                 *pk:        t.b.d.
*              - const uint8                 *sig:        t.b.d.
*              - const uint8                 *msg:        t.b.d.
*              - const sphincs_sha2_192s_ctx *ctx:         t.b.d.
*              -       uint32                 addr[8]:     t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsSha2_192sSimple_wots_pk_from_sig(uint8 *pk, const uint8 *sig, const uint8 *msg,
                                                   const sphincs_sha2_192s_ctx *ctx, uint32 addr[8])
{
    uint32 lengths[FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_LEN];
    uint32 i;

    FsmSw_SphincsSha2_192sSimple_chain_lengths(lengths, msg);

    for (i = 0; i < FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_LEN; i++)
    {
        FsmSw_SphincsSha2_set_chain_addr(addr, i);
        fsmsw_sphincssha2_192ssimple_GenChain(&pk[i * FSMSW_SPHINCSSHA2_192SSIMPLE_N], 
                                               &sig[i * FSMSW_SPHINCSSHA2_192SSIMPLE_N],
                                               lengths[i], FSMSW_SPHINCSSHA2_192SSIMPLE_WOTS_W - 1u - lengths[i], 
                                               ctx, addr);
    }
}
