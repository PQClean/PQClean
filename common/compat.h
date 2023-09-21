#ifndef  PQCLEAN_COMMON_COMPAT_H
#define PQCLEAN_COMMON_COMPAT_H

/* This file serves to solve compatibility issues between different
 * versions of compilers.
 *
 * This file is allowed to use #ifdefs and toggle things by compiler versions.
 */

// From https://github.com/gcc-mirror/gcc/blob/af73a8b2027d9ab64944d7dbbb48e207d7790ce6/gcc/config/i386/avxintrin.h#L62-L71
/* Unaligned versions of the vector types */
#define UNALIGNED_VECTOR_POLYFILL_GCC \
    typedef float __m256_u __attribute__ ((__vector_size__ (32), __may_alias__, __aligned__ (1))); \
    typedef double __m256d_u __attribute__ ((__vector_size__ (32), __may_alias__, __aligned__ (1))); \
    typedef long long __m256i_u __attribute__ ((__vector_size__ (32), __may_alias__, __aligned__ (1)));

#if defined(__GNUC__) && !defined(__clang__)
#include <features.h>

#  if !__GNUC_PREREQ(7, 1) // at least GCC 7.1
/* Versions of the GCC pre-7.1 don't have __m256*_u types */
UNALIGNED_VECTOR_POLYFILL_GCC
#  endif // __GNUC_PREREQ(7,1)

#elif defined(__GNUC__) && defined(__clang__)

#  if __clang__major__ < 9
/* Versions of Clang pre-9.0 don't have __m256*_u types */
UNALIGNED_VECTOR_POLYFILL_GCC
#  endif

#elif defined(_MSC_VER)
// MSVC simply doesn't have these types
#define __m256_u    __m256
#define __m256d_u   __m256d
#define __m256i_u   __m256i

#else
#error UNSUPPORTED COMPILER!?!?
#endif // compiler selector

/************************
 * Portable VLA support *
 ************************/

/* To support MSVC use alloca() instead of VLAs. */
#ifdef _MSC_VER
/* MSVC defines _alloca in malloc.h */
# include <malloc.h>
/* Note: _malloca(), which is recommended over deprecated _alloca,
   requires that you call _freea(). So we stick with _alloca */
# define PQCLEAN_VLA(__t,__x,__s) __t *__x = (__t*)_alloca((__s)*sizeof(__t))
#else
# define PQCLEAN_VLA(__t,__x,__s) __t __x[__s]
#endif

#endif // PQCLEAN_COMMON_COMPAT_H
