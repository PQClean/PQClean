#include "add_gf2n.h"
#include "arch.h"
#include "config_HFE.h"
#include "dotProduct_gf2n.h"
#include "genCanonicalBasis_gf2n.h"
#include "genSecretMQS_gf2.h"
#include "gf2nx.h"
#include "mul_gf2n.h"
#include "rem_gf2n.h"
#include "tools_gf2n.h"
#include <stdlib.h>



/*****************************************************************************/
/*****************************************************************************/
/********************************* REFERENCE *********************************/
/*****************************************************************************/
/*****************************************************************************/


/* Compute a secret MQ System from a HFE polynomial */
/* This version replaces directly X by sum a_i x_i in the HFE polynomial F. */
/* For each term of F, X is replaced by sum a_i x_i. */
/* This version is the slowest, because each multiplication is following by a
modular reduction. */

#define LINEAR_VCASE_REF(a_veci) \
    (a_veci)-=(HFEn-1)*NB_WORD_GFqn;\
    MQS_cp=MQS+(HFEn+1)*NB_WORD_GFqn;\
    for(j=0;j<HFEv;++j)\
    {\
        add2_gf2n(MQS_cp,F_cp+j*NB_WORD_GFqn);\
        MQS_cp+=NB_WORD_GFqn;\
    }\
    \
    for(ja=1;ja<HFEn;++ja)\
    {\
        MQS_cp+=(HFEn-ja)*NB_WORD_GFqn;\
        for(j=0;j<HFEv;++j)\
        {\
            mul_gf2n(tmp1,F_cp+j*NB_WORD_GFqn,a_veci);\
            add2_gf2n(MQS_cp,tmp1);\
            MQS_cp+=NB_WORD_GFqn;\
        }\
        (a_veci)+=NB_WORD_GFqn;\
    }\
    F_cp+=HFEv*NB_WORD_GFqn;


#define LINEAR_CASE_INIT_REF(a_vec) \
    lin_cp=lin;\
    /* j=0 : mul(*F_cp,1) */\
    copy_gf2n(lin_cp,F_cp);\
    lin_cp+=NB_WORD_GFqn;\
    \
    for(j=1;j<HFEn;++j)\
    {\
        mul_gf2n(lin_cp,F_cp,a_vec);\
        (a_vec)+=NB_WORD_GFqn;\
        lin_cp+=NB_WORD_GFqn;\
    }\
    F_cp+=NB_WORD_GFqn;


#define LINEAR_CASE_REF(a_vec) \
    lin_cp=lin;\
    /* j=0 : mul(*F_cp,1)=*F_cp */\
    add2_gf2n(lin_cp,F_cp);\
    lin_cp+=NB_WORD_GFqn;\
    \
    for(j=1;j<HFEn;++j)\
    {\
        mul_gf2n(tmp1,F_cp,a_vec);\
        add2_gf2n(lin_cp,tmp1);\
        (a_vec)+=NB_WORD_GFqn;\
        lin_cp+=NB_WORD_GFqn;\
    }\
    F_cp+=NB_WORD_GFqn;\
    LINEAR_VCASE_REF(a_vec);


/* Replace *F_cp by 1 */
#define LINEAR_MONIC_CASE_REF(a_vec) \
    lin_cp=lin;\
    /* j=0 : mul(*F_cp,1)=*F_cp */\
    *lin_cp^=1;\
    lin_cp+=NB_WORD_GFqn;\
    \
    for(j=1;j<HFEn;++j)\
    {\
        add2_gf2n(lin_cp,a_vec);\
        (a_vec)+=NB_WORD_GFqn;\
        lin_cp+=NB_WORD_GFqn;\
    }


#define JUMP_VINEGAR_REF MQS_cp+=HFEv*NB_WORD_GFqn;

