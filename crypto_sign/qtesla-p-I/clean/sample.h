#ifndef PQCLEAN_QTESLAPI_CLEAN_SAMPLE_H
#define PQCLEAN_QTESLAPI_CLEAN_SAMPLE_H

#include "params.h"
#include "poly.h"
#include <stdint.h>

void PQCLEAN_QTESLAPI_CLEAN_sample_y(poly y, const unsigned char *seed, int nonce);
void PQCLEAN_QTESLAPI_CLEAN_encode_c(uint32_t *pos_list, int16_t *sign_list, unsigned char *c_bin);

#endif
