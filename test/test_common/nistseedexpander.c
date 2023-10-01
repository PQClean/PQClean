#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "nistseedexpander.h"

const uint8_t seed[32] = {0};
const uint8_t expected_result[27] = { 0x55, 0x98, 0x87, 0xae, 0x24, 0x28, 0x5d, 0x7e, 0x42, 0x02, 0x74, 0x27, 0x73, 0x31, 0x03, 0xf6, 0xaf, 0x2e, 0xb6, 0xf1, 0xec, 0xfe, 0xdf, 0xfb, 0xd3, 0x50, 0x31, };

int test_seedexpander() {
    AES_XOF_struct state;
    uint8_t result[27] = {0};

    seedexpander_init(&state, seed, seed, 28);
    seedexpander(&state, result, 27);

    if (memcmp(expected_result, result, 27) != 0) {
        printf("Got\tExpected\n");
        for (size_t i = 0; i < 27; i++) {
            printf("0x%02x\t0x%02x\n", result[i], expected_result[i]);
        }
        return 1;
    }
    return 0;
}

int main(void) {

    return test_seedexpander();
}
