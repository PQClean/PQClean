#ifndef _SIGN_OPENHFE_H
#define _SIGN_OPENHFE_H
#include "arch.h"
#include <stddef.h>
#include <stdint.h>


int PQCLEAN_GEMSS128_AVX2_sign_openHFE_uncomp_pk(const unsigned char *m, size_t len,
        const unsigned char *sm8,
        const uint8_t *pk,
        void (*eval_pk)());


int PQCLEAN_GEMSS128_AVX2_sign_openHFE_huncomp_pk(const unsigned char *m, size_t len,
        const unsigned char *sm8,
        const uint8_t *pk, const uint8_t *hpk,
        void (*eval_pk)());


/* Verify a signature: m is the original message, sm8 is the signed message,
 * pk is the public-key. */
int PQCLEAN_GEMSS128_AVX2_sign_openHFE(const unsigned char *m, size_t len,
                                       const unsigned char *sm8, const uint8_t *pk);


#endif
