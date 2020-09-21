#include "rem_gf2n.h"
#include "rem_gf2x.h"
#include "simd.h"



/***********************************************************************/
/***********************************************************************/
/************************ Without SIMD version *************************/
/***********************************************************************/
/***********************************************************************/


/**
 * @brief   Reduction in GF(2^n) of a (2n-1)-coefficients polynomial in
 * GF(2)[x].
 * @param[in]   Pol A (2n-1)-coefficients polynomial in GF(2)[x].
 * @param[out]  P   P is Pol reduced in GF(2^n).
 * @remark  Requirement: the n-degree irreducible polynomial defining GF(2^n)
 * must be a trinomial or a pentanomial.
 * @remark  Constant-time implementation.
 * @todo    >>6 and &63 are not generic for the UINT type.
 */
void PQCLEAN_GEMSSRED192_AVX2_rem_gf2n_ref(static_gf2n P[NB_WORD_GFqn],
        cst_static_gf2x Pol[NB_WORD_MUL]) {
    static_gf2x res[NB_WORD_MUL];
    UINT bit_i;
    unsigned int i, ind;

    for (i = 0; i < NB_WORD_MUL; ++i) {
        res[i] = Pol[i];
    }

    for (i = (HFEn - 1) << 1; i >= HFEn; --i) {
        /* Extraction of bit_i x^i */
        bit_i = (res[i >> 6] >> (i & 63))&UINT_1;
        /* x^n = 1 + ... */
        ind = i - HFEn;
        res[ind >> 6] ^= bit_i << (ind & 63);
        /* ... + x^K3 */
        ind = i - HFEn + K3;
        res[ind >> 6] ^= bit_i << (ind & 63);
    }

    for (i = 0; i < NB_WORD_GFqn; ++i) {
        P[i] = res[i];
    }

    P[NB_WORD_GFqn - 1] &= MASK_GF2n;
}


/**
 * @brief   Reduction in GF(2^n) of a (2n-1)-coefficients polynomial in
 * GF(2)[x].
 * @param[in]   Pol A (2n-1)-coefficients polynomial in GF(2)[x].
 * @param[out]  P   P is Pol reduced in GF(2^n).
 * @remark  Requirement: the n-degree irreducible polynomial defining GF(2^n)
 * must be a trinomial or a pentanomial.
 * @remark  Requirement: K3<33, or (n,K3) in {(265,42),(266,47),(354,99),
 * (358,57)}.
 * @remark  Requirement: K1<K2<33.
 * @remark  Constant-time implementation.
 */
void PQCLEAN_GEMSSRED192_AVX2_rem_gf2n_ref2(static_gf2n P[NB_WORD_GFqn],
        cst_static_gf2x Pol[NB_WORD_MUL]) {
    uint64_t R;
    unsigned int i;


    static_gf2x Q[NB_WORD_GFqn];

    /* Q: Quotient of Pol/x^n, by word of 64-bit */
    for (i = NB_WORD_GFqn; i < NB_WORD_MMUL; ++i) {
        Q[i - NB_WORD_GFqn] = (Pol[i - 1] >> KI) ^ (Pol[i] << KI64);
    }
    Q[i - NB_WORD_GFqn] = Pol[i - 1] >> KI;

    for (i = 0; i < NB_WORD_GFqn; ++i) {
        P[i] = Pol[i] ^ Q[i];
    }

    P[0] ^= Q[0] << K3mod64;
    for (i = 1; i < NB_WORD_GFqn; ++i) {
        P[i] ^= (Q[i - 1] >> K364) ^ (Q[i] << K3mod64);
    }

    /* R: Quotient of Pol/x^(2n-K3), by word of 64-bit */
    R = (Q[NB_WORD_GFqn - 2] >> (K364 + KI))
        ^ (Q[NB_WORD_GFqn - 1] << (K3mod64 - KI));

    P[0] ^= R;
    P[0] ^= R << K3mod64;
    (P)[1] ^= R >> K364;
    P[NB_WORD_GFqn - 1] &= MASK_GF2n;
}


/**
 * @brief   Reduction in GF(2^n) of a (2n-1)-coefficients square in GF(2)[x].
 * @details The odd degree terms are assumed to be null, and so are not
 * considered.
 * @param[in]   Pol A (2n-1)-coefficients square in GF(2)[x].
 * @param[out]  P   P is Pol reduced in GF(2^n).
 * @remark  Requirement: the odd degree terms of Pol are null.
 * @remark  Requirement: the n-degree irreducible polynomial defining GF(2^n)
 * must be a trinomial or a pentanomial.
 * @remark  Constant-time implementation.
 * @todo    >>6 and &63 are not generic for the UINT type.
 */
void PQCLEAN_GEMSSRED192_AVX2_remsqr_gf2n_ref(static_gf2n P[NB_WORD_GFqn],
        cst_static_gf2x Pol[NB_WORD_MUL]) {
    static_gf2x res[NB_WORD_MUL];
    UINT bit_i;
    unsigned int i, ind;

    for (i = 0; i < NB_WORD_MUL; ++i) {
        res[i] = Pol[i];
    }

    /* Only the even degree terms are not zero */
    for (i = (HFEn - 1) << 1; i >= (HFEn - 1 + K3); i -= 2) {
        /* Extraction of bit_i x^i */
        bit_i = (res[i >> 6] >> (i & 63))&UINT_1;
        /* x^n = 1 + ... */
        ind = i - HFEn;
        res[ind >> 6] ^= bit_i << (ind & 63);
        /* ... + x^K3 */
        ind = i - HFEn + K3;
        res[ind >> 6] ^= bit_i << (ind & 63);
    }

    for (++i; i >= HFEn; --i) {
        /* Extraction of bit_i x^i */
        bit_i = (res[i >> 6] >> (i & 63))&UINT_1;
        /* x^n = 1 + ... */
        ind = i - HFEn;
        res[ind >> 6] ^= bit_i << (ind & 63);
        /* ... + x^K3 */
        ind = i - HFEn + K3;
        res[ind >> 6] ^= bit_i << (ind & 63);
    }

    for (i = 0; i < NB_WORD_GFqn; ++i) {
        P[i] = res[i];
    }

    P[NB_WORD_GFqn - 1] &= MASK_GF2n;
}


/* TODO: no generic for the UINT type. */
void PQCLEAN_GEMSSRED192_AVX2_rem_no_simd_gf2n(static_gf2n P[NB_WORD_GFqn],
        cst_static_gf2x Pol[NB_WORD_MUL]) {
    uint64_t Q[NB_WORD_GFqn];
    uint64_t R;
    REM_GF2N(P, Pol, Q, R);
}
