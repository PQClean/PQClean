#include "crypto_sort_int32.h"
#define int32 int32_t
#define int32_sort crypto_sort_int32

#define int32_MINMAX(a,b) \
    do { \
        int32 ab = b ^ a; \
        int32 c = b - a; \
        c ^= ab & (c ^ b); \
        c >>= 31; \
        c &= ab; \
        a ^= c; \
        b ^= c; \
    } while(0)

void int32_sort(int32 *x, size_t n) {
    size_t top, p, q, r, i;

    if (n < 2) {
        return;
    }
    top = 1;
    while (top < n - top) {
        top += top;
    }

    for (p = top; p > 0; p >>= 1) {
        for (i = 0; i < n - p; ++i) {
            if (!(i & p)) {
                int32_MINMAX(x[i], x[i + p]);
            }
        }
        i = 0;
        for (q = top; q > p; q >>= 1) {
            for (; i < n - q; ++i) {
                if (!(i & p)) {
                    int32 a = x[i + p];
                    for (r = q; r > p; r >>= 1) {
                        int32_MINMAX(a, x[i + r]);
                    }
                    x[i + p] = a;
                }
            }
        }
    }
}
