#include "SABER_params.h"
#include "pack_unpack.h"
#include "poly.h"
#include <string.h>

void PQCLEAN_SABER_CLEAN_POLT2BS(uint8_t bytes[SABER_SCALEBYTES_KEM], const poly *data) {
    size_t j;
    const uint16_t *in = data->coeffs;
    uint8_t *out = bytes;
    for (j = 0; j < SABER_N / 2; j++) {
        out[0] = (in[0] & 0x0f) | ((in[1] & 0x0f) << 4);
        in += 2;
        out += 1;
    }
}

void PQCLEAN_SABER_CLEAN_BS2POLT(poly *data, const uint8_t bytes[SABER_SCALEBYTES_KEM]) {
    size_t j;
    const uint8_t *in = bytes;
    uint16_t *out = data->coeffs;
    for (j = 0; j < SABER_N / 2; j++) {
        out[0] = in[0] & 0x0f;
        out[1] = (in[0] >> 4) & 0x0f;
        in += 1;
        out += 2;
    }
}

static void POLq2BS(uint8_t bytes[SABER_POLYBYTES], const poly *data) {
    size_t j;
    const uint16_t *in = data->coeffs;
    uint8_t *out = bytes;
    for (j = 0; j < SABER_N / 8; j++) {
        out[0] = (in[0] & (0xff));
        out[1] = ((in[0] >> 8) & 0x1f) | ((in[1] & 0x07) << 5);
        out[2] = ((in[1] >> 3) & 0xff);
        out[3] = ((in[1] >> 11) & 0x03) | ((in[2] & 0x3f) << 2);
        out[4] = ((in[2] >> 6) & 0x7f) | ((in[3] & 0x01) << 7);
        out[5] = ((in[3] >> 1) & 0xff);
        out[6] = ((in[3] >> 9) & 0x0f) | ((in[4] & 0x0f) << 4);
        out[7] = ((in[4] >> 4) & 0xff);
        out[8] = ((in[4] >> 12) & 0x01) | ((in[5] & 0x7f) << 1);
        out[9] = ((in[5] >> 7) & 0x3f) | ((in[6] & 0x03) << 6);
        out[10] = ((in[6] >> 2) & 0xff);
        out[11] = ((in[6] >> 10) & 0x07) | ((in[7] & 0x1f) << 3);
        out[12] = ((in[7] >> 5) & 0xff);
        in += 8;
        out += 13;
    }
}

static void BS2POLq(poly *data, const uint8_t bytes[SABER_POLYBYTES]) {
    size_t j;
    const uint8_t *in = bytes;
    uint16_t *out = data->coeffs;
    for (j = 0; j < SABER_N / 8; j++) {
        out[0] = (in[0] & (0xff)) | ((in[1] & 0x1f) << 8);
        out[1] = (in[1] >> 5 & (0x07)) | ((in[2] & 0xff) << 3) | ((in[3] & 0x03) << 11);
        out[2] = (in[3] >> 2 & (0x3f)) | ((in[4] & 0x7f) << 6);
        out[3] = (in[4] >> 7 & (0x01)) | ((in[5] & 0xff) << 1) | ((in[6] & 0x0f) << 9);
        out[4] = (in[6] >> 4 & (0x0f)) | ((in[7] & 0xff) << 4) | ((in[8] & 0x01) << 12);
        out[5] = (in[8] >> 1 & (0x7f)) | ((in[9] & 0x3f) << 7);
        out[6] = (in[9] >> 6 & (0x03)) | ((in[10] & 0xff) << 2) | ((in[11] & 0x07) << 10);
        out[7] = (in[11] >> 3 & (0x1f)) | ((in[12] & 0xff) << 5);
        in += 13;
        out += 8;
    }
}

static void POLp2BS(uint8_t bytes[SABER_POLYCOMPRESSEDBYTES], const poly *data) {
    size_t j;
    const uint16_t *in = data->coeffs;
    uint8_t *out = bytes;
    for (j = 0; j < SABER_N / 4; j++) {
        out[0] = (in[0] & (0xff));
        out[1] = ((in[0] >> 8) & 0x03) | ((in[1] & 0x3f) << 2);
        out[2] = ((in[1] >> 6) & 0x0f) | ((in[2] & 0x0f) << 4);
        out[3] = ((in[2] >> 4) & 0x3f) | ((in[3] & 0x03) << 6);
        out[4] = ((in[3] >> 2) & 0xff);
        in += 4;
        out += 5;
    }
}

static void BS2POLp(poly *data, const uint8_t bytes[SABER_POLYCOMPRESSEDBYTES]) {
    size_t j;
    const uint8_t *in = bytes;
    uint16_t *out = data->coeffs;
    for (j = 0; j < SABER_N / 4; j++) {
        out[0] = (in[0] & (0xff)) | ((in[1] & 0x03) << 8);
        out[1] = ((in[1] >> 2) & (0x3f)) | ((in[2] & 0x0f) << 6);
        out[2] = ((in[2] >> 4) & (0x0f)) | ((in[3] & 0x3f) << 4);
        out[3] = ((in[3] >> 6) & (0x03)) | ((in[4] & 0xff) << 2);
        in += 5;
        out += 4;
    }
}

void PQCLEAN_SABER_CLEAN_POLVECq2BS(uint8_t bytes[SABER_POLYVECBYTES], const poly data[SABER_L]) {
    size_t i;
    for (i = 0; i < SABER_L; i++) {
        POLq2BS(bytes + i * SABER_POLYBYTES, &data[i]);
    }
}

void PQCLEAN_SABER_CLEAN_BS2POLVECq(poly data[SABER_L], const uint8_t bytes[SABER_POLYVECBYTES]) {
    size_t i;
    for (i = 0; i < SABER_L; i++) {
        BS2POLq(&data[i], bytes + i * SABER_POLYBYTES);
    }
}

void PQCLEAN_SABER_CLEAN_POLVECp2BS(uint8_t bytes[SABER_POLYVECCOMPRESSEDBYTES], const poly data[SABER_L]) {
    size_t i;
    for (i = 0; i < SABER_L; i++) {
        POLp2BS(bytes + i * SABER_POLYCOMPRESSEDBYTES, &data[i]);
    }
}

void PQCLEAN_SABER_CLEAN_BS2POLVECp(poly data[SABER_L], const uint8_t bytes[SABER_POLYVECCOMPRESSEDBYTES]) {
    size_t i;
    for (i = 0; i < SABER_L; i++) {
        BS2POLp(&data[i], bytes + i * SABER_POLYCOMPRESSEDBYTES);
    }
}

void PQCLEAN_SABER_CLEAN_BS2POLmsg(poly *data, const uint8_t bytes[SABER_KEYBYTES]) {
    size_t i, j;
    for (j = 0; j < SABER_KEYBYTES; j++) {
        for (i = 0; i < 8; i++) {
            data->coeffs[j * 8 + i] = ((bytes[j] >> i) & 0x01);
        }
    }
}

void PQCLEAN_SABER_CLEAN_POLmsg2BS(uint8_t bytes[SABER_KEYBYTES], const poly *data) {
    size_t i, j;
    memset(bytes, 0, SABER_KEYBYTES);

    for (j = 0; j < SABER_KEYBYTES; j++) {
        for (i = 0; i < 8; i++) {
            bytes[j] = bytes[j] | ((data->coeffs[j * 8 + i] & 0x01) << i);
        }
    }
}
