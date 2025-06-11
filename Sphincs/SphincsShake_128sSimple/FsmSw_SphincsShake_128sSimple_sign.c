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
#include "FsmSw_SphincsShake_128sSimple_FctWrapper.h"
#include "FsmSw_SphincsShake_128sSimple_context.h"
#include "FsmSw_SphincsShake_128sSimple_fors.h"
#include "FsmSw_SphincsShake_128sSimple_hash.h"
#include "FsmSw_SphincsShake_128sSimple_merkle.h"
#include "FsmSw_SphincsShake_128sSimple_params.h"
#include "FsmSw_SphincsShake_128sSimple_thash.h"
#include "FsmSw_SphincsShake_128sSimple_utils.h"
#include "FsmSw_SphincsShake_128sSimple_wots.h"
#include "FsmSw_Sphincs_shake_address.h"

#include "FsmSw_SphincsShake_128sSimple_sign.h"

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
static sint8 fsmsw_sphincsshake_128ssimple_crypto_sign_SeedKeyPair(uint8 *pk, uint8 *sk, const uint8 *seed);
/**********************************************************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS                                                                                      */
/**********************************************************************************************************************/
/***********************************************************************************************************************
 * Name:        fsmsw_sphincsshake_128ssimple_crypto_sign_SeedKeyPair
 *
 * Description: Generates an SPX key pair given a seed of length
 *              Format sk: [SK_SEED || SK_PRF || PUB_SEED || root]
 *              Format pk: [PUB_SEED || root]
 *
 * Arguments:   -       uint8 *pk:   t.b.d.
 * Arguments:   -       uint8 *sk:   t.b.d.
 *              - const uint8 *seed: t.b.d.
 *
 * Returns 0.
 *
 **********************************************************************************************************************/
static sint8 fsmsw_sphincsshake_128ssimple_crypto_sign_SeedKeyPair(uint8 *pk, uint8 *sk, const uint8 *seed)
{
  sphincs_shake_128s_ctx ctx = {{0}};

  /* Initialize SK_SEED, SK_PRF and PUB_SEED from seed. */
  FsmSw_CommonLib_MemCpy(sk, seed, FSMSW_SPHINCSSHAKE_128SSIMPLE_CRYPTO_SEEDBYTES);

  FsmSw_CommonLib_MemCpy(pk, &sk[2u * FSMSW_SPHINCSSHAKE_128SSIMPLE_N], FSMSW_SPHINCSSHAKE_128SSIMPLE_N);

  FsmSw_CommonLib_MemCpy(ctx.pub_seed, pk, FSMSW_SPHINCSSHAKE_128SSIMPLE_N);
  FsmSw_CommonLib_MemCpy(ctx.sk_seed, sk, FSMSW_SPHINCSSHAKE_128SSIMPLE_N);

  /* This hook allows the hash function instantiation to do whatever preparation or computation it needs, based on
   * the public seed. */
  FsmSw_SphincsShake_128sSimple_InitializeHashFunction(&ctx);

  /* Compute root node of the top-most subtree. */
  FsmSw_SphincsShake_128sSimple_Merkle_GenRoot(&sk[3u * FSMSW_SPHINCSSHAKE_128SSIMPLE_N], &ctx);

  /* cleanup */
  FsmSw_SphincsShake_128sSimple_1FreeHashFunction(&ctx);

  FsmSw_CommonLib_MemCpy(&pk[FSMSW_SPHINCSSHAKE_128SSIMPLE_N], &sk[3u * FSMSW_SPHINCSSHAKE_128SSIMPLE_N],
                         FSMSW_SPHINCSSHAKE_128SSIMPLE_N);

  return 0;
}
/**********************************************************************************************************************/
/* PUBLIC FUNCTIONS DEFINITIONS                                                                                       */
/**********************************************************************************************************************/
/***********************************************************************************************************************
 * Name:        FsmSw_SphincsShake_128sSimple_Crypto_Sign_KeyPair
 *
 * Description: Generates an SPX key pair.
 *              Format sk: [SK_SEED || SK_PRF || PUB_SEED || root]
 *              Format pk: [PUB_SEED || root]
 *
 * Arguments:   -       uint8 *pk:   t.b.d.
 * Arguments:   -       uint8 *sk:   t.b.d.
 *
 * Returns 0.
 *
 **********************************************************************************************************************/
