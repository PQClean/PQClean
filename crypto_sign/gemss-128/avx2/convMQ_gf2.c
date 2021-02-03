#include "MQ_gf2.h"
#include "convMQ_gf2.h"
#include "parameters_HFE.h"




/**
 * @brief   Cast between two representations of a MQ equation in
 * GF(2)[x1,...,x_(n+v)] (upper to lower triangular matrix).
 * @details pk = (c,Q), with c the constant part in GF(2) and Q is an upper
 * triangular matrix of size (n+v)*(n+v) in GF(2). pk2 will have the same
 * format, but with a lower triangular matrix. These equations are compressed.
 * @param[in]   pk  A MQ equation in GF(2)[x1,...,x_(n+v)].
 * @param[out]  pk2 A MQ equation in GF(2)[x1,...,x_(n+v)].
 * @remark  Requires to allocate NB_BYTES_EQUATION bytes for pk2. This size is
 * the theoretical size.
 * @remark  Constant-time implementation.
 */
void PQCLEAN_GEMSS128_AVX2_convMQ_UL_gf2(uint8_t *pk2, const uint8_t *pk) {
    unsigned int i, k, nb_bits;
    int j;

    /* Constant + x_0*x_0 */
    *pk2 = (*pk) & 3;

    for (i = 1; i < NB_BYTES_EQUATION; ++i) {
        pk2[i] = 0;
    }

    /* For each row of the output (the first is already done) */
    for (k = 2, i = 2; i <= HFEnv; ++i) {
        nb_bits = i;
        /* For each column */
        for (j = HFEnv - 1; j >= (int)(HFEnv - i); --j, ++k) {
            pk2[k >> 3] ^= ((pk[nb_bits >> 3] >> (nb_bits & 7))&ONE8) << (k & 7);
            nb_bits += j;
        }
    }
}


/* Number of lost bits by the zero padding of each equation
   (without the last) */
#define LOST_BITS ((HFEmr8-1)*HFENr8c)
/* Size of the last equation after to have removed the LOST_BITS last bits. */
#define SIZE_LAST_EQUATION ((NB_MONOMIAL_PK-((HFEmr8-1)*HFENr8c)+7)>>3)



#define HFEnvqm1 ((HFEnv-1)>>6)
#define HFEnvrm1 ((HFEnv-1)&63)

/**
 * @brief   Decompression of a compressed MQ equation in GF(2)[x1,...,x_(n+v)].
 * Both use a lower triangular matrix.
 * @details pk = (c,Q), with c the constant part in GF(2) and Q is a lower
 * triangular matrix of size (n+v)*(n+v) in GF(2). pk2 will have the same
 * format, but the equation will be decompressed. Here, the last bits of pk
 * are missing (cf. the output of convMQ_last_UL_gf2). Moreover, the last byte
 * of pk is padded with null bits.
 * @param[in]   pk  A MQ equation in GF(2)[x1,...,x_(n+v)].
 * @param[out]  pk2 A MQ equation in GF(2)[x1,...,x_(n+v)].
 * @return  The constant c of pk2, in GF(2).
 * @remark  Requires to allocate NB_WORD_UNCOMP_EQ 64-bit words for pk2.
 * @remark  This function is a modified copy of convMQ_uncompressL_gf2.
 * @remark  Constant-time implementation.
 */
