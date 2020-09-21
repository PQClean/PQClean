#ifndef _MACRO_H
#define _MACRO_H


/** Auxiliar macro. */
#define CONCAT2(a,b) a ## b
/** This macro permits to concat the names. */
#define CONCAT(a,b) CONCAT2(a,b)


/** Print a name as a string. */
#define PRINTF_NAME(name) puts(#name);
#define PRINTF_NAME1(name) PRINTF_NAME(name)
#define PRINTF_NAME2(name) PRINTF_NAME1(name)


/** Compute Floor(a/b) with a and b positive integers, a can be zero. */
#define DIV_FLOOR(a,b) ((a)/(b))
#define DIV_CEIL1(a,b) (((a)/(b))+(((a)%(b))?1:0))
/* Faster but overflow if (a+b-1) >= 2^x for x=size_of_the_type_in_bits */
#define DIV_CEIL2(a,b) (((a)+(b)-1)/(b))
/* Faster but incorrect only when a == 0 and b>1 */
#define DIV_CEIL3(a,b) ((((a)-1)/(b))+1)
/** Compute Ceiling(a/b) with a and b positive integers, a can be zero. */
#define DIV_CEIL DIV_CEIL2


/** Return the minimum. */
#define MINI(a,b) (((a)<(b))?(a):(b))
/** Return the maximum. */
#define MAXI(a,b) (((a)>(b))?(a):(b))


#endif

