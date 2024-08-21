
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
.global PQCLEAN_MLKEM768_AARCH64__asm_point_mul_extended
.global _PQCLEAN_MLKEM768_AARCH64__asm_point_mul_extended
PQCLEAN_MLKEM768_AARCH64__asm_point_mul_extended:
_PQCLEAN_MLKEM768_AARCH64__asm_point_mul_extended:

    push_all
    Q         .req w20
    des       .req x0
    src1      .req x1
    src2ex    .req x2
    counter   .req x19

    ldrsh Q, [x3]

    dup v28.8H, Q

    ldr  q0, [src1, #0*16]
    ldr  q1, [src1, #1*16]
    ldr  q2, [src1, #2*16]
    ldr  q3, [src1, #3*16]
    ldr  q4, [src1, #4*16]
    ldr  q5, [src1, #5*16]
    ldr  q6, [src1, #6*16]
    ldr  q7, [src1, #7*16]

    add src1, src1, #8*16

    uzp2  v1.8H,  v0.8H,  v1.8H
    uzp2  v3.8H,  v2.8H,  v3.8H
    uzp2  v5.8H,  v4.8H,  v5.8H
    uzp2  v7.8H,  v6.8H,  v7.8H

    ldr  q8, [src2ex, #0*16]
    ldr q10, [src2ex, #2*16]
    ldr q12, [src2ex, #4*16]
    ldr q14, [src2ex, #6*16]
    ldr  q9, [src2ex, #1*16]
    ldr q11, [src2ex, #3*16]
    ldr q13, [src2ex, #5*16]
    ldr q15, [src2ex, #7*16]

    add src2ex, src2ex, #8*16

    ldr q16, [src1, #0*16]
    sqrdmulh  v0.8H,  v1.8H,  v8.8H
    ldr q17, [src1, #1*16]
    sqrdmulh  v2.8H,  v3.8H, v10.8H
    ldr q18, [src1, #2*16]
    sqrdmulh  v4.8H,  v5.8H, v12.8H
    ldr q19, [src1, #3*16]
    sqrdmulh  v6.8H,  v7.8H, v14.8H

    ldr q20, [src1, #4*16]
    mul       v1.8H,  v1.8H,  v9.8H
    uzp2 v17.8H, v16.8H, v17.8H
    ldr q21, [src1, #5*16]
    mul       v3.8H,  v3.8H, v11.8H
    uzp2 v19.8H, v18.8H, v19.8H
    ldr q22, [src1, #6*16]
    mul       v5.8H,  v5.8H, v13.8H
    uzp2 v21.8H, v20.8H, v21.8H
    ldr q23, [src1, #7*16]
    mul       v7.8H,  v7.8H, v15.8H
    uzp2 v23.8H, v22.8H, v23.8H

    add src1, src1, #8*16

    ldr  q8, [src2ex, #0*16]
    mls       v1.8H,  v0.8H, v28.8H
    ldr q10, [src2ex, #2*16]
    mls       v3.8H,  v2.8H, v28.8H
    ldr q12, [src2ex, #4*16]
    mls       v5.8H,  v4.8H, v28.8H
    ldr q14, [src2ex, #6*16]
    mls       v7.8H,  v6.8H, v28.8H

    ldr  q9, [src2ex, #1*16]
    str  q1, [des, #0*16]
    ldr q11, [src2ex, #3*16]
    str  q3, [des, #1*16]
    ldr q13, [src2ex, #5*16]
    str  q5, [des, #2*16]
    ldr q15, [src2ex, #7*16]
    str  q7, [des, #3*16]

    add des, des, #4*16

    add src2ex, src2ex, #8*16

    ldr  q0, [src1, #0*16]
    sqrdmulh v16.8H, v17.8H,  v8.8H
    ldr  q1, [src1, #1*16]
    sqrdmulh v18.8H, v19.8H, v10.8H
    ldr  q2, [src1, #2*16]
    sqrdmulh v20.8H, v21.8H, v12.8H
    ldr  q3, [src1, #3*16]
    sqrdmulh v22.8H, v23.8H, v14.8H

    ldr  q4, [src1, #4*16]
    mul      v17.8H, v17.8H,  v9.8H
    uzp2  v1.8H,  v0.8H,  v1.8H
    ldr  q5, [src1, #5*16]
    mul      v19.8H, v19.8H, v11.8H
    uzp2  v3.8H,  v2.8H,  v3.8H
    ldr  q6, [src1, #6*16]
    mul      v21.8H, v21.8H, v13.8H
    uzp2  v5.8H,  v4.8H,  v5.8H
    ldr  q7, [src1, #7*16]
    mul      v23.8H, v23.8H, v15.8H
    uzp2  v7.8H,  v6.8H,  v7.8H

    add src1, src1, #8*16

    ldr  q8, [src2ex, #0*16]
    mls      v17.8H, v16.8H, v28.8H
    ldr q10, [src2ex, #2*16]
    mls      v19.8H, v18.8H, v28.8H
    ldr q12, [src2ex, #4*16]
    mls      v21.8H, v20.8H, v28.8H
    ldr q14, [src2ex, #6*16]
    mls      v23.8H, v22.8H, v28.8H

    ldr  q9, [src2ex, #1*16]
    str q17, [des, #0*16]
    ldr q11, [src2ex, #3*16]
    str q19, [des, #1*16]
    ldr q13, [src2ex, #5*16]
    str q21, [des, #2*16]
    ldr q15, [src2ex, #7*16]
    str q23, [des, #3*16]

    add des, des, #4*16

    add src2ex, src2ex, #8*16

    ldr q16, [src1, #0*16]
    sqrdmulh  v0.8H,  v1.8H,  v8.8H
    ldr q17, [src1, #1*16]
    sqrdmulh  v2.8H,  v3.8H, v10.8H
    ldr q18, [src1, #2*16]
    sqrdmulh  v4.8H,  v5.8H, v12.8H
    ldr q19, [src1, #3*16]
    sqrdmulh  v6.8H,  v7.8H, v14.8H

    ldr q20, [src1, #4*16]
    mul       v1.8H,  v1.8H,  v9.8H
    uzp2 v17.8H, v16.8H, v17.8H
    ldr q21, [src1, #5*16]
    mul       v3.8H,  v3.8H, v11.8H
    uzp2 v19.8H, v18.8H, v19.8H
    ldr q22, [src1, #6*16]
    mul       v5.8H,  v5.8H, v13.8H
    uzp2 v21.8H, v20.8H, v21.8H
    ldr q23, [src1, #7*16]
    mul       v7.8H,  v7.8H, v15.8H
    uzp2 v23.8H, v22.8H, v23.8H

    add src1, src1, #8*16

    ldr  q8, [src2ex, #0*16]
    mls       v1.8H,  v0.8H, v28.8H
    ldr q10, [src2ex, #2*16]
    mls       v3.8H,  v2.8H, v28.8H
    ldr q12, [src2ex, #4*16]
    mls       v5.8H,  v4.8H, v28.8H
    ldr q14, [src2ex, #6*16]
    mls       v7.8H,  v6.8H, v28.8H

    ldr  q9, [src2ex, #1*16]
    str  q1, [des, #0*16]
    ldr q11, [src2ex, #3*16]
    str  q3, [des, #1*16]
    ldr q13, [src2ex, #5*16]
    str  q5, [des, #2*16]
    ldr q15, [src2ex, #7*16]
    str  q7, [des, #3*16]

    add des, des, #4*16

    add src2ex, src2ex, #8*16

    sqrdmulh v16.8H, v17.8H,  v8.8H
    sqrdmulh v18.8H, v19.8H, v10.8H
    sqrdmulh v20.8H, v21.8H, v12.8H
    sqrdmulh v22.8H, v23.8H, v14.8H

    mul      v17.8H, v17.8H,  v9.8H
    mul      v19.8H, v19.8H, v11.8H
    mul      v21.8H, v21.8H, v13.8H
    mul      v23.8H, v23.8H, v15.8H

    mls      v17.8H, v16.8H, v28.8H
    mls      v19.8H, v18.8H, v28.8H
    mls      v21.8H, v20.8H, v28.8H
    mls      v23.8H, v22.8H, v28.8H

    str q17, [des, #0*16]
    str q19, [des, #1*16]
    str q21, [des, #2*16]
    str q23, [des, #3*16]

    add des, des, #4*16


    .unreq    Q
    .unreq    des
    .unreq    src1
    .unreq    src2ex
    .unreq    counter
    pop_all

    ret


.align 2
.global PQCLEAN_MLKEM768_AARCH64__asm_asymmetric_mul
.global _PQCLEAN_MLKEM768_AARCH64__asm_asymmetric_mul
PQCLEAN_MLKEM768_AARCH64__asm_asymmetric_mul:
_PQCLEAN_MLKEM768_AARCH64__asm_asymmetric_mul:

    push_all
    des       .req x11
    src1_0    .req x0
    src2_0    .req x1
    src2asy_0 .req x2
    src1_1    .req x4
    src2_1    .req x5
    src2asy_1 .req x6
    src1_2    .req x8
    src2_2    .req x9
    src2asy_2 .req x10
    src1_3    .req x12
    src2_3    .req x13
    src2asy_3 .req x14
    counter   .req x19

    ldr s4, [x3]

    add des, x4, #0

    add     src1_1,    src1_0, #512*1
    add     src2_1,    src2_0, #512*1
    add  src2asy_1, src2asy_0, #256*1

#if KYBER_K > 2
    add     src1_2,    src1_0, #512*2
    add     src2_2,    src2_0, #512*2
    add  src2asy_2, src2asy_0, #256*2
#endif

#if KYBER_K > 3
    add     src1_3,    src1_0, #512*3
    add     src2_3,    src2_0, #512*3
    add  src2asy_3, src2asy_0, #256*3
#endif

    ldr q20, [src1_0, #0*16]
    ldr q21, [src1_0, #1*16]
    ldr q22, [src2_0, #0*16]
    ldr q23, [src2_0, #1*16]

    add src1_0, src1_0, #32
    add src2_0, src2_0, #32

    uzp1  v0.8H, v20.8H, v21.8H
    uzp2  v1.8H, v20.8H, v21.8H
    uzp1  v2.8H, v22.8H, v23.8H
    uzp2  v3.8H, v22.8H, v23.8H

    ld1 {v28.8H}, [src2asy_0], #16

    smull  v16.4S,  v0.4H,  v2.4H
    ldr q20, [src1_1, #0*16]
    smull2 v18.4S,  v0.8H,  v2.8H
    ldr q21, [src1_1, #1*16]
    smull  v17.4S,  v0.4H,  v3.4H
    ldr q22, [src2_1, #0*16]
    smull2 v19.4S,  v0.8H,  v3.8H
    ldr q23, [src2_1, #1*16]

    add src1_1, src1_1, #32
    add src2_1, src2_1, #32

    smlal  v16.4S,  v1.4H, v28.4H
    uzp1  v8.8H, v20.8H, v21.8H
    smlal2 v18.4S,  v1.8H, v28.8H
    uzp2  v9.8H, v20.8H, v21.8H
    smlal  v17.4S,  v1.4H,  v2.4H
    uzp1 v10.8H, v22.8H, v23.8H
    smlal2 v19.4S,  v1.8H,  v2.8H
    uzp2 v11.8H, v22.8H, v23.8H

    ld1 {v29.8H}, [src2asy_1], #16

#if KYBER_K > 2

    smlal  v16.4S,  v8.4H, v10.4H
    ldr q20, [src1_2, #0*16]
    smlal2 v18.4S,  v8.8H, v10.8H
    ldr q21, [src1_2, #1*16]
    smlal  v17.4S,  v8.4H, v11.4H
    ldr q22, [src2_2, #0*16]
    smlal2 v19.4S,  v8.8H, v11.8H
    ldr q23, [src2_2, #1*16]

    add src1_2, src1_2, #32
    add src2_2, src2_2, #32

    smlal  v16.4S,  v9.4H, v29.4H
    uzp1 v12.8H, v20.8H, v21.8H
    smlal2 v18.4S,  v9.8H, v29.8H
    uzp2 v13.8H, v20.8H, v21.8H
    smlal  v17.4S,  v9.4H, v10.4H
    uzp1 v14.8H, v22.8H, v23.8H
    smlal2 v19.4S,  v9.8H, v10.8H
    uzp2 v15.8H, v22.8H, v23.8H

    ld1 {v30.8H}, [src2asy_2], #16

#if KYBER_K > 3

    smlal  v16.4S, v12.4H, v14.4H
    ldr q20, [src1_3, #0*16]
    smlal2 v18.4S, v12.8H, v14.8H
    ldr q21, [src1_3, #1*16]
    smlal  v17.4S, v12.4H, v15.4H
    ldr q22, [src2_3, #0*16]
    smlal2 v19.4S, v12.8H, v15.8H
    ldr q23, [src2_3, #1*16]

    add src1_3, src1_3, #32
    add src2_3, src2_3, #32

    smlal  v16.4S, v13.4H, v30.4H
    uzp1 v24.8H, v20.8H, v21.8H
    smlal2 v18.4S, v13.8H, v30.8H
    uzp2 v25.8H, v20.8H, v21.8H
    smlal  v17.4S, v13.4H, v14.4H
    uzp1 v26.8H, v22.8H, v23.8H
    smlal2 v19.4S, v13.8H, v14.8H
    uzp2 v27.8H, v22.8H, v23.8H

    ld1 {v31.8H}, [src2asy_3], #16

    smlal  v16.4S, v24.4H, v26.4H
    smlal2 v18.4S, v24.8H, v26.8H
    smlal  v17.4S, v24.4H, v27.4H
    smlal2 v19.4S, v24.8H, v27.8H

    smlal  v16.4S, v25.4H, v31.4H
    smlal2 v18.4S, v25.8H, v31.8H
    smlal  v17.4S, v25.4H, v26.4H
    smlal2 v19.4S, v25.8H, v26.8H

#else

    smlal  v16.4S, v12.4H, v14.4H
    smlal2 v18.4S, v12.8H, v14.8H
    smlal  v17.4S, v12.4H, v15.4H
    smlal2 v19.4S, v12.8H, v15.8H

    smlal  v16.4S, v13.4H, v30.4H
    smlal2 v18.4S, v13.8H, v30.8H
    smlal  v17.4S, v13.4H, v14.4H
    smlal2 v19.4S, v13.8H, v14.8H

#endif

#else

    smlal  v16.4S,  v8.4H, v10.4H
    smlal2 v18.4S,  v8.8H, v10.8H
    smlal  v17.4S,  v8.4H, v11.4H
    smlal2 v19.4S,  v8.8H, v11.8H

    smlal  v16.4S,  v9.4H, v29.4H
    smlal2 v18.4S,  v9.8H, v29.8H
    smlal  v17.4S,  v9.4H, v10.4H
    smlal2 v19.4S,  v9.8H, v10.8H

#endif

    // TODO:interleaving
    mov counter, #15
    _asymmetric_mul_loop:

    ldr q20, [src1_0, #0*16]
    uzp1    v6.8H, v16.8H, v18.8H
    ldr q21, [src1_0, #1*16]
    uzp1    v7.8H, v17.8H, v19.8H

    ldr q22, [src2_0, #0*16]
    mul     v6.8H,  v6.8H, v4.H[1]
    ldr q23, [src2_0, #1*16]
    mul     v7.8H,  v7.8H, v4.H[1]

    add src1_0, src1_0, #32
    add src2_0, src2_0, #32

    smlal  v16.4S,  v6.4H, v4.H[0]
    uzp1  v0.8H, v20.8H, v21.8H
    smlal2 v18.4S,  v6.8H, v4.H[0]
    uzp2  v1.8H, v20.8H, v21.8H
    smlal  v17.4S,  v7.4H, v4.H[0]
    uzp1  v2.8H, v22.8H, v23.8H
    smlal2 v19.4S,  v7.8H, v4.H[0]
    uzp2  v3.8H, v22.8H, v23.8H

    ld1 {v28.8H}, [src2asy_0], #16
    uzp2    v6.8H, v16.8H, v18.8H
    uzp2    v7.8H, v17.8H, v19.8H

    st2 { v6.8H,  v7.8H}, [des], #32

    smull  v16.4S,  v0.4H,  v2.4H
    ldr q20, [src1_1, #0*16]
    smull2 v18.4S,  v0.8H,  v2.8H
    ldr q21, [src1_1, #1*16]
    smull  v17.4S,  v0.4H,  v3.4H
    ldr q22, [src2_1, #0*16]
    smull2 v19.4S,  v0.8H,  v3.8H
    ldr q23, [src2_1, #1*16]

    add src1_1, src1_1, #32
    add src2_1, src2_1, #32

    smlal  v16.4S,  v1.4H, v28.4H
    uzp1  v8.8H, v20.8H, v21.8H
    smlal2 v18.4S,  v1.8H, v28.8H
    uzp2  v9.8H, v20.8H, v21.8H
    smlal  v17.4S,  v1.4H,  v2.4H
    uzp1 v10.8H, v22.8H, v23.8H
    smlal2 v19.4S,  v1.8H,  v2.8H
    uzp2 v11.8H, v22.8H, v23.8H

    ld1 {v29.8H}, [src2asy_1], #16

#if KYBER_K > 2

    smlal  v16.4S,  v8.4H, v10.4H
    ldr q20, [src1_2, #0*16]
    smlal2 v18.4S,  v8.8H, v10.8H
    ldr q21, [src1_2, #1*16]
    smlal  v17.4S,  v8.4H, v11.4H
    ldr q22, [src2_2, #0*16]
    smlal2 v19.4S,  v8.8H, v11.8H
    ldr q23, [src2_2, #1*16]

    add src1_2, src1_2, #32
    add src2_2, src2_2, #32

    smlal  v16.4S,  v9.4H, v29.4H
    uzp1 v12.8H, v20.8H, v21.8H
    smlal2 v18.4S,  v9.8H, v29.8H
    uzp2 v13.8H, v20.8H, v21.8H
    smlal  v17.4S,  v9.4H, v10.4H
    uzp1 v14.8H, v22.8H, v23.8H
    smlal2 v19.4S,  v9.8H, v10.8H
    uzp2 v15.8H, v22.8H, v23.8H

    ld1 {v30.8H}, [src2asy_2], #16

#if KYBER_K > 3

    smlal  v16.4S, v12.4H, v14.4H
    ldr q20, [src1_3, #0*16]
    smlal2 v18.4S, v12.8H, v14.8H
    ldr q21, [src1_3, #1*16]
    smlal  v17.4S, v12.4H, v15.4H
    ldr q22, [src2_3, #0*16]
    smlal2 v19.4S, v12.8H, v15.8H
    ldr q23, [src2_3, #1*16]

    add src1_3, src1_3, #32
    add src2_3, src2_3, #32

    smlal  v16.4S, v13.4H, v30.4H
    uzp1 v24.8H, v20.8H, v21.8H
    smlal2 v18.4S, v13.8H, v30.8H
    uzp2 v25.8H, v20.8H, v21.8H
    smlal  v17.4S, v13.4H, v14.4H
    uzp1 v26.8H, v22.8H, v23.8H
    smlal2 v19.4S, v13.8H, v14.8H
    uzp2 v27.8H, v22.8H, v23.8H

    ld1 {v31.8H}, [src2asy_3], #16

    smlal  v16.4S, v24.4H, v26.4H
    smlal2 v18.4S, v24.8H, v26.8H
    smlal  v17.4S, v24.4H, v27.4H
    smlal2 v19.4S, v24.8H, v27.8H

    smlal  v16.4S, v25.4H, v31.4H
    smlal2 v18.4S, v25.8H, v31.8H
    smlal  v17.4S, v25.4H, v26.4H
    smlal2 v19.4S, v25.8H, v26.8H

#else

    smlal  v16.4S, v12.4H, v14.4H
    smlal2 v18.4S, v12.8H, v14.8H
    smlal  v17.4S, v12.4H, v15.4H
    smlal2 v19.4S, v12.8H, v15.8H

    smlal  v16.4S, v13.4H, v30.4H
    smlal2 v18.4S, v13.8H, v30.8H
    smlal  v17.4S, v13.4H, v14.4H
    smlal2 v19.4S, v13.8H, v14.8H

#endif

#else

    smlal  v16.4S,  v8.4H, v10.4H
    smlal2 v18.4S,  v8.8H, v10.8H
    smlal  v17.4S,  v8.4H, v11.4H
    smlal2 v19.4S,  v8.8H, v11.8H

    smlal  v16.4S,  v9.4H, v29.4H
    smlal2 v18.4S,  v9.8H, v29.8H
    smlal  v17.4S,  v9.4H, v10.4H
    smlal2 v19.4S,  v9.8H, v10.8H

#endif

    sub counter, counter, #1
    cbnz counter, _asymmetric_mul_loop

    uzp1    v6.8H, v16.8H, v18.8H
    uzp1    v7.8H, v17.8H, v19.8H

    mul     v6.8H,  v6.8H, v4.H[1]
    mul     v7.8H,  v7.8H, v4.H[1]

    smlal  v16.4S,  v6.4H, v4.H[0]
    smlal2 v18.4S,  v6.8H, v4.H[0]
    smlal  v17.4S,  v7.4H, v4.H[0]
    smlal2 v19.4S,  v7.8H, v4.H[0]

    uzp2    v6.8H, v16.8H, v18.8H
    uzp2    v7.8H, v17.8H, v19.8H

    st2 { v6.8H,  v7.8H}, [des], #32

    .unreq    des
    .unreq    src1_0
    .unreq    src2_0
    .unreq    src2asy_0
    .unreq    src1_1
    .unreq    src2_1
    .unreq    src2asy_1
    .unreq    src1_2
    .unreq    src2_2
    .unreq    src2asy_2
    .unreq    src1_3
    .unreq    src2_3
    .unreq    src2asy_3
    .unreq    counter
    pop_all

    ret


.align 2
.global PQCLEAN_MLKEM768_AARCH64__asm_asymmetric_mul_montgomery
.global _PQCLEAN_MLKEM768_AARCH64__asm_asymmetric_mul_montgomery
PQCLEAN_MLKEM768_AARCH64__asm_asymmetric_mul_montgomery:
_PQCLEAN_MLKEM768_AARCH64__asm_asymmetric_mul_montgomery:

    push_all
    des       .req x11
    src1_0    .req x0
    src2_0    .req x1
    src2asy_0 .req x2
    src1_1    .req x4
    src2_1    .req x5
    src2asy_1 .req x6
    src1_2    .req x8
    src2_2    .req x9
    src2asy_2 .req x10
    src1_3    .req x12
    src2_3    .req x13
    src2asy_3 .req x14
    counter   .req x19

    ldr q4, [x3]

    add des, x4, #0

    add     src1_1,    src1_0, #512*1
    add     src2_1,    src2_0, #512*1
    add  src2asy_1, src2asy_0, #256*1

#if KYBER_K > 2
    add     src1_2,    src1_0, #512*2
    add     src2_2,    src2_0, #512*2
    add  src2asy_2, src2asy_0, #256*2
#endif

#if KYBER_K > 3
    add     src1_3,    src1_0, #512*3
    add     src2_3,    src2_0, #512*3
    add  src2asy_3, src2asy_0, #256*3
#endif

    ldr q20, [src1_0, #0*16]
    ldr q21, [src1_0, #1*16]
    ldr q22, [src2_0, #0*16]
    ldr q23, [src2_0, #1*16]

    add src1_0, src1_0, #32
    add src2_0, src2_0, #32

    uzp1  v0.8H, v20.8H, v21.8H
    uzp2  v1.8H, v20.8H, v21.8H
    uzp1  v2.8H, v22.8H, v23.8H
    uzp2  v3.8H, v22.8H, v23.8H

    ld1 {v28.8H}, [src2asy_0], #16

    smull  v16.4S,  v0.4H,  v2.4H
    ldr q20, [src1_1, #0*16]
    smull2 v18.4S,  v0.8H,  v2.8H
    ldr q21, [src1_1, #1*16]
    smull  v17.4S,  v0.4H,  v3.4H
    ldr q22, [src2_1, #0*16]
    smull2 v19.4S,  v0.8H,  v3.8H
    ldr q23, [src2_1, #1*16]

    add src1_1, src1_1, #32
    add src2_1, src2_1, #32

    smlal  v16.4S,  v1.4H, v28.4H
    uzp1  v8.8H, v20.8H, v21.8H
    smlal2 v18.4S,  v1.8H, v28.8H
    uzp2  v9.8H, v20.8H, v21.8H
    smlal  v17.4S,  v1.4H,  v2.4H
    uzp1 v10.8H, v22.8H, v23.8H
    smlal2 v19.4S,  v1.8H,  v2.8H
    uzp2 v11.8H, v22.8H, v23.8H

    ld1 {v29.8H}, [src2asy_1], #16

#if KYBER_K > 2

    smlal  v16.4S,  v8.4H, v10.4H
    ldr q20, [src1_2, #0*16]
    smlal2 v18.4S,  v8.8H, v10.8H
    ldr q21, [src1_2, #1*16]
    smlal  v17.4S,  v8.4H, v11.4H
    ldr q22, [src2_2, #0*16]
    smlal2 v19.4S,  v8.8H, v11.8H
    ldr q23, [src2_2, #1*16]

    add src1_2, src1_2, #32
    add src2_2, src2_2, #32

    smlal  v16.4S,  v9.4H, v29.4H
    uzp1 v12.8H, v20.8H, v21.8H
    smlal2 v18.4S,  v9.8H, v29.8H
    uzp2 v13.8H, v20.8H, v21.8H
    smlal  v17.4S,  v9.4H, v10.4H
    uzp1 v14.8H, v22.8H, v23.8H
    smlal2 v19.4S,  v9.8H, v10.8H
    uzp2 v15.8H, v22.8H, v23.8H

    ld1 {v30.8H}, [src2asy_2], #16

#if KYBER_K > 3

    smlal  v16.4S, v12.4H, v14.4H
    ldr q20, [src1_3, #0*16]
    smlal2 v18.4S, v12.8H, v14.8H
    ldr q21, [src1_3, #1*16]
    smlal  v17.4S, v12.4H, v15.4H
    ldr q22, [src2_3, #0*16]
    smlal2 v19.4S, v12.8H, v15.8H
    ldr q23, [src2_3, #1*16]

    add src1_3, src1_3, #32
    add src2_3, src2_3, #32

    smlal  v16.4S, v13.4H, v30.4H
    uzp1 v24.8H, v20.8H, v21.8H
    smlal2 v18.4S, v13.8H, v30.8H
    uzp2 v25.8H, v20.8H, v21.8H
    smlal  v17.4S, v13.4H, v14.4H
    uzp1 v26.8H, v22.8H, v23.8H
    smlal2 v19.4S, v13.8H, v14.8H
    uzp2 v27.8H, v22.8H, v23.8H

    ld1 {v31.8H}, [src2asy_3], #16

    smlal  v16.4S, v24.4H, v26.4H
    smlal2 v18.4S, v24.8H, v26.8H
    smlal  v17.4S, v24.4H, v27.4H
    smlal2 v19.4S, v24.8H, v27.8H

    smlal  v16.4S, v25.4H, v31.4H
    smlal2 v18.4S, v25.8H, v31.8H
    smlal  v17.4S, v25.4H, v26.4H
    smlal2 v19.4S, v25.8H, v26.8H

#else

    smlal  v16.4S, v12.4H, v14.4H
    smlal2 v18.4S, v12.8H, v14.8H
    smlal  v17.4S, v12.4H, v15.4H
    smlal2 v19.4S, v12.8H, v15.8H

    smlal  v16.4S, v13.4H, v30.4H
    smlal2 v18.4S, v13.8H, v30.8H
    smlal  v17.4S, v13.4H, v14.4H
    smlal2 v19.4S, v13.8H, v14.8H

#endif

#else

    smlal  v16.4S,  v8.4H, v10.4H
    smlal2 v18.4S,  v8.8H, v10.8H
    smlal  v17.4S,  v8.4H, v11.4H
    smlal2 v19.4S,  v8.8H, v11.8H

    smlal  v16.4S,  v9.4H, v29.4H
    smlal2 v18.4S,  v9.8H, v29.8H
    smlal  v17.4S,  v9.4H, v10.4H
    smlal2 v19.4S,  v9.8H, v10.8H

#endif

    mov counter, #15
    _asymmetric_mul_montgomery_loop:

    uzp1    v6.8H, v16.8H, v18.8H
    uzp1    v7.8H, v17.8H, v19.8H

    mul     v6.8H,  v6.8H, v4.H[1]
    mul     v7.8H,  v7.8H, v4.H[1]

    ldr q20, [src1_0, #0*16]
    smlal  v16.4S,  v6.4H, v4.H[0]
    ldr q21, [src1_0, #1*16]
    smlal2 v18.4S,  v6.8H, v4.H[0]
    ldr q22, [src2_0, #0*16]
    smlal  v17.4S,  v7.4H, v4.H[0]
    ldr q23, [src2_0, #1*16]
    smlal2 v19.4S,  v7.8H, v4.H[0]

    add src1_0, src1_0, #32
    add src2_0, src2_0, #32

    uzp2    v6.8H, v16.8H, v18.8H
    uzp2    v7.8H, v17.8H, v19.8H

    uzp1  v0.8H, v20.8H, v21.8H
    sqrdmulh v16.8H,  v6.8H,  v4.H[4]
    uzp2  v1.8H, v20.8H, v21.8H
    sqrdmulh v17.8H,  v7.8H,  v4.H[4]

    uzp1  v2.8H, v22.8H, v23.8H
    mul       v6.8H,  v6.8H,  v4.H[5]
    uzp2  v3.8H, v22.8H, v23.8H
    mul       v7.8H,  v7.8H,  v4.H[5]

    mls       v6.8H, v16.8H,  v4.H[0]
    mls       v7.8H, v17.8H,  v4.H[0]

    st2 { v6.8H,  v7.8H}, [des], #32

    ld1 {v28.8H}, [src2asy_0], #16

    smull  v16.4S,  v0.4H,  v2.4H
    ldr q20, [src1_1, #0*16]
    smull2 v18.4S,  v0.8H,  v2.8H
    ldr q21, [src1_1, #1*16]
    smull  v17.4S,  v0.4H,  v3.4H
    ldr q22, [src2_1, #0*16]
    smull2 v19.4S,  v0.8H,  v3.8H
    ldr q23, [src2_1, #1*16]

    add src1_1, src1_1, #32
    add src2_1, src2_1, #32

    smlal  v16.4S,  v1.4H, v28.4H
    uzp1  v8.8H, v20.8H, v21.8H
    smlal2 v18.4S,  v1.8H, v28.8H
    uzp2  v9.8H, v20.8H, v21.8H
    smlal  v17.4S,  v1.4H,  v2.4H
    uzp1 v10.8H, v22.8H, v23.8H
    smlal2 v19.4S,  v1.8H,  v2.8H
    uzp2 v11.8H, v22.8H, v23.8H

    ld1 {v29.8H}, [src2asy_1], #16

#if KYBER_K > 2

    smlal  v16.4S,  v8.4H, v10.4H
    ldr q20, [src1_2, #0*16]
    smlal2 v18.4S,  v8.8H, v10.8H
    ldr q21, [src1_2, #1*16]
    smlal  v17.4S,  v8.4H, v11.4H
    ldr q22, [src2_2, #0*16]
    smlal2 v19.4S,  v8.8H, v11.8H
    ldr q23, [src2_2, #1*16]

    add src1_2, src1_2, #32
    add src2_2, src2_2, #32

    smlal  v16.4S,  v9.4H, v29.4H
    uzp1 v12.8H, v20.8H, v21.8H
    smlal2 v18.4S,  v9.8H, v29.8H
    uzp2 v13.8H, v20.8H, v21.8H
    smlal  v17.4S,  v9.4H, v10.4H
    uzp1 v14.8H, v22.8H, v23.8H
    smlal2 v19.4S,  v9.8H, v10.8H
    uzp2 v15.8H, v22.8H, v23.8H

    ld1 {v30.8H}, [src2asy_2], #16

#if KYBER_K > 3

    smlal  v16.4S, v12.4H, v14.4H
    ldr q20, [src1_3, #0*16]
    smlal2 v18.4S, v12.8H, v14.8H
    ldr q21, [src1_3, #1*16]
    smlal  v17.4S, v12.4H, v15.4H
    ldr q22, [src2_3, #0*16]
    smlal2 v19.4S, v12.8H, v15.8H
    ldr q23, [src2_3, #1*16]

    add src1_3, src1_3, #32
    add src2_3, src2_3, #32

    smlal  v16.4S, v13.4H, v30.4H
    uzp1 v24.8H, v20.8H, v21.8H
    smlal2 v18.4S, v13.8H, v30.8H
    uzp2 v25.8H, v20.8H, v21.8H
    smlal  v17.4S, v13.4H, v14.4H
    uzp1 v26.8H, v22.8H, v23.8H
    smlal2 v19.4S, v13.8H, v14.8H
    uzp2 v27.8H, v22.8H, v23.8H

    ld1 {v31.8H}, [src2asy_3], #16

    smlal  v16.4S, v24.4H, v26.4H
    smlal2 v18.4S, v24.8H, v26.8H
    smlal  v17.4S, v24.4H, v27.4H
    smlal2 v19.4S, v24.8H, v27.8H

    smlal  v16.4S, v25.4H, v31.4H
    smlal2 v18.4S, v25.8H, v31.8H
    smlal  v17.4S, v25.4H, v26.4H
    smlal2 v19.4S, v25.8H, v26.8H

#else

    smlal  v16.4S, v12.4H, v14.4H
    smlal2 v18.4S, v12.8H, v14.8H
    smlal  v17.4S, v12.4H, v15.4H
    smlal2 v19.4S, v12.8H, v15.8H

    smlal  v16.4S, v13.4H, v30.4H
    smlal2 v18.4S, v13.8H, v30.8H
    smlal  v17.4S, v13.4H, v14.4H
    smlal2 v19.4S, v13.8H, v14.8H

#endif

#else

    smlal  v16.4S,  v8.4H, v10.4H
    smlal2 v18.4S,  v8.8H, v10.8H
    smlal  v17.4S,  v8.4H, v11.4H
    smlal2 v19.4S,  v8.8H, v11.8H

    smlal  v16.4S,  v9.4H, v29.4H
    smlal2 v18.4S,  v9.8H, v29.8H
    smlal  v17.4S,  v9.4H, v10.4H
    smlal2 v19.4S,  v9.8H, v10.8H

#endif

    sub counter, counter, #1
    cbnz counter, _asymmetric_mul_montgomery_loop

    uzp1    v6.8H, v16.8H, v18.8H
    uzp1    v7.8H, v17.8H, v19.8H

    mul     v6.8H,  v6.8H, v4.H[1]
    mul     v7.8H,  v7.8H, v4.H[1]

    smlal  v16.4S,  v6.4H, v4.H[0]
    smlal2 v18.4S,  v6.8H, v4.H[0]
    smlal  v17.4S,  v7.4H, v4.H[0]
    smlal2 v19.4S,  v7.8H, v4.H[0]

    uzp2    v6.8H, v16.8H, v18.8H
    uzp2    v7.8H, v17.8H, v19.8H

    sqrdmulh v16.8H,  v6.8H,  v4.H[4]
    sqrdmulh v17.8H,  v7.8H,  v4.H[4]

    mul       v6.8H,  v6.8H,  v4.H[5]
    mul       v7.8H,  v7.8H,  v4.H[5]

    mls       v6.8H, v16.8H,  v4.H[0]
    mls       v7.8H, v17.8H,  v4.H[0]

    st2 { v6.8H,  v7.8H}, [des], #32

    .unreq    des
    .unreq    src1_0
    .unreq    src2_0
    .unreq    src2asy_0
    .unreq    src1_1
    .unreq    src2_1
    .unreq    src2asy_1
    .unreq    src1_2
    .unreq    src2_2
    .unreq    src2asy_2
    .unreq    src1_3
    .unreq    src2_3
    .unreq    src2asy_3
    .unreq    counter
    pop_all

    ret