sint8 FsmSw_SphincsShake_128sSimple_Crypto_Sign_KeyPair(uint8 *pk, uint8 *sk)
{
  uint8 seed[FSMSW_SPHINCSSHAKE_128SSIMPLE_CRYPTO_SEEDBYTES] = {0};
  (void)FsmSw_CommonLib_RandomBytes(seed, FSMSW_SPHINCSSHAKE_128SSIMPLE_CRYPTO_SEEDBYTES);
  (void)fsmsw_sphincsshake_128ssimple_crypto_sign_SeedKeyPair(pk, sk, seed);

  return 0;
}

/***********************************************************************************************************************
 * Name:        FsmSw_SphincsSha2_128sSimple_Crypto_Sign_Signature
 *
 * Description: Returns an array containing a detached signature.
 *
 * Arguments:   -       uint8  *sig:    t.b.d.
 *              -       uint32 *siglen: t.b.d.
 *              - const uint8  *m:      t.b.d.
 *              -       uint32  mlen:   t.b.d.
 *              - const uint8  *sk:     t.b.d.
 *
 * Returns 0.
 *
 **********************************************************************************************************************/
/* polyspace +6 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:8.7 [Justified:]"This is an interface function
designed for use by other systems that aim to integrate the Sphincs." */
sint8 FsmSw_SphincsShake_128sSimple_Crypto_Sign_Signature(uint8 *sig, uint32 *siglen, const uint8 *m, uint32 mlen,
                                                          const uint8 *sk)
{
  sphincs_shake_128s_ctx ctx = {{0}};

  const uint8 *sk_prf = &sk[FSMSW_SPHINCSSHAKE_128SSIMPLE_N];
  const uint8 *pk     = &sk[2u * FSMSW_SPHINCSSHAKE_128SSIMPLE_N];

  uint8 optrand[FSMSW_SPHINCSSHAKE_128SSIMPLE_N];
  uint8 mhash[FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_MSG_BYTES];
  uint8 root[FSMSW_SPHINCSSHAKE_128SSIMPLE_N];
  uint32 i;
  uint64 tree;
  uint32 idx_leaf;
  uint32 wots_addr[8] = {0};
  uint32 tree_addr[8] = {0};

  /* sig_temp is used to avoid modifying the input. */
  uint8 *sig_temp = sig;

  FsmSw_CommonLib_MemCpy(ctx.sk_seed, sk, FSMSW_SPHINCSSHAKE_128SSIMPLE_N);
  FsmSw_CommonLib_MemCpy(ctx.pub_seed, pk, FSMSW_SPHINCSSHAKE_128SSIMPLE_N);

  /* This hook allows the hash function instantiation to do whatever preparation or computation it needs,
   * based on the public seed. */
  FsmSw_SphincsShake_128sSimple_InitializeHashFunction(&ctx);

  FsmSw_SphincsShake_SetType(wots_addr, FSMSW_SPHINCS_ADDR_TYPE_WOTS);
  FsmSw_SphincsShake_SetType(tree_addr, FSMSW_SPHINCS_ADDR_TYPE_HASHTREE);

  /* Optionally, signing can be made non-deterministic using optrand. This can help counter side-channel attacks
   * that would benefit from getting a large number of traces when the signer uses the same nodes. */
  (void)FsmSw_CommonLib_RandomBytes(optrand, FSMSW_SPHINCSSHAKE_128SSIMPLE_N);
  /* Compute the digest randomization value. */
  FsmSw_SphincsShake_128sSimple_GenMessageRandom(sig_temp, sk_prf, optrand, m, mlen, &ctx);

  /* Derive the message digest and leaf index from R, PK and M. */
  FsmSw_SphincsShake_128sSimple_HashMessage(mhash, &tree, &idx_leaf, sig_temp, pk, m, mlen, &ctx);
  sig_temp = &sig_temp[FSMSW_SPHINCSSHAKE_128SSIMPLE_N];

  FsmSw_SphincsShake_SetTreeAddr(wots_addr, tree);
  FsmSw_SphincsShake_128sSimple_set_keypair_addr(wots_addr, idx_leaf);

  /* Sign the message hash using FORS. */
  FsmSw_SphincsShake_128sSimple_Fors_Sign(sig_temp, root, mhash, &ctx, wots_addr);
  sig_temp = &sig_temp[FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_BYTES];

  for (i = 0; i < FSMSW_SPHINCSSHAKE_128SSIMPLE_D; i++)
  {
    FsmSw_SphincsShake_SetLayerAddr(tree_addr, i);
    FsmSw_SphincsShake_SetTreeAddr(tree_addr, tree);

    FsmSw_SphincsShake_CopySubTreeAddr(wots_addr, tree_addr);
    FsmSw_SphincsShake_128sSimple_set_keypair_addr(wots_addr, idx_leaf);

    FsmSw_SphincsShake_128sSimple_Merkle_Sign(sig_temp, root, &ctx, wots_addr, tree_addr, idx_leaf);
    sig_temp = &sig_temp[FSMSW_SPHINCSSHAKE_128SSIMPLE_WOTS_BYTES +
                         (FSMSW_SPHINCSSHAKE_128SSIMPLE_TREE_HEIGHT * FSMSW_SPHINCSSHAKE_128SSIMPLE_N)];

    /* Update the indices for the next layer. */
    idx_leaf = (uint32)(tree & (((uint64)((uint64)1u << FSMSW_SPHINCSSHAKE_128SSIMPLE_TREE_HEIGHT)) - 1u));
    tree     = tree >> FSMSW_SPHINCSSHAKE_128SSIMPLE_TREE_HEIGHT;
  }

  FsmSw_SphincsShake_128sSimple_1FreeHashFunction(&ctx);

  *siglen = FSMSW_SPHINCSSHAKE_128SSIMPLE_BYTES;

  return 0;
}

