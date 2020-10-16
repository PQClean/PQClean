#include "api.h"
#include "pack_unpack.h"
#include <string.h>

void PQCLEAN_SABER_CLEAN_POLT2BS(uint8_t bytes[SABER_SCALEBYTES_KEM], const uint16_t data[SABER_N]) {
    size_t j, offset_byte, offset_data;
    for (j = 0; j < SABER_N / 2; j++) {
        offset_byte = j;
        offset_data = 2 * j;
        bytes[offset_byte] = (data[offset_data] & 0x0f) | ((data[offset_data + 1] & 0x0f) << 4);
    }
}

void PQCLEAN_SABER_CLEAN_BS2POLT(uint16_t data[SABER_N], const uint8_t bytes[SABER_SCALEBYTES_KEM]) {
    size_t j, offset_byte, offset_data;
    for (j = 0; j < SABER_N / 2; j++) {
        offset_byte = j;
        offset_data = 2 * j;
        data[offset_data] = bytes[offset_byte] & 0x0f;
        data[offset_data + 1] = (bytes[offset_byte] >> 4) & 0x0f;
    }
}

static void POLq2BS(uint8_t bytes[SABER_POLYBYTES], const uint16_t data[SABER_N]) {
    size_t j, offset_byte, offset_data;
    for (j = 0; j < SABER_N / 8; j++) {
        offset_byte = 13 * j;
        offset_data = 8 * j;
        bytes[offset_byte + 0] = (data[offset_data + 0] & (0xff));
        bytes[offset_byte + 1] = ((data[offset_data + 0] >> 8) & 0x1f) | ((data[offset_data + 1] & 0x07) << 5);
        bytes[offset_byte + 2] = ((data[offset_data + 1] >> 3) & 0xff);
        bytes[offset_byte + 3] = ((data[offset_data + 1] >> 11) & 0x03) | ((data[offset_data + 2] & 0x3f) << 2);
        bytes[offset_byte + 4] = ((data[offset_data + 2] >> 6) & 0x7f) | ((data[offset_data + 3] & 0x01) << 7);
        bytes[offset_byte + 5] = ((data[offset_data + 3] >> 1) & 0xff);
        bytes[offset_byte + 6] = ((data[offset_data + 3] >> 9) & 0x0f) | ((data[offset_data + 4] & 0x0f) << 4);
        bytes[offset_byte + 7] = ((data[offset_data + 4] >> 4) & 0xff);
        bytes[offset_byte + 8] = ((data[offset_data + 4] >> 12) & 0x01) | ((data[offset_data + 5] & 0x7f) << 1);
        bytes[offset_byte + 9] = ((data[offset_data + 5] >> 7) & 0x3f) | ((data[offset_data + 6] & 0x03) << 6);
        bytes[offset_byte + 10] = ((data[offset_data + 6] >> 2) & 0xff);
        bytes[offset_byte + 11] = ((data[offset_data + 6] >> 10) & 0x07) | ((data[offset_data + 7] & 0x1f) << 3);
        bytes[offset_byte + 12] = ((data[offset_data + 7] >> 5) & 0xff);
    }
}

static void BS2POLq(uint16_t data[SABER_N], const uint8_t bytes[SABER_POLYBYTES]) {
    size_t j, offset_byte, offset_data;
    for (j = 0; j < SABER_N / 8; j++) {
        offset_byte = 13 * j;
        offset_data = 8 * j;
        data[offset_data + 0] = (bytes[offset_byte + 0] & (0xff)) | ((bytes[offset_byte + 1] & 0x1f) << 8);
        data[offset_data + 1] = (bytes[offset_byte + 1] >> 5 & (0x07)) | ((bytes[offset_byte + 2] & 0xff) << 3) | ((bytes[offset_byte + 3] & 0x03) << 11);
        data[offset_data + 2] = (bytes[offset_byte + 3] >> 2 & (0x3f)) | ((bytes[offset_byte + 4] & 0x7f) << 6);
        data[offset_data + 3] = (bytes[offset_byte + 4] >> 7 & (0x01)) | ((bytes[offset_byte + 5] & 0xff) << 1) | ((bytes[offset_byte + 6] & 0x0f) << 9);
        data[offset_data + 4] = (bytes[offset_byte + 6] >> 4 & (0x0f)) | ((bytes[offset_byte + 7] & 0xff) << 4) | ((bytes[offset_byte + 8] & 0x01) << 12);
        data[offset_data + 5] = (bytes[offset_byte + 8] >> 1 & (0x7f)) | ((bytes[offset_byte + 9] & 0x3f) << 7);
        data[offset_data + 6] = (bytes[offset_byte + 9] >> 6 & (0x03)) | ((bytes[offset_byte + 10] & 0xff) << 2) | ((bytes[offset_byte + 11] & 0x07) << 10);
        data[offset_data + 7] = (bytes[offset_byte + 11] >> 3 & (0x1f)) | ((bytes[offset_byte + 12] & 0xff) << 5);
    }
}

