
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
.global PQCLEAN_MLKEM512_AARCH64__asm_ntt_SIMD_top
.global _PQCLEAN_MLKEM512_AARCH64__asm_ntt_SIMD_top
PQCLEAN_MLKEM512_AARCH64__asm_ntt_SIMD_top:
_PQCLEAN_MLKEM512_AARCH64__asm_ntt_SIMD_top:

    push_simd
    Q         .req w8
    src       .req x0
    table     .req x1
    counter   .req x11

    ldrsh Q, [x2, #0]

    ldr  q0, [table, # 0*16]
    ldr  q1, [table, # 1*16]
    ldr  q2, [table, # 2*16]
    ldr  q3, [table, # 3*16]

    mov v0.H[0], Q

    ldr q13, [src, # 9*32]
    ldr q15, [src, #11*32]
    ldr q17, [src, #13*32]
    ldr q19, [src, #15*32]

    qo_butterfly_topl \
            v13, v15, v17, v19, v28, v29, v30, v31, \
            v0, \
            v0, 2, 3,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3, \
            src, \
            q5, q7, q9, q11, \
            #1*32, #3*32, #5*32, #7*32

    qo_butterfly_mixll \
             v5,  v7,  v9, v11, v13, v15, v17, v19, v28, v29, v30, v31, \
            v12, v14, v16, v18, v20, v21, v22, v23, \
            v0, \
            v0, 2, 3,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3, \
            src, \
            q12, q14, q16, q18, \
            #8*32, #10*32, #12*32, #14*32, \
            src, \
             q4,  q6,  q8, q10, \
            #0*32, #2*32, #4*32, #6*32

    qo_butterfly_mix \
             v4,  v6,  v8, v10, v12, v14, v16, v18, v20, v21, v22, v23, \
             v5,  v7, v13, v15,  v9, v11, v17, v19, v28, v29, v30, v31, \
             v0, \
             v0, 2, 3,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3, \
             v0, 4, 5,  v0, 4, 5,  v0, 6, 7,  v0, 6, 7

    qo_butterfly_mix \
             v5,  v7, v13, v15,  v9, v11, v17, v19, v28, v29, v30, v31, \
             v4,  v6, v12, v14,  v8, v10, v16, v18, v20, v21, v22, v23, \
             v0, \
             v0, 4, 5,  v0, 4, 5,  v0, 6, 7,  v0, 6, 7, \
             v0, 4, 5,  v0, 4, 5,  v0, 6, 7,  v0, 6, 7

    qo_butterfly_mix \
             v4,  v6, v12, v14,  v8, v10, v16, v18, v20, v21, v22, v23, \
             v5,  v9, v13, v17,  v7, v11, v15, v19, v28, v29, v30, v31, \
             v0, \
             v0, 4, 5,  v0, 4, 5,  v0, 6, 7,  v0, 6, 7, \
             v1, 0, 1,  v1, 2, 3,  v1, 4, 5,  v1, 6, 7

    qo_butterfly_mix \
             v5,  v9, v13, v17,  v7, v11, v15, v19, v28, v29, v30, v31, \
             v4,  v8, v12, v16,  v6, v10, v14, v18, v20, v21, v22, v23, \
             v0, \
             v1, 0, 1,  v1, 2, 3,  v1, 4, 5,  v1, 6, 7, \
             v1, 0, 1,  v1, 2, 3,  v1, 4, 5,  v1, 6, 7

    qo_butterfly_mix \
             v4,  v8, v12, v16,  v6, v10, v14, v18, v20, v21, v22, v23, \
             v4,  v6,  v8, v10,  v5,  v7,  v9, v11, v28, v29, v30, v31, \
             v0, \
             v1, 0, 1,  v1, 2, 3,  v1, 4, 5,  v1, 6, 7, \
             v2, 0, 1,  v2, 2, 3,  v2, 4, 5,  v2, 6, 7

    qo_butterfly_mixsls \
             v4,  v6,  v8, v10,  v5,  v7,  v9, v11, v28, v29, v30, v31, \
            v13, v15, v17, v19, v20, v21, v22, v23, \
             v0, \
             v3, 0, 1,  v3, 2, 3,  v3, 4, 5,  v3, 6, 7, \
            src, \
             q5,  q7,  q9, q11, \
             #1*32, #3*32, #5*32, #7*32, \
            src, \
             q5,  q7,  q9, q11, \
             #(16+1*32), #(16+3*32), #(16+5*32), #(16+7*32), \
            src, \
             q4,  q6,  q8, q10, \
             #0*32, #2*32, #4*32, #6*32

    qo_butterfly_botsls \
            v12, v14, v16, v18, v13, v15, v17, v19, v20, v21, v22, v23, \
            src, \
            q13, q15, q17, q19, \
             #9*32, #11*32, #13*32, #15*32, \
            src, \
            q13, q15, q17, q19, \
             #(16+9*32), #(16+11*32), #(16+13*32), #(16+15*32), \
            src, \
            q12, q14, q16, q18, \
             #8*32, #10*32, #12*32, #14*32

    qo_butterfly_topl \
             v13, v15, v17, v19, v28, v29, v30, v31, \
             v0, \
             v0, 2, 3,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3, \
            src, \
            q12, q14, q16, q18, \
            #(16+8*32), #(16+10*32), #(16+12*32), #(16+14*32)

    qo_butterfly_mixl \
             v5,  v7,  v9, v11, v13, v15, v17, v19, v28, v29, v30, v31, \
            v12, v14, v16, v18, v20, v21, v22, v23, \
             v0, \
             v0, 2, 3,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3, \
             src, \
             q4,  q6,  q8, q10, \
             #(16+0*32), #(16+2*32), #(16+4*32), #(16+6*32)

    qo_butterfly_mix \
             v4,  v6,  v8, v10, v12, v14, v16, v18, v20, v21, v22, v23, \
             v5,  v7, v13, v15,  v9, v11, v17, v19, v28, v29, v30, v31, \
             v0, \
             v0, 2, 3,  v0, 2, 3,  v0, 2, 3,  v0, 2, 3, \
             v0, 4, 5,  v0, 4, 5,  v0, 6, 7,  v0, 6, 7

    qo_butterfly_mix \
             v5,  v7, v13, v15,  v9, v11, v17, v19, v28, v29, v30, v31, \
             v4,  v6, v12, v14,  v8, v10, v16, v18, v20, v21, v22, v23, \
             v0, \
             v0, 4, 5,  v0, 4, 5,  v0, 6, 7,  v0, 6, 7, \
             v0, 4, 5,  v0, 4, 5,  v0, 6, 7,  v0, 6, 7

    qo_butterfly_mix \
             v4,  v6, v12, v14,  v8, v10, v16, v18, v20, v21, v22, v23, \
             v5,  v9, v13, v17,  v7, v11, v15, v19, v28, v29, v30, v31, \
             v0, \
             v0, 4, 5,  v0, 4, 5,  v0, 6, 7,  v0, 6, 7, \
             v1, 0, 1,  v1, 2, 3,  v1, 4, 5,  v1, 6, 7

    qo_butterfly_mix \
             v5,  v9, v13, v17,  v7, v11, v15, v19, v28, v29, v30, v31, \
             v4,  v8, v12, v16,  v6, v10, v14, v18, v20, v21, v22, v23, \
             v0, \
             v1, 0, 1,  v1, 2, 3,  v1, 4, 5,  v1, 6, 7, \
             v1, 0, 1,  v1, 2, 3,  v1, 4, 5,  v1, 6, 7

    qo_butterfly_mix \
             v4,  v8, v12, v16,  v6, v10, v14, v18, v20, v21, v22, v23, \
             v4,  v6,  v8, v10,  v5,  v7,  v9, v11, v28, v29, v30, v31, \
             v0, \
             v1, 0, 1,  v1, 2, 3,  v1, 4, 5,  v1, 6, 7, \
             v2, 0, 1,  v2, 2, 3,  v2, 4, 5,  v2, 6, 7

    qo_butterfly_mixss \
             v4,  v6,  v8, v10,  v5,  v7,  v9, v11, v28, v29, v30, v31, \
            v13, v15, v17, v19, v20, v21, v22, v23, \
             v0, \
             v3, 0, 1,  v3, 2, 3,  v3, 4, 5,  v3, 6, 7, \
             src, \
             q5,  q7,  q9, q11, \
             #(16+1*32), #(16+3*32), #(16+5*32), #(16+7*32), \
             src, \
             q4,  q6,  q8, q10, \
             #(16+0*32), #(16+2*32), #(16+4*32), #(16+6*32)

    qo_butterfly_botss \
            v12, v14, v16, v18, v13, v15, v17, v19, v20, v21, v22, v23, \
            src, \
            q13, q15, q17, q19, \
            #(16+9*32), #(16+11*32), #(16+13*32), #(16+15*32), \
            src, \
            q12, q14, q16, q18, \
            #(16+8*32), #(16+10*32), #(16+12*32), #(16+14*32)

    .unreq    Q
    .unreq    src
    .unreq    table
    .unreq    counter
    pop_simd

    ret

.align 2
.global PQCLEAN_MLKEM512_AARCH64__asm_ntt_SIMD_bot
.global _PQCLEAN_MLKEM512_AARCH64__asm_ntt_SIMD_bot
PQCLEAN_MLKEM512_AARCH64__asm_ntt_SIMD_bot:
_PQCLEAN_MLKEM512_AARCH64__asm_ntt_SIMD_bot:

    push_simd
    Q         .req w8
    BarrettM  .req w9
    src0      .req x0
    src1      .req x1
    table     .req x10
    counter   .req x11

    ldrsh Q, [x2, #0]
    ldrsh BarrettM, [x2, #8]

    add table, x1, #64

    add src0, x0, #256*0
    add src1, x0, #256*1

    mov v0.H[0], Q
    mov v0.H[1], BarrettM

    ldr q28, [src0, # 1*16]
    ldr q29, [src1, # 1*16]
    ldr q30, [src0, # 3*16]
    ldr q31, [src1, # 3*16]

    trn_4x4_l3 v28, v29, v30, v31, v20, v21, v22, v23, table, q1, q2, q3, #1*16, #2*16, #3*16

    do_butterfly_vec_top_2ltrn_4x4 \
            v29, v31, v18, v19,  v0,  v2,  v3,  v2,  v3, \
            src0, src1, \
            q24, q25, q26, q27, #0*16, #0*16, #2*16, #2*16, \
            v24, v25, v26, v27, v20, v21, v22, v23

    do_butterfly_vec_mixl \
            v25, v27, v29, v31, v18, v19, \
            v28, v30, v16, v17, \
             v0, \
             v2, v3,  v2,  v3, \
            table, \
             q4,  q5,  q6,  q7, #4*16, #5*16, #6*16, #7*16

    do_butterfly_vec_mixl \
            v24, v26, v28, v30, v16, v17, \
            v27, v31, v18, v19, \
             v0, \
             v4,  v5,  v6,  v7, \
            table, \
             q8,  q9, q10, q11, #8*16, #9*16, #10*16, #11*16

    do_butterfly_vec_mixl \
            v25, v29, v27, v31, v18, v19, \
            v26, v30, v16, v17, \
             v0, \
             v4,  v5,  v6,  v7, \
            table, \
            q12, q13, q14, q15, #12*16, #13*16, #14*16, #15*16

    add table, table, #256

    do_butterfly_vec_mix v24, v28, v26, v30, v16, v17, v24, v26, v25, v27, v18, v19,  v0,  v4,  v5,  v6,  v7,  v8,  v9, v10, v11

    do_butterfly_vec_mix v24, v26, v25, v27, v18, v19, v28, v30, v29, v31, v16, v17,  v0,  v8,  v9, v10, v11, v12, v13, v14, v15

    do_butterfly_vec_bot_oo_barrett_trn_4x4 \
            v28, v30, v29, v31, v16, v17, \
            v24, v25, v26, v27, v20, v21, v22, v23, v28, v29, v30, v31, v16, v17, v18, v19,  v0,  #11,  v0

    trn_4x4_2s4 v28, v29, v30, v31, v16, v17, v18, v19, src0, src1, q24, q25, q26, q27, #0*16, #0*16, #2*16, #2*16

    mov counter, #3
    _ntt_bot_loop:

    str q28, [src0, # 1*16]
    ldr q28, [src0, #(64+1*16)]
    str q29, [src1, # 1*16]
    ldr q29, [src1, #(64+1*16)]
    str q30, [src0, # 3*16]
    ldr q30, [src0, #(64+3*16)]
    str q31, [src1, # 3*16]
    ldr q31, [src1, #(64+3*16)]

    add src0, src0, #64
    add src1, src1, #64

    trn_4x4_l3 v28, v29, v30, v31, v20, v21, v22, v23, table, q1, q2, q3, #1*16, #2*16, #3*16

    do_butterfly_vec_top_2ltrn_4x4 \
            v29, v31, v18, v19,  v0,  v2,  v3,  v2,  v3, \
            src0, src1, \
            q24, q25, q26, q27, #0*16, #0*16, #2*16, #2*16, \
            v24, v25, v26, v27, v20, v21, v22, v23

    do_butterfly_vec_mixl \
            v25, v27, v29, v31, v18, v19, \
            v28, v30, v16, v17, \
             v0, \
             v2, v3,  v2,  v3, \
            table, \
             q4,  q5,  q6,  q7, #4*16, #5*16, #6*16, #7*16

    do_butterfly_vec_mixl \
            v24, v26, v28, v30, v16, v17, \
            v27, v31, v18, v19, \
             v0, \
             v4,  v5,  v6,  v7, \
            table, \
             q8,  q9, q10, q11, #8*16, #9*16, #10*16, #11*16

    do_butterfly_vec_mixl \
            v25, v29, v27, v31, v18, v19, \
            v26, v30, v16, v17, \
             v0, \
             v4,  v5,  v6,  v7, \
            table, \
            q12, q13, q14, q15, #12*16, #13*16, #14*16, #15*16

    add table, table, #256

    do_butterfly_vec_mix v24, v28, v26, v30, v16, v17, v24, v26, v25, v27, v18, v19,  v0,  v4,  v5,  v6,  v7,  v8,  v9, v10, v11

    do_butterfly_vec_mix v24, v26, v25, v27, v18, v19, v28, v30, v29, v31, v16, v17,  v0,  v8,  v9, v10, v11, v12, v13, v14, v15

    do_butterfly_vec_bot_oo_barrett_trn_4x4 \
            v28, v30, v29, v31, v16, v17, \
            v24, v25, v26, v27, v20, v21, v22, v23, v28, v29, v30, v31, v16, v17, v18, v19,  v0,  #11,  v0

    trn_4x4_2s4 v28, v29, v30, v31, v16, v17, v18, v19, src0, src1, q24, q25, q26, q27, #0*16, #0*16, #2*16, #2*16

    sub counter, counter, #1
    cbnz counter, _ntt_bot_loop

    str q28, [src0, # 1*16]
    str q29, [src1, # 1*16]
    str q30, [src0, # 3*16]
    str q31, [src1, # 3*16]

    add src0, src0, #64
    add src1, src1, #64

    .unreq    Q
    .unreq    BarrettM
    .unreq    src0
    .unreq    src1
    .unreq    table
    .unreq    counter
    pop_simd

    ret










