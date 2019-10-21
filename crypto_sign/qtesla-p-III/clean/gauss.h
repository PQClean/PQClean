#ifndef PQCLEAN_QTESLAPIII_CLEAN_GAUSS_H
#define PQCLEAN_QTESLAPIII_CLEAN_GAUSS_H

#include "config.h"
#include "params.h"
#include "poly.h"
#include <stdint.h>

#define CHUNK_SIZE 512   // Fix chunk size for sampling

void PQCLEAN_QTESLAPIII_CLEAN_sample_gauss_poly(poly z, const uint8_t *seed, uint16_t nonce);

#endif
