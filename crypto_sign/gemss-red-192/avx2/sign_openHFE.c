#include "add_gf2x.h"
#include "arch.h"
#include "choice_crypto.h"
#include "convMQ_gf2.h"
#include "evalMQShybrid_gf2.h"
#include "hash.h"
#include "sign_openHFE.h"
#include "sizes_HFE.h"
#include "tools_gf2m.h"
#include "tools_gf2nv.h"
#include <stdlib.h>



/* Number of bits to complete the byte of sm64, in [0,7] */
#define VAL_BITS_M (((48)<(8-HFEmr8))?(HFEDELTA+HFEv):(8-HFEmr8))

/* Uncompress the signature */
static void uncompress_signHFE(UINT *sm, const unsigned char *sm8) {
    unsigned char *sm64;
    unsigned int k2;

    sm64 = (unsigned char *)sm;
    /* Take the (n+v) first bits */
    for (k2 = 0; k2 < NB_BYTES_GFqnv; ++k2) {
        sm64[k2] = sm8[k2];
    }

    /* Clean the last byte */
    sm64[k2 - 1] &= MASK8_GF2nv;

    /* Take the (Delta+v)*(nb_ite-1) bits */
    unsigned int k1, nb_bits, nb_rem2, nb_rem_m, val_n;
    int nb_rem;

    /* HFEnv bits are already extracted from sm8 */
    nb_bits = HFEnv;
    sm64 += (NB_WORD_GF2nv << 3) + (HFEmq8 & 7U);

    for (k1 = 1; k1 < NB_ITE; ++k1) {
        /* Number of bits to complete the byte of sm8, in [0,7] */
        if ((48) < ((8 - (nb_bits & 7U)) & 7U)) {
            val_n = (48);
        } else {
            val_n = ((8 - (nb_bits & 7U)) & 7U);
        }

        /* First byte of sm8 */
        if (nb_bits & 7U) {
            *sm64 ^= (sm8[nb_bits >> 3] >> (nb_bits & 7U)) << HFEmr8;

            /* Number of bits to complete the first byte of sm8 */
            nb_rem = (int)((val_n - VAL_BITS_M));
            if (nb_rem >= 0) {
                /* We take the next byte since we used
                   VAL_BITS_M bits */
                ++sm64;
            }
            if (nb_rem > 0) {
                nb_bits += VAL_BITS_M;
                *sm64 ^= sm8[nb_bits >> 3] >> (nb_bits & 7U);
                nb_bits += nb_rem;
            } else {
                nb_bits += val_n;
            }
        }

        /* Other bytes of sm8 */
        nb_rem2 = (48) - val_n;
        /*nb_rem2 can be zero only in this case */
        /* Number of bits used of sm64, mod 8 */
        nb_rem_m = (HFEm + val_n) & 7U;

        /* Other bytes */
        if (nb_rem_m) {
            /* -1 to take the ceil of /8, -1 */
            for (k2 = 0; k2 < ((nb_rem2 - 1) >> 3); ++k2) {
                *sm64 ^= sm8[nb_bits >> 3] << nb_rem_m;
                ++sm64;
                *sm64 = sm8[nb_bits >> 3] >> (8 - nb_rem_m);

                nb_bits += 8;
            }
            /* The last byte of sm8, between 1 and 8 bits to put */
            *sm64 ^= sm8[nb_bits >> 3] << nb_rem_m;
            ++sm64;

            /* nb_rem2 between 1 and 8 bits */
            nb_rem2 = ((nb_rem2 + 7U) & 7U) + 1U;
            if (nb_rem2 > (8 - nb_rem_m)) {
                *sm64 = sm8[nb_bits >> 3] >> (8 - nb_rem_m);
                ++sm64;
            }

            nb_bits += nb_rem2;
        } else {
            /* We are at the beginning of the bytes of sm8 and sm64 */

            /* +7 to take the ceil of /8 */
            for (k2 = 0; k2 < ((nb_rem2 + 7) >> 3); ++k2) {
                *sm64 = sm8[nb_bits >> 3];
                nb_bits += 8;
                ++sm64;
            }
            /* The last byte has AT MOST 8 bits. */
            nb_bits -= (8 - (nb_rem2 & 7U)) & 7U;
        }

        /* Clean the last byte */
        sm64[-1] &= MASK8_GF2nv;

        /* We complete the word. Then we search the first byte. */
        sm64 += ((8 - (NB_BYTES_GFqnv & 7U)) & 7U) + (HFEmq8 & 7U);
    }
}