/* Compute (*F_cp)*a_vec[ia]*a_vec[ja] */
/* a_vec[ia]*a_vec[ja] is the term x_ia * x_(ja+1) */
#define QUADRATIC_CASE_INIT_REF(a_vec,a_vecj) \
    /* One term */\
    MQS_cp=MQS+NB_WORD_GFqn;\
    \
    /* ia = 0 */\
    \
    /* Compute the coefficient of x_0^2 : it is (a^0)^2 = 1 */\
    copy_gf2n(MQS_cp,F_cp);\
    MQS_cp+=NB_WORD_GFqn;\
    \
    /* Compute the coefficient of x_0*x_(ja+1) : it is 1 for x_0 */\
    for(ja=0;ja<HFEn-1;++ja)\
    {\
        /* x_0*x_(ja+1) + x_(ja+1)*x_0 */\
        add_gf2n(tmp1,(a_vecj)+ja*NB_WORD_GFqn,(a_vec)+ja*NB_WORD_GFqn);\
        mul_gf2n(MQS_cp,tmp1,F_cp);\
        MQS_cp+=NB_WORD_GFqn;\
    }\
    \
    JUMP_VINEGAR_REF;\
    for(ia=1;ia<HFEn;++ia)\
    {\
        mul_gf2n(tmp_i,a_vec,F_cp);\
        mul_gf2n(tmp_j,a_vecj,F_cp);\
        \
        /* Compute the coefficient of x_ia^2 */\
        mul_gf2n(MQS_cp,a_vec,tmp_j);\
        MQS_cp+=NB_WORD_GFqn;\
        \
        /* Compute the coefficient of x_ia*x_(ja+1) */\
        for(ja=1;ja<(HFEn-ia);++ja)\
        {\
            /* Compute the coefficient of x_ia*x_(ja+1) */\
            mul_gf2n(tmp1,tmp_i,(a_vecj)+ja*NB_WORD_GFqn);\
            copy_gf2n(MQS_cp,tmp1);\
            /* Compute the coefficient of x_(ja+1)*x_ia */\
            mul_gf2n(tmp1,tmp_j,(a_vec)+ja*NB_WORD_GFqn);\
            add2_gf2n(MQS_cp,tmp1);\
            MQS_cp+=NB_WORD_GFqn;\
        }\
        JUMP_VINEGAR_REF;\
        (a_vec)+=NB_WORD_GFqn;\
        (a_vecj)+=NB_WORD_GFqn;\
    }\
    F_cp+=NB_WORD_GFqn;


/* Compute (*F_cp)*a_vec[ia]*a_vec[ja] */
/* a_vec[ia]*a_vec[ja] is the term x_ia * x_(ja+1) */
#define QUADRATIC_CASE_REF(a_veci,a_vecj) \
    /* One term */\
    MQS_cp=MQS+NB_WORD_GFqn;\
    /* Here a_veci = row i */\
    /* Here, a_vecj = row j */\
    \
    /* ia = 0 */\
    \
    /* Compute the coefficient of x_0^2 : it is (a^0)^2 = 1 */\
    add2_gf2n(MQS_cp,F_cp);\
    MQS_cp+=NB_WORD_GFqn;\
    \
    /* Compute the coefficient of x_0*x_(ja+1) : it is 1 for x_0 */\
    for(ja=0;ja<HFEn-1;++ja)\
    {\
        /* x_0*x_(ja+1) + x_(ja+1)*x_0 */\
        add_gf2n(tmp1,(a_vecj)+ja*NB_WORD_GFqn,(a_veci)+ja*NB_WORD_GFqn);\
        mul_gf2n(tmp_i,tmp1,F_cp);\
        add2_gf2n(MQS_cp,tmp_i);\
        MQS_cp+=NB_WORD_GFqn;\
    }\
    \
    JUMP_VINEGAR_REF;\
    for(ia=1;ia<HFEn;++ia)\
    {\
        mul_gf2n(tmp_i,a_veci,F_cp);\
        mul_gf2n(tmp_j,a_vecj,F_cp);\
        \
        /* Compute the coefficient of x_ia^2 */\
        mul_gf2n(tmp1,a_veci,tmp_j);\
        add2_gf2n(MQS_cp,tmp1);\
        MQS_cp+=NB_WORD_GFqn;\
        \
        /* Compute the coefficient of x_ia*x_(ja+1) */\
        for(ja=1;ja<(HFEn-ia);++ja)\
        {\
            /* Compute the coefficient of x_ia*x_(ja+1) */\
            mul_gf2n(tmp1,tmp_i,(a_vecj)+ja*NB_WORD_GFqn);\
            add2_gf2n(MQS_cp,tmp1);\
            /* Compute the coefficient of x_(ja+1)*x_ia */\
            mul_gf2n(tmp1,tmp_j,(a_veci)+ja*NB_WORD_GFqn);\
            add2_gf2n(MQS_cp,tmp1);\
            MQS_cp+=NB_WORD_GFqn;\
        }\
        JUMP_VINEGAR_REF;\
        (a_veci)+=NB_WORD_GFqn;\
        (a_vecj)+=NB_WORD_GFqn;\
    }\
    /* Here, a_veci = row i+1 */\
    /* Here, a_vecj = row j+1 */\
    F_cp+=NB_WORD_GFqn;


