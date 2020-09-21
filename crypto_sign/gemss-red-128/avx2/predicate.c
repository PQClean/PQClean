#include "bit.h"
#include "predicate.h"



/**
 * @brief   Test if a buffer is the multi-precision integer zero.
 * @param[in]   a  A buffer of UINT.
 * @param[in]   size    The length in UINT of a.
 * @return  One if the buffer is the multi-precision integer zero, else zero.
 * @remark  This implementation is not in constant-time.
 */
int PQCLEAN_GEMSSRED128_AVX2_ISZERO_NOCST(const UINT *a, unsigned int size) {
    unsigned int i;

    for (i = 0; i < size; ++i) {
        if (a[i]) {
            return 0;
        }
    }
    return 1;
}


/**
 * @brief   Test if a buffer is the multi-precision integer zero.
 * @details a|b == 0 iff a == 0 and b == 0
 * @param[in]   a  A buffer of UINT.
 * @param[in]   size    The length in UINT of a.
 * @return  One if the buffer is the multi-precision integer zero, else zero.
 * @remark  Constant-time implementation when size is not secret.
 */
int PQCLEAN_GEMSSRED128_AVX2_ISZERO(const UINT *a, unsigned int size) {
    UINT r;
    unsigned int i;

    r = a[0];
    for (i = 1; i < size; ++i) {
        r |= a[i];
    }
    NORBITS_UINT(r);
    return r;
}


/**
 * @brief   Test if a buffer is different from the multi-precision integer zero.
 * @details a|b == 0 iff a == 0 and b == 0
 * @param[in]   a  A buffer of UINT.
 * @param[in]   size    The length in UINT of a.
 * @return  Zero if the buffer is the multi-precision integer zero, else one.
 * @remark  Constant-time implementation when size is not secret.
 */
int PQCLEAN_GEMSSRED128_AVX2_ISNOTZERO(const UINT *a, unsigned int size) {
    UINT r;
    unsigned int i;

    r = a[0];
    for (i = 1; i < size; ++i) {
        r |= a[i];
    }
    ORBITS_UINT(r);
    return r;
}


/**
 * @brief   Test if a buffer is the multi-precision integer one.
 * @param[in]   a  A buffer of UINT.
 * @param[in]   size    The length in UINT of a.
 * @return  One if the buffer is the multi-precision integer one, else zero.
 * @remark  This implementation is not in constant-time.
 */
int PQCLEAN_GEMSSRED128_AVX2_ISONE_NOCST(const UINT *a, unsigned int size) {
    unsigned int i;

    if (a[0] != 1) {
        return 0;
    }
    for (i = 1; i < size; ++i) {
        if (a[i]) {
            return 0;
        }
    }
    return 1;
}


/**
 * @brief   Test if a buffer is the multi-precision integer one.
 * @details PQCLEAN_GEMSSRED128_AVX2_ISZERO(a-1).
 * @param[in]   a  A buffer of UINT.
 * @param[in]   size    The length in UINT of a.
 * @return  One if the buffer is the multi-precision integer one, else zero.
 * @remark  Constant-time implementation when size is not secret.
 */
int PQCLEAN_GEMSSRED128_AVX2_ISONE(const UINT *a, unsigned int size) {
    UINT r;
    unsigned int i;

    r = a[0] - 1;
    for (i = 1; i < size; ++i) {
        r |= a[i];
    }
    NORBITS_UINT(r);
    return r;
}


/**
 * @brief   Test if two buffers are equal.
 * @param[in]   a  A buffer of UINT.
 * @param[in]   b  A buffer of UINT.
 * @param[in]   size    The length in UINT of a and b.
 * @return  One if the buffers are equal, else zero.
 * @remark  This implementation is not in constant-time.
 */
int PQCLEAN_GEMSSRED128_AVX2_ISEQUAL_NOCST(const UINT *a, const UINT *b, unsigned int size) {
    unsigned int i;

    for (i = 0; i < size; ++i) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}


/**
 * @brief   Test if two buffers are equal.
 * @details a^b == 0 iff a==b, then, a|b == 0 iff a == 0 and b == 0.
 * @param[in]   a  A buffer of UINT.
 * @param[in]   b  A buffer of UINT.
 * @param[in]   size    The length in UINT of a and b.
 * @return  One if the buffers are equal, else zero.
 * @remark  Constant-time implementation when size is not secret.
 */
int PQCLEAN_GEMSSRED128_AVX2_ISEQUAL(const UINT *a, const UINT *b, unsigned int size) {
    UINT r;
    unsigned int i;

    r = a[0] ^ b[0];
    for (i = 1; i < size; ++i) {
        r |= a[i] ^ b[i];
    }
    NORBITS_UINT(r);
    return r;
}


/**
 * @brief   Test if a multi-precision integer is strictly less than an other.
 * @param[in]   a  A buffer of UINT.
 * @param[in]   b  A buffer of UINT.
 * @param[in]   size    The length in UINT of a and b.
 * @return  Different of zero if a<b, else zero.
 * @remark  This implementation is not in constant-time.
 */
int PQCLEAN_GEMSSRED128_AVX2_CMP_LT_NOCST(const UINT *a, const UINT *b, unsigned int size) {
    unsigned int i;

    for (i = size - 1; i > 0; --i) {
        if (a[i] != b[i]) {
            return (a[i] < b[i]);
        }
    }
    return (a[0] < b[0]);
}


/**
 * @brief   Test if a multi-precision integer is strictly less than an other.
 * @param[in]   a  A buffer of UINT.
 * @param[in]   b  A buffer of UINT.
 * @param[in]   size    The length in UINT of a and b.
 * @return  One if a<b, else zero.
 * @remark  Constant-time implementation when size is not secret.
 */
int PQCLEAN_GEMSSRED128_AVX2_CMP_LT(const UINT *a, const UINT *b, unsigned int size) {
    UINT d, bo, mask;
    unsigned int i;

    /* Compute d the larger index such as a[d]!=b[d], in constant-time */
    d = 0;
    mask = 0;
    for (i = size - 1; i > 0; --i) {
        bo = a[i] ^ b[i];
        ORBITS_UINT(bo);
        mask |= bo;
        d += mask;
    }

    /* Return a[d]<b[d] in constant-time */
    mask = 0;
    for (i = 0; i < size; ++i) {
        bo = i ^ d;
        NORBITS_UINT(bo);
        mask |= (-bo)&CMP_LT_UINT(*a, *b);
        ++a;
        ++b;
    }

    return (int)mask;
}
