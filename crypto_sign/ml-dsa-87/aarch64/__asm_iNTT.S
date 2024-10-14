
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

.align 2
.global PQCLEAN_MLDSA87_AARCH64__asm_intt_SIMD_top
.global _PQCLEAN_MLDSA87_AARCH64__asm_intt_SIMD_top
PQCLEAN_MLDSA87_AARCH64__asm_intt_SIMD_top:
_PQCLEAN_MLDSA87_AARCH64__asm_intt_SIMD_top:

    push_all
    Q         .req w20
    Qhalf     .req w21
    nQhalf    .req w22
    invNR2ph  .req w24
    invNR2dp  .req w25
    invNWR2ph .req w26
    invNWR2dp .req w27
    src       .req  x0
    counter   .req x19

    ldr Q, [x2, #0]
    lsr Qhalf, Q, #1
    neg nQhalf, Qhalf

    ldr invNR2ph,  [x2, #16]
    ldr invNR2dp,  [x2, #20]
    ldr invNWR2ph, [x2, #24]
    ldr invNWR2dp, [x2, #28]

    ldr q20, [x1, #0*16]
    ldr q21, [x1, #1*16]
    ldr q22, [x1, #2*16]
    ldr q23, [x1, #3*16]
    ldr q24, [x1, #4*16]
    ldr q25, [x1, #5*16]
    ldr q26, [x1, #6*16]
    ldr q27, [x1, #7*16]

    mov v20.S[0], Q

    ldr  q0, [src, # 0*64]
    ldr  q1, [src, # 1*64]

    ldr  q2, [src, # 2*64]
    ldr  q3, [src, # 3*64]

    ldr  q4, [src, # 4*64]
    ldr  q5, [src, # 5*64]

    ldr  q6, [src, # 6*64]
    ldr  q7, [src, # 7*64]

    qq_butterfly_botll \
             v0,  v2,  v4,  v6, v16, v17, v18, v19,  v1,  v3,  v5,  v7, \
            src, \
             q8,  q9, q10, q11, \
            #8*64, #9*64, #10*64, #11*64, \
            src, \
            q12, q13, q14, q15, \
            #12*64, #13*64, #14*64, #15*64

    qq_butterfly_mix_rev  v0,  v2,  v4,  v6, v16, v17, v18, v19,  v1,  v3,  v5,  v7,  v8, v10, v12, v14, v28, v29, v30, v31,  v9, v11, v13, v15, v20, v24, 0, 1, v24, 2, 3, v25, 0, 1, v25, 2, 3, v26, 0, 1, v26, 2, 3, v27, 0, 1, v27, 2, 3
    qq_butterfly_mix_rev  v8, v10, v12, v14, v28, v29, v30, v31,  v9, v11, v13, v15,  v0,  v1,  v4,  v5, v16, v17, v18, v19,  v2,  v3,  v6,  v7, v20, v26, 0, 1, v26, 2, 3, v27, 0, 1, v27, 2, 3, v22, 0, 1, v22, 0, 1, v22, 2, 3, v22, 2, 3
    qq_butterfly_mix_rev  v0,  v1,  v4,  v5, v16, v17, v18, v19,  v2,  v3,  v6,  v7,  v8,  v9, v12, v13, v28, v29, v30, v31, v10, v11, v14, v15, v20, v22, 0, 1, v22, 0, 1, v22, 2, 3, v22, 2, 3, v23, 0, 1, v23, 0, 1, v23, 2, 3, v23, 2, 3
    qq_butterfly_mix_rev  v8,  v9, v12, v13, v28, v29, v30, v31, v10, v11, v14, v15,  v0,  v1,  v2,  v3, v16, v17, v18, v19,  v4,  v5,  v6,  v7, v20, v23, 0, 1, v23, 0, 1, v23, 2, 3, v23, 2, 3, v21, 0, 1, v21, 0, 1, v21, 0, 1, v21, 0, 1
    qq_butterfly_mix_rev  v0,  v1,  v2,  v3, v16, v17, v18, v19,  v4,  v5,  v6,  v7,  v8,  v9, v10, v11, v28, v29, v30, v31, v12, v13, v14, v15, v20, v21, 0, 1, v21, 0, 1, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3, v21, 2, 3, v21, 2, 3
    qq_butterfly_top  v8,  v9, v10, v11, v28, v29, v30, v31, v12, v13, v14, v15, v20, v21, 2, 3, v21, 2, 3, v21, 2, 3, v21, 2, 3

    qq_sub_add v16, v17, v18, v19, v28, v29, v30, v31,  v0,  v2,  v4,  v6,  v8, v10, v12, v14
    qq_sub_add  v0,  v2,  v4,  v6,  v8, v10, v12, v14,  v1,  v3,  v5,  v7,  v9, v11, v13, v15

    mov v20.S[2], invNR2ph
    mov v20.S[3], invNR2dp

    qq_montgomery_mul  v1,  v3,  v5,  v7,  v0,  v2,  v4,  v6, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3
    qq_montgomery_mul  v0,  v2,  v4,  v6, v16, v17, v18, v19, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3

    mov v20.S[2], invNWR2ph
    mov v20.S[3], invNWR2dp

    qq_montgomery_mul  v9, v11, v13, v15,  v8, v10, v12, v14, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3
    qq_montgomery_mul  v8, v10, v12, v14, v28, v29, v30, v31, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3

    mov counter, #3
    _intt_top_loop:

    dup v29.4S, Q
    dup v30.4S, Qhalf
    dup v31.4S, nQhalf

    cmge v18.4S, v31.4S,  v0.4S
    cmge v19.4S, v31.4S,  v1.4S
    cmge v16.4S,  v0.4S, v30.4S
    cmge v17.4S,  v1.4S, v30.4S

    sub  v16.4S, v16.4S, v18.4S
    sub  v17.4S, v17.4S, v19.4S

    mla   v0.4S, v16.4S, v29.4S
    cmge v18.4S, v31.4S,  v2.4S
    mla   v1.4S, v17.4S, v29.4S
    cmge v19.4S, v31.4S,  v3.4S

    str  q0, [src, #0*64]
    cmge v16.4S,  v2.4S, v30.4S
    ldr  q0, [src, #(16 + 0*64)]
    str  q1, [src, #1*64]
    cmge v17.4S,  v3.4S, v30.4S
    ldr  q1, [src, #(16 + 1*64)]

    sub  v16.4S, v16.4S, v18.4S
    sub  v17.4S, v17.4S, v19.4S

    mla   v2.4S, v16.4S, v29.4S
    cmge v18.4S, v31.4S,  v4.4S
    mla   v3.4S, v17.4S, v29.4S
    cmge v19.4S, v31.4S,  v5.4S

    str  q2, [src, #2*64]
    cmge v16.4S,  v4.4S, v30.4S
    ldr  q2, [src, #(16 + 2*64)]
    str  q3, [src, #3*64]
    cmge v17.4S,  v5.4S, v30.4S
    ldr  q3, [src, #(16 + 3*64)]

    sub  v16.4S, v16.4S, v18.4S
    sub  v17.4S, v17.4S, v19.4S

    mla   v4.4S, v16.4S, v29.4S
    cmge v18.4S, v31.4S,  v6.4S
    mla   v5.4S, v17.4S, v29.4S
    cmge v19.4S, v31.4S,  v7.4S

    str  q4, [src, #4*64]
    cmge v16.4S,  v6.4S, v30.4S
    ldr  q4, [src, #(16 + 4*64)]
    str  q5, [src, #5*64]
    cmge v17.4S,  v7.4S, v30.4S
    ldr  q5, [src, #(16 + 5*64)]

    sub  v16.4S, v16.4S, v18.4S
    sub  v17.4S, v17.4S, v19.4S

    mla   v6.4S, v16.4S, v29.4S
    cmge v18.4S, v31.4S,  v8.4S
    mla   v7.4S, v17.4S, v29.4S
    cmge v19.4S, v31.4S,  v9.4S

    str  q6, [src, #6*64]
    cmge v16.4S,  v8.4S, v30.4S
    ldr  q6, [src, #(16 + 6*64)]
    str  q7, [src, #7*64]
    cmge v17.4S,  v9.4S, v30.4S
    ldr  q7, [src, #(16 + 7*64)]

    sub  v16.4S, v16.4S, v18.4S
    sub  v17.4S, v17.4S, v19.4S

    mla   v8.4S, v16.4S, v29.4S
    cmge v18.4S, v31.4S, v10.4S
    mla   v9.4S, v17.4S, v29.4S
    cmge v19.4S, v31.4S, v11.4S

    str  q8, [src,  #8*64]
    cmge v16.4S, v10.4S, v30.4S
    str  q9, [src,  #9*64]
    cmge v17.4S, v11.4S, v30.4S

    sub  v16.4S, v16.4S, v18.4S
    sub  v17.4S, v17.4S, v19.4S

    mla  v10.4S, v16.4S, v29.4S
    cmge v18.4S, v31.4S, v12.4S
    mla  v11.4S, v17.4S, v29.4S
    cmge v19.4S, v31.4S, v13.4S

    str q10, [src, #10*64]
    cmge v16.4S, v12.4S, v30.4S
    str q11, [src, #11*64]
    cmge v17.4S, v13.4S, v30.4S

    sub  v16.4S, v16.4S, v18.4S
    sub  v17.4S, v17.4S, v19.4S

    mla  v12.4S, v16.4S, v29.4S
    cmge v18.4S, v31.4S, v14.4S
    mla  v13.4S, v17.4S, v29.4S
    cmge v19.4S, v31.4S, v15.4S

    str q12, [src, #12*64]
    cmge v16.4S, v14.4S, v30.4S
    str q13, [src, #13*64]
    cmge v17.4S, v15.4S, v30.4S

    sub  v16.4S, v16.4S, v18.4S
    sub  v17.4S, v17.4S, v19.4S

    mla  v14.4S, v16.4S, v29.4S
    mla  v15.4S, v17.4S, v29.4S

    str q14, [src, #14*64]
    str q15, [src, #15*64]

    add src, src, #16

    qq_butterfly_botll \
             v0,  v2,  v4,  v6, v16, v17, v18, v19,  v1,  v3,  v5,  v7, \
            src, \
             q8,  q9, q10, q11, \
            #8*64, #9*64, #10*64, #11*64, \
            src, \
            q12, q13, q14, q15, \
            #12*64, #13*64, #14*64, #15*64

    qq_butterfly_mix_rev  v0,  v2,  v4,  v6, v16, v17, v18, v19,  v1,  v3,  v5,  v7,  v8, v10, v12, v14, v28, v29, v30, v31,  v9, v11, v13, v15, v20, v24, 0, 1, v24, 2, 3, v25, 0, 1, v25, 2, 3, v26, 0, 1, v26, 2, 3, v27, 0, 1, v27, 2, 3
    qq_butterfly_mix_rev  v8, v10, v12, v14, v28, v29, v30, v31,  v9, v11, v13, v15,  v0,  v1,  v4,  v5, v16, v17, v18, v19,  v2,  v3,  v6,  v7, v20, v26, 0, 1, v26, 2, 3, v27, 0, 1, v27, 2, 3, v22, 0, 1, v22, 0, 1, v22, 2, 3, v22, 2, 3
    qq_butterfly_mix_rev  v0,  v1,  v4,  v5, v16, v17, v18, v19,  v2,  v3,  v6,  v7,  v8,  v9, v12, v13, v28, v29, v30, v31, v10, v11, v14, v15, v20, v22, 0, 1, v22, 0, 1, v22, 2, 3, v22, 2, 3, v23, 0, 1, v23, 0, 1, v23, 2, 3, v23, 2, 3
    qq_butterfly_mix_rev  v8,  v9, v12, v13, v28, v29, v30, v31, v10, v11, v14, v15,  v0,  v1,  v2,  v3, v16, v17, v18, v19,  v4,  v5,  v6,  v7, v20, v23, 0, 1, v23, 0, 1, v23, 2, 3, v23, 2, 3, v21, 0, 1, v21, 0, 1, v21, 0, 1, v21, 0, 1
    qq_butterfly_mix_rev  v0,  v1,  v2,  v3, v16, v17, v18, v19,  v4,  v5,  v6,  v7,  v8,  v9, v10, v11, v28, v29, v30, v31, v12, v13, v14, v15, v20, v21, 0, 1, v21, 0, 1, v21, 0, 1, v21, 0, 1, v21, 2, 3, v21, 2, 3, v21, 2, 3, v21, 2, 3
    qq_butterfly_top  v8,  v9, v10, v11, v28, v29, v30, v31, v12, v13, v14, v15, v20, v21, 2, 3, v21, 2, 3, v21, 2, 3, v21, 2, 3

    qq_sub_add v16, v17, v18, v19, v28, v29, v30, v31,  v0,  v2,  v4,  v6,  v8, v10, v12, v14
    qq_sub_add  v0,  v2,  v4,  v6,  v8, v10, v12, v14,  v1,  v3,  v5,  v7,  v9, v11, v13, v15

    mov v20.S[2], invNR2ph
    mov v20.S[3], invNR2dp

    qq_montgomery_mul  v1,  v3,  v5,  v7,  v0,  v2,  v4,  v6, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3
    qq_montgomery_mul  v0,  v2,  v4,  v6, v16, v17, v18, v19, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3

    mov v20.S[2], invNWR2ph
    mov v20.S[3], invNWR2dp

    qq_montgomery_mul  v9, v11, v13, v15,  v8, v10, v12, v14, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3
    qq_montgomery_mul  v8, v10, v12, v14, v28, v29, v30, v31, v20, v20, 2, 3, v20, 2, 3, v20, 2, 3, v20, 2, 3

    sub counter, counter, #1
    cbnz counter, _intt_top_loop

    dup v29.4S, Q
    dup v30.4S, Qhalf
    dup v31.4S, nQhalf

    cmge v18.4S, v31.4S,  v0.4S
    cmge v19.4S, v31.4S,  v1.4S
    cmge v16.4S,  v0.4S, v30.4S
    cmge v17.4S,  v1.4S, v30.4S

    sub  v16.4S, v16.4S, v18.4S
    sub  v17.4S, v17.4S, v19.4S

    mla   v0.4S, v16.4S, v29.4S
    mla   v1.4S, v17.4S, v29.4S

    str  q0, [src, #0*64]
    str  q1, [src, #1*64]

    cmge v18.4S, v31.4S,  v2.4S
    cmge v19.4S, v31.4S,  v3.4S
    cmge v16.4S,  v2.4S, v30.4S
    cmge v17.4S,  v3.4S, v30.4S

    sub  v16.4S, v16.4S, v18.4S
    sub  v17.4S, v17.4S, v19.4S

    mla   v2.4S, v16.4S, v29.4S
    mla   v3.4S, v17.4S, v29.4S

    str  q2, [src, #2*64]
    str  q3, [src, #3*64]

    cmge v18.4S, v31.4S,  v4.4S
    cmge v19.4S, v31.4S,  v5.4S
    cmge v16.4S,  v4.4S, v30.4S
    cmge v17.4S,  v5.4S, v30.4S

    sub  v16.4S, v16.4S, v18.4S
    sub  v17.4S, v17.4S, v19.4S

    mla   v4.4S, v16.4S, v29.4S
    mla   v5.4S, v17.4S, v29.4S

    str  q4, [src, #4*64]
    str  q5, [src, #5*64]

    cmge v18.4S, v31.4S,  v6.4S
    cmge v19.4S, v31.4S,  v7.4S
    cmge v16.4S,  v6.4S, v30.4S
    cmge v17.4S,  v7.4S, v30.4S

    sub  v16.4S, v16.4S, v18.4S
    sub  v17.4S, v17.4S, v19.4S

    mla   v6.4S, v16.4S, v29.4S
    mla   v7.4S, v17.4S, v29.4S

    str  q6, [src, #6*64]
    str  q7, [src, #7*64]

    cmge v18.4S, v31.4S,  v8.4S
    cmge v19.4S, v31.4S,  v9.4S
    cmge v16.4S,  v8.4S, v30.4S
    cmge v17.4S,  v9.4S, v30.4S

    sub  v16.4S, v16.4S, v18.4S
    sub  v17.4S, v17.4S, v19.4S

    mla   v8.4S, v16.4S, v29.4S
    mla   v9.4S, v17.4S, v29.4S

    str  q8, [src,  #8*64]
    str  q9, [src,  #9*64]

    cmge v18.4S, v31.4S, v10.4S
    cmge v19.4S, v31.4S, v11.4S
    cmge v16.4S, v10.4S, v30.4S
    cmge v17.4S, v11.4S, v30.4S

    sub  v16.4S, v16.4S, v18.4S
    sub  v17.4S, v17.4S, v19.4S

    mla  v10.4S, v16.4S, v29.4S
    mla  v11.4S, v17.4S, v29.4S

    str q10, [src, #10*64]
    str q11, [src, #11*64]

    cmge v18.4S, v31.4S, v12.4S
    cmge v19.4S, v31.4S, v13.4S
    cmge v16.4S, v12.4S, v30.4S
    cmge v17.4S, v13.4S, v30.4S

    sub  v16.4S, v16.4S, v18.4S
    sub  v17.4S, v17.4S, v19.4S

    mla  v12.4S, v16.4S, v29.4S
    mla  v13.4S, v17.4S, v29.4S

    str q12, [src, #12*64]
    str q13, [src, #13*64]

    cmge v18.4S, v31.4S, v14.4S
    cmge v19.4S, v31.4S, v15.4S
    cmge v16.4S, v14.4S, v30.4S
    cmge v17.4S, v15.4S, v30.4S

    sub  v16.4S, v16.4S, v18.4S
    sub  v17.4S, v17.4S, v19.4S

    mla  v14.4S, v16.4S, v29.4S
    mla  v15.4S, v17.4S, v29.4S

    str q14, [src, #14*64]
    str q15, [src, #15*64]

    add src, src, #16


    .unreq    Q
    .unreq    Qhalf
    .unreq    nQhalf
    .unreq    invNR2ph
    .unreq    invNR2dp
    .unreq    invNWR2ph
    .unreq    invNWR2dp
    .unreq    src
    .unreq    counter
    pop_all

    ret

.align 2
.global PQCLEAN_MLDSA87_AARCH64__asm_intt_SIMD_bot
.global _PQCLEAN_MLDSA87_AARCH64__asm_intt_SIMD_bot
PQCLEAN_MLDSA87_AARCH64__asm_intt_SIMD_bot:
_PQCLEAN_MLDSA87_AARCH64__asm_intt_SIMD_bot:

    push_all
    Q         .req w20
    RphRdp    .req x21
    src0      .req x0
    src1      .req x2
    table0    .req x28
    table1    .req x27
    counter   .req x19

    ldr Q, [x2]
    ldr RphRdp, [x2, #8]

    add table0, x1, #128
    add table1, table0, #1024

    add src1, src0, #512

    ldr  q8, [table0, #4*16]
    ldr  q9, [table0, #5*16]
    ldr q10, [table0, #6*16]
    ldr q11, [table0, #7*16]

    ldr q24, [table1, #4*16]
    ldr q25, [table1, #5*16]
    ldr q26, [table1, #6*16]
    ldr q27, [table1, #7*16]

    ldr  q0, [src0, # 0*16]
    ldr  q1, [src0, # 1*16]

    ldr q16, [src1, # 0*16]
    ldr q17, [src1, # 1*16]

    ldr  q2, [src0, # 2*16]
    ldr  q3, [src0, # 3*16]

    ldr q18, [src1, # 2*16]
    ldr q19, [src1, # 3*16]

    trn_4x4_l4 \
             v0,  v1,  v2,  v3, v12, v13, v14, v15, \
            table0, \
             q4,  q5,  q6,  q7, \
            #0*16, #1*16, #2*16, #3*16

    trn_4x4_l4 \
            v16, v17, v18, v19, v28, v29, v30, v31, \
            table1, \
            q20, q21, q22, q23, \
            #0*16, #1*16, #2*16, #3*16

    mov v4.S[0], Q
    mov v20.D[0], RphRdp

    dq_butterfly_vec_bot  v0,  v2, v12, v13,  v1,  v3,  v4,  v8,  v9, v10, v11
    dq_butterfly_vec_mix_rev  v0,  v2, v12, v13,  v1,  v3, v16, v18, v28, v29, v17, v19,  v4,  v8,  v9, v10, v11, v24, v25, v26, v27
    dq_butterfly_vec_mix_rev v16, v18, v28, v29, v17, v19,  v0,  v1, v12, v13,  v2,  v3,  v4, v24, v25, v26, v27,  v6,  v7,  v6,  v7
    dq_butterfly_vec_mix_rev  v0,  v1, v12, v13,  v2,  v3, v16, v17, v28, v29, v18, v19,  v4,  v6,  v7,  v6,  v7, v22, v23, v22, v23

    mov counter, #7
    _intt_bot_loop:

    dq_butterfly_vec_top_ltrn_4x4 \
            v28, v29, v18, v19,  v4, v22, v23, v22, v23, \
            table0, \
             q8,  q9, q10, q11, \
            #(128+4*16), #(128+5*16), #(128+6*16), #(128+7*16), \
             v0,  v1,  v2,  v3, v12, v13, v14, v15

    trn_4x4_l4 \
            v16, v17, v18, v19, v28, v29, v30, v31, \
            table1, \
            q24, q25, q26, q27, \
            #(128+4*16), #(128+5*16), #(128+6*16), #(128+7*16)

    dq_butterfly_bot  v0,  v2, v12, v13,  v1,  v3,  v4,  v5, 0, 1,  v5, 2, 3
    dq_butterfly_mix_rev  v0,  v2, v12, v13,  v1,  v3, v16, v18, v28, v29, v17, v19,  v4,  v5, 0, 1,  v5, 2, 3, v21, 0, 1, v21, 2, 3
    dq_butterfly_mix_rev v16, v18, v28, v29, v17, v19,  v0,  v1, v12, v13,  v2,  v3,  v4, v21, 0, 1, v21, 2, 3,  v4, 2, 3,  v4, 2, 3
    dq_butterfly_mix_rev  v0,  v1, v12, v13,  v2,  v3, v16, v17, v28, v29, v18, v19,  v4,  v4, 2, 3,  v4, 2, 3, v20, 2, 3, v20, 2, 3
    dq_butterfly_top v16, v17, v28, v29, v18, v19,  v4, v20, 2, 3, v20, 2, 3

    str  q2, [src0, # 2*16]
    srshr v14.4S,  v0.4S, #23
    ldr  q2, [src0, #(64+ 2*16)]
    str  q3, [src0, # 3*16]
    srshr v15.4S,  v1.4S, #23
    ldr  q3, [src0, #(64+ 3*16)]
    str q18, [src1, # 2*16]
    srshr v30.4S, v16.4S, #23
    ldr q18, [src1, #(64+ 2*16)]
    str q19, [src1, # 3*16]
    srshr v31.4S, v17.4S, #23
    ldr q19, [src1, #(64+ 3*16)]

    mls    v0.4S, v14.4S, v4.S[0]
    str  q0, [src0, # 0*16]
    ldr  q0, [src0, #(64+ 0*16)]
    mls    v1.4S, v15.4S, v4.S[0]
    str  q1, [src0, # 1*16]
    ldr  q1, [src0, #(64+ 1*16)]
    mls   v16.4S, v30.4S, v4.S[0]
    str q16, [src1, # 0*16]
    ldr q16, [src1, #(64+ 0*16)]
    mls   v17.4S, v31.4S, v4.S[0]
    str q17, [src1, # 1*16]
    ldr q17, [src1, #(64+ 1*16)]

    add table0, table0, #128
    add table1, table1, #128

    add src0, src0, #64
    add src1, src1, #64

    trn_4x4_l4 \
             v0,  v1,  v2,  v3, v12, v13, v14, v15, \
            table0, \
             q4,  q5,  q6,  q7, \
            #0*16, #1*16, #2*16, #3*16

    trn_4x4_l4 \
            v16, v17, v18, v19, v28, v29, v30, v31, \
            table1, \
            q20, q21, q22, q23, \
            #0*16, #1*16, #2*16, #3*16

    mov v4.S[0], Q
    mov v20.D[0], RphRdp

    dq_butterfly_vec_bot  v0,  v2, v12, v13,  v1,  v3,  v4,  v8,  v9, v10, v11
    dq_butterfly_vec_mix_rev  v0,  v2, v12, v13,  v1,  v3, v16, v18, v28, v29, v17, v19,  v4,  v8,  v9, v10, v11, v24, v25, v26, v27
    dq_butterfly_vec_mix_rev v16, v18, v28, v29, v17, v19,  v0,  v1, v12, v13,  v2,  v3,  v4, v24, v25, v26, v27,  v6,  v7,  v6,  v7
    dq_butterfly_vec_mix_rev  v0,  v1, v12, v13,  v2,  v3, v16, v17, v28, v29, v18, v19,  v4,  v6,  v7,  v6,  v7, v22, v23, v22, v23

    sub counter, counter, #1
    cbnz counter, _intt_bot_loop

    dq_butterfly_vec_top_trn_4x4 \
            v16, v17, v28, v29, v18, v19,  v4, v22, v23, v22, v23, \
             v0,  v1,  v2,  v3, v12, v13, v14, v15

    trn_4x4 v16, v17, v18, v19, v28, v29, v30, v31

    dq_butterfly_bot  v0,  v2, v12, v13,  v1,  v3,  v4,  v5, 0, 1,  v5, 2, 3
    dq_butterfly_mix_rev  v0,  v2, v12, v13,  v1,  v3, v16, v18, v28, v29, v17, v19,  v4,  v5, 0, 1,  v5, 2, 3, v21, 0, 1, v21, 2, 3
    dq_butterfly_mix_rev v16, v18, v28, v29, v17, v19,  v0,  v1, v12, v13,  v2,  v3,  v4, v21, 0, 1, v21, 2, 3,  v4, 2, 3,  v4, 2, 3
    dq_butterfly_mix_rev  v0,  v1, v12, v13,  v2,  v3, v16, v17, v28, v29, v18, v19,  v4,  v4, 2, 3,  v4, 2, 3, v20, 2, 3, v20, 2, 3
    dq_butterfly_top v16, v17, v28, v29, v18, v19,  v4, v20, 2, 3, v20, 2, 3

    str  q2, [src0, # 2*16]
    str  q3, [src0, # 3*16]
    str q18, [src1, # 2*16]
    str q19, [src1, # 3*16]

    srshr v14.4S,  v0.4S, #23
    srshr v15.4S,  v1.4S, #23
    srshr v30.4S, v16.4S, #23
    srshr v31.4S, v17.4S, #23

    mls    v0.4S, v14.4S, v4.S[0]
    mls    v1.4S, v15.4S, v4.S[0]
    mls   v16.4S, v30.4S, v4.S[0]
    mls   v17.4S, v31.4S, v4.S[0]

    str  q0, [src0, # 0*16]
    str  q1, [src0, # 1*16]
    str q16, [src1, # 0*16]
    str q17, [src1, # 1*16]

    add table0, table0, #128
    add table1, table1, #128

    add src0, src0, #64
    add src1, src1, #64

    .unreq    Q
    .unreq    RphRdp
    .unreq    src0
    .unreq    src1
    .unreq    table0
    .unreq    table1
    .unreq    counter
    pop_all

    ret