/* Replace *F_cp by 1 */
#define QUADRATIC_MONIC_CASE_REF(a_veci,a_vecj) \
    /* One term */\
    MQS_cp=MQS+NB_WORD_GFqn;\
    /* Here a_veci = row i */\
    /* Here, a_vecj = row j */\
    \
    /* ia = 0 */\
    \
    /* Compute the coefficient of x_0^2 : it is (a^0)^2 = 1 */\
    *MQS_cp^=1;\
    MQS_cp+=NB_WORD_GFqn;\
    \
    /* Compute the coefficient of x_0*x_(ja+1) : it is 1 for x_0 */\
    for(ja=0;ja<HFEn-1;++ja)\
    {\
        /* x_0*x_(ja+1) + x_(ja+1)*x_0 */\
        add_gf2n(tmp1,(a_vecj)+ja*NB_WORD_GFqn,(a_veci)+ja*NB_WORD_GFqn);\
        add2_gf2n(MQS_cp,tmp1);\
        MQS_cp+=NB_WORD_GFqn;\
    }\
    \
    JUMP_VINEGAR_REF;\
    for(ia=1;ia<HFEn;++ia)\
    {\
        /* Compute the coefficient of x_ia^2 */\
        mul_gf2n(tmp1,a_veci,a_vecj);\
        add2_gf2n(MQS_cp,tmp1);\
        MQS_cp+=NB_WORD_GFqn;\
        \
        /* Compute the coefficient of x_ia*x_(ja+1) */\
        for(ja=1;ja<(HFEn-ia);++ja)\
        {\
            /* Compute the coefficient of x_ia*x_(ja+1) */\
            mul_gf2n(tmp1,a_veci,(a_vecj)+ja*NB_WORD_GFqn);\
            add2_gf2n(MQS_cp,tmp1);\
            /* Compute the coefficient of x_(ja+1)*x_ia */\
            mul_gf2n(tmp1,a_vecj,(a_veci)+ja*NB_WORD_GFqn);\
            add2_gf2n(MQS_cp,tmp1);\
            MQS_cp+=NB_WORD_GFqn;\
        }\
        JUMP_VINEGAR_REF;\
        (a_veci)+=NB_WORD_GFqn;\
        (a_vecj)+=NB_WORD_GFqn;\
    }\
    /* Here, a_veci = row i+1 */\
    /* Here, a_vecj = row j+1 */


/* Replace *F_cp by 1 */
#define QUADRATIC_MONIC_CASE_INIT_REF(a_vec,a_vecj) \
    /* One term */\
    MQS_cp=MQS+NB_WORD_GFqn;\
    \
    /* ia = 0 */\
    \
    /* Compute the coefficient of x_0^2 : it is (a^0)^2 = 1 */\
    set1_gf2n(MQS_cp);\
    MQS_cp+=NB_WORD_GFqn;\
    \
    /* Compute the coefficient of x_0*x_(ja+1) : it is 1 for x_0 */\
    for(ja=0;ja<HFEn-1;++ja)\
    {\
        /* x_0*x_(ja+1) + x_(ja+1)*x_0 */\
        add_gf2n(MQS_cp,(a_vecj)+ja*NB_WORD_GFqn,(a_vec)+ja*NB_WORD_GFqn);\
        MQS_cp+=NB_WORD_GFqn;\
    }\
    \
    JUMP_VINEGAR_REF;\
    for(ia=1;ia<HFEn;++ia)\
    {\
        /* Compute the coefficient of x_ia^2 */\
        mul_gf2n(MQS_cp,a_vec,a_vecj);\
        MQS_cp+=NB_WORD_GFqn;\
        \
        /* Compute the coefficient of x_ia*x_(ja+1) */\
        for(ja=1;ja<(HFEn-ia);++ja)\
        {\
            /* Compute the coefficient of x_ia*x_(ja+1) */\
            mul_gf2n(tmp1,a_vec,(a_vecj)+ja*NB_WORD_GFqn);\
            copy_gf2n(MQS_cp,tmp1);\
            /* Compute the coefficient of x_(ja+1)*x_ia */\
            mul_gf2n(tmp1,a_vecj,(a_vec)+ja*NB_WORD_GFqn);\
            add2_gf2n(MQS_cp,tmp1);\
            MQS_cp+=NB_WORD_GFqn;\
        }\
        JUMP_VINEGAR_REF;\
        (a_vec)+=NB_WORD_GFqn;\
        (a_vecj)+=NB_WORD_GFqn;\
    }



