/*****************************************************************************
 *  Integer sorting routine code imported and adapted from djbsort
 *  https://sorting.cr.yp.to/index.html
 *  Original code available as public domain, the same licensing applies to
 *  the modifications made to adapt it to the LEDAcrypt codebase.
*****************************************************************************/

#ifndef DJBSORT_H
#define DJBSORT_H

#include <stdint.h>

void int32_sort(int32_t *x,long long n);

#endif



