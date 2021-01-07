#ifndef PQCLEAN_DILITHIUM5R_AVX2_CDECL
#define PQCLEAN_DILITHIUM5R_AVX2_CDECL

#include "params.h"

#define _8XQINV      0
#define _8XQ         8
#define _MASK       16
#define _8X23ONES   24
#define _8XDIV      32
#define _ZETAS      40
#define _ZETAS_INV 296


/* The C ABI on MacOS exports all symbols with a leading
 * underscore. This means that any symbols we refer to from
 * C files (functions) can't be found, and all symbols we
 * refer to from ASM also can't be found (nttconsts.c).
 *
 * This define helps us get around this
 */

#if defined(__WIN32__) || defined(__APPLE__)
#define decorate(s) _##s
#define _cdecl(s) decorate(s)
#define cdecl(s) _cdecl(DILITHIUM_NAMESPACE(_##s))
#else
#define cdecl(s) DILITHIUM_NAMESPACE(_##s)
#endif

#endif