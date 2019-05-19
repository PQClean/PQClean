#pragma once

#define LITTLE_ENDIAN
#include <libkeccak.a.headers/KeccakHash.h>

static inline
void sha3_256(const unsigned char *input,
              unsigned int inputByteLen,
              unsigned char *output) {
    Keccak_HashInstance state;
    Keccak_HashInitialize(&state, 1088,  512, 256, 0x06);
    Keccak_HashUpdate(&state, input, inputByteLen * 8);
    Keccak_HashFinal(&state, output);
}

/**
  *  Function to compute SHA3-384 on the input message.
  *  The output length is fixed to 48 bytes.
  */
static inline
void sha3_384(const unsigned char *input,
              unsigned int inputByteLen,
              unsigned char *output) {
    Keccak_HashInstance state;
    Keccak_HashInitialize(&state, 832,  768, 384, 0x06);
    Keccak_HashUpdate(&state, input, inputByteLen * 8);
    Keccak_HashFinal(&state, output);
}

/**
  *  Function to compute SHA3-512 on the input message.
  *  The output length is fixed to 64 bytes.
  */
static inline
void sha3_512(const unsigned char *input,
              unsigned int inputByteLen,
              unsigned char *output) {
    Keccak_HashInstance state;
    Keccak_HashInitialize(&state, 576,  1024, 512, 0x06);
    Keccak_HashUpdate(&state, input, inputByteLen * 8);
    Keccak_HashFinal(&state, output);
}

