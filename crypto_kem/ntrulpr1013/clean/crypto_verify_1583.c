#include "crypto_verify_1583.h"


int PQCLEAN_NTRULPR1013_CLEAN_crypto_verify_1583(const unsigned char *x, const unsigned char *y) {
    unsigned int differentbits = 0;
    int i;

    for (i = 0; i < PQCLEAN_NTRULPR1013_CLEAN_crypto_verify_1583_BYTES; ++i) {
        differentbits |= x[i] ^ y[i];
    }

    return (int) (1 & ((differentbits - 1) >> 8)) - 1;
}
