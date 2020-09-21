#include "div_gf2nx.h"
#include "gcd_gf2nx.h"
#include "init.h"
#include "inv_gf2n.h"
#include "mul_gf2n.h"
#include "tools_gf2n.h"



/**
 * @brief   Gcd in GF(2^n)[X], in-place.
 * @param[in,out]   A   An element of GF(2^n)[X].
 * @param[in]   da  The degree of A.
 * @param[in,out]   B   An element of GF(2^n)[X].
 * @param[in]   db  The degree of B.
 * @return  The degree of the last remainder R which is strictly greater than 0.
 * At the end of the algorithm, R=(1-(*b))*A + (*b)*B, R2=(*b)*A + (1-(*b))*B,
 * degree(R) = returned value, degree(R2)=0.
 * If R2==0, then R=GCD(A,B). Else, R2=GCD(A,B) and its degree is 0.
 * @remark  Requirement: da>db.
 * @remark  This implementation is not in constant-time.
 */
unsigned int PQCLEAN_GEMSSRED256_AVX2_gcd_gf2nx(unsigned int *b, gf2nx A, unsigned int da,
        gf2nx B, unsigned int db) {
    static_gf2n inv[NB_WORD_GFqn];
    gf2nx tmp;
    unsigned int i;

    /* *b = 0: B is the last remainder
       *b = 1: A is the last remainder */
    *b = 0;

    while (db) {
        /* Computation of A = A mod B, of degree da */

        /* Minimizes the number of multiplications by an inverse */
        /* 2db > da */
        if ( (db << 1) > da ) {
            /* At most da-db+1 multiplications by an inverse */
            da = PQCLEAN_GEMSSRED256_AVX2_div_r_gf2nx(A, da, B, db);
        } else {
            /* B becomes monic: db multiplications by an inverse */
            inv_gf2n(inv, B + db * NB_WORD_GFqn);
            set1_gf2n(B + db * NB_WORD_GFqn);
            for (i = db - 1; i != (unsigned int)(-1); --i) {
                mul_gf2n(B + i * NB_WORD_GFqn, B + i * NB_WORD_GFqn, inv);
            }
            da = div_r_monic_gf2nx(A, da, B, db);
        }

        /* Swaps A and B */
        tmp = A;
        A = B;
        B = tmp;
        /* Swaps da and db */
        SWAP_WORD(da, db);
        /* 0 becomes 1 and 1 becomes 0 */
        *b = (1 - (*b));
    }

    return da;
}
