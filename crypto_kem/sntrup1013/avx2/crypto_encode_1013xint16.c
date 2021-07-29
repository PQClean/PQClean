#include "crypto_encode_1013xint16.h"


void PQCLEAN_SNTRUP1013_AVX2_crypto_encode_1013xint16(unsigned char *s, const void *v) {
    const uint16_t *x = v;
    int i;

    for (i = 0; i < 1013; ++i) {
        uint16_t u = *x++;
        *s++ = (unsigned char) u;
        *s++ = (unsigned char) (u >> 8);
    }
}
