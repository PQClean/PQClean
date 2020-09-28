#include "add_gf2n.h"
#include "arch.h"
#include "choice_crypto.h"
#include "chooseRootHFE_gf2nx.h"
#include "config_HFE.h"
#include "evalMQSv_gf2.h"
#include "gf2nx.h"
#include "hash.h"
#include "initListDifferences_gf2nx.h"
#include "randMatrix_gf2.h"
#include "rand_gf2nx.h"
#include "randombytes.h"
#include "signHFE.h"
#include "sizes_HFE.h"
#include "tools_gf2m.h"
#include "tools_gf2n.h"
#include "tools_gf2v.h"
#include "vecMatProduct_gf2.h"
#include <stdlib.h>





/* When the root finding fails, the minus and vinegars are regenerated */
#define GEN_MINUS_VINEGARS 1



/* Number of bits to complete the byte of sm64, in [0,7] */
#define VAL_BITS_M 4

static void compress_signHFE(unsigned char *sm8, const UINT *sm) {
    unsigned char *sm64;
    unsigned int k2;

    sm64 = (unsigned char *)sm;

    /* Take the (n+v) first bits */
    for (k2 = 0; k2 < NB_BYTES_GFqnv; ++k2) {
        sm8[k2] = sm64[k2];
    }

    /* Take the (Delta+v)*(nb_ite-1) bits */
    unsigned int k1, nb_bits, nb_rem2, nb_rem_m, val_n;
    int nb_rem;

    /* HFEnv bits are already stored in sm8 */
    nb_bits = HFEnv;
    sm64 += (NB_WORD_GF2nv << 3) + (HFEmq8 & 7U);

    for (k1 = 1; k1 < NB_ITE; ++k1) {
        /* Number of bits to complete the byte of sm8, in [0,7] */
        if ((63) < ((8 - (nb_bits & 7U)) & 7U)) {
            val_n = (63);
        } else {
            val_n = ((8 - (nb_bits & 7U)) & 7U);
        }

        /* First byte of sm8 */
        if (nb_bits & 7U) {
            sm8[nb_bits >> 3] ^= ((*sm64) >> HFEmr8) << (nb_bits & 7U);

            /* Number of bits to complete the first byte of sm8 */
            nb_rem = (int)((val_n - VAL_BITS_M));
            if (nb_rem >= 0) {
                /* We take the next byte since we used
                   VAL_BITS_M bits */
                ++sm64;
            }
            if (nb_rem > 0) {
                nb_bits += VAL_BITS_M;
                sm8[nb_bits >> 3] ^= (*sm64) << (nb_bits & 7U);
                nb_bits += nb_rem;
            } else {
                nb_bits += val_n;
            }
        }

        /* Other bytes of sm8 */
        nb_rem2 = (63) - val_n;
        /*nb_rem2 can be zero only in this case */
        /* Number of bits used of sm64, mod 8 */
        nb_rem_m = (HFEm + val_n) & 7U;

        /* Other bytes */
        if (nb_rem_m) {
            /* -1 to take the ceil of /8, -1 */
            for (k2 = 0; k2 < ((nb_rem2 - 1) >> 3); ++k2) {
                sm8[nb_bits >> 3] = ((*sm64) >> nb_rem_m)
                                    ^ ((*(sm64 + 1)) << (8 - nb_rem_m));
                nb_bits += 8;
                ++sm64;
            }
            /* The last byte of sm8, between 1 and 8 bits to put */
            sm8[nb_bits >> 3] = (*sm64) >> nb_rem_m;
            ++sm64;

            /* nb_rem2 between 1 and 8 bits */
            nb_rem2 = ((nb_rem2 + 7U) & 7U) + 1U;
            if (nb_rem2 > (8 - nb_rem_m)) {
                sm8[nb_bits >> 3] ^= (*sm64) << (8 - nb_rem_m);
                ++sm64;
            }

            nb_bits += nb_rem2;
        } else {
            /* We are at the beginning of the bytes of sm8 and sm64 */

            /* +7 to take the ceil of /8 */
            for (k2 = 0; k2 < ((nb_rem2 + 7) >> 3); ++k2) {
                sm8[nb_bits >> 3] = *sm64;
                nb_bits += 8;
                ++sm64;
            }
            /* The last byte has AT MOST 8 bits. */
            nb_bits -= (8 - (nb_rem2 & 7U)) & 7U;
        }
        /* We complete the word. Then we search the first byte. */
        sm64 += ((8 - (NB_BYTES_GFqnv & 7U)) & 7U) + (HFEmq8 & 7U);
    }
}


