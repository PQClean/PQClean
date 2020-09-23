#ifndef _TOOLS_GF2N_H
#define _TOOLS_GF2N_H
#include "arch.h"
#include "bit.h"
#include "init.h"
#include "loadMask.h"
#include "macro.h"
#include "parameters_HFE.h"
#include "predicate.h"



/* This type stores an element of GF(2^n). */
typedef UINT *gf2n;
typedef const UINT *cst_gf2n;
/* to use data[length] */
typedef UINT static_gf2n;
typedef const UINT cst_static_gf2n;


/* Floor(Log(2,n)) */
#define LOG2_N 8

#define POW_FLOG2_N (1<<LOG2_N)


/* n = NB_BITS_UINT*quo + rem */
#define HFEnq (HFEn/NB_BITS_UINT)
#define HFEnr (38)
/* The number of word that an element of GF(2^n) needs */
#define NB_WORD_GFqn_TMP (HFEnq+1)

/* Mask for arithmetic in GF(2^n) */
#define MASK_GF2n maskUINT(HFEnr)

#define HFEnr8 (HFEn&7)
#define MASK8_GF2n ((1U<<HFEnr8)-1)
/* Number of bytes that an element of GF(2^n) needs */
#define NB_BYTES_GFqn ((HFEn>>3)+((HFEnr8)?1:0))




/* To choose macro for NB_WORD_GFqn*64 bits */
#define NB_BITS_GFqn_SUP 384
#define NB_WORD_GFqn 6

#define CONCAT_NB_BITS_GFqn_SUP(name) CONCAT(name,NB_BITS_GFqn_SUP)
#define CONCAT_NB_WORD_GFqn_SUP(name) CONCAT(name,NB_WORD_GFqn)





#define PINIT_GF2N CONCAT_NB_BITS_GFqn_SUP(PINIT)
#define isEqual_gf2n(a,b) PQCLEAN_GEMSSRED256_AVX2_ISEQUAL(a,b,NB_WORD_GFqn)
#define is0_gf2n(a) PQCLEAN_GEMSSRED256_AVX2_ISZERO(a,NB_WORD_GFqn)
#define isNot0_gf2n(a) PQCLEAN_GEMSSRED256_AVX2_ISNOTZERO(a,NB_WORD_GFqn)
#define is1_gf2n(a) PQCLEAN_GEMSSRED256_AVX2_ISONE(a,NB_WORD_GFqn)
#define cmp_lt_gf2n(a,b) PQCLEAN_GEMSSRED256_AVX2_CMP_LT(a,b,NB_WORD_GFqn)
#define cmp_gt_gf2n(a,b) PQCLEAN_GEMSSRED256_AVX2_CMP_GT(a,b,NB_WORD_GFqn)

#define set0_gf2n(c) SET0((unsigned char *)(c),8*NB_WORD_GFqn)

#define swap_gf2n CONCAT_NB_WORD_GFqn_SUP(SWAP)
#define copy_gf2n CONCAT_NB_WORD_GFqn_SUP(COPY)
#define set1_gf2n CONCAT_NB_WORD_GFqn_SUP(SET1_)
#define xorLoadMask1_gf2n CONCAT_NB_WORD_GFqn_SUP(XORLOADMASK1_)

#define isEqual_nocst_gf2n CONCAT(CONCAT_NB_WORD_GFqn_SUP(PQCLEAN_GEMSSRED256_AVX2_ISEQUAL),_NOCST)
#define cmp_lt_nocst_gf2n CONCAT(CONCAT_NB_WORD_GFqn_SUP(PQCLEAN_GEMSSRED256_AVX2_CMP_LT),_NOCST)

#define cmp_gt_nocst_gf2n(a,b) cmp_lt_nocst_gf2n(b,a)




/****************** MEMORY ALIGNMENT ***************************/


/* Size of the alignment in byte */
/* XXX Disabled XXX */
#define SIZE_ALIGNED_GFqn 0/*16*/

#define ALIGNED_GFqn_FREE NO_ALIGNED_FREE

/* No alignment */
#define ALIGNED_GFqn
#define ALIGNED_GFqn_MALLOC NO_ALIGNED_MALLOC
#define ALIGNED_GFqn_CALLOC NO_ALIGNED_CALLOC

#define LOAD128_GFqn PLOADU
#define LOAD256_GFqn VPLOADU
#define STORE128_GFqn PSTOREU
#define STORE256_GFqn VPSTOREU

#define INIT128_GFqn(A128,A) A128=LOAD128_GFqn(A);
#define INIT256_GFqn(A256,A) A256=LOAD256_GFqn(A);





/* Number of words of the result of a multiplication */
/* Only the multiplication of gf2x in 64 bits does not
   support an odd number of words for the product */
/* The squaring supports the minimal number of words for the product */
/* The mod reduction supports the minimal number of words for the product */

/* Minimal number of words:
   The leading monomial is X^(2*(n-1)), so it needs sup(((2*(n-1)+1)/64))
   words. sup(((2*(n-1)+1)/64)) = inf(((2*(n-1)+1)/64) + 1) because
   (2*(n-1)+1) is odd. A little trick: it is equal to
   inf(((2*(n-1))/64) + 1) */
#define NB_WORD_MUL ((((HFEn-1)<<1)/NB_BITS_UINT)+1)



/* To choose macro for NB_WORD_MUL*32 bits */
#define NB_BITS_MUL_SUP 384

#define CONCAT_NB_BITS_MUL_SUP(name) CONCAT(name,NB_BITS_MUL_SUP)



/* Minimum number of words to store the product of 2 (n-1)-elements of GF2[x] */
#define NB_WORD_MMUL_TMP ((((HFEn-1)<<1)/NB_BITS_UINT)+1)


#define NB_WORD_MMUL 12
#define NB_BITS_MMUL_SUP 384


#define CONCAT_NB_WORD_MMUL(name) CONCAT(name,NB_WORD_MMUL)
#define CONCAT_NB_BITS_MMUL_SUP(name) CONCAT(name,NB_BITS_MMUL_SUP)



#define set0_product_gf2n(c) SET0((unsigned char*)(c),8*NB_WORD_MMUL)

#define set0_high_product_gf2n(c)             SET0((c)+NB_WORD_GFqn,NB_WORD_MMUL-NB_WORD_GFqn)



#endif
