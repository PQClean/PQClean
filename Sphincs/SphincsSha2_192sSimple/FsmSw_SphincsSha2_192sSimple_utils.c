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
#include "FsmSw_SphincsSha2_192sSimple_hash.h"
#include "FsmSw_SphincsSha2_192sSimple_params.h"
#include "FsmSw_SphincsSha2_192sSimple_thash.h"
#include "FsmSw_Sphincs_sha2_address.h"

#include "FsmSw_SphincsSha2_192sSimple_utils.h"

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
 * Name:        FsmSw_SphincsSha2_192sSimple_ComputeRoot
 *
 * Description: Computes a root node given a leaf and an auth path. Expects address to be complete other than the
 *              tree_height and tree_index.
 *
 * Arguments:   -       uint8                 *root:        t.b.d.
 *              - const uint8                 *leaf:        t.b.d.
 *              -       uint32                 leaf_idx:    t.b.d.
 *              -       uint32                 idx_offset:  t.b.d.
 *              - const uint8                 *auth_path:   t.b.d.
 *              -       uint32                 tree_height: t.b.d.
 *              - const sphincs_sha2_192s_ctx *ctx:         t.b.d.
 *              -       uint32                 addr[8]:     t.b.d.
 *
 **********************************************************************************************************************/
void FsmSw_SphincsSha2_192sSimple_ComputeRoot(uint8 *root, const uint8 *leaf, uint32 leaf_idx, uint32 idx_offset,
                                              const uint8 *auth_path, uint32 tree_height,
                                              const sphincs_sha2_192s_ctx *ctx, uint32 addr[8])
{
  uint32 i                                          = 0;
  uint8 buffer[2u * FSMSW_SPHINCSSHA2_192SSIMPLE_N] = {0};

  /* leaf_idx_temp, idx_offset_temp and auth_path_temp are used to avoid modifying the input. */
  uint32 leaf_idx_temp        = leaf_idx;
  uint32 idx_offset_temp      = idx_offset;
  const uint8 *auth_path_temp = auth_path;

  /* If leaf_idx is odd (last bit = 1), current path element is a right child
     and auth_path has to go left. Otherwise it is the other way around. */
  if (0u < (leaf_idx_temp & 1u))
  {
    FsmSw_CommonLib_MemCpy(&buffer[FSMSW_SPHINCSSHA2_192SSIMPLE_N], leaf, FSMSW_SPHINCSSHA2_192SSIMPLE_N);
    FsmSw_CommonLib_MemCpy(buffer, auth_path_temp, FSMSW_SPHINCSSHA2_192SSIMPLE_N);
  }
  else
  {
    FsmSw_CommonLib_MemCpy(buffer, leaf, FSMSW_SPHINCSSHA2_192SSIMPLE_N);
    FsmSw_CommonLib_MemCpy(&buffer[FSMSW_SPHINCSSHA2_192SSIMPLE_N], auth_path_temp, FSMSW_SPHINCSSHA2_192SSIMPLE_N);
  }
  auth_path_temp = &auth_path_temp[FSMSW_SPHINCSSHA2_192SSIMPLE_N];

  for (i = 0; i < (tree_height - 1u); i++)
  {
    leaf_idx_temp >>= 1;
    idx_offset_temp >>= 1;
    /* Set the address of the node we're creating. */
    FsmSw_SphincsSha2_SetTreeHeight(addr, i + 1u);
    FsmSw_SphincsSha2_SetTreeIndex(addr, leaf_idx_temp + idx_offset_temp);

    /* Pick the right or left neighbor, depending on parity of the node. */
    if (0u < (leaf_idx_temp & 1u))
    {
      FsmSw_SphincsSha2_192sSimple_Thash(&buffer[FSMSW_SPHINCSSHA2_192SSIMPLE_N], buffer, 2, ctx, addr);
      FsmSw_CommonLib_MemCpy(buffer, auth_path_temp, FSMSW_SPHINCSSHA2_192SSIMPLE_N);
    }
    else
    {
      FsmSw_SphincsSha2_192sSimple_Thash(buffer, buffer, 2, ctx, addr);
      FsmSw_CommonLib_MemCpy(&buffer[FSMSW_SPHINCSSHA2_192SSIMPLE_N], auth_path_temp, FSMSW_SPHINCSSHA2_192SSIMPLE_N);
    }
    auth_path_temp = &auth_path_temp[FSMSW_SPHINCSSHA2_192SSIMPLE_N];
  }

  /* The last iteration is exceptional; we do not copy an auth_path node. */
  leaf_idx_temp >>= 1;
  idx_offset_temp >>= 1;
  FsmSw_SphincsSha2_SetTreeHeight(addr, tree_height);
  FsmSw_SphincsSha2_SetTreeIndex(addr, leaf_idx_temp + idx_offset_temp);
  FsmSw_SphincsSha2_192sSimple_Thash(root, buffer, 2, ctx, addr);
}

