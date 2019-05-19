/**
 *
 * <niederreiter_decrypt.c>
 *
 * @version 2.0 (March 2019)
 *
 * Reference ISO-C11 Implementation of the LEDAcrypt KEM-LT cipher using GCC built-ins.
 *
 * In alphabetical order:
 *
 * @author Marco Baldi <m.baldi@univpm.it>
 * @author Alessandro Barenghi <alessandro.barenghi@polimi.it>
 * @author Franco Chiaraluce <f.chiaraluce@univpm.it>
 * @author Gerardo Pelosi <gerardo.pelosi@polimi.it>
 * @author Paolo Santini <p.santini@pm.univpm.it>
 *
 * This code is hereby placed in the public domain.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **/

#include "niederreiter_decrypt.h"

#include "qc_ldpc_parameters.h"
#include "gf2x_arith_mod_xPplusOne.h"
#include "H_Q_matrices_generation.h"

#include "bf_decoding.h"
#include "dfr_test.h"
#include <string.h>

/*----------------------------------------------------------------------------*/

int decrypt_niederreiter(DIGIT err[],            // N0 circ poly
                         const privateKeyNiederreiter_t *const sk,
                         const DIGIT syndrome[]  // 1 circ poly
                        ) {
    AES_XOF_struct niederreiter_decrypt_expander;
    seedexpander_from_trng(&niederreiter_decrypt_expander,
                           sk->prng_seed);

    /**************************************************************************/
    // sequence of N0 circ block matrices (p x p):
    POSITION_T HPosOnes[N0][DV];
    POSITION_T HtrPosOnes[N0][DV];
    POSITION_T QPosOnes[N0][M];
    int rejections = sk->rejections;
    POSITION_T LPosOnes[N0][DV * M];
    do {
        generateHPosOnes_HtrPosOnes(HPosOnes, HtrPosOnes,
                                    &niederreiter_decrypt_expander);
        generateQsparse(QPosOnes, &niederreiter_decrypt_expander);
        for (int i = 0; i < N0; i++) {
            for (int j = 0; j < DV * M; j++) {
                LPosOnes[i][j] = INVALID_POS_VALUE;
            }
        }

        POSITION_T auxPosOnes[DV * M];
        unsigned char processedQOnes[N0] = {0};
        for (int colQ = 0; colQ < N0; colQ++) {
            for (int i = 0; i < N0; i++) {
                gf2x_mod_mul_sparse(DV * M, auxPosOnes,
                                    DV, HPosOnes[i],
                                    qBlockWeights[i][colQ], QPosOnes[i] + processedQOnes[i]);
                gf2x_mod_add_sparse(DV * M, LPosOnes[colQ],
                                    DV * M, LPosOnes[colQ],
                                    DV * M, auxPosOnes);
                processedQOnes[i] += qBlockWeights[i][colQ];
            }
        }
        rejections--;
    } while (rejections >= 0);

    POSITION_T QtrPosOnes[N0][M];
    unsigned transposed_ones_idx[N0] = {0x00};
    for (unsigned source_row_idx = 0; source_row_idx < N0 ; source_row_idx++) {
        int currQoneIdx = 0; // position in the column of QtrPosOnes[][...]
        int endQblockIdx = 0;
        for (int blockIdx = 0; blockIdx < N0; blockIdx++) {
            endQblockIdx += qBlockWeights[source_row_idx][blockIdx];
            for (; currQoneIdx < endQblockIdx; currQoneIdx++) {
                QtrPosOnes[blockIdx][transposed_ones_idx[blockIdx]] = (P -
                        QPosOnes[source_row_idx][currQoneIdx]) % P;
                transposed_ones_idx[blockIdx]++;
            }
        }
    }

    POSITION_T auxSparse[DV * M];
    POSITION_T Ln0trSparse[DV * M];
    for (int i = 0; i < DV * M; i++) {
        Ln0trSparse[i] = INVALID_POS_VALUE;
        auxSparse[i] = INVALID_POS_VALUE;
    }

    for (int i = 0; i < N0; i++) {
        gf2x_mod_mul_sparse(DV * M, auxSparse,
                            DV,   HPosOnes[i],
                            qBlockWeights[i][N0 - 1], &QPosOnes[i][ M - qBlockWeights[i][N0 - 1] ]
                           );
        gf2x_mod_add_sparse(DV * M, Ln0trSparse,
                            DV * M, Ln0trSparse,
                            DV * M, auxSparse
                           );
    } // end for i
    gf2x_transpose_in_place_sparse(DV * M, Ln0trSparse);

    DIGIT privateSyndrome[NUM_DIGITS_GF2X_ELEMENT];
    gf2x_mod_mul_dense_to_sparse(privateSyndrome,
                                 syndrome,
                                 Ln0trSparse,
                                 DV * M);

    /* prepare mockup error vector in case a decoding failure occurs */
    DIGIT mockup_error_vector[N0 * NUM_DIGITS_GF2X_ELEMENT];
    memset(mockup_error_vector, 0x00, N0 * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);
    memcpy(mockup_error_vector, syndrome, NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);
    seedexpander(&niederreiter_decrypt_expander,
                 ((unsigned char *) mockup_error_vector) + (NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B), TRNG_BYTE_LENGTH);

    int decryptOk = 0;
    memset(err, 0x00, N0 * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);
    decryptOk = bf_decoding(err, (const POSITION_T (*)[DV]) HtrPosOnes,
                            (const POSITION_T (*)[M]) QtrPosOnes, privateSyndrome);

    int err_weight = 0;
    for (int i = 0 ; i < N0; i++) {
        err_weight += population_count(err + (NUM_DIGITS_GF2X_ELEMENT * i));
    }
    decryptOk = decryptOk && (err_weight == NUM_ERRORS_T);

    if (!decryptOk) {
        memcpy(err, mockup_error_vector, N0 * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);
    }

    return decryptOk;
} // end decrypt_niederreiter

/*----------------------------------------------------------------------------*/


