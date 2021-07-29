#include "crypto_encode_1013x2393.h"
#include "crypto_encode_1013x2393round.h"

#define int16 int16_t

#define p 1013

void PQCLEAN_SNTRUP1013_CLEAN_crypto_encode_1013x2393round(unsigned char *out, const void *v) {
    const int16 *a = v;
    int16 x[p];
    int i;

    for (i = 0; i < p; ++i) {
        x[i] = 3 * ((10923 * a[i] + 16384) >> 15);
    }
    PQCLEAN_SNTRUP1013_CLEAN_crypto_encode_1013x2393(out, x);
}