/**
 * @brief   Verify the signature of the document m of length len bytes, using a
 * (HFEv-)-based signature scheme. pk can be evaluated with the eval_pk function
 * @details eval_pk takes 3 arguments here.
 * @param[in]   m   A pointer on a document.
 * @param[in]   len The length in bytes of the document m.
 * @param[in]   sm8 A signature generated by a (HFEv-)-based signature scheme.
 * @param[in]   pk  The public-key, a MQ system with m equations in
 * GF(2)[x1,...,x_(n+v)].
 * @param[in]   eval_pk The function allowing to evaluate pk. This choice
 * depends on the chosen representation of pk.
 * @return  0 for a valid signature, !=0 else.
 * @remark  Requirement: when SSE or AVX is enabled, the public-key must be
 * aligned respectively on 16 or 32 bytes. However, this requirement and the
 * alignment are disabled for the public/stable version of MQsoft (to be simple
 * to use, generic for the allocation of pk and to avoid segmentation faults).
 * @remark  This function does not require a constant-time implementation.
 * @todo    To simply the use of the memory alignment for the public-key.
 */
int PQCLEAN_GEMSSRED192_AVX2_sign_openHFE_uncomp_pk(const unsigned char *m, size_t len,
        const unsigned char *sm8,
        const uint8_t *pk,
        void (*eval_pk)()) {
    UINT sm[SIZE_SIGN_UNCOMPRESSED - SIZE_SALT_WORD] = {0};

    static_vecnv_gf2 Si_tab[NB_WORD_GF2nv] ALIGNED_GFqm,
                     Si1_tab[NB_WORD_GF2nv] ALIGNED_GFqm;
    /* Copy of pointer */
    vecnv_gf2 tmp, Si = Si_tab, Si1 = Si1_tab;
    /* Vector of D_1, ..., D_(NB_ITE) */
    UINT D[NB_ITE * SIZE_DIGEST_UINT];
    unsigned int i;
    unsigned int index;

    uncompress_signHFE(sm, sm8);


    /* Compute H1 = H(m), the m first bits are D1 */
    HASH((unsigned char *)D, m, len);

    for (i = 1; i < NB_ITE; ++i) {
        /* Compute Hi = H(H_(i-1)), the m first bits are Di */
        HASH((unsigned char *)(D + i * SIZE_DIGEST_UINT),
             (unsigned char *)(D + (i - 1)*SIZE_DIGEST_UINT), SIZE_DIGEST);
        /* Clean the previous hash (= extract D_(i-1) from H_(i-1)) */
        D[SIZE_DIGEST_UINT * (i - 1) + NB_WORD_GF2m - 1] &= MASK_GF2m;
    }
    /* Clean the previous hash (= extract D_(i-1) from H_(i-1)) */
    D[SIZE_DIGEST_UINT * (i - 1) + NB_WORD_GF2m - 1] &= MASK_GF2m;

    /* Compute p(S_(NB_IT),X_(NB_IT)) */
    eval_pk(Si, sm, pk);

    for (i = NB_ITE - 1; i > 0; --i) {
        /* Compute Si = xor(p(S_i+1,X_i+1),D_i+1) */
        add2_gf2m(Si, D + i * SIZE_DIGEST_UINT);

        /* Compute Si||Xi */
        index = NB_WORD_GF2nv + (NB_ITE - 1 - i) * NB_WORD_GF2nvm;
        Si[NB_WORD_GF2m - 1] &= MASK_GF2m;

        /* Concatenation(Si,Xi): the intersection between S1 and X1 is
         * not null */
        Si[NB_WORD_GF2m - 1] ^= sm[index];
        ++index;
        copy_gf2nvm1(Si + NB_WORD_GF2m, sm + index);

        /* Compute p(Si,Xi) */
        eval_pk(Si1, Si, pk);

        /* Permutation of pointers */
        tmp = Si1;
        Si1 = Si;
        Si = tmp;

    }

    /* D1'' == D1 */
    return !isEqual_nocst_gf2m(Si, D);
}


