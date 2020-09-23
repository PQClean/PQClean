#ifndef _TOOLS_GF2M_H
#define _TOOLS_GF2M_H
#include "add_gf2x.h"
#include "arch.h"
#include "bit.h"
#include "dotProduct_gf2.h"
#include "init.h"
#include "loadMask.h"
#include "macro.h"
#include "parameters_HFE.h"
#include "predicate.h"



/* This type stores an element of GF(2^m). */
typedef UINT *gf2m;
typedef const UINT *cst_gf2m;
/* to use data[length] */
typedef UINT static_gf2m;


/* m = NB_BITS_UINT*quo + rem */
#define HFEmq (HFEm/NB_BITS_UINT)
#define HFEmr (HFEm%NB_BITS_UINT)
/* The number of word that an element of GF(2^m) needs */
#define NB_WORD_GF2m_TMP (HFEmq+1)

/* Mask to truncate the last word */
#define MASK_GF2m maskUINT(HFEmr)

#define HFEmq8 (HFEm>>3)
#define HFEmr8 (HFEm&7U)
#define HFEmr8c ((8-HFEmr8)&7)

/* Number of bytes that an element of GF(2^m) needs */
#define NB_BYTES_GFqm (HFEmq8+1)

/* To choose macro for NB_WORD_GF2m*64 bits */
#define NB_BITS_GFqm_SUP 192
#define NB_WORD_GF2m 3

#define CONCAT_NB_BITS_GFqm_SUP(name) CONCAT(name,NB_BITS_GFqm_SUP)
#define CONCAT_NB_WORD_GF2m_SUP(name) CONCAT(name,NB_WORD_GF2m)





#define isEqual_gf2m(a,b) PQCLEAN_GEMSSRED128_CLEAN_ISEQUAL(a,b,NB_WORD_GF2m)

#define set0_gf2m(c) SET0((unsigned char *)(c),8*NB_WORD_GF2m)
#define xorLoadMask1_gf2m(res,a,b) XORLOADMASK1((unsigned char *)(res),(unsigned char *)(a),b,8*NB_WORD_GF2m)

#define add_gf2m CONCAT(CONCAT_NB_WORD_GF2m_SUP(ADD),_GF2X)
#define add2_gf2m CONCAT(CONCAT_NB_WORD_GF2m_SUP(ADD),_2_GF2X)
#define copy_gf2m CONCAT_NB_WORD_GF2m_SUP(COPY)
#define dotProduct_gf2_m CONCAT_NB_WORD_GF2m_SUP(DOTPRODUCT)

#define isEqual_nocst_gf2m CONCAT(CONCAT_NB_WORD_GF2m_SUP(PQCLEAN_GEMSSRED128_CLEAN_ISEQUAL),_NOCST)




/* To choose macro for HFEmq*64 bits */
#define NB_BITS_mq_SUP 128

#define CONCAT_NB_BITS_mq_SUP(name) CONCAT(name,NB_BITS_mq_SUP)





/****************** MEMORY ALIGNMENT ***************************/


/* Size of the alignment in byte */
/* XXX Disabled XXX */
#define SIZE_ALIGNED_GFqm 0

#define ALIGNED_TMP_BUF16
#define ALIGNED_TMP_BUF32
#define STORE128_TMP_BUF PSTOREU
#define STORE256_TMP_BUF VPSTOREU
#define ALIGNED_GFqm_FREE NO_ALIGNED_FREE

/* No alignment */
#define ALIGNED_GFqm
#define ALIGNED_GFqm_MALLOC NO_ALIGNED_MALLOC
#define ALIGNED_GFqm_CALLOC NO_ALIGNED_CALLOC

#define LOAD128_GFqm PLOADU
#define LOAD256_GFqm VPLOADU
#define STORE128_GFqm PSTOREU
#define STORE256_GFqm VPSTOREU

#define INIT128_GFqm(A128,A) A128=LOAD128_GFqm(A);
#define INIT256_GFqm(A256,A) A256=LOAD256_GFqm(A);





#endif
