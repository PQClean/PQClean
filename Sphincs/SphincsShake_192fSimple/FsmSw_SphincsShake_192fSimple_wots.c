/***********************************************************************************************************************
 *
 *                                          IAV GmbH
 *
 **********************************************************************************************************************/
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
// TODO clarify address expectations, and make them more uniform.
// TODO i.e. do we expect types to be set already?
// TODO and do we expect modifications or copies?
/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_CommonLib.h"
#include "FsmSw_SphincsShake_192fSimple_hash.h"
#include "FsmSw_SphincsShake_192fSimple_params.h"
#include "FsmSw_SphincsShake_192fSimple_thash.h"
#include "FsmSw_SphincsShake_192fSimple_utils.h"
#include "FsmSw_SphincsShake_192fSimple_utilsx1.h"
#include "FsmSw_SphincsShake_192fSimple_wotsx1.h"
#include "FsmSw_Sphincs_shake_address.h"
#include "FsmSw_Sphincs_utils.h"

#include "FsmSw_SphincsShake_192fSimple_wots.h"

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
static void fsmsw_sphincsshake_192fsimple_wots_GenChain(uint8 *out, const uint8 *in, uint32 start, uint32 steps,
                                                        const sphincs_shake_192f_ctx *ctx, uint32 addr[8]);
static void fsmsw_sphincsshake_192fsimple_wots_BaseW(uint32 *output, const sint32 out_len, const uint8 *input);
static void fsmsw_sphincsshake_192fsimple_wots_Checksum(uint32 *csum_base_w, const uint32 *msg_base_w);

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/
/***********************************************************************************************************************
 * Name:        fsmsw_sphincsshake_192fsimple_wots_GenChain
 *
 * Description: Computes the chaining function. out and in have to be n-byte arrays.
 *              Interprets in as start-th value of the chain. addr has to contain the address of the chain.
 *
 * Arguments:   -       uint8                  *out:     t.b.d.
 *              - const uint8                  *in:      t.b.d.
 *              -       uint32                  start:   t.b.d.
 *              -       uint32                  steps:   t.b.d.
 *              - const sphincs_shake_192f_ctx *ctx:     t.b.d.
 *              -       uint32                  addr[8]: t.b.d.
 *
 **********************************************************************************************************************/
static void fsmsw_sphincsshake_192fsimple_wots_GenChain(uint8 *out, const uint8 *in, uint32 start, uint32 steps,
                                                        const sphincs_shake_192f_ctx *ctx, uint32 addr[8])
{
  uint32 i = 0;

  /* Initialize out with the value at position 'start'. */
  FsmSw_CommonLib_MemCpy(out, in, FSMSW_SPHINCSSHAKE_192FSIMPLE_N);

  /* Iterate 'steps' calls to the hash function. */
  for (i = start; (i < (start + steps)) && (i < FSMSW_SPHINCSSHAKE_192FSIMPLE_WOTS_W); i++)
  {
    FsmSw_SphincsShake_SetHashAddr(addr, i);
    FsmSw_SphincsShake_192fSimple_Thash(out, out, 1, ctx, addr);
  }
}

/***********************************************************************************************************************
 * Name:        fsmsw_sphincsshake_192fsimple_wots_BaseW
 *
 * Description: base_w algorithm as described in draft. Interprets an array of bytes as integers in base w. This only
 *              works when log_w is a divisor of 8.
 *
 * Arguments:   -       uint32 *output   t.b.d.
 *              -       sint32  out_len: t.b.d.
 *              - const uint8  *input:   t.b.d.
 *
 **********************************************************************************************************************/
static void fsmsw_sphincsshake_192fsimple_wots_BaseW(uint32 *output, const sint32 out_len, const uint8 *input)
{
  sint32 in       = 0;
  sint32 out      = 0;
  uint8 total     = 0;
  sint32 bits     = 0;
  sint32 consumed = 0;

  for (consumed = 0; consumed < out_len; consumed++)
  {
    if (bits == 0)
    {
      total = input[in];
      in++;
      bits += 8;
    }
    bits        = bits - (sint32)FSMSW_SPHINCSSHAKE_192FSIMPLE_WOTS_LOGW;
    output[out] = ((uint32)total >> (uint32)bits) & (FSMSW_SPHINCSSHAKE_192FSIMPLE_WOTS_W - 1u);
    out++;
  }
}

