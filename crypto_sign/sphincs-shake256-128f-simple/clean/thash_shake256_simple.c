#include <stdint.h>
#include <string.h>

#include "thash.h"
#include "address.h"
#include "params.h"

#include "fips202.h"

/**
 * Takes an array of inblocks concatenated arrays of SPX_N bytes.
 */
void PQCLEAN_SPHINCSSHAKE256128FSIMPLE_CLEAN_thash(
    unsigned char *out, const unsigned char *in, unsigned int inblocks,
    const unsigned char *pub_seed, uint32_t addr[8]) {
    unsigned char buf[SPX_N + SPX_ADDR_BYTES + inblocks * SPX_N];

    memcpy(buf, pub_seed, SPX_N);
    PQCLEAN_SPHINCSSHAKE256128FSIMPLE_CLEAN_addr_to_bytes(buf + SPX_N, addr);
    memcpy(buf + SPX_N + SPX_ADDR_BYTES, in, inblocks * SPX_N);

    shake256(out, SPX_N, buf, SPX_N + SPX_ADDR_BYTES + inblocks * SPX_N);
}
