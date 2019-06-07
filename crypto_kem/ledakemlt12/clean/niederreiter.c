#include "H_Q_matrices_generation.h"
#include "bf_decoding.h"
#include "dfr_test.h"
#include "gf2x_arith_mod_xPplusOne.h"
#include "niederreiter.h"
#include "qc_ldpc_parameters.h"
#include "rng.h"

#include <string.h>

void PQCLEAN_LEDAKEMLT12_CLEAN_niederreiter_keygen(publicKeyNiederreiter_t *pk, privateKeyNiederreiter_t *sk, AES_XOF_struct *keys_expander) {

    // sequence of N0 circ block matrices (p x p): Hi
    POSITION_T HPosOnes[N0][DV];
    POSITION_T HtrPosOnes[N0][DV];
    /* Sparse representation of the transposed circulant matrix H,
    with weight DV. Each index contains the position of a '1' digit in the
    corresponding Htr block */

    /* Sparse representation of the matrix (Q).
    A matrix containing the positions of the ones in the circulant
    blocks of Q. Each row contains the position of the
    ones of all the blocks of a row of Q as exponent+
    P*block_position */
    POSITION_T QPosOnes[N0][M];

    /*Rejection-sample for a full L*/
    POSITION_T LPosOnes[N0][DV * M];
    int is_L_full = 0;
    int isDFRok = 0;
    sk->rejections = (int8_t) 0;
    do {
        PQCLEAN_LEDAKEMLT12_CLEAN_generateHPosOnes_HtrPosOnes(HPosOnes, HtrPosOnes, keys_expander);
        PQCLEAN_LEDAKEMLT12_CLEAN_generateQsparse(QPosOnes, keys_expander);
        for (int i = 0; i < N0; i++) {
            for (int j = 0; j < DV * M; j++) {
                LPosOnes[i][j] = INVALID_POS_VALUE;
            }
        }

        POSITION_T auxPosOnes[DV * M];
        unsigned char processedQOnes[N0] = {0};
        for (int colQ = 0; colQ < N0; colQ++) {
            for (int i = 0; i < N0; i++) {
                PQCLEAN_LEDAKEMLT12_CLEAN_gf2x_mod_mul_sparse(DV * M, auxPosOnes,
                        DV, HPosOnes[i],
                        qBlockWeights[i][colQ], QPosOnes[i] + processedQOnes[i]);
                PQCLEAN_LEDAKEMLT12_CLEAN_gf2x_mod_add_sparse(DV * M, LPosOnes[colQ],
                        DV * M, LPosOnes[colQ],
                        DV * M, auxPosOnes);
                processedQOnes[i] += qBlockWeights[i][colQ];
            }
        }
        is_L_full = 1;
        for (int i = 0; i < N0; i++) {
            is_L_full = is_L_full && (LPosOnes[i][DV * M - 1] != INVALID_POS_VALUE);
        }
        sk->rejections = sk->rejections + 1;
        if (is_L_full) {
            isDFRok = PQCLEAN_LEDAKEMLT12_CLEAN_DFR_test(LPosOnes);
        }
    } while (!is_L_full || !isDFRok);
    sk->rejections = sk->rejections - 1;

    DIGIT Ln0dense[NUM_DIGITS_GF2X_ELEMENT] = {0x00};
    for (int j = 0; j < DV * M; j++) {
        if (LPosOnes[N0 - 1][j] != INVALID_POS_VALUE) {
            gf2x_set_coeff(Ln0dense, LPosOnes[N0 - 1][j], 1);
        }
    }
    DIGIT Ln0Inv[NUM_DIGITS_GF2X_ELEMENT] = {0x00};
    PQCLEAN_LEDAKEMLT12_CLEAN_gf2x_mod_inverse(Ln0Inv, Ln0dense);
    for (int i = 0; i < N0 - 1; i++) {
        PQCLEAN_LEDAKEMLT12_CLEAN_gf2x_mod_mul_dense_to_sparse(pk->Mtr + i * NUM_DIGITS_GF2X_ELEMENT,
                Ln0Inv,
                LPosOnes[i],
                DV * M);
    }

    for (int i = 0; i < N0 - 1; i++) {
        PQCLEAN_LEDAKEMLT12_CLEAN_gf2x_transpose_in_place(pk->Mtr + i * NUM_DIGITS_GF2X_ELEMENT);
    }
}


