#ifndef PACK_UNPACK_H
#define PACK_UNPACK_H
#include "SABER_params.h"
#include <stdint.h>
#include <stdio.h>

void PQCLEAN_SABER_CLEAN_POLT2BS(uint8_t bytes[SABER_SCALEBYTES_KEM], const uint16_t data[SABER_N]);

void PQCLEAN_SABER_CLEAN_BS2POLT(uint16_t data[SABER_N], const uint8_t bytes[SABER_SCALEBYTES_KEM]);


void PQCLEAN_SABER_CLEAN_POLVECq2BS(uint8_t bytes[SABER_POLYVECBYTES], const uint16_t data[SABER_L][SABER_N]);

void PQCLEAN_SABER_CLEAN_POLVECp2BS(uint8_t bytes[SABER_POLYVECCOMPRESSEDBYTES], const uint16_t data[SABER_L][SABER_N]);


void PQCLEAN_SABER_CLEAN_BS2POLVECq(uint16_t data[SABER_L][SABER_N], const uint8_t bytes[SABER_POLYVECBYTES]);

void PQCLEAN_SABER_CLEAN_BS2POLVECp(uint16_t data[SABER_L][SABER_N], const uint8_t bytes[SABER_POLYVECCOMPRESSEDBYTES]);


void PQCLEAN_SABER_CLEAN_BS2POLmsg(uint16_t data[SABER_N], const uint8_t bytes[SABER_KEYBYTES]);

void PQCLEAN_SABER_CLEAN_POLmsg2BS(uint8_t bytes[SABER_KEYBYTES], const uint16_t data[SABER_N]);


#endif
