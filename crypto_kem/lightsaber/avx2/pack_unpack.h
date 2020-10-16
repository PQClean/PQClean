#ifndef PACK_UNPACK_H
#define PACK_UNPACK_H
#include "SABER_params.h"
#include <stdint.h>
#include <stdio.h>

void PQCLEAN_LIGHTSABER_AVX2_BS2POLq(uint16_t data[SABER_N], const uint8_t *bytes);

void PQCLEAN_LIGHTSABER_AVX2_BS2POLVEC(uint16_t data[SABER_K][SABER_N], const uint8_t *bytes, uint16_t modulus);

void PQCLEAN_LIGHTSABER_AVX2_BS2POLVECq(uint16_t data[SABER_K][SABER_N], const uint8_t *bytes);

void PQCLEAN_LIGHTSABER_AVX2_BS2POLVECp(uint16_t data[SABER_K][SABER_N], const uint8_t *bytes);


void PQCLEAN_LIGHTSABER_AVX2_POLVEC2BS(uint8_t *bytes, const uint16_t data[SABER_K][SABER_N], uint16_t modulus);

void PQCLEAN_LIGHTSABER_AVX2_POLVECq2BS(uint8_t *bytes, const uint16_t data[SABER_K][SABER_N]);

void PQCLEAN_LIGHTSABER_AVX2_POLVECp2BS(uint8_t *bytes, const uint16_t data[SABER_K][SABER_N]);


void PQCLEAN_LIGHTSABER_AVX2_SABER_pack_3bit(uint8_t *bytes, const uint16_t *data);

void PQCLEAN_LIGHTSABER_AVX2_SABER_pack_4bit(uint8_t *bytes, const uint16_t *data);

void PQCLEAN_LIGHTSABER_AVX2_SABER_pack_6bit(uint8_t *bytes, const uint16_t *data);

void PQCLEAN_LIGHTSABER_AVX2_SABER_pack10bit(uint8_t *bytes, const uint16_t data[SABER_K][SABER_N]);

void PQCLEAN_LIGHTSABER_AVX2_SABER_pack11bit(uint8_t *bytes, const uint16_t data[SABER_K][SABER_N]);

void PQCLEAN_LIGHTSABER_AVX2_SABER_pack13bit(uint8_t *bytes, const uint16_t data[SABER_K][SABER_N]);

void PQCLEAN_LIGHTSABER_AVX2_SABER_pack14bit(uint8_t *bytes, const uint16_t data[SABER_K][SABER_N]);


void PQCLEAN_LIGHTSABER_AVX2_SABER_poly_un_pack13bit(uint16_t data[SABER_N], const uint8_t *bytes);


void PQCLEAN_LIGHTSABER_AVX2_SABER_un_pack3bit(uint16_t *data, const uint8_t *bytes);

void PQCLEAN_LIGHTSABER_AVX2_SABER_un_pack4bit(uint16_t *data, const uint8_t *bytes);

void PQCLEAN_LIGHTSABER_AVX2_SABER_un_pack6bit(uint16_t *data, const uint8_t *bytes);

void PQCLEAN_LIGHTSABER_AVX2_SABER_un_pack10bit(uint16_t data[SABER_K][SABER_N], const uint8_t *bytes);

void PQCLEAN_LIGHTSABER_AVX2_SABER_un_pack11bit(uint16_t data[SABER_K][SABER_N], const uint8_t *bytes);

void PQCLEAN_LIGHTSABER_AVX2_SABER_un_pack13bit(uint16_t data[SABER_K][SABER_N], const uint8_t *bytes);

void PQCLEAN_LIGHTSABER_AVX2_SABER_un_pack14bit(uint16_t data[SABER_K][SABER_N], const uint8_t *bytes);


#endif