/**
 * @brief   Computation of the multivariate representation of a HFEv polynomial.
 * @details Here, for each term of F, X is replaced by sum a_i x_i.
 * @param[in]   F   A monic HFEv polynomial in GF(2^n)[X,x_(n+1),...,x_(n+v)]
 * stored with a sparse representation.
 * @param[out]  MQS The multivariate representation of F, a MQ system with
 * n equations in GF(2)[x1,...,x_(n+v)]. MQS is stored as one equation in
 * GF(2^n)[x1,...,x_(n+v)] (monomial representation + quadratic form cst||Q).
 * @return   0 if the result is correct, ERROR_ALLOC for error from
 * malloc/calloc functions.
 * @remark  Requires to allocate MQnv_GFqn_SIZE words for MQS.
 * @remark  Requirement: F is monic.
 * @remark  Constant-time implementation.
 */
int PQCLEAN_GEMSSBLUE256_AVX2_genSecretMQS_gf2_ref(mqsnv_gf2n MQS, cst_sparse_monic_gf2nx F) {
    /* if there is not quadratic terms X^(2^i + 2^j) */
    mqsnv_gf2n MQS_cp;
    UINT lin[HFEn * NB_WORD_GFqn] = {0};
    UINT *lin_cp;
    cst_vec_gf2n a_vec;
    unsigned int i, j;
    static_gf2n tmp1[NB_WORD_GFqn];
    static_gf2n tmp_i[NB_WORD_GFqn], tmp_j[NB_WORD_GFqn];
    cst_sparse_monic_gf2nx F_cp;
    cst_vec_gf2n a_veci, a_vecj;
    unsigned int ia, ja;

    /* Precompute alpha_vec is disabled in the submission */
    /* Matrix in GF(2^n) with (HFEDegI+1) rows and HFEn-1 columns */
    /* calloc is useful when it initialises a multiple precision element
       to 1 */
    UINT alpha_vec[(HFEDegI + 1) * (HFEn - 1)*NB_WORD_GFqn] = {0};

    PQCLEAN_GEMSSBLUE256_AVX2_genCanonicalBasis_gf2n(alpha_vec);

    /* Constant : copy the first coefficient of F in MQS */
    copy_gf2n(MQS, F);

    /* The case X^0 is just the coef of X^0 of F,
       so it is useless to return the constant of MQ System */
    F_cp = F + NB_WORD_GFqn;
    /* +NB_WORD_GFqn because the constant is counted 2 times */
    MQS_cp = MQS + MQnv_GFqn_SIZE - MQv_GFqn_SIZE + NB_WORD_GFqn;
    /* Copy the linear and quadratic terms of the constant in
       GF(2^n)[y1,...,yv] */
    for (i = 1; i < NB_MONOMIAL_VINEGAR; ++i) {
        copy_gf2n(MQS_cp, F_cp);
        MQS_cp += NB_WORD_GFqn;
        F_cp += NB_WORD_GFqn;
    }

    a_vec = alpha_vec;
    /* Linear term X */

    /* Monic case */

    LINEAR_CASE_INIT_REF(a_vec);
    a_veci = alpha_vec;
    MQS_cp = MQS + (HFEn + 1) * NB_WORD_GFqn;
    for (j = 0; j < HFEv; ++j) {
        copy_gf2n(MQS_cp, F_cp + j * NB_WORD_GFqn);
        MQS_cp += NB_WORD_GFqn;
    }

    for (i = 1; i < HFEn; ++i) {
        MQS_cp += (HFEn - i) * NB_WORD_GFqn;
        for (j = 0; j < HFEv; ++j) {
            mul_gf2n(MQS_cp, F_cp + j * NB_WORD_GFqn, a_veci);
            MQS_cp += NB_WORD_GFqn;
        }
        a_veci += NB_WORD_GFqn;
    }
    F_cp += HFEv * NB_WORD_GFqn;

    /* Linear term X^2 */
    /* Here a_vec = row 1 */
    LINEAR_CASE_REF(a_veci);

    /* Quadratic term X^3 */
    /* The quadratic terms of MQS are not initialised */
    a_vecj = alpha_vec;
    QUADRATIC_CASE_INIT_REF(a_vec, a_vecj);

    /* Here a_vec = row 2 */
    /* Here a_veci = row 2 */

    /* Linear term X^4 */
    LINEAR_CASE_REF(a_veci);

    /* Other terms, begin at X^5 */
    /* The current term is X^(q^i + q^j) */
    for (i = 2; i < HFEDegI; ++i) {
        j = 0;
        /* Here a_vec = row i */
        a_vecj = alpha_vec + j * (HFEn - 1) * NB_WORD_GFqn;
        for (; j < i; ++j) {
            a_veci = a_vec;
            QUADRATIC_CASE_REF(a_veci, a_vecj);
        }
        a_vec = a_veci;
        /* Here a_vec = row i+1 */

        /* j=i */
        LINEAR_CASE_REF(a_veci);
    }


    /* Remainder */
    /* i = HFEDegi */
    /* The current term is X^(q^HFEDegi + q^j) */

    /* Here a_vec = row i */
    j = 0;
    /* Here a_vec = row i */
    a_vecj = alpha_vec + j * (HFEn - 1) * NB_WORD_GFqn;
    /* Here a_veci = row i+1 */

    /* j=HFEDegJ */
    a_veci = a_vec;
    QUADRATIC_MONIC_CASE_REF(a_veci, a_vecj);

    /* Put linear part on "diagonal" of MQS */
    lin_cp = lin;
    MQS_cp = MQS + NB_WORD_GFqn;
    for (i = HFEnv; i > HFEv; --i) {
        add2_gf2n(MQS_cp, lin_cp);
        lin_cp += NB_WORD_GFqn;
        MQS_cp += i * NB_WORD_GFqn;
    }

    return 0;
}



