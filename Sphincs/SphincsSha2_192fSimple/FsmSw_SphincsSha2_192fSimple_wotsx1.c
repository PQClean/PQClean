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

/**********************************************************************************************************************/
/* INCLUDES                                                                                                           */
/**********************************************************************************************************************/
#include "FsmSw_CommonLib.h"
#include "FsmSw_SphincsSha2_192fSimple_FctWrapper.h"
#include "FsmSw_SphincsSha2_192fSimple_hash.h"
#include "FsmSw_SphincsSha2_192fSimple_params.h"
#include "FsmSw_SphincsSha2_192fSimple_thash.h"
#include "FsmSw_SphincsSha2_192fSimple_utils.h"
#include "FsmSw_SphincsSha2_192fSimple_wots.h"
#include "FsmSw_Sphincs_sha2_address.h"

#include "FsmSw_SphincsSha2_192fSimple_wotsx1.h"

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
 * Name:        FsmSw_SphincsSha2_192fSimple_Wots_GenLeafX1
 *
 * Description: This generates a WOTS public key. It also generates the WOTS signature if leaf_info indicates that we're
 *              signing with this WOTS key.
 *
 * Arguments:   -       uint8                 *dest:     t.b.d.
 *              - const sphincs_sha2_192f_ctx *ctx:      t.b.d.
 *              -       uint32                 leaf_idx: t.b.d.
 *              -       void                  *v_info:   t.b.d.
 *
 **********************************************************************************************************************/
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity
and avoids confusion with other functions. Therefore, this warning is a false positive." */
void FsmSw_SphincsSha2_192fSimple_Wots_GenLeafX1(uint8 *dest, const sphincs_sha2_192f_ctx *ctx, uint32 leaf_idx,
                                                 void *v_info)
{
  /* polyspace +2 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality.
  Ensured proper alignment and validity." */
  Fsmsw_Sphincssha2_192fSimple_LeafInfoX1_T *info          = v_info;
  uint32 *leaf_addr                                        = info->leaf_addr;
  uint32 *pk_addr                                          = info->pk_addr;
  uint32 i                                                 = 0;
  uint32 k                                                 = 0;
  uint8 pk_buffer[FSMSW_SPHINCSSHA2_192FSIMPLE_WOTS_BYTES] = {0};
  uint8 *buffer                                            = (uint8 *)NULL_PTR;
  uint32 wots_k_mask                                       = 0;

  if (leaf_idx == info->wots_sign_leaf)
  {
    /* We're traversing the leaf that's signing; generate the WOTS signature. */
    wots_k_mask = 0;
  }
  else
  {
    /* Nope, we're just generating pk's; turn off the signature logic */
    wots_k_mask = (uint32)~0u;
  }

  FsmSw_SphincsSha2_192fSimple_set_keypair_addr(leaf_addr, leaf_idx);
  FsmSw_SphincsSha2_192fSimple_set_keypair_addr(pk_addr, leaf_idx);

  buffer = pk_buffer;

  for (i = 0; i < FSMSW_SPHINCSSHA2_192FSIMPLE_WOTS_LEN; i++)
  {
    /* Set wots_k to the step if we're generating a signature, ~0 if we're not */
    uint32 wots_k = info->wots_steps[i] | wots_k_mask;

    /* Start with the secret seed */
    FsmSw_SphincsSha2_SetChainAddr(leaf_addr, i);
    FsmSw_SphincsSha2_SetHashAddr(leaf_addr, 0);
    FsmSw_SphincsSha2_SetType(leaf_addr, FSMSW_SPHINCS_ADDR_TYPE_WOTSPRF);

    FsmSw_SphincsSha2_192fSimple_PrfAddr(buffer, ctx, leaf_addr);

    FsmSw_SphincsSha2_SetType(leaf_addr, FSMSW_SPHINCS_ADDR_TYPE_WOTS);

    /* Iterate down the WOTS chain */
    for (k = 0; k < 0xFFFFFFFFu; k++)
    {
      /* Check if this is the value that needs to be saved as a part of the WOTS signature */
      if (k == wots_k)
      {
        FsmSw_CommonLib_MemCpy(&info->wots_sig[i * FSMSW_SPHINCSSHA2_192FSIMPLE_N], buffer,
                               FSMSW_SPHINCSSHA2_192FSIMPLE_N);
      }

      /* Check if we hit the top of the chain */
      if (k == FSMSW_SPHINCSSHA2_192FSIMPLE_WOTS_W - 1u)
      {
        break;
      }

      /* Iterate one step on the chain */
      FsmSw_SphincsSha2_SetHashAddr(leaf_addr, k);

      FsmSw_SphincsSha2_192fSimple_Thash(buffer, buffer, 1, ctx, leaf_addr);
    }

    buffer = &buffer[FSMSW_SPHINCSSHA2_192FSIMPLE_N];
  }

  /* Do the final thash to generate the public keys */
  FsmSw_SphincsSha2_192fSimple_Thash(dest, pk_buffer, FSMSW_SPHINCSSHA2_192FSIMPLE_WOTS_LEN, ctx, pk_addr);
}
