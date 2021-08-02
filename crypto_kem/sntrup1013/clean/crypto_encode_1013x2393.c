#include "crypto_encode_1013x2393.h"

/* auto-generated; do not edit */

#define int16 int16_t
#define uint16 uint16_t
#define uint32 uint32_t

void PQCLEAN_SNTRUP1013_CLEAN_crypto_encode_1013x2393(unsigned char *out, const void *v) {
    const int16 *R0 = v;
    /* XXX: caller could overlap R with input */
    uint16 R[507];
    long i;
    uint16 r0, r1;
    uint32 r2;

    for (i = 0; i < 506; ++i) {
        r0 = (uint16) ((((R0[2 * i] + 3588) & 16383) * 10923) >> 15);
        r1 = (uint16) ((((R0[2 * i + 1] + 3588) & 16383) * 10923) >> 15);
        r2 = r0 + r1 * (uint32)2393;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    R[506] = (uint16) ((((R0[1012] + 3588) & 16383) * 10923) >> 15);

    for (i = 0; i < 253; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)88;
        R[i] = (uint16) r2;
    }
    R[253] = R[506];

    for (i = 0; i < 127; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)7744;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }

    for (i = 0; i < 63; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)916;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    R[63] = R[126];

    for (i = 0; i < 31; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)3278;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }
    r0 = R[62];
    r1 = R[63];
    r2 = r0 + r1 * (uint32)3278;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    R[31] = (uint16) r2;

    for (i = 0; i < 16; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)164;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }

    for (i = 0; i < 7; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)106;
        R[i] = (uint16) r2;
    }
    r0 = R[14];
    r1 = R[15];
    r2 = r0 + r1 * (uint32)106;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    R[7] = (uint16) r2;

    for (i = 0; i < 4; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)11236;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }

    for (i = 0; i < 2; ++i) {
        r0 = R[2 * i];
        r1 = R[2 * i + 1];
        r2 = r0 + r1 * (uint32)1927;
        *out++ = (unsigned char) r2;
        r2 >>= 8;
        R[i] = (uint16) r2;
    }

    r0 = R[0];
    r1 = R[1];
    r2 = r0 + r1 * (uint32)14506;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    *out++ = (unsigned char) r2;
    r2 >>= 8;
    R[0] = (uint16) r2;

    r0 = R[0];
    *out++ = (unsigned char) r0;
    r0 >>= 8;
    *out++ = (unsigned char) r0;
}
