#include "niederreiter_encrypt.h"
#include "qc_ldpc_parameters.h"
#include "gf2x_arith_mod_xPplusOne.h"

#include <string.h> // memset(...)

void encrypt_niederreiter(DIGIT syndrome[],                // 1  polynomial
                          const publicKeyNiederreiter_t *const pk,
                          const DIGIT err[]) {             // N0  polynomials
    int i;
    DIGIT saux[NUM_DIGITS_GF2X_ELEMENT];

    memset(syndrome, 0x00, NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);

    for (i = 0; i < N0 - 1; i++) {
        gf2x_mod_mul(saux,
                     pk->Mtr + i * NUM_DIGITS_GF2X_ELEMENT,
                     err + i * NUM_DIGITS_GF2X_ELEMENT
                    );
        gf2x_mod_add(syndrome, syndrome, saux);
    }  // end for
    gf2x_mod_add(syndrome, syndrome, err + (N0 - 1)*NUM_DIGITS_GF2X_ELEMENT);
} // end encrypt_niederreiter

/*----------------------------------------------------------------------------*/