/*****************************************************************************/
/*****************************************************************************/
/********************************* OPTIMIZED *********************************/
/*****************************************************************************/
/*****************************************************************************/


/* Compute a secret MQ System from a HFE polynomial */
/* The complexity of the used algorithm is the best:
   O(n Log(D) * (n + v + Log(D))) multiplications in GF(2)[x]
   and O(n * (n + v + Log(D))) modular reductions.
*/


#define SIZE_ROW (HFEDegI+1)




static void special_buffer(UINT *buf, cst_sparse_monic_gf2nx F,
                           cst_vec_gf2n alpha_vec) {
    cst_sparse_monic_gf2nx F_cp;
    unsigned int i, j;
    unsigned int k;


    /* Special case: alpha^0 */
    /* F begins to X^3, the first "quadratic" term */
    F += (NB_WORD_GFqn * (HFEv + 1)) << 1;

    /* X^3 */
    copy_gf2n(buf, F);
    buf += NB_WORD_GFqn;

    /* X^5: we jump X^4 because it is linear */
    F_cp = F + NB_WORD_GFqn * (HFEv + 2);
    /* A_i,j X^(2^i + 2^j) */
    for (i = 2; i < (SIZE_ROW - 1); ++i) {
        /* j=0: A_i,0 */
        copy_gf2n(buf, F_cp);
        for (j = 1; j < i; ++j) {
            F_cp += NB_WORD_GFqn;
            add2_gf2n(buf, F_cp);
        }
        buf += NB_WORD_GFqn;
        /* To jump a linear term X^(2^i) */
        F_cp += NB_WORD_GFqn * (HFEv + 2);
    }


    /* Monic case */
    set1_gf2n(buf);
    buf += NB_WORD_GFqn;


    /* Squares of (alpha^(k+1)) */
    for (k = 0; k < (HFEn - 1); ++k) {
        /* X^3: i=1,j=0 */
        mul_gf2n(buf, alpha_vec, F);
        buf += NB_WORD_GFqn;

        /* X^5: we jump X^4 because it is linear */
        F_cp = F + NB_WORD_GFqn * (HFEv + 2);
        /* A_i,j X^(2^i + 2^j) */
        for (i = 2; i < (SIZE_ROW - 1); ++i) {
            dotProduct_gf2n(buf, alpha_vec, F_cp, i);
            buf += NB_WORD_GFqn;

            /* To jump quadratic terms + a linear term X^(2^i) */
            F_cp += (i + HFEv + 1) * NB_WORD_GFqn;
        }

        /* j=0: A_i,0 */
        /* Monic case */
        copy_gf2n(buf, alpha_vec);
        buf += NB_WORD_GFqn;

        /* To change the row of alpha_vec */
        alpha_vec += SIZE_ROW * NB_WORD_GFqn;

    }
}



