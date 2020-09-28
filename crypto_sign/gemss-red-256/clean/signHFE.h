#ifndef _SIGNHFE_H
#define _SIGNHFE_H
#include "arch.h"
#include "config_HFE.h"
#include "gf2nx.h"
#include "matrix_gf2.h"
#include "parameters_HFE.h"
#include "sizes_HFE.h"
#include <stddef.h>


typedef struct _secret_key_HFE {
    complete_sparse_monic_gf2nx F_struct;
    sparse_monic_gf2nx F_HFEv;
    GLnv_gf2 S;
    GLn_gf2 T;

    UINT sk_uncomp[NB_UINT_HFEVPOLY
                   + (LTRIANGULAR_NV_SIZE << 1)
                   + (LTRIANGULAR_N_SIZE << 1) + SIZE_VECTOR_t
                   + MATRIXnv_SIZE + MATRIXn_SIZE];
} secret_key_HFE;


void PQCLEAN_GEMSSRED256_CLEAN_precSignHFESeed(secret_key_HFE *sk_HFE, const UINT *sk);

/* Precomputation for one secret-key */
int PQCLEAN_GEMSSRED256_CLEAN_precSignHFE(secret_key_HFE *sk_HFE, const UINT **linear_coefs,
        const UINT *sk);

/* Sign a message: sm is the signed message, m is the original message,
 *                 and sk is the secret-key. */

/* Function for nb_ite>0 */
int PQCLEAN_GEMSSRED256_CLEAN_signHFE_FeistelPatarin(unsigned char *sm8,
        const unsigned char *m, size_t len,
        const UINT *sk);




#define signHFE PQCLEAN_GEMSSRED256_CLEAN_signHFE_FeistelPatarin



#endif
