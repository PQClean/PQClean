#include "arch.h"
#include "changeVariablesMQS_gf2.h"
#include "config_HFE.h"
#include "convMQS_gf2.h"
#include "genSecretMQS_gf2.h"
#include "hash.h"
#include "invMatrixn_gf2.h"
#include "invMatrixnv_gf2.h"
#include "mixEquationsMQS_gf2.h"
#include "randMatrix_gf2.h"
#include "rand_gf2nx.h"
#include "randombytes.h"
#include "sign_keypairHFE.h"
#include "sizes_HFE.h"
#include <stdlib.h>



/* For the RBG */
/* For the expandSeed */


/* Format of the public-key:
    pk is stored as the concatenation of a constant and a triangular upper
    quadratic matrix, both in GF(2^m): the m-th bit of a coefficient is the bit
    of m-th equation.
    The quadratic matrix is the concatenation of the rows,
    without to take the 0 coefficient of the lower part.

    Example with n=4, v=0 and m=2:
    constant: 1+a
    quadratic matrix (n,n) in GF(2^m):
    1   a   1+a a
    0   a   0   1
    0   0   1+a a
    0   0   0   1
    (the term i,j is the monomial xi*xj, and xi^2=xi)

    pk[0]=0x3 (constant term)
    pk[1]=0x1,pk[2]=0x2,pk[3]=0x3,pk[4]=0x2 (first row)
    pk[5]=0x2,pk[6]=0x0,pk[7]=0x1 (second row)
    pk[8]=0x3,pk[9]=0x2 (third row)
    pk[10]=0x1 (fourth row)

    The two equations are:
    1+x0+x0x2+x1x3+x2+x3 (terms with 1)
    1+x0x1+x0x2+x0x3+x1+x2+x2x3 (terms with a)
*/
/* Format of the secret-key:
    sk  is the concatenation of the HFEv polynomial F + the matrix S^(-1) + the
    matrix T^(-1).
    - the HFEv polynomial in sk is just the concatenation of the useful
    coefficients:
    the coefficients of X^0,X^1, and X^(2^i + 2^j) is ascending order (j <= i)

    - The matrices S and T are the concatenation of the rows, and a row uses
    several words: for example, a row of 128 elements of GF(2) is just two
    64-bit words.

    Example of T with n=4:
    1 0 0 1
    1 1 1 1
    0 0 1 0
    0 1 0 1

    T[0]=0x9
    T[1]=0xF
    T[2]=0x4
    T[3]=0xA

    In the code, it is the inverse of S and T which are stored in the
    secret-key.
*/


/**
 * @brief   Generate the secret-key then the public-key of the (HFEv-)-based
 * schemes.
 * @details When ENABLED_SEED_SK is set to 1, sk is a seed.
 * @param[out]  pk  The public-key, a MQ system with m equations in
 * GF(2)[x1,...,x_(n+v)].
 * @param[out]  sk  The secret-key, which is the concatenation of a HFEv
 * polynomial in GF(2^n)[X], the S^(-1) matrix (n+v,n+v) in GL_{n+v}(GF(2))
 * and the T^(-1) matrix (n,n) in GLn(GF(2)).
 * @return   0 if the result is correct, ERROR_ALLOC for error from
 * malloc/calloc functions.
 * @remark  Requires to allocate SIZE_PK_HFE_WORD words for the public-key.
 * @remark  The theoretical minimum size of pk is SIZE_PK_THEORETICAL_HFE bits.
 * @remark  Requires to allocate SIZE_SK_HFE_WORD words for the secret-key.
 * @remark  The theoretical minimum size of sk is SIZE_SK_THEORETICAL_HFE bits.
 * @remark  Constant-time implementation.
 */
int PQCLEAN_GEMSS128_AVX2_sign_keypairHFE(uint8_t *pk, UINT *sk) {
    UINT Q[MQnv_GFqn_SIZE];
    sparse_monic_gf2nx F;
    /* Generate S for the linear transformation on variables */
    UINT S[MATRIXnv_SIZE];
    UINT *T = S;
    Tnv_gf2 L, U;
    UINT sk_uncomp[(NB_UINT_HFEVPOLY +
                    (LTRIANGULAR_NV_SIZE << 1) +
                    (LTRIANGULAR_N_SIZE << 1) +
                    SIZE_VECTOR_t)];
    uint8_t pk_tmp[MQ_GFqm8_SIZE];
    int ret;


    randombytes((uint8_t *)sk, SIZE_SEED_SK);


    /* The seed generates L,U and L',U' such as S=LU and T=L'U' */
    expandSeed((uint8_t *)sk_uncomp, (NB_UINT_HFEVPOLY
                                      + (LTRIANGULAR_NV_SIZE << 1)
                                      + (LTRIANGULAR_N_SIZE << 1) + SIZE_VECTOR_t) << 3,
               (uint8_t *)sk, SIZE_SEED_SK);

    /* zero padding for the HFEv polynomial F */
    F = sk_uncomp;
    PQCLEAN_GEMSS128_AVX2_cleanMonicHFEv_gf2nx(F);

    /* Generate f a secret MQ system,
       with the representation phi^-1(f) = cst + xQxt */

    /* Here, the first element (of GF(2^n)) of Q is reserved to store cst.
       The matrix Q is stored as upper triangular matrix. */

    ret = genSecretMQS_gf2(Q, F);
    if (ret) {
        return ret;
    }

    /* The random bytes are already generated from a seed */
    L = sk_uncomp + NB_UINT_HFEVPOLY;
    U = L + LTRIANGULAR_NV_SIZE;

    PQCLEAN_GEMSS128_AVX2_cleanLowerMatrixnv(L);
    PQCLEAN_GEMSS128_AVX2_cleanLowerMatrixnv(U);

    /* Generate S the inverse of S^(-1) */
    PQCLEAN_GEMSS128_AVX2_invMatrixLUnv_gf2(S, L, U);


    /* Compute Q'=S*Q*St (with Q an upper triangular matrix) */
    changeVariablesMQS_gf2(Q, S);


    /* The random bytes are already generated from a seed */
    L += (LTRIANGULAR_NV_SIZE << 1);
    U = L + LTRIANGULAR_N_SIZE;

    PQCLEAN_GEMSS128_AVX2_cleanLowerMatrixn(L);
    PQCLEAN_GEMSS128_AVX2_cleanLowerMatrixn(U);

    /* Generate T the inverse of T^(-1) */
    PQCLEAN_GEMSS128_AVX2_invMatrixLUn_gf2(T, L, U);


    /* Use T (variable S) to compute cst_pk and Q_pk */
    mixEquationsMQS8_gf2(pk_tmp, Q, T);

    /* Generate the inverse of T */


    convMQS_one_eq_to_hybrid_rep8_gf2(pk, pk_tmp);

    return 0;
}
