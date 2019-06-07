#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "sp800-185.h"

const unsigned char name[] = "";
const unsigned char customization[] = "Email Signature";

const unsigned char output1[32] = {
    0xC1, 0xC3, 0x69, 0x25, 0xB6, 0x40, 0x9A, 0x04, 0xF1, 0xB5, 0x04, 0xFC, 0xBC, 0xA9, 0xD8, 0x2B,
    0x40, 0x17, 0x27, 0x7C, 0xB5, 0xED, 0x2B, 0x20, 0x65, 0xFC, 0x1D, 0x38, 0x14, 0xD5, 0xAA, 0xF5
};
const unsigned char output2[32] = {
    0xC5, 0x22, 0x1D, 0x50, 0xE4, 0xF8, 0x22, 0xD9, 0x6A, 0x2E, 0x88, 0x81, 0xA9, 0x61, 0x42, 0x0F,
    0x29, 0x4B, 0x7B, 0x24, 0xFE, 0x3D, 0x20, 0x94, 0xBA, 0xED, 0x2C, 0x65, 0x24, 0xCC, 0x16, 0x6B
};
const unsigned char output3[64] = {
    0xD0, 0x08, 0x82, 0x8E, 0x2B, 0x80, 0xAC, 0x9D, 0x22, 0x18, 0xFF, 0xEE, 0x1D, 0x07, 0x0C, 0x48,
    0xB8, 0xE4, 0xC8, 0x7B, 0xFF, 0x32, 0xC9, 0x69, 0x9D, 0x5B, 0x68, 0x96, 0xEE, 0xE0, 0xED, 0xD1,
    0x64, 0x02, 0x0E, 0x2B, 0xE0, 0x56, 0x08, 0x58, 0xD9, 0xC0, 0x0C, 0x03, 0x7E, 0x34, 0xA9, 0x69,
    0x37, 0xC5, 0x61, 0xA7, 0x4C, 0x41, 0x2B, 0xB4, 0xC7, 0x46, 0x46, 0x95, 0x27, 0x28, 0x1C, 0x8C
};
const unsigned char output4[64] = {
    0x07, 0xDC, 0x27, 0xB1, 0x1E, 0x51, 0xFB, 0xAC, 0x75, 0xBC, 0x7B, 0x3C, 0x1D, 0x98, 0x3E, 0x8B,
    0x4B, 0x85, 0xFB, 0x1D, 0xEF, 0xAF, 0x21, 0x89, 0x12, 0xAC, 0x86, 0x43, 0x02, 0x73, 0x09, 0x17,
    0x27, 0xF4, 0x2B, 0x17, 0xED, 0x1D, 0xF6, 0x3E, 0x8E, 0xC1, 0x18, 0xF0, 0x4B, 0x23, 0x63, 0x3C,
    0x1D, 0xFB, 0x15, 0x74, 0xC8, 0xFB, 0x55, 0xCB, 0x45, 0xDA, 0x8E, 0x25, 0xAF, 0xB0, 0x92, 0xBB
};

static int test_cshake128(void) {
    unsigned char input[200];
    unsigned char output[32];
    int i;
    int returncode = 0;
    size_t namelen = strlen((const char *)name);
    size_t cstmlen = strlen((const char *)customization);

    for (i = 0; i < 4; i++) {
        input[i] = i;
    }

    cshake128(output, 32, name, namelen, customization, cstmlen, input, 4);

    if (memcmp(output1, output, 32) != 0) {
        puts("ERROR shake128 did not match test vector 1.");
        printf("  Expected: ");
        for (i = 0; i < 32; i++) {
            printf("%02X", output1[i]);
        }
        puts("");
        printf("  Received: ");
        for (i = 0; i < 32; i++) {
            printf("%02X", output[i]);
        }
        puts("");
        returncode = 1;
    }

    for (i = 0; i < 200; i++) {
        input[i] = i;
    }

    cshake128(output, 32, name, namelen, customization, cstmlen, input, 200);

    if (memcmp(output2, output, 32) != 0) {
        puts("ERROR shake128 did not match test vector 2.");
        printf("  Expected: ");
        for (i = 0; i < 32; i++) {
            printf("%02X", output2[i]);
        }
        puts("");
        printf("  Received: ");
        for (i = 0; i < 32; i++) {
            printf("%02X", output[i]);
        }
        puts("");
        returncode = 1;
    }

    return returncode;
}

static int test_cshake256(void) {
    unsigned char input[200];
    unsigned char output[64];
    int i;
    int returncode = 0;
    size_t namelen = strlen((const char *)name);
    size_t cstmlen = strlen((const char *)customization);

    for (i = 0; i < 4; i++) {
        input[i] = i;
    }

    cshake256(output, 64, name, namelen, customization, cstmlen, input, 4);

    if (memcmp(output3, output, 64) != 0) {
        puts("ERROR shake256 did not match test vector 3.");
        printf("  Expected: ");
        for (i = 0; i < 64; i++) {
            printf("%02X", output3[i]);
        }
        puts("");
        printf("  Received: ");
        for (i = 0; i < 64; i++) {
            printf("%02X", output[i]);
        }
        puts("");
        returncode = 1;
    }

    for (i = 0; i < 200; i++) {
        input[i] = i;
    }

    cshake256(output, 64, name, namelen, customization, cstmlen, input, 200);

    if (memcmp(output4, output, 64) != 0) {
        puts("ERROR shake256 did not match test vector 4.");
        printf("  Expected: ");
        for (i = 0; i < 64; i++) {
            printf("%02X", output4[i]);
        }
        puts("");
        printf("  Received: ");
        for (i = 0; i < 64; i++) {
            printf("%02X", output[i]);
        }
        puts("");
        returncode = 1;
    }

    return returncode;
}

int main(void) {
    int result = 0;
    result += test_cshake128();
    result += test_cshake256();

    if (result != 0) {
        puts("Errors occurred");
    }
    return result;
}