/* Precomputation for one secret-key */
void PQCLEAN_GEMSS256_CLEAN_precSignHFESeed(secret_key_HFE *sk_HFE, const UINT *sk) {
    Tnv_gf2 L, U;


    expandSeed((uint8_t *)(sk_HFE->sk_uncomp), (NB_UINT_HFEVPOLY
               + (LTRIANGULAR_NV_SIZE << 1)
               + (LTRIANGULAR_N_SIZE << 1) + SIZE_VECTOR_t) << 3,
               (uint8_t *)sk, SIZE_SEED_SK);

    sk_HFE->S = sk_HFE->sk_uncomp + NB_UINT_HFEVPOLY + (LTRIANGULAR_NV_SIZE << 1)
                + (LTRIANGULAR_N_SIZE << 1) + SIZE_VECTOR_t;
    sk_HFE->T = sk_HFE->S + MATRIXnv_SIZE;


    /* zero padding for the HFEv polynomial F */
    sk_HFE->F_HFEv = sk_HFE->sk_uncomp;
    PQCLEAN_GEMSS256_CLEAN_cleanMonicHFEv_gf2nx(sk_HFE->F_HFEv);


    /* The random bytes are already generated from a seed */
    L = sk_HFE->sk_uncomp + NB_UINT_HFEVPOLY;
    U = L + LTRIANGULAR_NV_SIZE;

    PQCLEAN_GEMSS256_CLEAN_cleanLowerMatrixnv(L);
    PQCLEAN_GEMSS256_CLEAN_cleanLowerMatrixnv(U);

    /* Generate S^(-1) = L*U */
    PQCLEAN_GEMSS256_CLEAN_mulMatricesLU_gf2_nv(sk_HFE->S, L, U);


    /* The random bytes are already generated from a seed */
    L += (LTRIANGULAR_NV_SIZE << 1);
    U = L + LTRIANGULAR_N_SIZE;

    PQCLEAN_GEMSS256_CLEAN_cleanLowerMatrixn(L);
    PQCLEAN_GEMSS256_CLEAN_cleanLowerMatrixn(U);

    /* Generate T^(-1) = L*U */
    PQCLEAN_GEMSS256_CLEAN_mulMatricesLU_gf2_n(sk_HFE->T, L, U);

}


/* Precomputation for one secret-key */
int PQCLEAN_GEMSS256_CLEAN_precSignHFE(secret_key_HFE *sk_HFE, const UINT **linear_coefs,
                                       const UINT *sk) {
    cst_sparse_monic_gf2nx F_HFEv;
    sparse_monic_gf2nx F;
    sparse_monic_gf2nx F_cp;
    unsigned int i;

    PQCLEAN_GEMSS256_CLEAN_precSignHFESeed(sk_HFE, sk);

    PQCLEAN_GEMSS256_CLEAN_initListDifferences_gf2nx(sk_HFE->F_struct.L);

    F = sk_HFE->F_struct.poly;
    F_HFEv = sk_HFE->F_HFEv;

    unsigned int j;

    /* X^(2^0) */
    linear_coefs[0] = F_HFEv + MQv_GFqn_SIZE;
    /* X^(2^1) */
    F_HFEv = linear_coefs[0] + MLv_GFqn_SIZE;
    F_cp = F + 2 * NB_WORD_GFqn;
    for (i = 0; i < HFEDegI; ++i) {
        /* Copy i quadratic terms */

        j = 0;
        for (; j < i; ++j) {
            /* X^(2^i + 2^j) */
            copy_gf2n(F_cp, F_HFEv);
            F_HFEv += NB_WORD_GFqn;
            F_cp += NB_WORD_GFqn;
        }
        /* Store the address of X^(2^(i+1)) */
        linear_coefs[i + 1] = F_HFEv;
        /* Linear term is not copied */
        F_HFEv += MLv_GFqn_SIZE;
        F_cp += NB_WORD_GFqn;
    }

    return 0;
}


