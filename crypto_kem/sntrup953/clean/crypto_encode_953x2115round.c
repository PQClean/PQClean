#include "crypto_encode_953x2115.h"
#include "crypto_encode_953x2115round.h"

#define int16 int16_t

#define p 953

void PQCLEAN_SNTRUP953_CLEAN_crypto_encode_953x2115round(unsigned char *out, const void *v) {
    const int16 *a = v;
    int16 x[p];
    int i;

    for (i = 0; i < p; ++i) {
        x[i] = 3 * ((10923 * a[i] + 16384) >> 15);
    }
    PQCLEAN_SNTRUP953_CLEAN_crypto_encode_953x2115(out, x);
}
