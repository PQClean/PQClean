#ifndef _ADD_GF2NX_H
#define _ADD_GF2NX_H
#include "tools_gf2n.h"



/**
 * @brief   Inlined addition in GF(2^n)[X].
 * @param[in,out]   res res = res+A, in GF(2^n)[X].
 * @param[in]   A   An element of GF(2^n)[X].
 * @param[in]   len The number of coefficients of A and res is at least len.
 * @param[in]   i   An integer for a for loop.
 * @remark  Constant-time implementation when len is not secret.
 */
#define add2_gf2nx(res,A,len,i)     for((i)=0;(i)<((len)*NB_WORD_GFqn);++(i))    {        (res)[i]^=(A)[i];    }


/**
 * @brief   Inlined copy in GF(2^n)[X].
 * @param[in]   A   An element of GF(2^n)[X].
 * @param[in]   len The number of coefficients of A is at least len.
 * @param[in]   i   An integer for a for loop.
 * @param[out]  res res = A, in GF(2^n)[X].
 * @remark  Requires to allocate len elements of GF(2^n) for res.
 * @remark  Constant-time implementation when len is not secret.
 */
#define copy_gf2nx(res,A,len,i)     for((i)=0;(i)<((len)*NB_WORD_GFqn);++(i))    {        (res)[i]=(A)[i];    }


/**
 * @brief   Inlined set to 0 in GF(2^n)[X].
 * @param[in,out]   res res = 0, in GF(2^n)[X].
 * @param[in]   len The number of coefficients of res is at least len.
 * @param[in]   i   An integer for a for loop.
 * @remark  Requires to allocate len elements of GF(2^n) for res.
 * @remark  Constant-time implementation when len is not secret.
 */
#define set0_gf2nx(res,len,i)     for((i)=0;(i)<((len)*NB_WORD_GFqn);++(i))    {        (res)[i]=0;    }


#endif
