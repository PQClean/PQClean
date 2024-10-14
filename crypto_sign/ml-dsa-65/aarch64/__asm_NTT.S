
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

#include "macros.inc"

#include "params.h"

.align 2
.global PQCLEAN_MLDSA65_AARCH64__asm_ntt_SIMD_top
.global _PQCLEAN_MLDSA65_AARCH64__asm_ntt_SIMD_top
PQCLEAN_MLDSA65_AARCH64__asm_ntt_SIMD_top:
_PQCLEAN_MLDSA65_AARCH64__asm_ntt_SIMD_top:

    push_simd
    Q         .req w8
    src       .req x0
    counter   .req x11

    ld1 {v20.4S, v21.4S, v22.4S, v23.4S}, [x1], #64
    ld1 {v24.4S, v25.4S, v26.4S, v27.4S}, [x1], #64

    ldr Q, [x2]
    mov v20.S[0], Q

    ldr   q9, [src,  #9*64]
    ldr  q11, [src, #11*64]
    ldr  q13, [src, #13*64]
    ldr  q15, [src, #15*64]

    qq_butterfly_topl \
             v9, v11, v13, v15, v16, v17, v18, v19, v20, \
            v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, \
            src, \
             q1,  q3,  q5,  q7, \
             #1*64, #3*64, #5*64, #7*64

    qq_butterfly_mixll \
             v1,  v3,  v5,  v7,  v9, v11, v13, v15, v16, v17, v18, v19, \
             v8, v10, v12, v14, v28, v29, v30, v31, \
            v20, \
            v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, \
            src, \
             q8, q10, q12, q14, \
             #8*64, #10*64, #12*64, #14*64, \
            src, \
             q0,  q2,  q4,  q6, \
             #0*64, #2*64, #4*64, #6*64

    qq_butterfly_mix  v0,  v2,  v4,  v6,  v8, v10, v12, v14, v28, v29, v30, v31,  v1,  v3,  v9, v11,  v5,  v7, v13, v15, v16, v17, v18, v19, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3
    qq_butterfly_mix  v1,  v3,  v9, v11,  v5,  v7, v13, v15, v16, v17, v18, v19,  v0,  v2,  v8, v10,  v4,  v6, v12, v14, v28, v29, v30, v31, v20, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3
    qq_butterfly_mix  v0,  v2,  v8, v10,  v4,  v6, v12, v14, v28, v29, v30, v31,  v1,  v5,  v9, v13,  v3,  v7, v11, v15, v16, v17, v18, v19, v20, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3
    qq_butterfly_mix  v1,  v5,  v9, v13,  v3,  v7, v11, v15, v16, v17, v18, v19,  v0,  v4,  v8, v12,  v2,  v6, v10, v14, v28, v29, v30, v31, v20, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3
    qq_butterfly_mix  v0,  v4,  v8, v12,  v2,  v6, v10, v14, v28, v29, v30, v31,  v8, v10, v12, v14,  v9, v11, v13, v15, v16, v17, v18, v19, v20, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3, v26, 0, 1, v26, 2, 3, v27, 0, 1, v27, 2, 3

    qq_butterfly_mixssl \
             v8, v10, v12, v14,  v9, v11, v13, v15, v16, v17, v18, v19, \
             v1,  v3,  v5,  v7, v28, v29, v30, v31, \
            v20, \
            v24, 0, 1, v24, 2, 3, v25, 0, 1, v25, 2, 3, \
            src, \
             q9, q11, q13, q15, \
             #9*64, #11*64, #13*64, #15*64, \
            src, \
             q8, q10, q12, q14, \
             #8*64, #10*64, #12*64, #14*64, \
            src, \
             q9, q11, q13, q15, \
             #(16+9*64), #(16+11*64), #(16+13*64), #(16+15*64)

    mov counter, #3
    _ntt_top_loop:

    qq_butterfly_mixssl \
             v0,  v2,  v4,  v6,  v1,  v3,  v5,  v7, v28, v29, v30, v31, \
             v9, v11, v13, v15, v16, v17, v18, v19, \
            v20, \
            v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, \
            src, \
             q1,  q3,  q5,  q7, \
             #1*64, #3*64, #5*64, #7*64, \
            src, \
             q0,  q2,  q4,  q6, \
             #0*64, #2*64, #4*64, #6*64, \
            src, \
             q1,  q3,  q5,  q7, \
             #(16+1*64), #(16+3*64), #(16+5*64), #(16+7*64)

    qq_butterfly_mixll \
             v1,  v3,  v5,  v7,  v9, v11, v13, v15, v16, v17, v18, v19, \
             v8, v10, v12, v14, v28, v29, v30, v31, \
            v20, \
            v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, \
            src, \
             q8, q10, q12, q14, \
             #(16+8*64), #(16+10*64), #(16+12*64), #(16+14*64), \
            src, \
             q0,  q2,  q4,  q6, \
             #(16+0*64), #(16+2*64), #(16+4*64), #(16+6*64)

    add src, src, #16

    qq_butterfly_mix  v0,  v2,  v4,  v6,  v8, v10, v12, v14, v28, v29, v30, v31,  v1,  v3,  v9, v11,  v5,  v7, v13, v15, v16, v17, v18, v19, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3
    qq_butterfly_mix  v1,  v3,  v9, v11,  v5,  v7, v13, v15, v16, v17, v18, v19,  v0,  v2,  v8, v10,  v4,  v6, v12, v14, v28, v29, v30, v31, v20, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3
    qq_butterfly_mix  v0,  v2,  v8, v10,  v4,  v6, v12, v14, v28, v29, v30, v31,  v1,  v5,  v9, v13,  v3,  v7, v11, v15, v16, v17, v18, v19, v20, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3
    qq_butterfly_mix  v1,  v5,  v9, v13,  v3,  v7, v11, v15, v16, v17, v18, v19,  v0,  v4,  v8, v12,  v2,  v6, v10, v14, v28, v29, v30, v31, v20, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3
    qq_butterfly_mix  v0,  v4,  v8, v12,  v2,  v6, v10, v14, v28, v29, v30, v31,  v8, v10, v12, v14,  v9, v11, v13, v15, v16, v17, v18, v19, v20, v22, 0, 1, v22, 2, 3, v23, 0, 1, v23, 2, 3, v26, 0, 1, v26, 2, 3, v27, 0, 1, v27, 2, 3

    qq_butterfly_mixssl \
             v8, v10, v12, v14,  v9, v11, v13, v15, v16, v17, v18, v19, \
             v1,  v3,  v5,  v7, v28, v29, v30, v31, \
            v20, \
            v24, 0, 1, v24, 2, 3, v25, 0, 1, v25, 2, 3, \
            src, \
             q9, q11, q13, q15, \
             #9*64, #11*64, #13*64, #15*64, \
            src, \
             q8, q10, q12, q14, \
             #8*64, #10*64, #12*64, #14*64, \
            src, \
             q9, q11, q13, q15, \
             #(16+9*64), #(16+11*64), #(16+13*64), #(16+15*64)

    sub counter, counter, #1
    cbnz counter, _ntt_top_loop

    qq_butterfly_botss \
             v0,  v2,  v4,  v6,  v1,  v3,  v5,  v7, v28, v29, v30, v31, \
            src, \
              q1,  q3,  q5,  q7, \
              #1*64, #3*64, #5*64, #7*64, \
            src, \
              q0,  q2,  q4,  q6, \
              #0*64, #2*64, #4*64, #6*64

    .unreq    Q
    .unreq    src
    .unreq    counter
    pop_simd

    ret

.align 2
.global PQCLEAN_MLDSA65_AARCH64__asm_ntt_SIMD_bot
.global _PQCLEAN_MLDSA65_AARCH64__asm_ntt_SIMD_bot
PQCLEAN_MLDSA65_AARCH64__asm_ntt_SIMD_bot:
_PQCLEAN_MLDSA65_AARCH64__asm_ntt_SIMD_bot:

    push_simd
    Q         .req w8
    src       .req x0
    table0    .req x9
    table1    .req x10
    counter   .req x11

    ldr Q, [x2]

    add table0, x1, #128
    add table1, table0, #1024

    ldr  q0, [src,  #0*16]
    ldr  q1, [src,  #1*16]
    ldr  q2, [src,  #2*16]
    ldr  q3, [src,  #3*16]

    ldr  q4, [table0, #0*16]
    ldr  q5, [table0, #1*16]
    ldr q20, [table1, #0*16]
    ldr q21, [table1, #1*16]

    dq_butterfly_topl4 \
             v0,  v1,  v2,  v3, v12, v13, v4,  v4, 2, 3,  v4, 2, 3, \
             src, \
             q16, q17, q18, q19, \
             #(512+0*16), #(512+1*16), #(512+2*16), #(512+3*16)

    dq_butterfly_mix  v0,  v1,  v2,  v3, v12, v13, v16, v17, v18, v19, v28, v29, v4,  v4, 2, 3,  v4, 2, 3, v20, 2, 3, v20, 2, 3

    dq_butterfly_mixl6 \
            v16, v17, v18, v19, v28, v29,  v0,  v2,  v1,  v3, v12, v13, \
             v4, \
            v20, 2, 3, v20, 2, 3,  v5, 0, 1,  v5, 2, 3, \
            table0, \
             q6,  q7,  q8,  q9, q10, q11, \
             #2*16, #3*16, #4*16, #5*16, #6*16, #7*16

    dq_butterfly_mixl6 \
             v0,  v2,  v1,  v3, v12, v13, v16, v18, v17, v19, v28, v29, \
             v4, \
             v5, 0, 1,  v5, 2, 3, v21, 0, 1, v21, 2, 3, \
            table1, \
            q22, q23, q24, q25, q26, q27, \
            #2*16, #3*16, #4*16, #5*16, #6*16, #7*16
    dq_butterfly_bot v16, v18, v17, v19, v28, v29, v4, v21, 0, 1, v21, 2, 3

    add table0, table0, #128
    add table1, table1, #128

    trn_4x4  v0,  v1,  v2,  v3, v12, v13, v14, v15

    dq_butterfly_vec_top_trn_4x4 \
             v0,  v1,  v2,  v3, v12, v13, v4,  v6,  v7,  v6,  v7, \
             v16, v17, v18, v19, v28, v29, v30, v31

    dq_butterfly_vec_mix  v0,  v1,  v2,  v3, v12, v13, v16, v17, v18, v19, v28, v29, v4,  v6,  v7,  v6,  v7, v22, v23, v22, v23
    dq_butterfly_vec_mix v16, v17, v18, v19, v28, v29,  v0,  v2,  v1,  v3, v12, v13, v4, v22, v23, v22, v23,  v8,  v9, v10, v11
    dq_butterfly_vec_mix  v0,  v2,  v1,  v3, v12, v13, v16, v18, v17, v19, v28, v29, v4,  v8,  v9, v10, v11, v24, v25, v26, v27


    trn_4x4_l4  v0,  v1,  v2,  v3,  v8,  v9, v10, v11, src, q12, q13, q14, q15, #(64+0*16), #(64+1*16), #(64+2*16), #(64+3*16)

    str  q0, [src,  #0*16]
    str  q2, [src,  #2*16]

    dq_butterfly_vec_bot v16, v18, v17, v19, v28, v29, v4, v24, v25, v26, v27

    str  q1, [src,  #1*16]
    str  q3, [src,  #3*16]

    add src, src, #64

    trn_4x4_l4 v16, v17, v18, v19, v24, v25, v26, v27, src, q28, q29, q30, q31, #(512+0*16), #(512+1*16), #(512+2*16), #(512+3*16)

    sub src, src, #64

    dq_butterfly_top2l4s4 \
            v12, v13, v14, v15,  v0,  v1, v4,  v4, 2, 3,  v4, 2, 3, \
            table0,  q4,  q5,  #0*16, #1*16, \
            table1, q20, q21,  #0*16, #1*16, \
            src, \
            q16, q17, q18, q19, \
            #(512+0*16), #(512+1*16), #(512+2*16), #(512+3*16)

    add src, src, #64

    dq_butterfly_mix v12, v13, v14, v15,  v0,  v1, v28, v29, v30, v31, v16, v17, v4,  v4, 2, 3,  v4, 2, 3, v20, 2, 3, v20, 2, 3

    dq_butterfly_mixl6 \
            v28, v29, v30, v31, v16, v17, v12, v14, v13, v15, v0, v1, \
             v4, \
            v20, 2, 3, v20, 2, 3,  v5, 0, 1,  v5, 2, 3, \
            table0, \
             q6,  q7,  q8,  q9, q10, q11, \
             #2*16, #3*16, #4*16, #5*16, #6*16, #7*16

    dq_butterfly_mixl6 \
            v12, v14, v13, v15,  v0,  v1, v28, v30, v29, v31, v16, v17, \
             v4, \
             v5, 0, 1,  v5, 2, 3, v21, 0, 1, v21, 2, 3, \
            table1, \
            q22, q23, q24, q25, q26, q27, \
            #2*16, #3*16, #4*16, #5*16, #6*16, #7*16

    dq_butterfly_bot v28, v30, v29, v31, v16, v17, v4, v21, 0, 1, v21, 2, 3

    add table0, table0, #128
    add table1, table1, #128

    trn_4x4 v12, v13, v14, v15, v0,  v1,  v2,  v3

    dq_butterfly_vec_top_trn_4x4 \
            v12, v13, v14, v15,  v0,  v1, v4,  v6,  v7,  v6,  v7, \
            v28, v29, v30, v31, v16, v17, v18, v19

    dq_butterfly_vec_mix v12, v13, v14, v15,  v0,  v1, v28, v29, v30, v31, v16, v17, v4,  v6,  v7,  v6,  v7, v22, v23, v22, v23
    dq_butterfly_vec_mix v28, v29, v30, v31, v16, v17, v12, v14, v13, v15,  v0,  v1, v4, v22, v23, v22, v23,  v8,  v9, v10, v11
    dq_butterfly_vec_mix v12, v14, v13, v15,  v0,  v1, v28, v30, v29, v31, v16, v17, v4,  v8,  v9, v10, v11, v24, v25, v26, v27

    mov counter, #3
    _ntt_bot_loop:

    trn_4x4_l4 v12, v13, v14, v15,  v8,  v9, v10, v11, src,  q0,  q1,  q2,  q3, #(64+0*16), #(64+1*16), #(64+2*16), #(64+3*16)

    str q12, [src, #0*16]
    str q13, [src, #1*16]

    dq_butterfly_vec_bot v28, v30, v29, v31, v16, v17, v4, v24, v25, v26, v27

    str q14, [src, #2*16]
    str q15, [src, #3*16]


    add src, src, #64

    trn_4x4_l4 v28, v29, v30, v31, v24, v25, v26, v27, src, q16, q17, q18, q19, #(512+0*16), #(512+1*16), #(512+2*16), #(512+3*16)

    sub src, src, #64

    dq_butterfly_top2l4s4 \
             v0,  v1,  v2,  v3, v12, v13, v4,  v4, 2, 3,  v4, 2, 3, \
             table0,  q4,  q5, #0*16, #1*16, \
             table1, q20, q21, #0*16, #1*16, \
             src, \
             q28, q29, q30, q31, \
             #(512+0*16), #(512+1*16), #(512+2*16), #(512+3*16)

    add src, src, #64

    dq_butterfly_mix  v0,  v1,  v2,  v3, v12, v13, v16, v17, v18, v19, v28, v29, v4,  v4, 2, 3,  v4, 2, 3, v20, 2, 3, v20, 2, 3

    dq_butterfly_mixl6 \
            v16, v17, v18, v19, v28, v29,  v0,  v2,  v1,  v3, v12, v13, \
             v4, \
            v20, 2, 3, v20, 2, 3,  v5, 0, 1,  v5, 2, 3, \
            table0, \
             q6,  q7,  q8,  q9, q10, q11, \
             #2*16, #3*16, #4*16, #5*16, #6*16, #7*16

    dq_butterfly_mixl6 \
             v0,  v2,  v1,  v3, v12, v13, v16, v18, v17, v19, v28, v29, \
             v4, \
             v5, 0, 1,  v5, 2, 3, v21, 0, 1, v21, 2, 3, \
            table1, \
            q22, q23, q24, q25, q26, q27, \
            #2*16, #3*16, #4*16, #5*16, #6*16, #7*16
    dq_butterfly_bot v16, v18, v17, v19, v28, v29, v4, v21, 0, 1, v21, 2, 3

    add table0, table0, #128
    add table1, table1, #128

    trn_4x4  v0,  v1,  v2,  v3, v12, v13, v14, v15

    dq_butterfly_vec_top_trn_4x4 \
             v0,  v1,  v2,  v3, v12, v13, v4,  v6,  v7,  v6,  v7, \
             v16, v17, v18, v19, v28, v29, v30, v31

    dq_butterfly_vec_mix  v0,  v1,  v2,  v3, v12, v13, v16, v17, v18, v19, v28, v29, v4,  v6,  v7,  v6,  v7, v22, v23, v22, v23
    dq_butterfly_vec_mix v16, v17, v18, v19, v28, v29,  v0,  v2,  v1,  v3, v12, v13, v4, v22, v23, v22, v23,  v8,  v9, v10, v11
    dq_butterfly_vec_mix  v0,  v2,  v1,  v3, v12, v13, v16, v18, v17, v19, v28, v29, v4,  v8,  v9, v10, v11, v24, v25, v26, v27


    trn_4x4_l4  v0,  v1,  v2,  v3,  v8,  v9, v10, v11, src, q12, q13, q14, q15, #(64+0*16), #(64+1*16), #(64+2*16), #(64+3*16)

    str  q0, [src,  #0*16]
    str  q2, [src,  #2*16]

    dq_butterfly_vec_bot v16, v18, v17, v19, v28, v29, v4, v24, v25, v26, v27

    str  q1, [src,  #1*16]
    str  q3, [src,  #3*16]

    add src, src, #64

    trn_4x4_l4 v16, v17, v18, v19, v24, v25, v26, v27, src, q28, q29, q30, q31, #(512+0*16), #(512+1*16), #(512+2*16), #(512+3*16)

    sub src, src, #64

    dq_butterfly_top2l4s4 \
            v12, v13, v14, v15,  v0,  v1, v4,  v4, 2, 3,  v4, 2, 3, \
            table0,  q4,  q5,  #0*16, #1*16, \
            table1, q20, q21,  #0*16, #1*16, \
            src, \
            q16, q17, q18, q19, \
            #(512+0*16), #(512+1*16), #(512+2*16), #(512+3*16)

    add src, src, #64

    dq_butterfly_mix v12, v13, v14, v15,  v0,  v1, v28, v29, v30, v31, v16, v17, v4,  v4, 2, 3,  v4, 2, 3, v20, 2, 3, v20, 2, 3

    dq_butterfly_mixl6 \
            v28, v29, v30, v31, v16, v17, v12, v14, v13, v15, v0, v1, \
             v4, \
            v20, 2, 3, v20, 2, 3,  v5, 0, 1,  v5, 2, 3, \
            table0, \
             q6,  q7,  q8,  q9, q10, q11, \
             #2*16, #3*16, #4*16, #5*16, #6*16, #7*16

    dq_butterfly_mixl6 \
            v12, v14, v13, v15,  v0,  v1, v28, v30, v29, v31, v16, v17, \
             v4, \
             v5, 0, 1,  v5, 2, 3, v21, 0, 1, v21, 2, 3, \
            table1, \
            q22, q23, q24, q25, q26, q27, \
            #2*16, #3*16, #4*16, #5*16, #6*16, #7*16

    dq_butterfly_bot v28, v30, v29, v31, v16, v17, v4, v21, 0, 1, v21, 2, 3

    add table0, table0, #128
    add table1, table1, #128

    trn_4x4 v12, v13, v14, v15, v0,  v1,  v2,  v3

    dq_butterfly_vec_top_trn_4x4 \
            v12, v13, v14, v15,  v0,  v1, v4,  v6,  v7,  v6,  v7, \
            v28, v29, v30, v31, v16, v17, v18, v19

    dq_butterfly_vec_mix v12, v13, v14, v15,  v0,  v1, v28, v29, v30, v31, v16, v17, v4,  v6,  v7,  v6,  v7, v22, v23, v22, v23
    dq_butterfly_vec_mix v28, v29, v30, v31, v16, v17, v12, v14, v13, v15,  v0,  v1, v4, v22, v23, v22, v23,  v8,  v9, v10, v11
    dq_butterfly_vec_mix v12, v14, v13, v15,  v0,  v1, v28, v30, v29, v31, v16, v17, v4,  v8,  v9, v10, v11, v24, v25, v26, v27

    sub counter, counter, #1
    cbnz counter, _ntt_bot_loop

    dq_butterfly_vec_bot v28, v30, v29, v31, v16, v17, v4, v24, v25, v26, v27

    trn_4x4 v12, v13, v14, v15,  v0,  v1,  v2,  v3
    trn_4x4_s4 v28, v29, v30, v31, v16, v17, v18, v19, src, q12, q13, q14, q15, #0*16,  #1*16,  #2*16,  #3*16

    str q28, [src,  #(512+0*16)]
    str q29, [src,  #(512+1*16)]
    str q30, [src,  #(512+2*16)]
    str q31, [src,  #(512+3*16)]

    add src, src, #64

    .unreq    Q
    .unreq    src
    .unreq    table0
    .unreq    table1
    .unreq    counter
    pop_simd

    ret










