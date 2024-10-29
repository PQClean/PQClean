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
#include "FsmSw_Sphincs_shake_address.h"
#include "FsmSw_SphincsShake_128sSimple_FctWrapper.h"
#include "FsmSw_SphincsShake_128sSimple_hash.h"
#include "FsmSw_SphincsShake_128sSimple_thash.h"
#include "FsmSw_SphincsShake_128sSimple_utils.h"
#include "FsmSw_SphincsShake_128sSimple_utilsx1.h"
#include "FsmSw_SphincsShake_128sSimple_fors.h"

/**********************************************************************************************************************/
/* DEFINES                                                                                                            */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* TYPES                                                                                                              */
/**********************************************************************************************************************/
struct fors_gen_leaf_info
{
    uint32 leaf_addrx[8];
};

/**********************************************************************************************************************/
/* GLOBAL VARIABLES                                                                                                   */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* MACROS                                                                                                             */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES                                                                                        */
/**********************************************************************************************************************/
static void fors_gen_sk(uint8 *sk, const sphincs_shake_128s_ctx *ctx, const uint32 fors_leaf_addr[8]);
static void fors_sk_to_leaf(uint8 *leaf, const uint8 *sk, const sphincs_shake_128s_ctx *ctx, 
                            const uint32 fors_leaf_addr[8]);
static void fors_gen_leafx1(uint8 *leaf, const sphincs_shake_128s_ctx *ctx, uint32 addr_idx, void *info);
static void message_to_indices(uint32 *indices, const uint8 *m);

/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        fors_gen_sk
*
* Description: t.b.d.
*
* Arguments:   -       uint8                  *sk:                t.b.d.
*              - const sphincs_shake_128s_ctx *ctx:               t.b.d.
*              - const uint32                  fors_leaf_addr[8]: t.b.d.
*
***********************************************************************************************************************/
static void fors_gen_sk(uint8 *sk, const sphincs_shake_128s_ctx *ctx, const uint32 fors_leaf_addr[8])
{
    FsmSw_SphincsShake_128sSimple_prf_addr(sk, ctx, fors_leaf_addr);
}

/***********************************************************************************************************************
* Name:        fors_sk_to_leaf
*
* Description: t.b.d.
*
* Arguments:   -       uint8                  *leaf:              t.b.d.
*              - const uint8                  *sk:                t.b.d.
*              - const sphincs_shake_128s_ctx *ctx:               t.b.d.
*              - const uint32                  fors_leaf_addr[8]: t.b.d.
*
***********************************************************************************************************************/
static void fors_sk_to_leaf(uint8 *leaf, const uint8 *sk, const sphincs_shake_128s_ctx *ctx, 
                            const uint32 fors_leaf_addr[8])
{
    FsmSw_SphincsShake_128sSimple_thash(leaf, sk, 1, ctx, fors_leaf_addr);
}

/***********************************************************************************************************************
* Name:        fors_gen_leafx1
*
* Description: t.b.d.
*
* Arguments:   -       uint8                  *leaf:     t.b.d.
*              - const sphincs_shake_128s_ctx *ctx:      t.b.d.
*              -       uint32                  addr_idx: t.b.d.
*              -       void                   *info:     t.b.d.
*
***********************************************************************************************************************/
static void fors_gen_leafx1(uint8 *leaf, const sphincs_shake_128s_ctx *ctx, uint32 addr_idx, void *info)
{
    /* polyspace +2 MISRA2012:11.5 [Justified:]"Necessary conversion from void* to object* for functionality. 
    Ensured proper alignment and validity." */
    struct fors_gen_leaf_info *fors_info = info;
    uint32 *fors_leaf_addr = fors_info->leaf_addrx;

    /* Only set the parts that the caller doesn't set */
    FsmSw_SphincsShake_set_tree_index(fors_leaf_addr, addr_idx);
    FsmSw_SphincsShake_set_type(fors_leaf_addr, FSMSW_SPHINCS_ADDR_TYPE_FORSPRF);
    fors_gen_sk(leaf, ctx, fors_leaf_addr);

    FsmSw_SphincsShake_set_type(fors_leaf_addr, FSMSW_SPHINCS_ADDR_TYPE_FORSTREE);
    fors_sk_to_leaf(leaf, leaf, ctx, fors_leaf_addr);
}