/***********************************************************************************************************************
 * Name:        FsmSw_SphincsSha2_192sSimple_TreeHash
 *
 * Description: For a given leaf index, computes the authentication path and the resulting root node using Merkle's
 *              TreeHash algorithm. Expects the layer and tree parts of the tree_addr to be set, as well as the tree
 *              type (i.e. FSMSW_SPHINCS_ADDR_TYPE_HASHTREE or FSMSW_SPHINCS_ADDR_TYPE_FORSTREE). Applies the offset
 *              idx_offset to indices before building addresses, so that it is possible to continue counting indices
 *              across trees.
 *
 * Arguments:   -       uint8                 *root:         t.b.d.
 *              -       uint8                 *auth_path:    t.b.d.
 *              - const sphincs_sha2_192s_ctx *ctx:          t.b.d.
 *              -       uint32                 leaf_idx:     t.b.d.
 *              -       uint32                 idx_offset:   t.b.d.
 *              -       uint32                 tree_height:  t.b.d.
 *              -       void                 (*gen_leaf)
 *                                             (
 *                                                     uint8                 *leaf:         t.b.d.
 *                                               const sphincs_sha2_192s_ctx *ctx:          t.b.d.
 *                                                     uint32                 idx:          t.b.d.
 *                                               const uint32                 tree_addr[8]: t.b.d.
 *                                             )
 *              -       uint32                 tree_addr[8]: t.b.d.
 *
 * Note:        This function is currently not used.
 **********************************************************************************************************************/
void FsmSw_SphincsSha2_192sSimple_TreeHash(uint8 *root, uint8 *auth_path, const sphincs_sha2_192s_ctx *ctx,
                                           uint32 leaf_idx, uint32 idx_offset, uint32 tree_height,
                                           void (*gen_leaf)(uint8 *leaf, const sphincs_sha2_192s_ctx *ctx,
                                                            uint32 addr_idx, const uint32 tree_addr[8]),
                                           uint32 tree_addr[8])
{
  uint8 stack[(FSMSW_SPHINCSSHA2_192SSIMPLE_TREEHASH_BUF_LEN + 1u) * FSMSW_SPHINCSSHA2_192SSIMPLE_N];
  uint32 heights[FSMSW_SPHINCSSHA2_192SSIMPLE_TREEHASH_BUF_LEN + 1u];
  uint32 offset = 0;
  uint32 idx;
  uint32 tree_idx;

  for (idx = 0; idx < (uint32)((uint32)1u << tree_height); idx++)
  {
    /* Add the next leaf node to the stack. */
    gen_leaf(&stack[offset * FSMSW_SPHINCSSHA2_192SSIMPLE_N], ctx, idx + idx_offset, tree_addr);
    offset++;
    heights[offset - 1u] = 0;

    /* If this is a node we need for the auth path. */
    if ((leaf_idx ^ 0x1u) == idx)
    {
      FsmSw_CommonLib_MemCpy(auth_path, &stack[(offset - 1u) * FSMSW_SPHINCSSHA2_192SSIMPLE_N],
                             FSMSW_SPHINCSSHA2_192SSIMPLE_N);
    }

    /* While the top-most nodes are of equal height. */
    while ((offset >= 2u) && (heights[offset - 1u] == heights[offset - 2u]))
    {
      /* Compute index of the new node, in the next layer. */
      tree_idx = (idx >> (heights[offset - 1u] + 1u));

      /* Set the address of the node we're creating. */
      FsmSw_SphincsSha2_SetTreeHeight(tree_addr, heights[offset - 1u] + 1u);
      FsmSw_SphincsSha2_SetTreeIndex(tree_addr, tree_idx + (idx_offset >> (heights[offset - 1u] + 1u)));
      /* Hash the top-most nodes from the stack together. */
      FsmSw_SphincsSha2_192sSimple_Thash(&stack[(offset - 2u) * FSMSW_SPHINCSSHA2_192SSIMPLE_N],
                                         &stack[(offset - 2u) * FSMSW_SPHINCSSHA2_192SSIMPLE_N], 2u, ctx, tree_addr);
      offset--;
      /* Note that the top-most node is now one layer higher. */
      heights[offset - 1u]++;

      /* If this is a node we need for the auth path.. */
      if (((leaf_idx >> heights[offset - 1u]) ^ 0x1u) == tree_idx)
      {
        FsmSw_CommonLib_MemCpy(&auth_path[heights[offset - 1u] * FSMSW_SPHINCSSHA2_192SSIMPLE_N],
                               &stack[(offset - 1u) * FSMSW_SPHINCSSHA2_192SSIMPLE_N], FSMSW_SPHINCSSHA2_192SSIMPLE_N);
      }
    }
  }
  FsmSw_CommonLib_MemCpy(root, stack, FSMSW_SPHINCSSHA2_192SSIMPLE_N);
}