/***********************************************************************************************************************
 * Name:        FsmSw_SphincsShake_128sSimple_Crypto_Sign_Verify
 *
 * Description: Verifies a detached signature and message under a given public key.
 *
 * Arguments:   - const uint8  *sig:    t.b.d.
 *              -       uint32  siglen: t.b.d.
 *              - const uint8  *m:      t.b.d.
 *              -       uint32  mlen:   t.b.d.
 *              - const uint8  *pk:     t.b.d.
 *
 * Returns 0.
 *
 **********************************************************************************************************************/
/* polyspace +6 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +4 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:8.7 [Justified:]"This is an interface function
designed for use by other systems that aim to integrate the Sphincs." */
sint8 FsmSw_SphincsShake_128sSimple_Crypto_Sign_Verify(const uint8 *sig, uint32 siglen, const uint8 *m, uint32 mlen,
                                                       const uint8 *pk)
{
  sphincs_shake_128s_ctx ctx                                = {{0}};
  const uint8 *pub_root                                     = &pk[FSMSW_SPHINCSSHAKE_128SSIMPLE_N];
  uint8 mhash[FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_MSG_BYTES] = {0};
  uint8 wots_pk[FSMSW_SPHINCSSHAKE_128SSIMPLE_WOTS_BYTES]   = {0};
  uint8 root[FSMSW_SPHINCSSHAKE_128SSIMPLE_N]               = {0};
  uint8 leaf[FSMSW_SPHINCSSHAKE_128SSIMPLE_N]               = {0};
  uint32 i                                                  = 0;
  uint64 tree                                               = 0;
  uint32 idx_leaf                                           = 0;
  uint32 wots_addr[8]                                       = {0};
  uint32 tree_addr[8]                                       = {0};
  uint32 wots_pk_addr[8]                                    = {0};
  sint8 retVal                                              = 0;

  /* sig_temp is used to avoid modifying the input. */
  const uint8 *sig_temp = sig;

  if (siglen != FSMSW_SPHINCSSHAKE_128SSIMPLE_BYTES)
  {
    retVal = -1;
  }

  FsmSw_CommonLib_MemCpy(ctx.pub_seed, pk, FSMSW_SPHINCSSHAKE_128SSIMPLE_N);

  /* This hook allows the hash function instantiation to do whatever preparation or computation it needs, based on
   * the public seed. */
  FsmSw_SphincsShake_128sSimple_InitializeHashFunction(&ctx);

  FsmSw_SphincsShake_SetType(wots_addr, FSMSW_SPHINCS_ADDR_TYPE_WOTS);
  FsmSw_SphincsShake_SetType(tree_addr, FSMSW_SPHINCS_ADDR_TYPE_HASHTREE);
  FsmSw_SphincsShake_SetType(wots_pk_addr, FSMSW_SPHINCS_ADDR_TYPE_WOTSPK);

  /* Derive the message digest and leaf index from R || PK || M. */
  /* The additional FSMSW_SPHINCSSHAKE_128SSIMPLE_N is a result of the hash domain separator. */
  FsmSw_SphincsShake_128sSimple_HashMessage(mhash, &tree, &idx_leaf, sig_temp, pk, m, mlen, &ctx);
  sig_temp = &sig_temp[FSMSW_SPHINCSSHAKE_128SSIMPLE_N];

  /* Layer correctly defaults to 0, so no need to FsmSw_Sphincs_set_layer_addr */
  FsmSw_SphincsShake_SetTreeAddr(wots_addr, tree);
  FsmSw_SphincsShake_128sSimple_set_keypair_addr(wots_addr, idx_leaf);

  FsmSw_SphincsShake_128sSimple_Fors_PkFromSig(root, sig_temp, mhash, &ctx, wots_addr);
  sig_temp = &sig_temp[FSMSW_SPHINCSSHAKE_128SSIMPLE_FORS_BYTES];

  /* For each subtree.. */
  for (i = 0; i < FSMSW_SPHINCSSHAKE_128SSIMPLE_D; i++)
  {
    FsmSw_SphincsShake_SetLayerAddr(tree_addr, i);
    FsmSw_SphincsShake_SetTreeAddr(tree_addr, tree);

    FsmSw_SphincsShake_CopySubTreeAddr(wots_addr, tree_addr);
    FsmSw_SphincsShake_128sSimple_set_keypair_addr(wots_addr, idx_leaf);

    FsmSw_SphincsShake_128sSimple_copy_keypair_addr(wots_pk_addr, wots_addr);

    /* The WOTS public key is only correct if the signature was correct. Initially, root is the FORS pk, but on
     * subsequent iterations it is the root of the subtree below the currently processed subtree. */
    FsmSw_SphincsShake_128sSimple_Wots_PkFromSig(wots_pk, sig_temp, root, &ctx, wots_addr);
    sig_temp = &sig_temp[FSMSW_SPHINCSSHAKE_128SSIMPLE_WOTS_BYTES];

    /* Compute the leaf node using the WOTS public key. */
    FsmSw_SphincsShake_128sSimple_Thash(leaf, wots_pk, FSMSW_SPHINCSSHAKE_128SSIMPLE_WOTS_LEN, &ctx, wots_pk_addr);

    /* Compute the root node of this subtree. */
    FsmSw_SphincsShake_128sSimple_ComputeRoot(root, leaf, idx_leaf, 0, sig_temp,
                                              FSMSW_SPHINCSSHAKE_128SSIMPLE_TREE_HEIGHT, &ctx, tree_addr);
    sig_temp = &sig_temp[FSMSW_SPHINCSSHAKE_128SSIMPLE_TREE_HEIGHT * FSMSW_SPHINCSSHAKE_128SSIMPLE_N];

    /* Update the indices for the next layer. */
    idx_leaf = (uint32)(tree & (((uint64)((uint64)1u << FSMSW_SPHINCSSHAKE_128SSIMPLE_TREE_HEIGHT)) - 1u));
    tree     = tree >> FSMSW_SPHINCSSHAKE_128SSIMPLE_TREE_HEIGHT;
  }

  /* cleanup */
  FsmSw_SphincsShake_128sSimple_1FreeHashFunction(&ctx);

  /* Check if the root node equals the root node in the public key. */
  if (FsmSw_CommonLib_MemCmp(root, pub_root, FSMSW_SPHINCSSHAKE_128SSIMPLE_N) != 0u)
  {
    retVal = -1;
  }

  return retVal;
}