/***********************************************************************************************************************
* Name:        message_to_indices
*
* Description: Interprets m as FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_HEIGHT-bit uint32 Assumes m contains at least
*              FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_HEIGHT * FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_TREES bits. Assumes
*              indices has space for FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_TREES integers.
*
* Arguments:   -       uint32 *indices: t.b.d.
*              - const uint8  *m:       t.b.d.
*
***********************************************************************************************************************/
static void message_to_indices(uint32 *indices, const uint8 *m)
{
    uint32 i, j;
    uint32 offset = 0;

    for (i = 0; i < FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_TREES; i++)
    {
        indices[i] = 0;
        for (j = 0; j < FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_HEIGHT; j++)
        {
            indices[i] ^= (uint32)(((((uint32)(m[offset >> 3]) >> (offset & 0x7u)) & 0x1u)) << j);
            offset++;
        }
    }
}

/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/***********************************************************************************************************************
* Name:        FsmSw_SphincsShake_128sSimple_fors_sign
*
* Description: Signs a message m, deriving the secret key from sk_seed and the FTS address. Assumes m contains at least
*              FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_HEIGHT * FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_TREES bits.
*
* Arguments:   -       uint8                  *sig:          t.b.d.
*              -       uint8                  *pk:           t.b.d.
*              - const uint8                  *m:            t.b.d.
*              - const sphincs_shake_128s_ctx *ctx:          t.b.d.
*              - const uint32                  fors_addr[8]: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsShake_128sSimple_fors_sign(uint8 *sig, uint8 *pk, const uint8 *m, const sphincs_shake_128s_ctx *ctx,
                                            const uint32 fors_addr[8])
{
    uint32 indices[FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_TREES];
    uint8 roots[FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_TREES * FSMSW_SPHINCSSHAKE_128SSIMPLE_N];
    uint32 fors_tree_addr[8] = {0};
    struct fors_gen_leaf_info fors_info = {{0}};
    uint32 *fors_leaf_addr = fors_info.leaf_addrx;
    uint32 fors_pk_addr[8] = {0};
    uint32 idx_offset;
    uint32 i;

    /* sig_temp is used to avoid modifying the input. */
    uint8 *sig_temp = sig;

    FsmSw_SphincsShake_128sSimple_copy_keypair_addr(fors_tree_addr, fors_addr);
    FsmSw_SphincsShake_128sSimple_copy_keypair_addr(fors_leaf_addr, fors_addr);

    FsmSw_SphincsShake_128sSimple_copy_keypair_addr(fors_pk_addr, fors_addr);
    FsmSw_SphincsShake_set_type(fors_pk_addr, FSMSW_SPHINCS_ADDR_TYPE_FORSPK);

    message_to_indices(indices, m);

    for (i = 0; i < FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_TREES; i++)
    {
        idx_offset = i * (uint32)((uint64)((uint64)1u << FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_HEIGHT));

        FsmSw_SphincsShake_set_tree_height(fors_tree_addr, 0);
        FsmSw_SphincsShake_set_tree_index(fors_tree_addr, indices[i] + idx_offset);
        FsmSw_SphincsShake_set_type(fors_tree_addr, FSMSW_SPHINCS_ADDR_TYPE_FORSPRF);

        /* Include the secret key part that produces the selected leaf node. */
        fors_gen_sk(sig_temp, ctx, fors_tree_addr);
        FsmSw_SphincsShake_set_type(fors_tree_addr, FSMSW_SPHINCS_ADDR_TYPE_FORSTREE);
        sig_temp = &sig_temp[FSMSW_SPHINCSSHAKE_128SSIMPLE_N];

        /* Compute the authentication path for this leaf node. */
        FsmSw_SphincsShake_128sSimple_treehashx1(&roots[i * FSMSW_SPHINCSSHAKE_128SSIMPLE_N], sig_temp, ctx,
                                                 indices[i], idx_offset, FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_HEIGHT,
                                                 fors_gen_leafx1, fors_tree_addr, &fors_info);

        sig_temp = &sig_temp[FSMSW_SPHINCSSHAKE_128SSIMPLE_N * FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_HEIGHT];
    }

    /* Hash horizontally across all tree roots to derive the public key. */
    FsmSw_SphincsShake_128sSimple_thash(pk, roots, FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_TREES, ctx, fors_pk_addr);
}