static void POLp2BS(uint8_t bytes[SABER_POLYCOMPRESSEDBYTES], const uint16_t data[SABER_N]) {
    size_t j, offset_byte, offset_data;
    for (j = 0; j < SABER_N / 4; j++) {
        offset_byte = 5 * j;
        offset_data = 4 * j;
        bytes[offset_byte + 0] = (data[offset_data + 0] & (0xff));
        bytes[offset_byte + 1] = ((data[offset_data + 0] >> 8) & 0x03) | ((data[offset_data + 1] & 0x3f) << 2);
        bytes[offset_byte + 2] = ((data[offset_data + 1] >> 6) & 0x0f) | ((data[offset_data + 2] & 0x0f) << 4);
        bytes[offset_byte + 3] = ((data[offset_data + 2] >> 4) & 0x3f) | ((data[offset_data + 3] & 0x03) << 6);
        bytes[offset_byte + 4] = ((data[offset_data + 3] >> 2) & 0xff);
    }
}

static void BS2POLp(uint16_t data[SABER_N], const uint8_t bytes[SABER_POLYCOMPRESSEDBYTES]) {
    size_t j, offset_byte, offset_data;
    for (j = 0; j < SABER_N / 4; j++) {
        offset_byte = 5 * j;
        offset_data = 4 * j;
        data[offset_data + 0] = (bytes[offset_byte + 0] & (0xff)) | ((bytes[offset_byte + 1] & 0x03) << 8);
        data[offset_data + 1] = ((bytes[offset_byte + 1] >> 2) & (0x3f)) | ((bytes[offset_byte + 2] & 0x0f) << 6);
        data[offset_data + 2] = ((bytes[offset_byte + 2] >> 4) & (0x0f)) | ((bytes[offset_byte + 3] & 0x3f) << 4);
        data[offset_data + 3] = ((bytes[offset_byte + 3] >> 6) & (0x03)) | ((bytes[offset_byte + 4] & 0xff) << 2);
    }
}

void PQCLEAN_SABER_CLEAN_POLVECq2BS(uint8_t bytes[SABER_POLYVECBYTES], const uint16_t data[SABER_L][SABER_N]) {
    size_t i;
    for (i = 0; i < SABER_L; i++) {
        POLq2BS(bytes + i * SABER_POLYBYTES, data[i]);
    }
}

void PQCLEAN_SABER_CLEAN_BS2POLVECq(uint16_t data[SABER_L][SABER_N], const uint8_t bytes[SABER_POLYVECBYTES]) {
    size_t i;
    for (i = 0; i < SABER_L; i++) {
        BS2POLq(data[i], bytes + i * SABER_POLYBYTES);
    }
}

void PQCLEAN_SABER_CLEAN_POLVECp2BS(uint8_t bytes[SABER_POLYVECCOMPRESSEDBYTES], const uint16_t data[SABER_L][SABER_N]) {
    size_t i;
    for (i = 0; i < SABER_L; i++) {
        POLp2BS(bytes + i * (SABER_EP * SABER_N / 8), data[i]);
    }
}

void PQCLEAN_SABER_CLEAN_BS2POLVECp(uint16_t data[SABER_L][SABER_N], const uint8_t bytes[SABER_POLYVECCOMPRESSEDBYTES]) {
    size_t i;
    for (i = 0; i < SABER_L; i++) {
        BS2POLp(data[i], bytes + i * (SABER_EP * SABER_N / 8));
    }
}

void PQCLEAN_SABER_CLEAN_BS2POLmsg(uint16_t data[SABER_N], const uint8_t bytes[SABER_KEYBYTES]) {
    size_t i, j;
    for (j = 0; j < SABER_KEYBYTES; j++) {
        for (i = 0; i < 8; i++) {
            data[j * 8 + i] = ((bytes[j] >> i) & 0x01);
        }
    }
}

void PQCLEAN_SABER_CLEAN_POLmsg2BS(uint8_t bytes[SABER_KEYBYTES], const uint16_t data[SABER_N]) {
    size_t i, j;
    memset(bytes, 0, SABER_KEYBYTES);

    for (j = 0; j < SABER_KEYBYTES; j++) {
        for (i = 0; i < 8; i++) {
            bytes[j] = bytes[j] | ((data[j * 8 + i] & 0x01) << i);
        }
    }
}
