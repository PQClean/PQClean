#include "crypto_sort_int32.h"
#include "crypto_sort_uint32.h"
#include <stdint.h>

#define uint32 uint32_t

/* can save time by vectorizing xor loops */
/* can save time by integrating xor loops with int32_sort */

void PQCLEAN_NTRULPR857_AVX2_crypto_sort_uint32(uint32_t *array, size_t n) {
    uint32 *x = array;
    size_t j;
    for (j = 0; j < n; ++j) {
        x[j] ^= 0x80000000;
    }
    PQCLEAN_NTRULPR857_AVX2_crypto_sort_int32((int32_t *)array, n);
    for (j = 0; j < n; ++j) {
        x[j] ^= 0x80000000;
    }
}
