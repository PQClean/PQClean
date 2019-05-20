#ifndef SYMMETRIC_H
#define SYMMETRIC_H

#include "fips202.h"
#include "params.h"

typedef struct {
    uint64_t s[25];
} keccak_state;

void PQCLEAN_DILITHIUM4_CLEAN_shake256_absorb(keccak_state *state, const uint8_t *input, size_t inlen);
void PQCLEAN_DILITHIUM4_CLEAN_shake256_squeezeblocks(uint8_t *output, size_t nblocks, keccak_state *state);
void PQCLEAN_DILITHIUM4_CLEAN_shake128_absorb(keccak_state *state, const uint8_t *input, size_t inlen);
void PQCLEAN_DILITHIUM4_CLEAN_shake128_squeezeblocks(uint8_t *output, size_t nblocks, keccak_state *state);
#define PQCLEAN_DILITHIUM4_CLEAN_shake256_absorb(STATE, IN, INBYTES) (shake256_absorb((uint64_t *) (STATE), IN, INBYTES))
#define PQCLEAN_DILITHIUM4_CLEAN_shake256_squeezeblocks(OUTPUT, OUTBLOCKS, STATE) (shake256_squeezeblocks(OUTPUT, OUTBLOCKS, (uint64_t *) (STATE)))
#define PQCLEAN_DILITHIUM4_CLEAN_shake128_absorb(STATE, IN, INBYTES) (shake128_absorb((uint64_t *) (STATE), IN, INBYTES))
#define PQCLEAN_DILITHIUM4_CLEAN_shake128_squeezeblocks(OUTPUT, OUTBLOCKS, STATE) (shake128_squeezeblocks(OUTPUT, OUTBLOCKS, (uint64_t *) (STATE)))


#define crh(OUT, IN, INBYTES) shake256(OUT, CRHBYTES, IN, INBYTES)
#define stream128_init(STATE, SEED, NONCE) PQCLEAN_DILITHIUM4_CLEAN_shake128_stream_init(STATE, SEED, NONCE)
#define stream128_squeezeblocks(OUT, OUTBLOCKS, STATE) PQCLEAN_DILITHIUM4_CLEAN_shake128_squeezeblocks(OUT, OUTBLOCKS, STATE)
#define stream256_init(STATE, SEED, NONCE) PQCLEAN_DILITHIUM4_CLEAN_shake256_stream_init(STATE, SEED, NONCE)
#define stream256_squeezeblocks(OUT, OUTBLOCKS, STATE) PQCLEAN_DILITHIUM4_CLEAN_shake256_squeezeblocks(OUT, OUTBLOCKS, STATE)

#define STREAM128_BLOCKBYTES SHAKE128_RATE
#define STREAM256_BLOCKBYTES SHAKE256_RATE

typedef keccak_state stream128_state;
typedef keccak_state stream256_state;


void PQCLEAN_DILITHIUM4_CLEAN_shake128_stream_init(keccak_state *state,
        const unsigned char *seed,
        uint16_t nonce);
void PQCLEAN_DILITHIUM4_CLEAN_shake256_stream_init(keccak_state *state,
        const unsigned char *seed,
        uint16_t nonce);

#endif
