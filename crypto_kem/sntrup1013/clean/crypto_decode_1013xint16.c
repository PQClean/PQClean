#include "crypto_decode_1013xint16.h"


void PQCLEAN_SNTRUP1013_CLEAN_crypto_decode_1013xint16(void *v, const unsigned char *s) {
    uint16_t *x = v;
    int i;

    for (i = 0; i < 1013; ++i) {
        uint16_t u0 = s[0];
        uint16_t u1 = s[1];
        u1 <<= 8;
        *x = u0 | u1;
        x += 1;
        s += 2;
    }
}