/**
 * @brief   Computation of the multivariate representation of a HFEv polynomial.
 * @details This transformation is achieved with matrix products.
 * It requires O(n Log(D) * (n + v + Log(D))) multiplications in GF(2)[x]
 * and O(n * (n + v + Log(D))) modular reductions.
 * @param[in]   F   A monic HFEv polynomial in GF(2^n)[X,x_(n+1),...,x_(n+v)]
 * stored with a sparse representation.
 * @param[out]  MQS The multivariate representation of F, a MQ system with
 * n equations in GF(2)[x1,...,x_(n+v)]. MQS is stored as one equation in
 * GF(2^n)[x1,...,x_(n+v)] (monomial representation + quadratic form cst||Q).
 * @return   0 if the result is correct, ERROR_ALLOC for error from
 * malloc/calloc functions.
 * @remark  Requires to allocate MQnv_GFqn_SIZE words for MQS.
 * @remark  Requirement: F is monic.
 * @remark  Constant-time implementation.
 */
int PQCLEAN_GEMSSBLUE256_AVX2_genSecretMQS_gf2_opt(mqsnv_gf2n MQS, cst_sparse_monic_gf2nx F) {
    cst_vec_gf2n a_vec_k;
    cst_vec_gf2n a_vec_kp, buf_k, buf_kp;
    cst_sparse_monic_gf2nx F_cp;
    static_gf2n tmp1[NB_WORD_MUL], tmp2[NB_WORD_MUL], tmp3[NB_WORD_GFqn];
    unsigned int i, k, kp;

    /* Vector with linear terms of F */
    UINT F_lin[(HFEDegI + 1) * (HFEv + 1)*NB_WORD_GFqn] = {0};

    F_cp = F + MQv_GFqn_SIZE;

    for (i = 0; i <= HFEDegI; ++i) {
        for (k = 0; k <= HFEv; ++k) {
            copy_gf2n(F_lin + (k * (HFEDegI + 1) + i)*NB_WORD_GFqn, F_cp);
            F_cp += NB_WORD_GFqn;
        }
        F_cp += i * NB_WORD_GFqn;
    }


    /* Precompute alpha_vec is disabled in the submission */
    /* Matrix in GF(2^n) with HFEn-1 rows and (HFEDegI+1) columns */
    /* calloc is useful when it initialises a multiple precision element
       to 1 */
    UINT alpha_vec[SIZE_ROW * (HFEn - 1)*NB_WORD_GFqn] = {0};

    PQCLEAN_GEMSSBLUE256_AVX2_genCanonicalBasisVertical_gf2n(alpha_vec);


    /* Constant: copy the first coefficient of F in MQS */
    copy_gf2n(MQS, F);
    F += MQv_GFqn_SIZE;
    MQS += NB_WORD_GFqn;


    /* Precompute an other table */
    UINT buf[HFEDegI * HFEn * NB_WORD_GFqn] = {0};
    special_buffer(buf, F, alpha_vec);



    /* Monic case */
    /* k=0 */
    buf_k = buf;
    /* kp=0 */
    buf_kp = buf;

    /* x_0*x_0: quadratic terms of F */
    /* i=0 */
    copy_gf2n(MQS, buf_kp);
    buf_kp += NB_WORD_GFqn;
    for (i = 1; i < HFEDegI; ++i) {
        add2_gf2n(MQS, buf_kp);
        buf_kp += NB_WORD_GFqn;
    }
    /* At this step, buf_kp corresponds to kp=1 */

    /* x_0: linear terms of F */
    F_cp = F_lin;
    /* X^(2^i) */
    for (i = 0; i <= HFEDegI; ++i) {
        /* Next linear term of F: X^(2^i) */
        add2_gf2n(MQS, F_cp);
        F_cp += NB_WORD_GFqn;
    }
    MQS += NB_WORD_GFqn;

    /* kp=1 (because kp=0 is not stored, it is just (1,1,1,...,1) */
    /* +NB_WORD_GFqn to jump (alpha^kp)^(2^0) */
    a_vec_kp = alpha_vec + NB_WORD_GFqn;


    /* k=0: x_0 x_kp */
    for (kp = 1; kp < HFEn; ++kp) {
        /* dot_product(a_vec_kp, buf_k) */
        dotProduct_gf2n(MQS, a_vec_kp, buf_k, HFEDegI);
        a_vec_kp += SIZE_ROW * NB_WORD_GFqn;

        /* dot_product(a_vec_k=(1,1,...,1) , buf_kp) */
        for (i = 0; i < HFEDegI; ++i) {
            add2_gf2n(MQS, buf_kp);
            buf_kp += NB_WORD_GFqn;
        }

        MQS += NB_WORD_GFqn;
    }

    /* Vinegar variables */
    for (; kp < HFEnv; ++kp) {
        copy_gf2n(MQS, F_cp);
        F_cp += NB_WORD_GFqn;
        for (i = 1; i <= HFEDegI; ++i) {
            add2_gf2n(MQS, F_cp);
            F_cp += NB_WORD_GFqn;
        }
        MQS += NB_WORD_GFqn;
    }


    /* k=0 becomes k=1 */
    /* +NB_WORD_GFqn to jump (alpha^k)^(2^0) */
    a_vec_k = alpha_vec + NB_WORD_GFqn;

    /* Compute the term x_k x_kp */
    for (k = 1; k < HFEn; ++k) {
        /* k=0 becomes k=1 */
        buf_k += HFEDegI * NB_WORD_GFqn;

        /* kp=k: x_k + x_k*x_k */
        a_vec_kp = a_vec_k;
        buf_kp = buf_k;

        /* Term X^(2^0) of F */
        mul_gf2x(tmp1, F_lin, a_vec_kp - NB_WORD_GFqn);

        /* dot_product(a_vec_k,buf_k) */
        /* i=0 */
        for (i = 1; i <= HFEDegI; ++i) {
            /* Next linear term of F: X^(2^i) */
            add_gf2n(tmp3, buf_kp, F_lin + i * NB_WORD_GFqn);
            mul_gf2x(tmp2, tmp3, a_vec_kp);
            add2_product_gf2n(tmp1, tmp2);
            buf_kp += NB_WORD_GFqn;
            a_vec_kp += NB_WORD_GFqn;
        }

        /* Monic case */
        /* To jump (alpha^kp)^(2^0) */
        a_vec_kp += NB_WORD_GFqn;

        rem_gf2n(MQS, tmp1);
        MQS += NB_WORD_GFqn;


        /* x_k*x_kp */
        for (kp = k + 1; kp < HFEn; ++kp) {
            doubleDotProduct_gf2n(MQS, a_vec_kp, buf_k, a_vec_k, buf_kp,
                                  HFEDegI);
            a_vec_kp += SIZE_ROW * NB_WORD_GFqn;
            buf_kp += HFEDegI * NB_WORD_GFqn;
            MQS += NB_WORD_GFqn;
        }

        /* Vinegar variables */
        F_cp = F_lin;
        a_vec_k -= NB_WORD_GFqn;
        for (; kp < HFEnv; ++kp) {
            F_cp += (HFEDegI + 1) * NB_WORD_GFqn;
            dotProduct_gf2n(MQS, a_vec_k, F_cp, HFEDegI + 1);
            MQS += NB_WORD_GFqn;
        }
        a_vec_k += NB_WORD_GFqn;

        /* k becomes k+1 */
        a_vec_k += SIZE_ROW * NB_WORD_GFqn;
    }

    /* MQS with v vinegar variables */
    F -= MQv_GFqn_SIZE;

    /* Copy the linear and quadratic terms of the constant
       in GF(2^n)[y1,...,yv] */
    for (k = 1; k < NB_MONOMIAL_VINEGAR; ++k) {
        F += NB_WORD_GFqn;
        copy_gf2n(MQS, F);
        MQS += NB_WORD_GFqn;
    }

    return 0;
}