/* Format of the signature: S_NB_ITE||X_NB_ITE||...||X1
    _ S_NB_ITE||X_NB_ITE: n+v bits
    _ X_(NB_ITE-1): (n+v-m) bits. When the signature is uncompressed, the first
      word is padding with m%64 zero bits. It is to simply the concatenation of
      S_(NB_ITE-1) and X_(NB_ITE-1) during the verifying process.
    _ It is the same format for X_(NB_ITE-2),...,X1
*/


/**
 * @brief   Generate the signature of the document m of length len bytes, using
 * a (HFEv-)-based signature scheme.
 * @details The Feistel-Patarin scheme is used with NB_ITE rounds.
 * @param[in]   m   A pointer on a document.
 * @param[in]   len The length in bytes of the document m.
 * @param[in]   sk  The secret-key, which is the concatenation of a HFEv
 * polynomial in GF(2^n)[X], the S^(-1) matrix (n+v,n+v) in GL_{n+v}(GF(2))
 * and the T^(-1) matrix (n,n) in GLn(GF(2)).
 * @param[out]  sm8 The signature of m generated by a (HFEv-)-based signature
 * scheme. Its length is SIZE_SIGN_HFE bytes, and more precisely
 * SIZE_SIGN_THEORETICAL_HFE bits. The last byte is padded with zero bits when
 * necessary.
 * @return   0 if the result is correct, ERROR_ALLOC for error from
 * malloc/calloc functions.
 * @remark  Requirement: when SSE or AVX is enabled, the secret-key must be
 * aligned respectively on 16 or 32 bytes. However, this requirement and the
 * alignment are disabled for the public/stable version of MQsoft (to be simple
 * to use, generic for the allocation of sk and to avoid segmentation faults).
 * @remark  A part of the implementation is not in constant-time.
 * @todo    To simply the use of the memory alignment for the secret-key.
 */

