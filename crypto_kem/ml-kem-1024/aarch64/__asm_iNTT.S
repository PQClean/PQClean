
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
.global PQCLEAN_MLKEM1024_AARCH64__asm_intt_SIMD_bot
.global _PQCLEAN_MLKEM1024_AARCH64__asm_intt_SIMD_bot
PQCLEAN_MLKEM1024_AARCH64__asm_intt_SIMD_bot:
_PQCLEAN_MLKEM1024_AARCH64__asm_intt_SIMD_bot:

    push_all
    Q         .req w20
    BarrettM  .req w21
    src0      .req x0
    src1      .req x1
    table     .req x28
    counter   .req x19

    ldrsh Q, [x2, #0]
    ldrsh BarrettM, [x2, #8]

    add table, x1, #64

    add src0, x0, #256*0
    add src1, x0, #256*1

    mov v0.H[0], Q
    mov v0.H[1], BarrettM

    ldr q28, [src0, #1*16]
    ldr q29, [src1, #1*16]
    ldr q30, [src0, #3*16]
    ldr q31, [src1, #3*16]

    trn_4x4_2l4 v28, v29, v30, v31, v20, v21, v22, v23, src0, src1, q24, q25, q26, q27, #0*16, #0*16, #2*16, #2*16

    trn_4x4_2l4 v24, v25, v26, v27, v20, v21, v22, v23, table, table, q12, q13, q14, q15, #12*16, #13*16, #14*16, #15*16

    do_butterfly_vec_bot v28, v30, v18, v19, v29, v31,  v0, v12, v13, v14, v15

    do_butterfly_vec_mix_rev_l4 \
            v18, v19, v29, v31, \
            v24, v26, v16, v17, v25, v27,  v0, v12, v13, v14, v15, \
            table, \
             q8,  q9, q10, q11, \
             #8*16, #9*16, #10*16, #11*16

    do_butterfly_vec_mix_rev_l4 \
            v16, v17, v25, v27, \
            v28, v29, v18, v19, v30, v31,  v0,  v8,  v9, v10, v11, \
            table, \
             q4,  q5,  q6,  q7, \
             #4*16, #5*16, #6*16, #7*16

    do_butterfly_vec_mix_rev_l3 \
            v18, v19, v30, v31, \
            v24, v25, v16, v17, v26, v27,  v0,  v6,  v7,  v6,  v7, \
            table, \
             q1,  q2,  q3, \
             #1*16, #2*16, #3*16

    do_butterfly_vec_mix_rev v24, v25, v16, v17, v26, v27, v24, v25, v18, v19, v28, v29,  v0,  v4,  v5,  v4,  v5,  v2,  v3,  v2,  v3
    do_butterfly_vec_mix_rev v24, v25, v18, v19, v28, v29, v26, v27, v16, v17, v30, v31,  v0,  v2,  v3,  v2,  v3,  v2,  v3,  v2,  v3
    do_butterfly_vec_top v26, v27, v16, v17, v30, v31,  v0,  v2,  v3,   v2, v3

    oo_barrett v24, v25, v26, v27, v16, v17, v18, v19, v28, v29, v30, v31, v20, v21, v22, v23, v0, #11, v0

    add table, table, #256

    trn_4x4 v28, v29, v30, v31, v16, v17, v18, v19

    trn_4x4_2s4 v24, v25, v26, v27, v16, v17, v18, v19, src0, src1, q28, q29, q30, q31, #1*16, #1*16, #3*16, #3*16

    mov counter, #3
    _intt_bot_loop:

    str q24, [src0, #0*16]
    ldr q28, [src0, #(64+1*16)]
    str q25, [src1, #0*16]
    ldr q29, [src1, #(64+1*16)]
    str q26, [src0, #2*16]
    ldr q30, [src0, #(64+3*16)]
    str q27, [src1, #2*16]
    ldr q31, [src1, #(64+3*16)]

    add src0, src0, #64
    add src1, src1, #64

    trn_4x4_2l4 v28, v29, v30, v31, v20, v21, v22, v23, src0, src1, q24, q25, q26, q27, #0*16, #0*16, #2*16, #2*16

    trn_4x4_2l4 v24, v25, v26, v27, v20, v21, v22, v23, table, table, q12, q13, q14, q15, #12*16, #13*16, #14*16, #15*16

    do_butterfly_vec_bot v28, v30, v18, v19, v29, v31,  v0, v12, v13, v14, v15

    do_butterfly_vec_mix_rev_l4 \
            v18, v19, v29, v31, \
            v24, v26, v16, v17, v25, v27,  v0, v12, v13, v14, v15, \
            table, \
             q8,  q9, q10, q11, \
             #8*16, #9*16, #10*16, #11*16

    do_butterfly_vec_mix_rev_l4 \
            v16, v17, v25, v27, \
            v28, v29, v18, v19, v30, v31,  v0,  v8,  v9, v10, v11, \
            table, \
             q4,  q5,  q6,  q7, \
             #4*16, #5*16, #6*16, #7*16

    do_butterfly_vec_mix_rev_l3 \
            v18, v19, v30, v31, \
            v24, v25, v16, v17, v26, v27,  v0,  v6,  v7,  v6,  v7, \
            table, \
             q1,  q2,  q3, \
             #1*16, #2*16, #3*16

    do_butterfly_vec_mix_rev v24, v25, v16, v17, v26, v27, v24, v25, v18, v19, v28, v29,  v0,  v4,  v5,  v4,  v5,  v2,  v3,  v2,  v3
    do_butterfly_vec_mix_rev v24, v25, v18, v19, v28, v29, v26, v27, v16, v17, v30, v31,  v0,  v2,  v3,  v2,  v3,  v2,  v3,  v2,  v3
    do_butterfly_vec_top v26, v27, v16, v17, v30, v31,  v0,  v2,  v3,   v2, v3

    oo_barrett v24, v25, v26, v27, v16, v17, v18, v19, v28, v29, v30, v31, v20, v21, v22, v23, v0, #11, v0

    add table, table, #256

    trn_4x4 v28, v29, v30, v31, v16, v17, v18, v19

    trn_4x4_2s4 v24, v25, v26, v27, v16, v17, v18, v19, src0, src1, q28, q29, q30, q31, #1*16, #1*16, #3*16, #3*16

    sub counter, counter, #1
    cbnz counter, _intt_bot_loop

    str q24, [src0, #0*16]
    str q25, [src1, #0*16]
    str q26, [src0, #2*16]
    str q27, [src1, #2*16]


    .unreq    Q
    .unreq    BarrettM
    .unreq    src0
    .unreq    src1
    .unreq    table
    .unreq    counter
    pop_all

    ret

.align 2
.global PQCLEAN_MLKEM1024_AARCH64__asm_intt_SIMD_top
.global _PQCLEAN_MLKEM1024_AARCH64__asm_intt_SIMD_top
PQCLEAN_MLKEM1024_AARCH64__asm_intt_SIMD_top:
_PQCLEAN_MLKEM1024_AARCH64__asm_intt_SIMD_top:

    push_all
    Q         .req w20
    BarrettM  .req w21
    invN      .req w22
    invN_f    .req w23
    src       .req x0
    table     .req x1
    counter   .req x19

    ldrsh        Q, [x2, #0]
    ldrsh BarrettM, [x2, #8]
    ldr       invN, [x2, #10]
    ldr     invN_f, [x2, #14]

    mov v4.S[0], invN
    mov v4.S[1], invN_f

    ldr q0, [table, #0*16]
    mov v0.H[0], Q

    ldr q1, [table, #1*16]
    ldr q2, [table, #2*16]
    ldr q3, [table, #3*16]

    ldr q16, [src, # 8*32]
    ldr q17, [src, # 9*32]
    ldr q18, [src, #10*32]
    ldr q19, [src, #11*32]
    ldr q20, [src, #12*32]
    ldr q21, [src, #13*32]
    ldr q22, [src, #14*32]
    ldr q23, [src, #15*32]

    qo_butterfly_botll \
            v16, v18, v20, v22, v28, v29, v30, v31, v17, v19, v21, v23, \
            src, \
             q8,  q9, q10, q11, \
             #0*32, #1*32, #2*32, #3*32, \
            src, \
            q12, q13, q14, q15, \
             #4*32, #5*32, #6*32, #7*32


    qo_butterfly_mix_rev v16, v18, v20, v22, v28, v29, v30, v31, v17, v19, v21, v23,  v8, v10, v12, v14, v24, v25, v26, v27,  v9, v11, v13, v15,  v0,  v3, 0, 1,  v3, 2, 3,  v3, 4, 5,  v3, 6, 7,  v3, 0, 1,  v3, 2, 3,  v3, 4, 5,  v3, 6, 7
    qo_butterfly_mix_rev  v8, v10, v12, v14, v24, v25, v26, v27,  v9, v11, v13, v15, v16, v17, v20, v21, v28, v29, v30, v31, v18, v19, v22, v23,  v0,  v2, 0, 1,  v2, 2, 3,  v2, 4, 5,  v2, 6, 7,  v1, 4, 5,  v1, 4, 5,  v1, 6, 7,  v1, 6, 7
    qo_butterfly_mix_rev v16, v17, v20, v21, v28, v29, v30, v31, v18, v19, v22, v23,  v8,  v9, v12, v13, v24, v25, v26, v27, v10, v11, v14, v15,  v0,  v1, 4, 5,  v1, 4, 5,  v1, 6, 7,  v1, 6, 7,  v1, 0, 1,  v1, 0, 1,  v1, 2, 3,  v1, 2, 3
    qo_butterfly_mix_rev  v8,  v9, v12, v13, v24, v25, v26, v27, v10, v11, v14, v15, v16, v17, v18, v19, v28, v29, v30, v31, v20, v21, v22, v23,  v0,  v1, 0, 1,  v1, 0, 1,  v1, 2, 3,  v1, 2, 3,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7
    qo_butterfly_mix_rev v16, v17, v18, v19, v28, v29, v30, v31, v20, v21, v22, v23,  v8,  v9, v10, v11, v24, v25, v26, v27, v12, v13, v14, v15,  v0,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5
    qo_butterfly_mix_rev  v8,  v9, v10, v11, v24, v25, v26, v27, v12, v13, v14, v15,  v8,  v9, v10, v11, v28, v29, v30, v31, v16, v17, v18, v19,  v0,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3
    qo_butterfly_mix_rev  v8,  v9, v10, v11, v28, v29, v30, v31, v16, v17, v18, v19, v12, v13, v14, v15, v24, v25, v26, v27, v20, v21, v22, v23,  v0,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3

    qo_butterfly_topsl \
            v24, v25, v26, v27, v20, v21, v22, v23,  v0,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3, \
            src, \
            q16, q17, q18, q19, \
            #8*32, #9*32, #10*32, #11*32, \
            src, \
            q16, q17, q18, q19, \
            #(16+8*32), #(16+9*32), #(16+10*32), #(16+11*32)

    qo_montgomery_mul_insl \
             v8,  v9, v10, v11, v28, v29, v30, v31,  v0,  v4, 1, 0,  v4, 1, 0,  v4, 1, 0,  v4, 1, 0, \
             src, \
             q20, q21, q22, q23, \
             #12*32, #13*32, #14*32, #15*32, \
             src, \
             q20, q21, q22, q23, \
             #(16+12*32), #(16+13*32), #(16+14*32), #(16+15*32)

    qo_butterfly_botsl_mul \
            v16, v18, v20, v22, v28, v29, v30, v31, v17, v19, v21, v23, \
            src, \
             q8,  q9, q10, q11, \
             #0*32, #1*32, #2*32, #3*32, \
            src, \
             q8,  q9, q10, q11, \
             #(16+0*32), #(16+1*32), #(16+2*32), #(16+3*32), \
             v12, v13, v14, v15, v24, v25, v26, v27, \
             v0,  v4, 1, 0,  v4, 1, 0,  v4, 1, 0,  v4, 1, 0

    str q12, [src, # 4*32]
    ldr q12, [src, #(16+ 4*32)]
    str q13, [src, # 5*32]
    ldr q13, [src, #(16+ 5*32)]
    str q14, [src, # 6*32]
    ldr q14, [src, #(16+ 6*32)]
    str q15, [src, # 7*32]
    ldr q15, [src, #(16+ 7*32)]

    qo_butterfly_mix_rev v16, v18, v20, v22, v28, v29, v30, v31, v17, v19, v21, v23,  v8, v10, v12, v14, v24, v25, v26, v27,  v9, v11, v13, v15,  v0,  v3, 0, 1,  v3, 2, 3,  v3, 4, 5,  v3, 6, 7,  v3, 0, 1,  v3, 2, 3,  v3, 4, 5,  v3, 6, 7
    qo_butterfly_mix_rev  v8, v10, v12, v14, v24, v25, v26, v27,  v9, v11, v13, v15, v16, v17, v20, v21, v28, v29, v30, v31, v18, v19, v22, v23,  v0,  v2, 0, 1,  v2, 2, 3,  v2, 4, 5,  v2, 6, 7,  v1, 4, 5,  v1, 4, 5,  v1, 6, 7,  v1, 6, 7
    qo_butterfly_mix_rev v16, v17, v20, v21, v28, v29, v30, v31, v18, v19, v22, v23,  v8,  v9, v12, v13, v24, v25, v26, v27, v10, v11, v14, v15,  v0,  v1, 4, 5,  v1, 4, 5,  v1, 6, 7,  v1, 6, 7,  v1, 0, 1,  v1, 0, 1,  v1, 2, 3,  v1, 2, 3
    qo_butterfly_mix_rev  v8,  v9, v12, v13, v24, v25, v26, v27, v10, v11, v14, v15, v16, v17, v18, v19, v28, v29, v30, v31, v20, v21, v22, v23,  v0,  v1, 0, 1,  v1, 0, 1,  v1, 2, 3,  v1, 2, 3,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7
    qo_butterfly_mix_rev v16, v17, v18, v19, v28, v29, v30, v31, v20, v21, v22, v23,  v8,  v9, v10, v11, v24, v25, v26, v27, v12, v13, v14, v15,  v0,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7,  v0, 6, 7,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5
    qo_butterfly_mix_rev  v8,  v9, v10, v11, v24, v25, v26, v27, v12, v13, v14, v15,  v8,  v9, v10, v11, v28, v29, v30, v31, v16, v17, v18, v19,  v0,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5,  v0, 4, 5,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3
    qo_butterfly_mix_rev  v8,  v9, v10, v11, v28, v29, v30, v31, v16, v17, v18, v19, v12, v13, v14, v15, v24, v25, v26, v27, v20, v21, v22, v23,  v0,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3

    qo_butterfly_tops \
            v24, v25, v26, v27, v20, v21, v22, v23,  v0,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3,  v4, 2, 3, \
            src, \
            q16, q17, q18, q19, \
            #(16+8*32), #(16+9*32), #(16+10*32), #(16+11*32)


    qo_montgomery_mul_ins \
             v8,  v9, v10, v11, v28, v29, v30, v31,  v0,  v4, 1, 0,  v4, 1, 0,  v4, 1, 0,  v4, 1, 0, \
             src, \
             q20, q21, q22, q23, \
             #(16+12*32), #(16+13*32), #(16+14*32), #(16+15*32)

    qo_montgomery_mul_ins \
            v12, v13, v14, v15, v24, v25, v26, v27,  v0,  v4, 1, 0,  v4, 1, 0,  v4, 1, 0,  v4, 1, 0, \
             src, \
              q8,  q9, q10, q11, \
             #(16+0*32), #(16+1*32), #(16+2*32), #(16+3*32)

    str q12, [src, #(16+ 4*32)]
    str q13, [src, #(16+ 5*32)]
    str q14, [src, #(16+ 6*32)]
    str q15, [src, #(16+ 7*32)]

    .unreq    Q
    .unreq    BarrettM
    .unreq    invN
    .unreq    invN_f
    .unreq    src
    .unreq    counter
    pop_all

    ret





















