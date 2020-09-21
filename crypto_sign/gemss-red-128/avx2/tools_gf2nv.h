#ifndef _TOOLS_GF2NV_H
#define _TOOLS_GF2NV_H
#include "add_gf2x.h"
#include "arch.h"
#include "bit.h"
#include "init.h"
#include "loadMask.h"
#include "macro.h"
#include "parameters_HFE.h"
#include "tools_gf2m.h"



/* This type stores an element of GF(2^(n+v)). */
typedef UINT *gf2nv;
typedef const UINT *cst_gf2nv;
/* to use data[length] */
typedef UINT static_gf2nv;


/* n+v = NB_BITS_UINT*quo + rem */
#define HFEnvq (HFEnv/NB_BITS_UINT)
#define HFEnvr (HFEnv%NB_BITS_UINT)
/* The number of word that an element of GF(2^(n+v)) needs */
#define NB_WORD_GF2nv_TMP HFEnvq

/* Mask for arithmetic in GF(2^(n+v)) */
#define MASK_GF2nv maskUINT(HFEnvr)

#define HFEnvq8 (HFEnv>>3)
#define HFEnvr8 (HFEnv&7)
#define MASK8_GF2nv ((1U<<HFEnvr8)-1)
/* Number of bytes that an element of GF(2^(n+v)) needs */
#define NB_BYTES_GFqnv (HFEnvq8+((HFEnvr8)?1:0))




/* To choose macro for NB_WORD_GF2nv*64 bits */
#define NB_BITS_GFqnv_SUP 192
#define NB_WORD_GF2nv 3

#define CONCAT_NB_BITS_GFqnv_SUP(name) CONCAT(name,NB_BITS_GFqnv_SUP)
#define CONCAT_NB_WORD_GF2nv_SUP(name) CONCAT(name,NB_WORD_GF2nv)





#define add_gf2nv CONCAT(CONCAT_NB_WORD_GF2nv_SUP(ADD),_GF2X)
#define add2_gf2nv CONCAT(CONCAT_NB_WORD_GF2nv_SUP(ADD),_2,_GF2X)
#define swap_gf2nv CONCAT_NB_WORD_GF2nv_SUP(SWAP)
#define set0_gf2nv CONCAT_NB_WORD_GF2nv_SUP(SET0_)
#define xorLoadMask1_gf2nv CONCAT_NB_WORD_GF2nv_SUP(XORLOADMASK1_)





#define NB_WORD_GF2nvm (NB_WORD_GF2nv-NB_WORD_GF2m+(HFEmr?1:0))

#define MASK_GF2nvm (~MASK_GF2m)


/* To choose macro for NB_WORD_GF2nvm*64 bits */
#define copy_gf2nvm COPY1





#endif
