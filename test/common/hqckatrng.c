/**
 * Implementation of a SHAKE-256 based PRNG
 * To be used only in generating KAT values for HQC
 * 
 * From shake_prng.c in hqc-submission_2023-04-30
 * Modified by Spencer Wilson for PQClean
 */

#include "fips202.h"
#include "randombytes.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#define PRNG_DOMAIN 1

shake256incctx shake_prng_state;

void hqc_kat_init(uint8_t *entropy_input, uint8_t *personalization_string, int security_strength);
void hqc_kat_release(void);

/**
 * @brief SHAKE-256 with incremental API and domain separation
 *
 * Derived from function SHAKE_256 in fips202.c
 *
 * @param[in] entropy_input Pointer to input entropy bytes
 * @param[in] personalization_string Pointer to the personalization string
 * @param[in] enlen Length of entropy string in bytes
 * @param[in] perlen Length of the personalization string in bytes
 */
void hqc_kat_init(uint8_t *entropy_input, uint8_t *personalization_string, int security_strength) {
    assert(security_strength == 256);
    uint8_t domain = PRNG_DOMAIN;
    shake256_inc_init(&shake_prng_state);
    shake256_inc_absorb(&shake_prng_state, entropy_input, 48);
    if (personalization_string) {
        shake256_inc_absorb(&shake_prng_state, personalization_string, 48);
    }
    shake256_inc_absorb(&shake_prng_state, &domain, 1);
    shake256_inc_finalize(&shake_prng_state);
}

/**
 * @brief A SHAKE-256 based PRNG
 *
 * Derived from function SHAKE_256 in fips202.c
 *
 * @param[out] output Pointer to output
 * @param[in] outlen length of output in bytes
 */
int randombytes(uint8_t *buf, size_t n) {
    shake256_inc_squeeze(buf, n, &shake_prng_state);
    return 0;
}

/**
 * @brief Release the PRNG context
 * @param[in] state Internal state of the PRNG
 */
void hqc_kat_release(void) {
    shake256_inc_ctx_release(&shake_prng_state);
}
