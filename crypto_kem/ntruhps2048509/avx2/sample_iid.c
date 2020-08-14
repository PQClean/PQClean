#include "poly.h"
#include "sample.h"

extern void PQCLEAN_NTRUHPS2048509_AVX2_vec32_sample_iid(poly *r, const unsigned char uniformbytes[PAD32(NTRU_SAMPLE_IID_BYTES)]);

void PQCLEAN_NTRUHPS2048509_AVX2_sample_iid(poly *r, const unsigned char uniformbytes[NTRU_SAMPLE_IID_BYTES]) {
    int i;
    unsigned char buffer[PAD32(NTRU_SAMPLE_IID_BYTES)] __attribute__((aligned(32)));
    for (i = 0; i < NTRU_SAMPLE_IID_BYTES; i++) {
        buffer[i] = uniformbytes[i];
    }
    for (i = NTRU_SAMPLE_IID_BYTES; i < PAD32(NTRU_SAMPLE_IID_BYTES); i++) {
        buffer[i] = 0;
    }
    PQCLEAN_NTRUHPS2048509_AVX2_vec32_sample_iid(r, buffer);
}