/***********************************************************************************************************************
 * Name:        fsmsw_sphincsshake_192fsimple_wots_Checksum
 *
 * Description: Computes the WOTS+ checksum over a message (in base_w).
 *
 * Arguments:   -       uint32                *csum_base_w: t.b.d.
 *              - const uint32                *msg_base_w:  t.b.d.
 *
 **********************************************************************************************************************/
static void fsmsw_sphincsshake_192fsimple_wots_Checksum(uint32 *csum_base_w, const uint32 *msg_base_w)
{
  uint32 csum                                                                                                       = 0;
  uint8 csum_bytes[((FSMSW_SPHINCSSHAKE_192FSIMPLE_WOTS_LEN2 * FSMSW_SPHINCSSHAKE_192FSIMPLE_WOTS_LOGW) + 7u) / 8u] = {
      0};
  uint32 i = 0;

  /* Compute checksum. */
  for (i = 0; i < FSMSW_SPHINCSSHAKE_192FSIMPLE_WOTS_LEN1; i++)
  {
    csum += FSMSW_SPHINCSSHAKE_192FSIMPLE_WOTS_W - 1u - msg_base_w[i];
  }

  /* Convert checksum to base_w. Make sure expected empty zero bits are the least significant bits. */
  csum =
      csum << ((8u - ((FSMSW_SPHINCSSHAKE_192FSIMPLE_WOTS_LEN2 * FSMSW_SPHINCSSHAKE_192FSIMPLE_WOTS_LOGW) % 8u)) % 8u);
  FsmSw_Sphincs_UllToBytes(csum_bytes, sizeof(csum_bytes), csum);
  fsmsw_sphincsshake_192fsimple_wots_BaseW(csum_base_w, (sint32)FSMSW_SPHINCSSHAKE_192FSIMPLE_WOTS_LEN2, csum_bytes);
}

/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/***********************************************************************************************************************
 * Name:        FsmSw_SphincsShake_192fSimple_Wots_ChainLengths
 *
 * Description: Takes a message and derives the matching chain lengths.
 *
 * Arguments:   -       uint32 *lengths: t.b.d.
 *              - const uint8  *msg:     t.b.d.
 *
 **********************************************************************************************************************/
void FsmSw_SphincsShake_192fSimple_Wots_ChainLengths(uint32 *lengths, const uint8 *msg)
{
  fsmsw_sphincsshake_192fsimple_wots_BaseW(lengths, (sint32)FSMSW_SPHINCSSHAKE_192FSIMPLE_WOTS_LEN1, msg);
  fsmsw_sphincsshake_192fsimple_wots_Checksum(&lengths[FSMSW_SPHINCSSHAKE_192FSIMPLE_WOTS_LEN1], lengths);
}

/***********************************************************************************************************************
 * Name:        FsmSw_SphincsShake_192fSimple_Wots_PkFromSig
 *
 * Description: Takes a WOTS signature and an n-byte message, computes a WOTS public key. Writes the computed public key
 *              to 'pk'.
 *
 * Arguments:   -       uint8                  *pk:      t.b.d.
 *              - const uint8                  *sig:     t.b.d.
 *              - const uint8                  *msg:     t.b.d.
 *              - const sphincs_shake_192f_ctx *ctx:     t.b.d.
 *              -       uint32                  addr[8]: t.b.d.
 *
 **********************************************************************************************************************/
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsShake_192fSimple_Wots_PkFromSig(uint8 *pk, const uint8 *sig, const uint8 *msg,
                                                  const sphincs_shake_192f_ctx *ctx, uint32 addr[8])
{
  uint32 lengths[FSMSW_SPHINCSSHAKE_192FSIMPLE_WOTS_LEN] = {0};
  uint32 i                                               = 0;

  FsmSw_SphincsShake_192fSimple_Wots_ChainLengths(lengths, msg);

  for (i = 0; i < FSMSW_SPHINCSSHAKE_192FSIMPLE_WOTS_LEN; i++)
  {
    FsmSw_SphincsShake_SetChainAddr(addr, i);
    fsmsw_sphincsshake_192fsimple_wots_GenChain(&pk[i * FSMSW_SPHINCSSHAKE_192FSIMPLE_N],
                                                &sig[i * FSMSW_SPHINCSSHAKE_192FSIMPLE_N], lengths[i],
                                                FSMSW_SPHINCSSHAKE_192FSIMPLE_WOTS_W - 1u - lengths[i], ctx, addr);
  }
}
