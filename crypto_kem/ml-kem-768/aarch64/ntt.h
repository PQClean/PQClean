#ifndef PQCLEAN_MLKEM768_AARCH64_NTT_H
#define PQCLEAN_MLKEM768_AARCH64_NTT_H

/*
 * We offer
 * CC0 1.0 Universal or the following MIT License for this file.
 * You may freely choose one of them that applies.
 *
 * MIT License
 *
 * Copyright (c) 2023: Hanno Becker, Vincent Hwang, Matthias J. Kannwischer, Bo-Yin Yang, and Shang-Yi Yang
 * Copyright (c) 2023: Vincent Hwang
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "params.h"
#include <stdint.h>

#include "NTT_params.h"

#define asymmetric_const KYBER_NAMESPACE(asymmetric_const)
#define constants KYBER_NAMESPACE(constants)
#define streamlined_CT_negacyclic_table_Q1_jump_extended KYBER_NAMESPACE(streamlined_CT_negacyclic_table_Q1_jump_extended)
#define pre_asymmetric_table_Q1_extended KYBER_NAMESPACE(pre_asymmetric_table_Q1_extended)
#define streamlined_inv_GS_negacyclic_table_Q1_jump_extended KYBER_NAMESPACE(streamlined_inv_GS_negacyclic_table_Q1_jump_extended)

extern void PQCLEAN_MLKEM768_AARCH64__asm_ntt_SIMD_top(int16_t *, const int16_t *, const int16_t *);
extern void PQCLEAN_MLKEM768_AARCH64__asm_ntt_SIMD_bot(int16_t *, const int16_t *, const int16_t *);

extern void PQCLEAN_MLKEM768_AARCH64__asm_intt_SIMD_bot(int16_t *, const int16_t *, const int16_t *);
extern void PQCLEAN_MLKEM768_AARCH64__asm_intt_SIMD_top(int16_t *, const int16_t *, const int16_t *);

extern void PQCLEAN_MLKEM768_AARCH64__asm_point_mul_extended(int16_t *, const int16_t *, const int16_t *, const int16_t *);
extern void PQCLEAN_MLKEM768_AARCH64__asm_asymmetric_mul(const int16_t *, const int16_t *, const int16_t *, const int16_t *, int16_t *);
extern void PQCLEAN_MLKEM768_AARCH64__asm_asymmetric_mul_montgomery(const int16_t *, const int16_t *, const int16_t *, const int16_t *, int16_t *);

extern
const int16_t asymmetric_const[8];
extern
const int16_t constants[16];

extern
const int16_t streamlined_CT_negacyclic_table_Q1_jump_extended[((NTT_N - 1) + (1 << 0) + (1 << 4) + NTT_N) << 1];

extern
const int16_t pre_asymmetric_table_Q1_extended[ARRAY_N];

extern
const int16_t streamlined_inv_GS_negacyclic_table_Q1_jump_extended[((NTT_N - 1) + (1 << 0) + (1 << 4) + NTT_N) << 1];

#define NTT(in) do { \
        PQCLEAN_MLKEM768_AARCH64__asm_ntt_SIMD_top(in, streamlined_CT_negacyclic_table_Q1_jump_extended, constants); \
        PQCLEAN_MLKEM768_AARCH64__asm_ntt_SIMD_bot(in, streamlined_CT_negacyclic_table_Q1_jump_extended, constants); \
    } while(0)

#define iNTT(in) do { \
        PQCLEAN_MLKEM768_AARCH64__asm_intt_SIMD_bot(in, streamlined_inv_GS_negacyclic_table_Q1_jump_extended, constants); \
        PQCLEAN_MLKEM768_AARCH64__asm_intt_SIMD_top(in, streamlined_inv_GS_negacyclic_table_Q1_jump_extended, constants); \
    } while(0)

#define ntt KYBER_NAMESPACE(ntt)
void ntt(int16_t r[256]);
#define invntt KYBER_NAMESPACE(invntt)
void invntt(int16_t r[256]);

#endif