/***********************************************************************************************************************
 * Name:        FsmSw_SphincsShake_128sSimple_Crypto_Sign
 *
 * Description: Returns an array containing the signature followed by the message.
 *
 * Arguments:   -       uint8  *sm:    t.b.d.
 *              -       uint32  smlen: t.b.d.
 *              - const uint8  *m:     t.b.d.
 *              -       uint32  mlen:  t.b.d.
 *              - const uint8  *sk:    t.b.d.
 *
 * Returns 0.
 *
 **********************************************************************************************************************/
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity
and avoids confusion with other functions. Therefore, this warning is a false positive." */
sint8 FsmSw_SphincsShake_128sSimple_Crypto_Sign(uint8 *sm, uint32 *smlen, const uint8 *m, uint32 mlen, const uint8 *sk)
{
  uint32 siglen = 0;

  (void)FsmSw_SphincsShake_128sSimple_Crypto_Sign_Signature(sm, &siglen, m, mlen, sk);

  FsmSw_CommonLib_MemMove(&sm[FSMSW_SPHINCSSHAKE_128SSIMPLE_BYTES], m, mlen);
  *smlen = siglen + mlen;

  return 0;
}

/***********************************************************************************************************************
 * Name:        FsmSw_SphincsShake_128sSimple_Crypto_Sign_Open
 *
 * Description: Verifies a given signature-message pair under a given public key.
 *
 * Arguments:   -       uint8  *m:     t.b.d.
 *              -       uint32 *mlen:  t.b.d.
 *              - const uint8  *sm:    t.b.d.
 *              -       uint32  smlen: t.b.d.
 *              - const uint8  *pk:    t.b.d.
 *
 * Returns 0.
 *
 **********************************************************************************************************************/