void PQCLEAN_LEDAKEMLT12_CLEAN_niederreiter_encrypt(DIGIT *syndrome, const publicKeyNiederreiter_t *pk, const DIGIT *err) {
    int i;
    DIGIT saux[NUM_DIGITS_GF2X_ELEMENT];

    memset(syndrome, 0x00, NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);

    for (i = 0; i < N0 - 1; i++) {
        PQCLEAN_LEDAKEMLT12_CLEAN_gf2x_mod_mul(saux,
                                               pk->Mtr + i * NUM_DIGITS_GF2X_ELEMENT,
                                               err + i * NUM_DIGITS_GF2X_ELEMENT);
        gf2x_mod_add(syndrome, syndrome, saux);
    }  // end for
    gf2x_mod_add(syndrome, syndrome, err + (N0 - 1)*NUM_DIGITS_GF2X_ELEMENT);
}


int PQCLEAN_LEDAKEMLT12_CLEAN_niederreiter_decrypt(DIGIT *err, const privateKeyNiederreiter_t *sk, const DIGIT *syndrome) {

    AES_XOF_struct niederreiter_decrypt_expander;
    PQCLEAN_LEDAKEMLT12_CLEAN_seedexpander_from_trng(&niederreiter_decrypt_expander, sk->prng_seed);

    // sequence of N0 circ block matrices (p x p):
    POSITION_T HPosOnes[N0][DV];
    POSITION_T HtrPosOnes[N0][DV];
    POSITION_T QPosOnes[N0][M];
    int rejections = sk->rejections;
    POSITION_T LPosOnes[N0][DV * M];
    do {
        PQCLEAN_LEDAKEMLT12_CLEAN_generateHPosOnes_HtrPosOnes(HPosOnes, HtrPosOnes, &niederreiter_decrypt_expander);
        PQCLEAN_LEDAKEMLT12_CLEAN_generateQsparse(QPosOnes, &niederreiter_decrypt_expander);
        for (int i = 0; i < N0; i++) {
            for (int j = 0; j < DV * M; j++) {
                LPosOnes[i][j] = INVALID_POS_VALUE;
            }
        }

        POSITION_T auxPosOnes[DV * M];
        unsigned char processedQOnes[N0] = {0};
        for (int colQ = 0; colQ < N0; colQ++) {
            for (int i = 0; i < N0; i++) {
                PQCLEAN_LEDAKEMLT12_CLEAN_gf2x_mod_mul_sparse(DV * M, auxPosOnes,
                        DV, HPosOnes[i],
                        qBlockWeights[i][colQ], QPosOnes[i] + processedQOnes[i]);
                PQCLEAN_LEDAKEMLT12_CLEAN_gf2x_mod_add_sparse(DV * M, LPosOnes[colQ],
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
        PQCLEAN_LEDAKEMLT12_CLEAN_gf2x_mod_mul_sparse(DV * M, auxSparse,
                DV, HPosOnes[i],
                qBlockWeights[i][N0 - 1], &QPosOnes[i][ M - qBlockWeights[i][N0 - 1]]);
        PQCLEAN_LEDAKEMLT12_CLEAN_gf2x_mod_add_sparse(DV * M, Ln0trSparse,
                DV * M, Ln0trSparse,
                DV * M, auxSparse);
    } // end for i

    PQCLEAN_LEDAKEMLT12_CLEAN_gf2x_transpose_in_place_sparse(DV * M, Ln0trSparse);

    DIGIT privateSyndrome[NUM_DIGITS_GF2X_ELEMENT];
    PQCLEAN_LEDAKEMLT12_CLEAN_gf2x_mod_mul_dense_to_sparse(privateSyndrome, syndrome, Ln0trSparse, DV * M);

    /* prepare mockup error vector in case a decoding failure occurs */
    DIGIT mockup_error_vector[N0 * NUM_DIGITS_GF2X_ELEMENT];
    memset(mockup_error_vector, 0x00, N0 * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);
    memcpy(mockup_error_vector, syndrome, NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);
    PQCLEAN_LEDAKEMLT12_CLEAN_seedexpander(&niederreiter_decrypt_expander,
                                           ((unsigned char *) mockup_error_vector) + (NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B),
                                           TRNG_BYTE_LENGTH);

    int decryptOk = 0;
    memset(err, 0x00, N0 * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);
    decryptOk = PQCLEAN_LEDAKEMLT12_CLEAN_bf_decoding(err, (const POSITION_T (*)[DV]) HtrPosOnes,
                (const POSITION_T (*)[M]) QtrPosOnes, privateSyndrome);

    int err_weight = 0;
    for (int i = 0 ; i < N0; i++) {
        err_weight += population_count(err + (NUM_DIGITS_GF2X_ELEMENT * i));
    }
    decryptOk = decryptOk && (err_weight == NUM_ERRORS_T);

    if (!decryptOk) { // TODO: not constant time
        memcpy(err, mockup_error_vector, N0 * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);
    }

    return decryptOk;
}
