/* Based on the public domain implementation in
 * crypto_hash/sha512/ref/ from http://bench.cr.yp.to/supercop.html
 * by D. J. Bernstein */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "sha2.h"
#include "sha256.h"
#include "utils.h"

/*
 * Compresses an address to a 22-byte sequence.
 * This reduces the number of required SHA256 compression calls, as the last
 * block of input is padded with at least 65 bits.
 */
void PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_compress_address(unsigned char *out, const uint32_t addr[8]) {
    PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_ull_to_bytes(out,      1, addr[0]); /* drop 3 bytes of the layer field */
    PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_ull_to_bytes(out + 1,  4, addr[2]); /* drop the highest tree address word */
    PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_ull_to_bytes(out + 5,  4, addr[3]);
    PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_ull_to_bytes(out + 9,  1, addr[4]); /* drop 3 bytes of the type field */
    PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_ull_to_bytes(out + 10, 4, addr[5]);
    PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_ull_to_bytes(out + 14, 4, addr[6]);
    PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_ull_to_bytes(out + 18, 4, addr[7]);
}

/**
 * Note that inlen should be sufficiently small that it still allows for
 * an array to be allocated on the stack. Typically 'in' is merely a seed.
 * Outputs outlen number of bytes
 */
void PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_mgf1(
    unsigned char *out, unsigned long outlen,
    const unsigned char *in, unsigned long inlen) {
    unsigned char inbuf[inlen + 4];
    unsigned char outbuf[SPX_SHA256_OUTPUT_BYTES];
    unsigned long i;

    memcpy(inbuf, in, inlen);

    /* While we can fit in at least another full block of SHA256 output.. */
    for (i = 0; (i + 1)*SPX_SHA256_OUTPUT_BYTES <= outlen; i++) {
        PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_ull_to_bytes(inbuf + inlen, 4, i);
        sha256(out, inbuf, inlen + 4);
        out += SPX_SHA256_OUTPUT_BYTES;
    }
    /* Until we cannot anymore, and we fill the remainder. */
    if (outlen > i * SPX_SHA256_OUTPUT_BYTES) {
        PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_ull_to_bytes(inbuf + inlen, 4, i);
        sha256(outbuf, inbuf, inlen + 4);
        memcpy(out, outbuf, outlen - i * SPX_SHA256_OUTPUT_BYTES);
    }
}

uint8_t PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_state_seeded[40];

/**
 * Absorb the constant pub_seed using one round of the compression function
 * This initializes state_seeded, which can then be reused in thash
 **/
void PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_seed_state(const unsigned char *pub_seed) {
    uint8_t block[SPX_SHA256_BLOCK_BYTES];
    size_t i;

    for (i = 0; i < SPX_N; ++i) {
        block[i] = pub_seed[i];
    }
    for (i = SPX_N; i < SPX_SHA256_BLOCK_BYTES; ++i) {
        block[i] = 0;
    }

    sha256_inc_init(PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_state_seeded);
    sha256_inc_blocks(PQCLEAN_SPHINCSSHA256256SSIMPLE_CLEAN_state_seeded, block, 1);
}