UINT PQCLEAN_GEMSS128_AVX2_convMQ_last_uncompressL_gf2(uint64_t *pk2, const uint8_t *pk) {
    unsigned int iq, ir, k, nb_bits;
    uint64_t t1, t2;
    const uint8_t *pk64 = pk;
    const uint8_t *pk_end;
    uint64_t end;
    unsigned int l;


    nb_bits = 1;
    /* For each row */
    for (iq = 0; iq < HFEnvqm1; ++iq) {
        for (ir = 1; ir < 64; ++ir) {
            if (nb_bits & 63) {
                for (k = 0; k < iq; ++k) {
                    LOAD_UINT(t1, &pk64[8 * k])
                    LOAD_UINT(t2, &pk64[8 * (k + 1)])
                    pk2[k] = (t1 >> (nb_bits & 63))
                             ^ (t2 << (64 - (nb_bits & 63)));
                }

                LOAD_UINT(t1, &pk64[8 * k])
                pk2[k] = t1 >> (nb_bits & 63);
                if (((nb_bits & 63) + ir) > 64) {
                    LOAD_UINT(t1, &pk64[8 * (k + 1)])
                    pk2[k] ^= t1 << (64 - (nb_bits & 63));
                }

                if (((nb_bits & 63) + ir) >= 64) {
                    pk64 += 8;
                }
            } else {
                for (k = 0; k <= iq; ++k) {
                    LOAD_UINT(t1, &pk64[8 * k])
                    pk2[k] = t1;
                }
            }

            pk64 += 8 * iq;
            /* 0 padding on the last word */
            pk2[iq] &= (ONE64 << ir) - ONE64;
            pk2 += iq + 1;
            nb_bits += (iq << 6) + ir;
        }

        /* ir=64 */
        if (nb_bits & 63) {
            for (k = 0; k <= iq; ++k) {
                LOAD_UINT(t1, &pk64[8 * k])
                LOAD_UINT(t2, &pk64[8 * (k + 1)])
                pk2[k] = (t1 >> (nb_bits & 63)) ^ (t2 << (64 - (nb_bits & 63)));
            }
        } else {
            for (k = 0; k <= iq; ++k) {
                LOAD_UINT(t1, &pk64[8 * k])
                pk2[k] = t1;
            }
        }
        pk64 += 8 * (iq + 1);
        pk2 += iq + 1;
        nb_bits += (iq + 1) << 6;
    }

    for (ir = 1; ir <= HFEnvrm1; ++ir) {
        if (nb_bits & 63) {
            for (k = 0; k < iq; ++k) {
                LOAD_UINT(t1, &pk64[8 * k])
                LOAD_UINT(t2, &pk64[8 * (k + 1)])
                pk2[k] = (t1 >> (nb_bits & 63))
                         ^ (t2 << (64 - (nb_bits & 63)));
            }

            LOAD_UINT(t1, &pk64[8 * k])
            pk2[k] = t1 >> (nb_bits & 63);
            if (((nb_bits & 63) + ir) > 64) {
                LOAD_UINT(t1, &pk64[8 * (k + 1)])
                pk2[k] ^= t1 << (64 - (nb_bits & 63));
            }

            if (((nb_bits & 63) + ir) >= 64) {
                pk64 += 8;
            }
        } else {
            for (k = 0; k <= iq; ++k) {
                LOAD_UINT(t1, &pk64[8 * k])
                pk2[k] = t1;
            }
        }

        pk64 += 8 * iq;
        /* 0 padding on the last word */
        pk2[iq] &= (ONE64 << ir) - ONE64;
        pk2 += iq + 1;
        nb_bits += (iq << 6) + ir;
    }

    /* Last row */
    /* The size of the last row is HFEnv-LOST_BITS bits */
#define LAST_ROW_Q ((HFEnv-LOST_BITS)>>6)
#define LAST_ROW_R ((HFEnv-LOST_BITS)&63)
    iq = LAST_ROW_Q;

    if (nb_bits & 63) {
#define NB_WHOLE_BLOCKS ((HFEnv-\
                          ((64-((NB_MONOMIAL_PK-LOST_BITS-HFEnvr)&63))&63))>>6)

        for (k = 0; k < NB_WHOLE_BLOCKS; ++k) {
            LOAD_UINT(t1, &pk64[8 * k])
            LOAD_UINT(t2, &pk64[8 * (k + 1)])
            pk2[k] = (t1 >> (nb_bits & 63))
                     ^ (t2 << (64 - (nb_bits & 63)));
        }

        LOAD_UINT(t1, &pk64[8 * k])
        pk2[k] = t1 >> (nb_bits & 63);

        if (((nb_bits & 63) + LAST_ROW_R) > 64) {
            end = 0;
            pk_end = pk64 + 8 * (k + 1);
            for (l = 0; l < (6); ++l) {
                end ^= ((uint64_t)(pk_end[l])) << (l << 3);
            }
            pk2[k] ^= end << (64 - (nb_bits & 63));
        }
    } else {
        for (k = 0; k < iq; ++k) {
            LOAD_UINT(t1, &pk64[8 * k])
            pk2[k] = t1;
        }

        end = 0;
        pk_end = pk64 + 8 * k;
        for (l = 0; l < (6); ++l) {
            end ^= ((uint64_t)(pk_end[l])) << (l << 3);
        }
        pk2[k] = end;
    }

    /* Constant */
    return (*pk) & 1;
}


