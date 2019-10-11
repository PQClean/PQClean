#ifndef PQCLEAN_QTESLAPI_CLEAN_PACK_H
#define PQCLEAN_QTESLAPI_CLEAN_PACK_H

#include "poly.h"
#include <stdint.h>

void PQCLEAN_QTESLAPI_CLEAN_pack_sk(uint8_t *sk, const poly s, const poly_k e, uint8_t *seeds);
void PQCLEAN_QTESLAPI_CLEAN_encode_pk(uint8_t *pk, const poly_k t, const uint8_t *seedA);
void PQCLEAN_QTESLAPI_CLEAN_decode_pk(int32_t *pk, uint8_t *seedA, const uint8_t *pk_in);
void PQCLEAN_QTESLAPI_CLEAN_encode_sig(uint8_t *sm, uint8_t *c, const poly z);
void PQCLEAN_QTESLAPI_CLEAN_decode_sig(uint8_t *c, poly z, const uint8_t *sm);

#endif
