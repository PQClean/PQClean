#ifndef PQCLEAN_QTESLAPIII_CLEAN_SAMPLE_H
#define PQCLEAN_QTESLAPIII_CLEAN_SAMPLE_H

#include "params.h"
#include "poly.h"
#include <stdint.h>

void PQCLEAN_QTESLAPIII_CLEAN_sample_y(poly y, const uint8_t *seed, uint16_t nonce);
void PQCLEAN_QTESLAPIII_CLEAN_encode_c(uint32_t *pos_list, int16_t *sign_list, uint8_t *c_bin);

#endif
