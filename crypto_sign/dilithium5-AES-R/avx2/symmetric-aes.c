#include "aes256ctr.h"
#include "symmetric.h"
#include <stdint.h>

void dilithium_aes256ctr_init(aes256ctr_ctx *state,
                              const uint8_t key[32],
                              uint16_t nonce) {
    aes256ctr_init(state, key, nonce);
}