/* polyspace +4 ISO-17961:funcdecl [Justified:]"The identifiers are distinct. The naming convention ensures clarity 
and avoids confusion with other functions. Therefore, this warning is a false positive." */
/* polyspace +2 MISRA2012:5.1 [Justified:]"The identifiers are distinct. The naming convention ensures clarity
and avoids confusion with other functions. Therefore, this warning is a false positive." */
sint8 FsmSw_SphincsShake_128sSimple_Crypto_Sign_Open(uint8 *m, uint32 *mlen, const uint8 *sm, uint32 smlen,
                                                     const uint8 *pk)
{
  sint8 retVal = 0;

  /* The API caller does not necessarily know what size a signature should be but SPHINCS+ signatures are always
   * exactly FSMSW_SPHINCSSHAKE_128SSIMPLE_BYTES. */
  if (smlen < FSMSW_SPHINCSSHAKE_128SSIMPLE_BYTES)
  {
    FsmSw_CommonLib_MemSet(m, 0, smlen);
    *mlen  = 0;
    retVal = -1;
  }

  *mlen = smlen - FSMSW_SPHINCSSHAKE_128SSIMPLE_BYTES;

  if (0 != FsmSw_SphincsShake_128sSimple_Crypto_Sign_Verify(sm, FSMSW_SPHINCSSHAKE_128SSIMPLE_BYTES,
                                                            &sm[FSMSW_SPHINCSSHAKE_128SSIMPLE_BYTES], *mlen, pk))
  {
    FsmSw_CommonLib_MemSet(m, 0, smlen);
    *mlen  = 0;
    retVal = -1;
  }

  /* If verification was successful, move the message to the right place. */
  FsmSw_CommonLib_MemMove(m, &sm[FSMSW_SPHINCSSHAKE_128SSIMPLE_BYTES], *mlen);

  return retVal;
}