/**
 * @brief   Decompression of a compressed MQ equation in GF(2)[x1,...,x_(n+v)].
 * Both use a lower triangular matrix.
 * @details pk = (c,Q), with c the constant part in GF(2) and Q is a lower
 * triangular matrix of size (n+v)*(n+v) in GF(2). pk2 will have the same
 * format, but the equation will be decompressed. Here, the last byte of pk is
 * padded with null bits.
 * @param[in]   pk  A MQ equation in GF(2)[x1,...,x_(n+v)].
 * @param[out]  pk2 A MQ equation in GF(2)[x1,...,x_(n+v)].
 * @return  The constant c of pk2, in GF(2).
 * @remark  Requires to allocate NB_WORD_UNCOMP_EQ 64-bit words for pk2.
 * @remark  Requirement: at least NB_BYTES_EQUATION
 * + ((8-(NB_BYTES_EQUATION mod 8)) mod 8) bytes have to be allocated for pk
 * (because pk is casted in 64-bit, and the last memory access requires that
 * is allocated a multiple of 64 bits).
 * @remark  Constant-time implementation.
 */
UINT PQCLEAN_GEMSS128_AVX2_convMQ_uncompressL_gf2(uint64_t *pk2, const uint8_t *pk) {
    const uint8_t *pk64;
    unsigned int iq, ir, k, nb_bits;
    uint64_t t1, t2;

    pk64 = pk;

    nb_bits = 1;
    /* For each row */
    for (iq = 0; iq < HFEnvq; ++iq) {
        for (ir = 1; ir < 64; ++ir) {
            if (nb_bits & 63) {
                for (k = 0; k < iq; ++k) {
                    LOAD_UINT(t1, &pk64[8 * k])
                    LOAD_UINT(t2, &pk64[8 * (k + 1)])
                    pk2[k] = (t1 >> (nb_bits & 63))
                             ^ (t2 << (64 - (nb_bits & 63)));
                }

                LOAD_UINT(t1, &pk64[8 * k])
                pk2[k] = t1 >> (nb_bits & 63);
                if (((nb_bits & 63) + ir) > 64) {
                    LOAD_UINT(t1, &pk64[8 * (k + 1)])
                    pk2[k] ^= t1 << (64 - (nb_bits & 63));
                }

                if (((nb_bits & 63) + ir) >= 64) {
                    pk64 += 8;
                }
            } else {
                for (k = 0; k <= iq; ++k) {
                    LOAD_UINT(t1, &pk64[8 * k])
                    pk2[k] = t1;
                }
            }

            pk64 += 8 * iq;
            /* 0 padding on the last word */
            pk2[iq] &= (ONE64 << ir) - ONE64;
            pk2 += iq + 1;
            nb_bits += (iq << 6) + ir;
        }

        /* ir=64 */
        if (nb_bits & 63) {
            for (k = 0; k <= iq; ++k) {
                LOAD_UINT(t1, &pk64[8 * k])
                LOAD_UINT(t2, &pk64[8 * (k + 1)])
                pk2[k] = (t1 >> (nb_bits & 63)) ^ (t2 << (64 - (nb_bits & 63)));
            }
        } else {
            for (k = 0; k <= iq; ++k) {
                LOAD_UINT(t1, &pk64[8 * k])
                pk2[k] = t1;
            }
        }
        pk64 += 8 * (iq + 1);
        pk2 += iq + 1;
        nb_bits += (iq + 1) << 6;
    }

    for (ir = 1; ir <= HFEnvr; ++ir) {
        if (nb_bits & 63) {
            for (k = 0; k < iq; ++k) {
                LOAD_UINT(t1, &pk64[8 * k])
                LOAD_UINT(t2, &pk64[8 * (k + 1)])
                pk2[k] = (t1 >> (nb_bits & 63))
                         ^ (t2 << (64 - (nb_bits & 63)));
            }

            LOAD_UINT(t1, &pk64[8 * k])
            pk2[k] = t1 >> (nb_bits & 63);
            if (((nb_bits & 63) + ir) > 64) {
                LOAD_UINT(t1, &pk64[8 * (k + 1)])
                pk2[k] ^= t1 << (64 - (nb_bits & 63));
            }

            if (((nb_bits & 63) + ir) >= 64) {
                pk64 += 8;
            }
        } else {
            for (k = 0; k <= iq; ++k) {
                LOAD_UINT(t1, &pk64[8 * k])
                pk2[k] = t1;
            }
        }

        pk64 += 8 * iq;
        /* 0 padding on the last word */
        pk2[iq] &= (ONE64 << ir) - ONE64;
        pk2 += iq + 1;
        nb_bits += (iq << 6) + ir;
    }

    /* Constant */
    return (*pk) & 1;
}
