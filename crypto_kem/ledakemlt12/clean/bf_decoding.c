#include "bf_decoding.h"
#include "gf2x_arith_mod_xPplusOne.h"

#include <string.h>
#include <assert.h>

#define ROTBYTE(a)   ( (a << 8) | (a >> (DIGIT_SIZE_b - 8)) )
#define ROTUPC(a)   ( (a >> 8) | (a << (DIGIT_SIZE_b - 8)) )

int thresholds[2] = {B0, (DV * M) / 2 + 1};

int PQCLEAN_LEDAKEMLT12_CLEAN_bf_decoding(DIGIT out[], // N0 polynomials
        const POSITION_T HtrPosOnes[N0][DV],
        const POSITION_T QtrPosOnes[N0][M],
        DIGIT privateSyndrome[]) {

    uint8_t unsatParityChecks[N0 * P];
    POSITION_T currQBlkPos[M], currQBitPos[M];
    DIGIT currSyndrome[NUM_DIGITS_GF2X_ELEMENT];
    int check;
    int iteration = 0;

    do {
        gf2x_copy(currSyndrome, privateSyndrome);
        memset(unsatParityChecks, 0x00, N0 * P * sizeof(uint8_t));
        for (int i = 0; i < N0; i++) {
            for (int valueIdx = 0; valueIdx < P; valueIdx++) {
                for (int HtrOneIdx = 0; HtrOneIdx < DV; HtrOneIdx++) {
                    POSITION_T tmp = (HtrPosOnes[i][HtrOneIdx] + valueIdx) >= P ? (HtrPosOnes[i][HtrOneIdx] + valueIdx) - P : (HtrPosOnes[i][HtrOneIdx] + valueIdx);
                    if (gf2x_get_coeff(currSyndrome, tmp)) {
                        unsatParityChecks[i * P + valueIdx]++;
                    }
                }
            }
        }

        /* iteration based threshold determination*/
        int corrt_syndrome_based = thresholds[iteration];

        //Computation of correlation  with a full Q matrix
        for (int i = 0; i < N0; i++) {
            for (int j = 0; j < P; j++) {
                int currQoneIdx = 0; // position in the column of QtrPosOnes[][...]
                int endQblockIdx = 0;
                int correlation = 0;

                for (int blockIdx = 0; blockIdx < N0; blockIdx++) {
                    endQblockIdx += qBlockWeights[blockIdx][i];
                    int currblockoffset = blockIdx * P;
                    for (; currQoneIdx < endQblockIdx; currQoneIdx++) {
                        int tmp = QtrPosOnes[i][currQoneIdx] + j;
                        tmp = tmp >= P ? tmp - P : tmp;
                        currQBitPos[currQoneIdx] = tmp;
                        currQBlkPos[currQoneIdx] = blockIdx;
                        correlation += unsatParityChecks[tmp + currblockoffset];
                    }
                }
                /* Correlation based flipping */
                if (correlation >= corrt_syndrome_based) {
                    gf2x_toggle_coeff(out + NUM_DIGITS_GF2X_ELEMENT * i, j);
                    for (int v = 0; v < M; v++) {
                        unsigned syndromePosToFlip;
                        for (int HtrOneIdx = 0; HtrOneIdx < DV; HtrOneIdx++) {
                            syndromePosToFlip = (HtrPosOnes[currQBlkPos[v]][HtrOneIdx] + currQBitPos[v] );
                            syndromePosToFlip = syndromePosToFlip >= P ? syndromePosToFlip - P : syndromePosToFlip;
                            gf2x_toggle_coeff(privateSyndrome, syndromePosToFlip);
                        }
                    } // end for v
                } // end if
            } // end for j
        } // end for i

        iteration = iteration + 1;
        check = 0;
        while (check < NUM_DIGITS_GF2X_ELEMENT && privateSyndrome[check++] == 0);

    } while (iteration < ITERATIONS_MAX && check < NUM_DIGITS_GF2X_ELEMENT);

    return (check == NUM_DIGITS_GF2X_ELEMENT);
}
