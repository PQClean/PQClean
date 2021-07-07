#ifndef _MATRIX_GF2_H
#define _MATRIX_GF2_H
#include "arch.h"
#include "parameters_HFE.h"
#include "tools_gf2m.h"
#include "tools_gf2n.h"
#include "tools_gf2nv.h"



/* This type stores a vector of v elements of GF(2) */
typedef UINT *vecv_gf2;
typedef const UINT *cst_vecv_gf2;
/* to use data[length] */
typedef UINT static_vecv_gf2;

/* This type stores a vector of n elements of GF(2) */
typedef UINT *vecn_gf2;
typedef const UINT *cst_vecn_gf2;
/* to use data[length] */
typedef UINT static_vecn_gf2;

/* This type stores a vector of n+v elements of GF(2) */
typedef UINT *vecnv_gf2;
typedef const UINT *cst_vecnv_gf2;
/* to use data[length] */
typedef UINT static_vecnv_gf2;

/* This type stores a vector of m elements of GF(2) */
typedef UINT *vecm_gf2;
typedef const UINT *cst_vecm_gf2;
/* to use data[length] */
typedef UINT static_vecm_gf2;

/* This type stores a vector of (m%64) elements of GF(2) */
typedef UINT *vecmr_gf2;



/* This type stores a matrix n*n in GF(2). */
typedef UINT *Mn_gf2;
typedef const UINT *cst_Mn_gf2;

/* This type stores a matrix (n+v)*(n+v) in GF(2). */
typedef UINT *Mnv_gf2;
typedef const UINT *cst_Mnv_gf2;

/* This type stores a matrix (n+v)*n in GF(2). */
typedef UINT *Mnv_n_gf2;
typedef const UINT *cst_Mnv_n_gf2;

/* This type stores a matrix v*n in GF(2). */
typedef UINT *Mv_n_gf2;
typedef const UINT *cst_Mv_n_gf2;

/* This type stores an invertible matrix n*n in GF(2). */
typedef UINT *GLn_gf2;
typedef const UINT *cst_GLn_gf2;

/* This type stores an invertible matrix (n+v)*(n+v) in GF(2). */
typedef UINT *GLnv_gf2;
typedef const UINT *cst_GLnv_gf2;

/* This type stores a triangular matrix n*n in GF(2).*/
typedef UINT *Tn_gf2;
typedef const UINT *cst_Tn_gf2;

/* This type stores a triangular matrix (n+v)*(n+v) in GF(2).*/
typedef UINT *Tnv_gf2;
typedef const UINT *cst_Tnv_gf2;



/* Number of UINT of matrix n*n in GF(2) */
#define MATRIXn_SIZE (HFEn*NB_WORD_GFqn)
/* Number of UINT of matrix (n+v)*(n+v) in GF(2) */
#define MATRIXnv_SIZE (HFEnv*NB_WORD_GF2nv)
/* Number of UINT of matrix m*m in GF(2) */
#define MATRIXm_SIZE (HFEm*NB_WORD_GF2m)



#define MALLOC_MATRIXn (UINT*)malloc(MATRIXn_SIZE*sizeof(UINT));
#define CALLOC_MATRIXn (UINT*)calloc(MATRIXn_SIZE,sizeof(UINT));

#define MALLOC_MATRIXnv (UINT*)malloc(MATRIXnv_SIZE*sizeof(UINT));
#define CALLOC_MATRIXnv (UINT*)calloc(MATRIXnv_SIZE,sizeof(UINT));



/* Size of lower triangular matrix n*n (in words) */
#define LTRIANGULAR_N_SIZE (((HFEnq*(HFEnq+1))>>1)*NB_BITS_UINT+\
                            NB_WORD_GFqn*HFEnr)
/* Size of lower triangular matrix (n+v)*(n+v) (in words) */
#define LTRIANGULAR_NV_SIZE (((HFEnvq*(HFEnvq+1))>>1)*NB_BITS_UINT+\
                             NB_WORD_GF2nv*HFEnvr)



#endif