int PQCLEAN_GEMSS256_CLEAN_signHFE_FeistelPatarin(unsigned char *sm8,
        const unsigned char *m, size_t len,
        const UINT *sk) {
    UINT sm[SIZE_SIGN_UNCOMPRESSED - SIZE_SALT_WORD] = {0};

    static_vecnv_gf2 DR[NB_WORD_GF2nv] = {0};
    static_gf2n U[NB_WORD_GFqn] = {0};
    UINT Hi_tab[SIZE_DIGEST_UINT] = {0};
    UINT Hi1_tab[SIZE_DIGEST_UINT] = {0};
    UINT *tmp, *Hi = Hi_tab, *Hi1 = Hi1_tab;
    unsigned int k;
    unsigned int index;
    unsigned char *DR_cp = (unsigned char *)DR;
    unsigned char rem_char;

    int nb_root;
    secret_key_HFE sk_HFE = {0};

    UINT *F;
    unsigned int i;
    static_vecv_gf2 V[NB_WORD_GFqv];
    static_gf2n tmp_n[NB_WORD_GFqn];


    const UINT *linear_coefs[HFEDegI + 1];
    nb_root = PQCLEAN_GEMSS256_CLEAN_precSignHFE(&sk_HFE, linear_coefs, sk);
    if (nb_root) {
        /* Error from malloc */
        return nb_root;
    }
    F = (UINT *)sk_HFE.F_struct.poly;


    /* Compute H1 = H(m) */
    HASH((unsigned char *)Hi, m, len);

    for (k = 1; k <= NB_ITE; ++k) {

        /* Compute xor(D_k,S_(k-1)) */
        add_gf2m(DR, sm, Hi);

        /* Clean the last char to compute rem_char (the last word is
           cleaned) */
        DR[NB_WORD_GF2m - 1] &= MASK_GF2m;
        /* Save the last byte because we need to erase this value by
         * randombytes */
        rem_char = DR_cp[NB_BYTES_GFqm - 1];

        /* When the root finding fails, the minus and vinegars are
           regenerated */
        do {
            /* Compute Dk||Rk: add random to have n bits, without erased the m
             * bits */
            /* Generation of Rk */
            randombytes(DR_cp + NB_BYTES_GFqm - 1,
                        (unsigned long long)(NB_BYTES_GFqn - NB_BYTES_GFqm + 1));
            /* Put HFEm&7 first bits to 0 */
            DR_cp[NB_BYTES_GFqm - 1] &= ~((1U << (4)) - 1);
            /* Store rem_char */
            DR_cp[NB_BYTES_GFqm - 1] ^= rem_char;

            /* To clean the last char (because of randombytes), the last word is
             * cleaned */
            DR[NB_WORD_GFqn - 1] &= MASK_GF2n;

            /* Compute Sk||Xk = Inv_p(Dk,Rk) */
            /* Firstly: compute c * T^(-1) */
            vecMatProductn_gf2(U, DR, sk_HFE.T);

            /* Secondly: find v with F_HFE(v) = U */

            /* Generation of vinegar variables: v bits */
            randombytes((unsigned char *)V,
                        (unsigned long long)(NB_BYTES_GFqv));
            /* Clean the last word */
            V[NB_WORD_GFqv - 1] &= HFE_MASKv;

            /* Evaluation of the constant, quadratic map with v vinegars */
            evalMQSv_gf2(F, V, sk_HFE.F_HFEv);

            /* Evaluation of the linear terms, linear maps with v
               vinegars */
            for (i = 0; i <= HFEDegI; ++i) {
                vecMatProductv_gf2(tmp_n, V, linear_coefs[i]
                                   + NB_WORD_GFqn);
                add_gf2n(F + NB_WORD_GFqn * (((i * (i + 1)) >> 1) + 1),
                         linear_coefs[i], tmp_n);
            }

            nb_root = PQCLEAN_GEMSS256_CLEAN_chooseRootHFE_gf2nx(DR, sk_HFE.F_struct, U);
            if (!nb_root) {
                /* fail: retry with an other Rk */
                continue;
            }
            if (nb_root < 0) {
                /* Error from chooseRootHFE */
                return nb_root;
            }

            break;
        } while (1);


        /* Add the v bits to DR */
        DR[NB_WORD_GFqn - 1] ^= V[0] << HFEnr;
        for (i = 0; (int)i < (NB_WORD_GFqv - 1); ++i) {
            DR[NB_WORD_GFqn + i] = (V[i] >> (64 - HFEnr)) ^ (V[i + 1] << HFEnr);
        }
        DR[NB_WORD_GFqn + i] = V[i] >> (64 - HFEnr);

        /* Finally: compute Sk||Xk = v * S^(-1) */
        vecMatProductnv_gf2(sm, DR, sk_HFE.S);

        if (k != NB_ITE) {

            /* Store X1 in the signature */
            index = NB_WORD_GF2nv + (NB_ITE - 1 - k) * NB_WORD_GF2nvm;
            copy_gf2nvm(sm + index, sm + NB_WORD_GF2nv - NB_WORD_GF2nvm);
            /* To put zeros at the beginning of the first word of X1 */
            sm[index] &= MASK_GF2nvm;

            /* Compute H2 = H(H1) */
            HASH((unsigned char *)Hi1, (unsigned char *)Hi, SIZE_DIGEST);

            /* Permutation of pointers */
            tmp = Hi1;
            Hi1 = Hi;
            Hi = tmp;
        }
    }

    compress_signHFE(sm8, sm);

    return 0;
}
