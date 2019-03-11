#include "sha2.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

const unsigned char plaintext[44] = "The quick brown fox jumps over the lazy dog";

const unsigned char expected_256[32] = {
    0xd7, 0xa8, 0xfb, 0xb3, 0x07, 0xd7, 0x80, 0x94, 0x69, 0xca, 0x9a, 0xbc,
    0xb0, 0x08, 0x2e, 0x4f, 0x8d, 0x56, 0x51, 0xe4, 0x6d, 0x3c, 0xdb, 0x76,
    0x2d, 0x02, 0xd0, 0xbf, 0x37, 0xc9, 0xe5, 0x92,
};

const unsigned char expected_384[48] = {
    0xca, 0x73, 0x7f, 0x10, 0x14, 0xa4, 0x8f, 0x4c, 0x0b, 0x6d, 0xd4, 0x3c,
    0xb1, 0x77, 0xb0, 0xaf, 0xd9, 0xe5, 0x16, 0x93, 0x67, 0x54, 0x4c, 0x49,
    0x40, 0x11, 0xe3, 0x31, 0x7d, 0xbf, 0x9a, 0x50, 0x9c, 0xb1, 0xe5, 0xdc,
    0x1e, 0x85, 0xa9, 0x41, 0xbb, 0xee, 0x3d, 0x7f, 0x2a, 0xfb, 0xc9, 0xb1,
};

const unsigned char expected_512[64] = {
    0x07, 0xe5, 0x47, 0xd9, 0x58, 0x6f, 0x6a, 0x73, 0xf7, 0x3f, 0xba, 0xc0,
    0x43, 0x5e, 0xd7, 0x69, 0x51, 0x21, 0x8f, 0xb7, 0xd0, 0xc8, 0xd7, 0x88,
    0xa3, 0x09, 0xd7, 0x85, 0x43, 0x6b, 0xbb, 0x64, 0x2e, 0x93, 0xa2, 0x52,
    0xa9, 0x54, 0xf2, 0x39, 0x12, 0x54, 0x7d, 0x1e, 0x8a, 0x3b, 0x5e, 0xd6,
    0xe1, 0xbf, 0xd7, 0x09, 0x78, 0x21, 0x23, 0x3f, 0xa0, 0x53, 0x8f, 0x3d,
    0xb8, 0x54, 0xfe, 0xe6,
};

static int test_sha256(void) {
    unsigned char output[32];
    int i = 0;

    sha256(output, plaintext, 43);

    if (memcmp(expected_256, output, 32)) {
        printf("ERROR sha256 output did not match test vector.\n");
        printf("Expected: ");
        for (i = 0; i < 32; i++) {
            printf("%02X", expected_256[i]);
        }
        printf("\n");
        printf("Received: ");
        for (i = 0; i < 32; i++) {
            printf("%02X", output[i]);
        }
        printf("\n");
        return 1;
    }

    return 0;
}

static int test_sha384(void) {
    unsigned char output[48];
    int i = 0;

    sha384(output, plaintext, 43);

    if (memcmp(expected_384, output, 48)) {
        printf("ERROR sha384 output did not match test vector.\n");
        printf("Expected: ");
        for (i = 0; i < 48; i++) {
            printf("%02X", expected_384[i]);
        }
        printf("\n");
        printf("Received: ");
        for (i = 0; i < 48; i++) {
            printf("%02X", output[i]);
        }
        printf("\n");
        return 1;
    }

    return 0;
}

static int test_sha512(void) {
    unsigned char output[64];
    int i = 0;

    sha512(output, plaintext, 43);

    if (memcmp(expected_512, output, 64)) {
        printf("ERROR sha512 output did not match test vector.\n");
        printf("Expected: ");
        for (i = 0; i < 64; i++) {
            printf("%02X", expected_512[i]);
        }
        printf("\n");
        printf("Received: ");
        for (i = 0; i < 64; i++) {
            printf("%02X", output[i]);
        }
        printf("\n");
        return 1;
    }

    return 0;
}

int main(void) {
    int result = 0;
    result += test_sha256();
    result += test_sha384();
    result += test_sha512();

    if (result != 0) {
        puts("Errors occurred");
    }
    return result;
}