/**
 * @brief   Verify the signature of the document m of length len bytes, using a
 * (HFEv-)-based signature scheme. pk can be evaluated with the eval_pk
 * function, and hpk is used during this evaluation.
 * @details eval_pk takes 4 arguments here.
 * @param[in]   m   A pointer on a document.
 * @param[in]   len The length in bytes of the document m.
 * @param[in]   sm8 A signature generated by a (HFEv-)-based signature scheme.
 * @param[in]   pk  The original public-key, a MQ system with m equations in
 * GF(2)[x1,...,x_(n+v)].
 * @param[in]   hpk The hybrid representation of one part of the public-key pk.
 * @param[in]   eval_pk The function allowing to evaluate pk. This choice
 * depends on the chosen representation of pk.
 * @return  0 for a valid signature, !=0 else.
 * @remark  Requirement: when SSE or AVX is enabled, the public-key must be
 * aligned respectively on 16 or 32 bytes. However, this requirement and the
 * alignment are disabled for the public/stable version of MQsoft (to be simple
 * to use, generic for the allocation of pk and to avoid segmentation faults).
 * @remark  This function does not require a constant-time implementation.
 * @todo    To simply the use of the memory alignment for the public-key.
 */
int PQCLEAN_GEMSSRED192_AVX2_sign_openHFE_huncomp_pk(const unsigned char *m, size_t len,
        const unsigned char *sm8,
        const uint8_t *pk, const uint8_t *hpk,
        void (*eval_pk)()) {
    UINT sm[SIZE_SIGN_UNCOMPRESSED - SIZE_SALT_WORD] = {0};

    static_vecnv_gf2 Si_tab[NB_WORD_GF2nv] ALIGNED_GFqm,
                     Si1_tab[NB_WORD_GF2nv] ALIGNED_GFqm;
    UINT cst;
    /* Copy of pointer */
    vecnv_gf2 tmp, Si = Si_tab, Si1 = Si1_tab;
    /* Vector of D_1, ..., D_(NB_ITE) */
    UINT D[NB_ITE * SIZE_DIGEST_UINT];
    unsigned int i;
    unsigned int index;

    cst = *((UINT *)hpk);
    /* We jump the constant (stored on 8 bytes) */
    hpk += 8;

    uncompress_signHFE(sm, sm8);


    /* Compute H1 = H(m), the m first bits are D1 */
    HASH((unsigned char *)D, m, len);

    for (i = 1; i < NB_ITE; ++i) {
        /* Compute Hi = H(H_(i-1)), the m first bits are Di */
        HASH((unsigned char *)(D + i * SIZE_DIGEST_UINT),
             (unsigned char *)(D + (i - 1)*SIZE_DIGEST_UINT), SIZE_DIGEST);
        /* Clean the previous hash (= extract D_(i-1) from H_(i-1)) */
        D[SIZE_DIGEST_UINT * (i - 1) + NB_WORD_GF2m - 1] &= MASK_GF2m;
    }
    /* Clean the previous hash (= extract D_(i-1) from H_(i-1)) */
    D[SIZE_DIGEST_UINT * (i - 1) + NB_WORD_GF2m - 1] &= MASK_GF2m;

    /* Compute p(S_(NB_IT),X_(NB_IT)) */
    eval_pk(Si, sm, pk, hpk);
    Si[HFEmq] ^= cst;

    for (i = NB_ITE - 1; i > 0; --i) {
        /* Compute Si = xor(p(S_i+1,X_i+1),D_i+1) */
        add2_gf2m(Si, D + i * SIZE_DIGEST_UINT);

        /* Compute Si||Xi */
        index = NB_WORD_GF2nv + (NB_ITE - 1 - i) * NB_WORD_GF2nvm;
        Si[NB_WORD_GF2m - 1] &= MASK_GF2m;

        /* Concatenation(Si,Xi): the intersection between S1 and X1 is
         * not null */
        Si[NB_WORD_GF2m - 1] ^= sm[index];
        ++index;
        copy_gf2nvm1(Si + NB_WORD_GF2m, sm + index);

        /* Compute p(Si,Xi) */
        eval_pk(Si1, Si, pk, hpk);
        Si1[HFEmq] ^= cst;

        /* Permutation of pointers */
        tmp = Si1;
        Si1 = Si;
        Si = tmp;

    }

    /* D1'' == D1 */
    return !isEqual_nocst_gf2m(Si, D);
}