/***********************************************************************************************************************
* Name:        FsmSw_SphincsShake_128sSimple_fors_pk_from_sig
*
* Description: Derives the FORS public key from a signature. This can be used for verification by comparing to a known
*              public key, or to subsequently verify a signature on the derived public key. The latter is the typical
*              use-case when used as an FTS below an OTS in a hypertree. Assumes m contains at least
*              FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_HEIGHT * FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_TREES bits.
*
* Arguments:   -       uint8                 *pk:           t.b.d.
*              - const uint8                 *sig:          t.b.d.
*              - const uint8                 *m:            t.b.d.
*              - const sphincs_sha2_128s_ctx *ctx:          t.b.d.
*              - const uint32                 fors_addr[8]: t.b.d.
*
***********************************************************************************************************************/
void FsmSw_SphincsShake_128sSimple_fors_pk_from_sig(uint8 *pk, const uint8 *sig, const uint8 *m,
                                                    const sphincs_shake_128s_ctx *ctx, const uint32 fors_addr[8])
{
    uint32 indices[FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_TREES];
    uint8 roots[FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_TREES * FSMSW_SPHINCSSHAKE_128SSIMPLE_N];
    uint8 leaf[FSMSW_SPHINCSSHAKE_128SSIMPLE_N];
    uint32 fors_tree_addr[8] = {0};
    uint32 fors_pk_addr[8] = {0};
    uint32 idx_offset;
    uint32 i;

    /* sig_temp is used to avoid modifying the input. */
    const uint8 *sig_temp = sig;

    FsmSw_SphincsShake_128sSimple_copy_keypair_addr(fors_tree_addr, fors_addr);
    FsmSw_SphincsShake_128sSimple_copy_keypair_addr(fors_pk_addr, fors_addr);

    FsmSw_SphincsShake_set_type(fors_tree_addr, FSMSW_SPHINCS_ADDR_TYPE_FORSTREE);
    FsmSw_SphincsShake_set_type(fors_pk_addr, FSMSW_SPHINCS_ADDR_TYPE_FORSPK);

    message_to_indices(indices, m);

    for (i = 0; i < FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_TREES; i++)
    {
        idx_offset = i * (uint32)((uint64)((uint64)1u << FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_HEIGHT));

        FsmSw_SphincsShake_set_tree_height(fors_tree_addr, 0);
        FsmSw_SphincsShake_set_tree_index(fors_tree_addr, indices[i] + idx_offset);

        /* Derive the leaf from the included secret key part. */
        fors_sk_to_leaf(leaf, sig_temp, ctx, fors_tree_addr);
        sig_temp = &sig_temp[FSMSW_SPHINCSSHAKE_128SSIMPLE_N];

        /* Derive the corresponding root node of this tree. */
        FsmSw_SphincsShake_128sSimple_compute_root(&roots[i * FSMSW_SPHINCSSHAKE_128SSIMPLE_N], leaf, indices[i],
                                                   idx_offset, sig_temp, FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_HEIGHT, ctx,
                                                   fors_tree_addr);
        sig_temp = &sig_temp[FSMSW_SPHINCSSHAKE_128SSIMPLE_N * FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_HEIGHT];
    }

    /* Hash horizontally across all tree roots to derive the public key. */
    FsmSw_SphincsShake_128sSimple_thash(pk, roots, FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_TREES, ctx, fors_pk_addr);
}
