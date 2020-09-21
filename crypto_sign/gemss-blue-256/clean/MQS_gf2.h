#ifndef _MQS_GF2_H
#define _MQS_GF2_H
#include "MQ_gf2.h"
#include "arch.h"
#include "choice_crypto.h"
#include "config_HFE.h"
#include "parameters_HFE.h"
#include "tools_gf2m.h"



/* This type stores a multivariate quadratic system with n equations in
GF(2)[x1,...,xn]. It is stored like one equation in GF(2^n)[x1,...,xn]. */
typedef UINT *mqsn_gf2n;
typedef const UINT *cst_mqsn_gf2n;

/* This type stores a multivariate quadratic system with n equations in
GF(2)[x1,...,x_(n+v)]. It is stored like one equation in
GF(2^n)[x1,...,x_(n+v)]. */
typedef UINT *mqsnv_gf2n;
typedef const UINT *cst_mqsnv_gf2n;

/* This type stores a multivariate quadratic system with n equations in
GF(2)[x1,...,xv]. It is stored like one equation in GF(2^n)[x1,...,xv]. */
typedef UINT *mqsv_gf2n;
typedef const UINT *cst_mqsv_gf2n;

/* This type stores a multivariate quadratic system with m equations in
GF(2)[x1,...,xn]. It is stored like one equation in GF(2^m)[x1,...,xn]. */
typedef UINT *mqsn_gf2m;
typedef const UINT *cst_mqsn_gf2m;

/* This type stores a multivariate quadratic system with m equations in
 * GF(2)[x1,...,x_(n+v)]. It is stored like one equation in
 * GF(2^n)[x1,...,x_(n+v)]. */
typedef UINT *mqsnv_gf2m;
typedef const UINT *cst_mqsnv_gf2m;
typedef uint8_t *mqsnv8_gf2m;
typedef const uint8_t *cst_mqsnv8_gf2m;

/* This type stores a multivariate quadratic system with m equations in
GF(2)[x1,...,x_(n+v)]. It is stored like m equations in GF(2)[x1,...,x_(n+v)].*/
typedef UINT *mqnv_gf2_m;
typedef const UINT *cst_mqnv_gf2_m;




/* Size for m compressed equations */
#define NB_WORD_mEQUATIONS (NB_WORD_EQUATION*HFEm)
/* Size for m uncompressed equations */
#define NB_WORD_UNCOMP_m_EQ (NB_WORD_UNCOMP_EQ*HFEm+NB_WORD_GF2m)


/* Remove the last word of each coefficient in GF(2^m),
   excepted for the constant */
#define ACCESS_last_equations ((NB_MONOMIAL_PK-1)*HFEmq+NB_WORD_GF2m)
/* It is padded to avoid to load data outside of memory
   during the public-key evaluation */
/* XXX We remove the padding here XXX */
#define NB_WORD_HYBRID_EQUATIONS (ACCESS_last_equations+            NB_WORD_UNCOMP_EQ*HFEmr/*+((4-(NB_WORD_GF2nv&3))&3)*/)


#define ACCESS_last_equations8 (NB_MONOMIAL_PK*HFEmq8)


/* Size of a MQ polynomial with n+v variables and with coefficients
   in GF(2^n) */
#define MQnv_GFqn_SIZE (NB_MONOMIAL_PK*NB_WORD_GFqn)
/* Size of a MQ polynomial with v variables and with coefficients in GF(2^n) */
#define MQv_GFqn_SIZE (NB_MONOMIAL_VINEGAR*NB_WORD_GFqn)
/* Size of a multivariate linear polynomial with v variables and with
 * coefficients in GF(2^n) */
#define MLv_GFqn_SIZE ((HFEv+1)*NB_WORD_GFqn)

#define MALLOC_MQSn (UINT*)malloc(MQnv_GFqn_SIZE*sizeof(UINT))
#define CALLOC_MQSn (UINT*)calloc(MQnv_GFqn_SIZE,sizeof(UINT))




/* Size of a MQ polynomial with coefficients in GF(2^m) */
/* It is padded to avoid to load data outside of memory
   during the public-key evaluation */
/* XXX We remove the padding here XXX */
#define MQ_GFqm_SIZE ((NB_MONOMIAL_PK*NB_WORD_GF2m)/*+((4-(NB_WORD_GF2m&3))&3)*/)
/* XXX 64-bit padding XXX */
#define MQ_GFqm8_SIZE             (NB_MONOMIAL_PK*NB_BYTES_GFqm+((8-(NB_BYTES_GFqm&7))&7))

#define MALLOC_MQSm (UINT*)malloc(MQ_GFqm_SIZE*sizeof(UINT))
#define CALLOC_MQSm (UINT*)calloc(MQ_GFqm_SIZE,sizeof(UINT))




#endif