/**
 * @brief   Verify the signature of the document m of length len bytes, using a
 * (HFEv-)-based signature scheme.
 * @param[in]   m   A pointer on a document.
 * @param[in]   len The length in bytes of the document m.
 * @param[in]   sm8 A signature generated by a (HFEv-)-based signature scheme.
 * @param[in]   pk  The public-key, a MQ system with m equations in
 * GF(2)[x1,...,x_(n+v)].
 * @return  0 for a valid signature, !=0 else.
 * @remark  Requirement: when SSE or AVX is enabled, the public-key must be
 * aligned respectively on 16 or 32 bytes. However, this requirement and the
 * alignment are disabled for the public/stable version of MQsoft (to be simple
 * to use, generic for the allocation of pk and to avoid segmentation faults).
 * @remark  This function does not require a constant-time implementation.
 * @todo    To simply the use of the memory alignment for the public-key.
 */
int PQCLEAN_GEMSSRED192_AVX2_sign_openHFE(const unsigned char *m, size_t len,
        const unsigned char *sm8, const uint8_t *pk) {
    int ret;

    /* Compute p(S_(NB_IT),X_(NB_IT)) */
    UINT *pk_tmp;
    unsigned int i;
    /* 1 to store the constant */
    pk_tmp = (UINT *)malloc((1 + NB_WORD_UNCOMP_EQ * HFEmr8) * sizeof(UINT));

    uint64_t val;
    const uint8_t *pk_cp;

    pk_cp = pk + ACCESS_last_equations8 - 1;
    val = 0;

    for (i = 0; i < (HFEmr8 - 1); ++i) {
        /* Last byte of the equation */
        pk_cp += NB_BYTES_EQUATION;
        val ^= ((uint64_t)((*pk_cp) >> HFENr8)) << (i * HFENr8c);
    }

    UINT cst;
    cst = 0;

    for (i = 0; i < (HFEmr8 - 1); i++) {
        cst ^= PQCLEAN_GEMSSRED192_AVX2_convMQ_uncompressL_gf2(pk_tmp + 1 + i * NB_WORD_UNCOMP_EQ,
                pk + ACCESS_last_equations8 + i * NB_BYTES_EQUATION) << i;
    }

    /* The last equation in input is smaller because compressed */
    cst ^= PQCLEAN_GEMSSRED192_AVX2_convMQ_last_uncompressL_gf2(pk_tmp + 1 + i * NB_WORD_UNCOMP_EQ,
            pk + ACCESS_last_equations8 + i * NB_BYTES_EQUATION) << i;

    /* Number of lost bits by the zero padding of each equation
       (without the last) */
#define LOST_BITS ((HFEmr8-1)*HFENr8c)

    pk_tmp[1 + (i + 1)*NB_WORD_UNCOMP_EQ - 1] ^= val << (HFEnvr - LOST_BITS);

    cst <<= HFEmr - HFEmr8;
    *pk_tmp = cst;


    ret = PQCLEAN_GEMSSRED192_AVX2_sign_openHFE_huncomp_pk(m, len, sm8, pk, (uint8_t *)pk_tmp,
            evalMQShybrid8_nocst_gf2_m);


    free(pk_tmp);

    return ret;
}
