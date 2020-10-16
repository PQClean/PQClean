#include "../SABER_params.h"

#define AVX_N (SABER_N >> 4)
#define small_len_avx (AVX_N >> 2)

#define SCHB_N 16

#define N_SB (SABER_N >> 2)
#define N_SB_RES (2*N_SB-1)

#define N_SB_16 (N_SB >> 2)
#define N_SB_16_RES (2*N_SB_16-1)

#define AVX_N1 16 /*N/16*/ 

#define SCM_SIZE 16

// The dimension of a vector. i.e vector has NUM_POLY elements and Matrix has NUM_POLY X NUM_POLY elements
#define NUM_POLY SABER_K
//int NUM_POLY=2; 
