
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

#ifndef MACROS_COMMON
#define MACROS_COMMON

// for ABI

.macro push_all

    sub sp, sp, #(9*16)
    stp x19, x20, [sp, #0*16]
    stp x21, x22, [sp, #1*16]
    stp x23, x24, [sp, #2*16]
    stp x25, x26, [sp, #3*16]
    stp x27, x28, [sp, #4*16]
    stp  d8,  d9, [sp, #5*16]
    stp d10, d11, [sp, #6*16]
    stp d12, d13, [sp, #7*16]
    stp d14, d15, [sp, #8*16]

.endm

.macro pop_all

    ldp x19, x20, [sp, #0*16]
    ldp x21, x22, [sp, #1*16]
    ldp x23, x24, [sp, #2*16]
    ldp x25, x26, [sp, #3*16]
    ldp x27, x28, [sp, #4*16]
    ldp  d8,  d9, [sp, #5*16]
    ldp d10, d11, [sp, #6*16]
    ldp d12, d13, [sp, #7*16]
    ldp d14, d15, [sp, #8*16]
    add sp, sp, #(9*16)

.endm

.macro push_simd

    sub sp, sp, #(4*16)
    stp  d8,  d9, [sp, #0*16]
    stp d10, d11, [sp, #1*16]
    stp d12, d13, [sp, #2*16]
    stp d14, d15, [sp, #3*16]

.endm

.macro pop_simd

    ldp  d8,  d9, [sp, #0*16]
    ldp d10, d11, [sp, #1*16]
    ldp d12, d13, [sp, #2*16]
    ldp d14, d15, [sp, #3*16]
    add sp, sp, #(4*16)

.endm

// vector-scalar butterflies

.macro wrap_dX_butterfly_top a0, a1, b0, b1, t0, t1, mod, z0, l0, h0, z1, l1, h1, wX, nX

    mul      \t0\wX, \b0\wX, \z0\nX[\h0]
    mul      \t1\wX, \b1\wX, \z1\nX[\h1]

    sqrdmulh \b0\wX, \b0\wX, \z0\nX[\l0]
    sqrdmulh \b1\wX, \b1\wX, \z1\nX[\l1]

    mls      \t0\wX, \b0\wX, \mod\nX[0]
    mls      \t1\wX, \b1\wX, \mod\nX[0]

.endm

.macro wrap_dX_butterfly_topl4 a0, a1, b0, b1, t0, t1, mod, z0, l0, h0, z1, l1, h1, wX, nX, src_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3

    ldr         \c0, [\src_ptr, \memc0]
    mul      \t0\wX, \b0\wX, \z0\nX[\h0]
    ldr         \c1, [\src_ptr, \memc1]
    mul      \t1\wX, \b1\wX, \z1\nX[\h1]

    ldr         \c2, [\src_ptr, \memc2]
    sqrdmulh \b0\wX, \b0\wX, \z0\nX[\l0]
    ldr         \c3, [\src_ptr, \memc3]
    sqrdmulh \b1\wX, \b1\wX, \z1\nX[\l1]

    mls      \t0\wX, \b0\wX, \mod\nX[0]
    mls      \t1\wX, \b1\wX, \mod\nX[0]

.endm

.macro wrap_dX_butterfly_top2l4s4 a0, a1, b0, b1, t0, t1, mod, z0, l0, h0, z1, l1, h1, wX, nX, srcc_ptr, c0, c1, memc0, memc1, srcd_ptr, d0, d1, memd0, memd1, srce_ptr, e0, e1, e2, e3, meme0, meme1, meme2, meme3

    ldr         \c0, [\srcc_ptr, \memc0]
    str         \e0, [\srce_ptr, \meme0]
    mul      \t0\wX, \b0\wX, \z0\nX[\h0]
    ldr         \c1, [\srcc_ptr, \memc1]
    str         \e1, [\srce_ptr, \meme1]
    mul      \t1\wX, \b1\wX, \z1\nX[\h1]

    ldr         \d0, [\srcd_ptr, \memd0]
    str         \e2, [\srce_ptr, \meme2]
    sqrdmulh \b0\wX, \b0\wX, \z0\nX[\l0]
    ldr         \d1, [\srcd_ptr, \memd1]
    str         \e3, [\srce_ptr, \meme3]
    sqrdmulh \b1\wX, \b1\wX, \z1\nX[\l1]

    mls      \t0\wX, \b0\wX, \mod\nX[0]
    mls      \t1\wX, \b1\wX, \mod\nX[0]

.endm


.macro wrap_dX_butterfly_bot a0, a1, b0, b1, t0, t1, mod, z0, l0, h0, z1, l1, h1, wX, nX

    sub     \b0\wX, \a0\wX, \t0\wX
    sub     \b1\wX, \a1\wX, \t1\wX

    add     \a0\wX, \a0\wX, \t0\wX
    add     \a1\wX, \a1\wX, \t1\wX

.endm

.macro wrap_dX_butterfly_mix a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, wX, nX

    sub      \b0\wX, \a0\wX, \t0\wX
    mul      \t2\wX, \b2\wX, \z2\nX[\h2]
    sub      \b1\wX, \a1\wX, \t1\wX
    mul      \t3\wX, \b3\wX, \z3\nX[\h3]

    add      \a0\wX, \a0\wX, \t0\wX
    sqrdmulh \b2\wX, \b2\wX, \z2\nX[\l2]
    add      \a1\wX, \a1\wX, \t1\wX
    sqrdmulh \b3\wX, \b3\wX, \z3\nX[\l3]

    mls      \t2\wX, \b2\wX, \mod\nX[0]
    mls      \t3\wX, \b3\wX, \mod\nX[0]

.endm

.macro wrap_dX_butterfly_mixl6 a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, wX, nX, srcc_ptr, c0, c1, c2, c3, c4, c5, memc0, memc1, memc2, memc3, memc4, memc5

    ldr         \c0, [\srcc_ptr, \memc0]
    sub      \b0\wX, \a0\wX, \t0\wX
    mul      \t2\wX, \b2\wX, \z2\nX[\h2]
    ldr         \c1, [\srcc_ptr, \memc1]
    sub      \b1\wX, \a1\wX, \t1\wX
    mul      \t3\wX, \b3\wX, \z3\nX[\h3]

    ldr         \c2, [\srcc_ptr, \memc2]
    add      \a0\wX, \a0\wX, \t0\wX
    sqrdmulh \b2\wX, \b2\wX, \z2\nX[\l2]
    ldr         \c3, [\srcc_ptr, \memc3]
    add      \a1\wX, \a1\wX, \t1\wX
    sqrdmulh \b3\wX, \b3\wX, \z3\nX[\l3]

    ldr         \c4, [\srcc_ptr, \memc4]
    mls      \t2\wX, \b2\wX, \mod\nX[0]
    ldr         \c5, [\srcc_ptr, \memc5]
    mls      \t3\wX, \b3\wX, \mod\nX[0]

.endm

.macro wrap_dX_butterfly_mix_rev a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, wX, nX

    mul      \t0\wX, \b0\wX, \z0\nX[\h0]
    sub      \b2\wX, \a2\wX, \t2\wX
    mul      \t1\wX, \b1\wX, \z1\nX[\h1]
    sub      \b3\wX, \a3\wX, \t3\wX

    sqrdmulh \b0\wX, \b0\wX, \z0\nX[\l0]
    add      \a2\wX, \a2\wX, \t2\wX
    sqrdmulh \b1\wX, \b1\wX, \z1\nX[\l1]
    add      \a3\wX, \a3\wX, \t3\wX

    mls      \t0\wX, \b0\wX, \mod\nX[0]
    mls      \t1\wX, \b1\wX, \mod\nX[0]

.endm

.macro wrap_qX_butterfly_top a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, wX, nX

    mul      \t0\wX, \b0\wX, \z0\nX[\h0]
    mul      \t1\wX, \b1\wX, \z1\nX[\h1]
    mul      \t2\wX, \b2\wX, \z2\nX[\h2]
    mul      \t3\wX, \b3\wX, \z3\nX[\h3]

    sqrdmulh \b0\wX, \b0\wX, \z0\nX[\l0]
    sqrdmulh \b1\wX, \b1\wX, \z1\nX[\l1]
    sqrdmulh \b2\wX, \b2\wX, \z2\nX[\l2]
    sqrdmulh \b3\wX, \b3\wX, \z3\nX[\l3]

    mls      \t0\wX, \b0\wX, \mod\nX[0]
    mls      \t1\wX, \b1\wX, \mod\nX[0]
    mls      \t2\wX, \b2\wX, \mod\nX[0]
    mls      \t3\wX, \b3\wX, \mod\nX[0]

.endm

.macro wrap_qX_butterfly_topl b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, wX, nX, src_ptr, c0, c1, c2, c3, mem0, mem1, mem2, mem3

    mul      \t0\wX, \b0\wX, \z0\nX[\h0]
    mul      \t1\wX, \b1\wX, \z1\nX[\h1]
    mul      \t2\wX, \b2\wX, \z2\nX[\h2]
    mul      \t3\wX, \b3\wX, \z3\nX[\h3]

    ldr         \c0, [\src_ptr, \mem0]
    sqrdmulh \b0\wX, \b0\wX, \z0\nX[\l0]
    ldr         \c1, [\src_ptr, \mem1]
    sqrdmulh \b1\wX, \b1\wX, \z1\nX[\l1]
    ldr         \c2, [\src_ptr, \mem2]
    sqrdmulh \b2\wX, \b2\wX, \z2\nX[\l2]
    ldr         \c3, [\src_ptr, \mem3]
    sqrdmulh \b3\wX, \b3\wX, \z3\nX[\l3]

    mls      \t0\wX, \b0\wX, \mod\nX[0]
    mls      \t1\wX, \b1\wX, \mod\nX[0]
    mls      \t2\wX, \b2\wX, \mod\nX[0]
    mls      \t3\wX, \b3\wX, \mod\nX[0]

.endm

.macro wrap_qX_butterfly_tops b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, wX, nX, src_ptr, c0, c1, c2, c3, mem0, mem1, mem2, mem3

    mul      \t0\wX, \b0\wX, \z0\nX[\h0]
    mul      \t1\wX, \b1\wX, \z1\nX[\h1]
    mul      \t2\wX, \b2\wX, \z2\nX[\h2]
    mul      \t3\wX, \b3\wX, \z3\nX[\h3]

    str         \c0, [\src_ptr, \mem0]
    sqrdmulh \b0\wX, \b0\wX, \z0\nX[\l0]
    str         \c1, [\src_ptr, \mem1]
    sqrdmulh \b1\wX, \b1\wX, \z1\nX[\l1]
    str         \c2, [\src_ptr, \mem2]
    sqrdmulh \b2\wX, \b2\wX, \z2\nX[\l2]
    str         \c3, [\src_ptr, \mem3]
    sqrdmulh \b3\wX, \b3\wX, \z3\nX[\l3]

    mls      \t0\wX, \b0\wX, \mod\nX[0]
    mls      \t1\wX, \b1\wX, \mod\nX[0]
    mls      \t2\wX, \b2\wX, \mod\nX[0]
    mls      \t3\wX, \b3\wX, \mod\nX[0]

.endm

.macro wrap_qX_butterfly_topsl b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, wX, nX, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3

    mul      \t0\wX, \b0\wX, \z0\nX[\h0]
    mul      \t1\wX, \b1\wX, \z1\nX[\h1]
    mul      \t2\wX, \b2\wX, \z2\nX[\h2]
    mul      \t3\wX, \b3\wX, \z3\nX[\h3]

    str         \c0, [\srcc_ptr, \memc0]
    ldr         \d0, [\srcd_ptr, \memd0]
    sqrdmulh \b0\wX, \b0\wX, \z0\nX[\l0]
    str         \c1, [\srcc_ptr, \memc1]
    ldr         \d1, [\srcd_ptr, \memd1]
    sqrdmulh \b1\wX, \b1\wX, \z1\nX[\l1]
    str         \c2, [\srcc_ptr, \memc2]
    ldr         \d2, [\srcd_ptr, \memd2]
    sqrdmulh \b2\wX, \b2\wX, \z2\nX[\l2]
    str         \c3, [\srcc_ptr, \memc3]
    ldr         \d3, [\srcd_ptr, \memd3]
    sqrdmulh \b3\wX, \b3\wX, \z3\nX[\l3]

    mls      \t0\wX, \b0\wX, \mod\nX[0]
    mls      \t1\wX, \b1\wX, \mod\nX[0]
    mls      \t2\wX, \b2\wX, \mod\nX[0]
    mls      \t3\wX, \b3\wX, \mod\nX[0]

.endm

.macro wrap_qX_butterfly_bot a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, wX, nX

    sub     \b0\wX, \a0\wX, \t0\wX
    sub     \b1\wX, \a1\wX, \t1\wX
    sub     \b2\wX, \a2\wX, \t2\wX
    sub     \b3\wX, \a3\wX, \t3\wX

    add     \a0\wX, \a0\wX, \t0\wX
    add     \a1\wX, \a1\wX, \t1\wX
    add     \a2\wX, \a2\wX, \t2\wX
    add     \a3\wX, \a3\wX, \t3\wX

.endm

.macro wrap_qX_butterfly_botll a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, wX, nX, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3

    sub     \b0\wX, \a0\wX, \t0\wX
    ldr        \c0, [\srcc_ptr, \memc0]
    sub     \b1\wX, \a1\wX, \t1\wX
    ldr        \c1, [\srcc_ptr, \memc1]
    sub     \b2\wX, \a2\wX, \t2\wX
    ldr        \c2, [\srcc_ptr, \memc2]
    sub     \b3\wX, \a3\wX, \t3\wX
    ldr        \c3, [\srcc_ptr, \memc3]

    add     \a0\wX, \a0\wX, \t0\wX
    ldr        \d0, [\srcd_ptr, \memd0]
    add     \a1\wX, \a1\wX, \t1\wX
    ldr        \d1, [\srcd_ptr, \memd1]
    add     \a2\wX, \a2\wX, \t2\wX
    ldr        \d2, [\srcd_ptr, \memd2]
    add     \a3\wX, \a3\wX, \t3\wX
    ldr        \d3, [\srcd_ptr, \memd3]

.endm

.macro wrap_qX_butterfly_botsl a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, wX, nX, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3

    sub     \b0\wX, \a0\wX, \t0\wX
    str        \c0, [\srcc_ptr, \memc0]
    sub     \b1\wX, \a1\wX, \t1\wX
    str        \c1, [\srcc_ptr, \memc1]
    sub     \b2\wX, \a2\wX, \t2\wX
    str        \c2, [\srcc_ptr, \memc2]
    sub     \b3\wX, \a3\wX, \t3\wX
    str        \c3, [\srcc_ptr, \memc3]

    add     \a0\wX, \a0\wX, \t0\wX
    ldr        \d0, [\srcd_ptr, \memd0]
    add     \a1\wX, \a1\wX, \t1\wX
    ldr        \d1, [\srcd_ptr, \memd1]
    add     \a2\wX, \a2\wX, \t2\wX
    ldr        \d2, [\srcd_ptr, \memd2]
    add     \a3\wX, \a3\wX, \t3\wX
    ldr        \d3, [\srcd_ptr, \memd3]

.endm

.macro wrap_qX_butterfly_botss a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, wX, nX, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3

    sub     \b0\wX, \a0\wX, \t0\wX
    str        \c0, [\srcc_ptr, \memc0]
    sub     \b1\wX, \a1\wX, \t1\wX
    str        \c1, [\srcc_ptr, \memc1]
    sub     \b2\wX, \a2\wX, \t2\wX
    str        \c2, [\srcc_ptr, \memc2]
    sub     \b3\wX, \a3\wX, \t3\wX
    str        \c3, [\srcc_ptr, \memc3]

    add     \a0\wX, \a0\wX, \t0\wX
    str        \d0, [\srcd_ptr, \memd0]
    add     \a1\wX, \a1\wX, \t1\wX
    str        \d1, [\srcd_ptr, \memd1]
    add     \a2\wX, \a2\wX, \t2\wX
    str        \d2, [\srcd_ptr, \memd2]
    add     \a3\wX, \a3\wX, \t3\wX
    str        \d3, [\srcd_ptr, \memd3]

.endm

.macro wrap_qX_butterfly_botsls a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, wX, nX, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3, srce_ptr, e0, e1, e2, e3, meme0, meme1, meme2, meme3

    sub     \b0\wX, \a0\wX, \t0\wX
    str        \c0, [\srcc_ptr, \memc0]
    ldr        \d0, [\srcd_ptr, \memd0]
    sub     \b1\wX, \a1\wX, \t1\wX
    str        \c1, [\srcc_ptr, \memc1]
    ldr        \d1, [\srcd_ptr, \memd1]
    sub     \b2\wX, \a2\wX, \t2\wX
    str        \c2, [\srcc_ptr, \memc2]
    ldr        \d2, [\srcd_ptr, \memd2]
    sub     \b3\wX, \a3\wX, \t3\wX
    str        \c3, [\srcc_ptr, \memc3]
    ldr        \d3, [\srcd_ptr, \memd3]

    add     \a0\wX, \a0\wX, \t0\wX
    str        \e0, [\srce_ptr, \meme0]
    add     \a1\wX, \a1\wX, \t1\wX
    str        \e1, [\srce_ptr, \meme1]
    add     \a2\wX, \a2\wX, \t2\wX
    str        \e2, [\srce_ptr, \meme2]
    add     \a3\wX, \a3\wX, \t3\wX
    str        \e3, [\srce_ptr, \meme3]

.endm

.macro wrap_qX_butterfly_botsl_mul a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, wX, nX, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3, a4, a5, a6, a7, t4, t5, t6, t7, mod, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7

    sqrdmulh \t4\wX, \a4\wX,  \z4\nX[\h4]
    sub      \b0\wX, \a0\wX,  \t0\wX
    str         \c0, [\srcc_ptr, \memc0]
    sqrdmulh \t5\wX, \a5\wX,  \z5\nX[\h5]
    sub      \b1\wX, \a1\wX,  \t1\wX
    str         \c1, [\srcc_ptr, \memc1]
    sqrdmulh \t6\wX, \a6\wX,  \z6\nX[\h6]
    sub      \b2\wX, \a2\wX,  \t2\wX
    str         \c2, [\srcc_ptr, \memc2]
    sqrdmulh \t7\wX, \a7\wX,  \z7\nX[\h7]
    sub      \b3\wX, \a3\wX,  \t3\wX
    str         \c3, [\srcc_ptr, \memc3]

    mul      \a4\wX, \a4\wX,  \z4\nX[\l4]
    add      \a0\wX, \a0\wX,  \t0\wX
    ldr         \d0, [\srcd_ptr, \memd0]
    mul      \a5\wX, \a5\wX,  \z5\nX[\l5]
    add      \a1\wX, \a1\wX,  \t1\wX
    ldr         \d1, [\srcd_ptr, \memd1]
    mul      \a6\wX, \a6\wX,  \z6\nX[\l6]
    add      \a2\wX, \a2\wX,  \t2\wX
    ldr         \d2, [\srcd_ptr, \memd2]
    mul      \a7\wX, \a7\wX,  \z7\nX[\l7]
    add      \a3\wX, \a3\wX,  \t3\wX
    ldr         \d3, [\srcd_ptr, \memd3]

    mls      \a4\wX, \t4\wX,  \mod\nX[0]
    mls      \a5\wX, \t5\wX,  \mod\nX[0]
    mls      \a6\wX, \t6\wX,  \mod\nX[0]
    mls      \a7\wX, \t7\wX,  \mod\nX[0]

.endm

.macro wrap_qX_butterfly_mix a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, a4, a5, a6, a7, b4, b5, b6, b7, t4, t5, t6, t7, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7, wX, nX

    sub      \b0\wX, \a0\wX, \t0\wX
    mul      \t4\wX, \b4\wX, \z4\nX[\h4]
    sub      \b1\wX, \a1\wX, \t1\wX
    mul      \t5\wX, \b5\wX, \z5\nX[\h5]
    sub      \b2\wX, \a2\wX, \t2\wX
    mul      \t6\wX, \b6\wX, \z6\nX[\h6]
    sub      \b3\wX, \a3\wX, \t3\wX
    mul      \t7\wX, \b7\wX, \z7\nX[\h7]

    add      \a0\wX, \a0\wX, \t0\wX
    sqrdmulh \b4\wX, \b4\wX, \z4\nX[\l4]
    add      \a1\wX, \a1\wX, \t1\wX
    sqrdmulh \b5\wX, \b5\wX, \z5\nX[\l5]
    add      \a2\wX, \a2\wX, \t2\wX
    sqrdmulh \b6\wX, \b6\wX, \z6\nX[\l6]
    add      \a3\wX, \a3\wX, \t3\wX
    sqrdmulh \b7\wX, \b7\wX, \z7\nX[\l7]

    mls      \t4\wX, \b4\wX, \mod\nX[0]
    mls      \t5\wX, \b5\wX, \mod\nX[0]
    mls      \t6\wX, \b6\wX, \mod\nX[0]
    mls      \t7\wX, \b7\wX, \mod\nX[0]

.endm

.macro wrap_qX_butterfly_mixl a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, b4, b5, b6, b7, t4, t5, t6, t7, mod, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7, wX, nX, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3

    ldr         \c0, [\srcc_ptr, \memc0]
    sub      \b0\wX, \a0\wX, \t0\wX
    mul      \t4\wX, \b4\wX, \z4\nX[\h4]
    ldr         \c1, [\srcc_ptr, \memc1]
    sub      \b1\wX, \a1\wX, \t1\wX
    mul      \t5\wX, \b5\wX, \z5\nX[\h5]
    ldr         \c2, [\srcc_ptr, \memc2]
    sub      \b2\wX, \a2\wX, \t2\wX
    mul      \t6\wX, \b6\wX, \z6\nX[\h6]
    ldr         \c3, [\srcc_ptr, \memc3]
    sub      \b3\wX, \a3\wX, \t3\wX
    mul      \t7\wX, \b7\wX, \z7\nX[\h7]

    add      \a0\wX, \a0\wX, \t0\wX
    sqrdmulh \b4\wX, \b4\wX, \z4\nX[\l4]
    add      \a1\wX, \a1\wX, \t1\wX
    sqrdmulh \b5\wX, \b5\wX, \z5\nX[\l5]
    add      \a2\wX, \a2\wX, \t2\wX
    sqrdmulh \b6\wX, \b6\wX, \z6\nX[\l6]
    add      \a3\wX, \a3\wX, \t3\wX
    sqrdmulh \b7\wX, \b7\wX, \z7\nX[\l7]

    mls      \t4\wX, \b4\wX, \mod\nX[0]
    mls      \t5\wX, \b5\wX, \mod\nX[0]
    mls      \t6\wX, \b6\wX, \mod\nX[0]
    mls      \t7\wX, \b7\wX, \mod\nX[0]

.endm

.macro wrap_qX_butterfly_mixll a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, b4, b5, b6, b7, t4, t5, t6, t7, mod, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7, wX, nX, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3

    ldr         \c0, [\srcc_ptr, \memc0]
    sub      \b0\wX, \a0\wX, \t0\wX
    mul      \t4\wX, \b4\wX, \z4\nX[\h4]
    ldr         \c1, [\srcc_ptr, \memc1]
    sub      \b1\wX, \a1\wX, \t1\wX
    mul      \t5\wX, \b5\wX, \z5\nX[\h5]
    ldr         \c2, [\srcc_ptr, \memc2]
    sub      \b2\wX, \a2\wX, \t2\wX
    mul      \t6\wX, \b6\wX, \z6\nX[\h6]
    ldr         \c3, [\srcc_ptr, \memc3]
    sub      \b3\wX, \a3\wX, \t3\wX
    mul      \t7\wX, \b7\wX, \z7\nX[\h7]

    ldr         \d0, [\srcd_ptr, \memd0]
    add      \a0\wX, \a0\wX, \t0\wX
    sqrdmulh \b4\wX, \b4\wX, \z4\nX[\l4]
    ldr         \d1, [\srcd_ptr, \memd1]
    add      \a1\wX, \a1\wX, \t1\wX
    sqrdmulh \b5\wX, \b5\wX, \z5\nX[\l5]
    ldr         \d2, [\srcd_ptr, \memd2]
    add      \a2\wX, \a2\wX, \t2\wX
    sqrdmulh \b6\wX, \b6\wX, \z6\nX[\l6]
    ldr         \d3, [\srcd_ptr, \memd3]
    add      \a3\wX, \a3\wX, \t3\wX
    sqrdmulh \b7\wX, \b7\wX, \z7\nX[\l7]

    mls      \t4\wX, \b4\wX, \mod\nX[0]
    mls      \t5\wX, \b5\wX, \mod\nX[0]
    mls      \t6\wX, \b6\wX, \mod\nX[0]
    mls      \t7\wX, \b7\wX, \mod\nX[0]

.endm

.macro wrap_qX_butterfly_mixss a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, b4, b5, b6, b7, t4, t5, t6, t7, mod, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7, wX, nX, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3

    mul      \t4\wX, \b4\wX, \z4\nX[\h4]
    sub      \b0\wX, \a0\wX, \t0\wX
    str         \c0, [\srcc_ptr, \memc0]
    mul      \t5\wX, \b5\wX, \z5\nX[\h5]
    sub      \b1\wX, \a1\wX, \t1\wX
    str         \c1, [\srcc_ptr, \memc1]
    mul      \t6\wX, \b6\wX, \z6\nX[\h6]
    sub      \b2\wX, \a2\wX, \t2\wX
    str         \c2, [\srcc_ptr, \memc2]
    mul      \t7\wX, \b7\wX, \z7\nX[\h7]
    sub      \b3\wX, \a3\wX, \t3\wX
    str         \c3, [\srcc_ptr, \memc3]

    sqrdmulh \b4\wX, \b4\wX, \z4\nX[\l4]
    add      \a0\wX, \a0\wX, \t0\wX
    str         \d0, [\srcd_ptr, \memd0]
    sqrdmulh \b5\wX, \b5\wX, \z5\nX[\l5]
    add      \a1\wX, \a1\wX, \t1\wX
    str         \d1, [\srcd_ptr, \memd1]
    sqrdmulh \b6\wX, \b6\wX, \z6\nX[\l6]
    add      \a2\wX, \a2\wX, \t2\wX
    str         \d2, [\srcd_ptr, \memd2]
    sqrdmulh \b7\wX, \b7\wX, \z7\nX[\l7]
    add      \a3\wX, \a3\wX, \t3\wX
    str         \d3, [\srcd_ptr, \memd3]

    mls      \t4\wX, \b4\wX, \mod\nX[0]
    mls      \t5\wX, \b5\wX, \mod\nX[0]
    mls      \t6\wX, \b6\wX, \mod\nX[0]
    mls      \t7\wX, \b7\wX, \mod\nX[0]

.endm

.macro wrap_qX_butterfly_mixsls a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, b4, b5, b6, b7, t4, t5, t6, t7, mod, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7, wX, nX, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3, srce_ptr, e0, e1, e2, e3, meme0, meme1, meme2, meme3

    sub      \b0\wX, \a0\wX, \t0\wX
    str         \c0, [\srcc_ptr, \memc0]
    mul      \t4\wX, \b4\wX, \z4\nX[\h4]
    sub      \b1\wX, \a1\wX, \t1\wX
    str         \c1, [\srcc_ptr, \memc1]
    mul      \t5\wX, \b5\wX, \z5\nX[\h5]
    sub      \b2\wX, \a2\wX, \t2\wX
    str         \c2, [\srcc_ptr, \memc2]
    mul      \t6\wX, \b6\wX, \z6\nX[\h6]
    sub      \b3\wX, \a3\wX, \t3\wX
    str         \c3, [\srcc_ptr, \memc3]
    mul      \t7\wX, \b7\wX, \z7\nX[\h7]

    ldr         \d0, [\srcd_ptr, \memd0]
    add      \a0\wX, \a0\wX, \t0\wX
    sqrdmulh \b4\wX, \b4\wX, \z4\nX[\l4]
    ldr         \d1, [\srcd_ptr, \memd1]
    add      \a1\wX, \a1\wX, \t1\wX
    sqrdmulh \b5\wX, \b5\wX, \z5\nX[\l5]
    ldr         \d2, [\srcd_ptr, \memd2]
    add      \a2\wX, \a2\wX, \t2\wX
    sqrdmulh \b6\wX, \b6\wX, \z6\nX[\l6]
    ldr         \d3, [\srcd_ptr, \memd3]
    add      \a3\wX, \a3\wX, \t3\wX
    sqrdmulh \b7\wX, \b7\wX, \z7\nX[\l7]

    str         \e0, [\srce_ptr, \meme0]
    mls      \t4\wX, \b4\wX, \mod\nX[0]
    str         \e1, [\srce_ptr, \meme1]
    mls      \t5\wX, \b5\wX, \mod\nX[0]
    str         \e2, [\srce_ptr, \meme2]
    mls      \t6\wX, \b6\wX, \mod\nX[0]
    str         \e3, [\srce_ptr, \meme3]
    mls      \t7\wX, \b7\wX, \mod\nX[0]

.endm

.macro wrap_qX_butterfly_mixssl a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, b4, b5, b6, b7, t4, t5, t6, t7, mod, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7, wX, nX, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3, srce_ptr, e0, e1, e2, e3, meme0, meme1, meme2, meme3

    mul      \t4\wX, \b4\wX, \z4\nX[\h4]
    sub      \b0\wX, \a0\wX, \t0\wX
    str         \c0, [\srcc_ptr, \memc0]
    mul      \t5\wX, \b5\wX, \z5\nX[\h5]
    sub      \b1\wX, \a1\wX, \t1\wX
    str         \c1, [\srcc_ptr, \memc1]
    mul      \t6\wX, \b6\wX, \z6\nX[\h6]
    sub      \b2\wX, \a2\wX, \t2\wX
    str         \c2, [\srcc_ptr, \memc2]
    mul      \t7\wX, \b7\wX, \z7\nX[\h7]
    sub      \b3\wX, \a3\wX, \t3\wX
    str         \c3, [\srcc_ptr, \memc3]

    sqrdmulh \b4\wX, \b4\wX, \z4\nX[\l4]
    add      \a0\wX, \a0\wX, \t0\wX
    str         \d0, [\srcd_ptr, \memd0]
    sqrdmulh \b5\wX, \b5\wX, \z5\nX[\l5]
    add      \a1\wX, \a1\wX, \t1\wX
    str         \d1, [\srcd_ptr, \memd1]
    sqrdmulh \b6\wX, \b6\wX, \z6\nX[\l6]
    add      \a2\wX, \a2\wX, \t2\wX
    str         \d2, [\srcd_ptr, \memd2]
    sqrdmulh \b7\wX, \b7\wX, \z7\nX[\l7]
    add      \a3\wX, \a3\wX, \t3\wX
    str         \d3, [\srcd_ptr, \memd3]

    mls      \t4\wX, \b4\wX, \mod\nX[0]
    ldr         \e0, [\srce_ptr, \meme0]
    mls      \t5\wX, \b5\wX, \mod\nX[0]
    ldr         \e1, [\srce_ptr, \meme1]
    mls      \t6\wX, \b6\wX, \mod\nX[0]
    ldr         \e2, [\srce_ptr, \meme2]
    mls      \t7\wX, \b7\wX, \mod\nX[0]
    ldr         \e3, [\srce_ptr, \meme3]

.endm

.macro wrap_qX_butterfly_mix_rev a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, a4, a5, a6, a7, b4, b5, b6, b7, t4, t5, t6, t7, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7, wX, nX

    mul      \t0\wX, \b0\wX, \z0\nX[\h0]
    sub      \b4\wX, \a4\wX, \t4\wX
    mul      \t1\wX, \b1\wX, \z1\nX[\h1]
    sub      \b5\wX, \a5\wX, \t5\wX
    mul      \t2\wX, \b2\wX, \z2\nX[\h2]
    sub      \b6\wX, \a6\wX, \t6\wX
    mul      \t3\wX, \b3\wX, \z3\nX[\h3]
    sub      \b7\wX, \a7\wX, \t7\wX

    sqrdmulh \b0\wX, \b0\wX, \z0\nX[\l0]
    add      \a4\wX, \a4\wX, \t4\wX
    sqrdmulh \b1\wX, \b1\wX, \z1\nX[\l1]
    add      \a5\wX, \a5\wX, \t5\wX
    sqrdmulh \b2\wX, \b2\wX, \z2\nX[\l2]
    add      \a6\wX, \a6\wX, \t6\wX
    sqrdmulh \b3\wX, \b3\wX, \z3\nX[\l3]
    add      \a7\wX, \a7\wX, \t7\wX

    mls      \t0\wX, \b0\wX, \mod\nX[0]
    mls      \t1\wX, \b1\wX, \mod\nX[0]
    mls      \t2\wX, \b2\wX, \mod\nX[0]
    mls      \t3\wX, \b3\wX, \mod\nX[0]

.endm

// vector-vector butterflies

.macro wrap_dX_butterfly_vec_top a0, a1, b0, b1, t0, t1, mod, l0, h0, l1, h1, wX, nX

    mul      \t0\wX, \b0\wX, \h0\wX
    mul      \t1\wX, \b1\wX, \h1\wX

    sqrdmulh \b0\wX, \b0\wX, \l0\wX
    sqrdmulh \b1\wX, \b1\wX, \l1\wX

    mls      \t0\wX, \b0\wX, \mod\nX[0]
    mls      \t1\wX, \b1\wX, \mod\nX[0]

.endm

.macro wrap_dX_butterfly_vec_top_trn_4x4 a0, a1, b0, b1, t0, t1, mod, l0, h0, l1, h1, wX, nX, trns0, trns1, trns2, trns3, trnt0, trnt1, trnt2, trnt3

    mul      \t0\wX, \b0\wX, \h0\wX
    trn1   \trnt0\().4S, \trns0\().4S, \trns1\().4S
    mul      \t1\wX, \b1\wX, \h1\wX
    trn2   \trnt1\().4S, \trns0\().4S, \trns1\().4S

    sqrdmulh \b0\wX, \b0\wX, \l0\wX
    trn1   \trnt2\().4S, \trns2\().4S, \trns3\().4S
    sqrdmulh \b1\wX, \b1\wX, \l1\wX
    trn2   \trnt3\().4S, \trns2\().4S, \trns3\().4S

    mls      \t0\wX, \b0\wX, \mod\nX[0]
    trn1   \trns0\().2D, \trnt0\().2D, \trnt2\().2D
    mls      \t1\wX, \b1\wX, \mod\nX[0]
    trn2   \trns2\().2D, \trnt0\().2D, \trnt2\().2D

    trn1   \trns1\().2D, \trnt1\().2D, \trnt3\().2D
    trn2   \trns3\().2D, \trnt1\().2D, \trnt3\().2D

.endm

.macro wrap_dX_butterfly_vec_top_ltrn_4x4 b0, b1, t0, t1, mod, l0, h0, l1, h1, wX, nX, src0_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, trns0, trns1, trns2, trns3, trnt0, trnt1, trnt2, trnt3

    ldr         \c0, [\src0_ptr, \memc0]
    mul      \t0\wX, \b0\wX, \h0\wX
    ldr         \c1, [\src0_ptr, \memc1]
    mul      \t1\wX, \b1\wX, \h1\wX

    ldr         \c2, [\src0_ptr, \memc2]
    trn1   \trnt0\().4S, \trns0\().4S, \trns1\().4S
    trn2   \trnt1\().4S, \trns0\().4S, \trns1\().4S
    ldr         \c3, [\src0_ptr, \memc3]
    trn1   \trnt2\().4S, \trns2\().4S, \trns3\().4S
    trn2   \trnt3\().4S, \trns2\().4S, \trns3\().4S

    sqrdmulh \b0\wX, \b0\wX, \l0\wX
    trn1   \trns0\().2D, \trnt0\().2D, \trnt2\().2D
    sqrdmulh \b1\wX, \b1\wX, \l1\wX
    trn2   \trns2\().2D, \trnt0\().2D, \trnt2\().2D

    mls      \t0\wX, \b0\wX, \mod\nX[0]
    trn1   \trns1\().2D, \trnt1\().2D, \trnt3\().2D
    mls      \t1\wX, \b1\wX, \mod\nX[0]
    trn2   \trns3\().2D, \trnt1\().2D, \trnt3\().2D

.endm

.macro wrap_dX_butterfly_vec_top_2ltrn_4x4 b0, b1, t0, t1, mod, l0, h0, l1, h1, wX, nX, src0_ptr, src1_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, trns0, trns1, trns2, trns3, trnt0, trnt1, trnt2, trnt3

    ldr         \c0, [\src0_ptr, \memc0]
    mul      \t0\wX, \b0\wX, \h0\wX
    ldr         \c1, [\src1_ptr, \memc1]
    mul      \t1\wX, \b1\wX, \h1\wX

    ldr         \c2, [\src0_ptr, \memc2]
    trn1   \trnt0\().4S, \trns0\().4S, \trns1\().4S
    trn2   \trnt1\().4S, \trns0\().4S, \trns1\().4S
    ldr         \c3, [\src1_ptr, \memc3]
    trn1   \trnt2\().4S, \trns2\().4S, \trns3\().4S
    trn2   \trnt3\().4S, \trns2\().4S, \trns3\().4S

    sqrdmulh \b0\wX, \b0\wX, \l0\wX
    trn1   \trns0\().2D, \trnt0\().2D, \trnt2\().2D
    sqrdmulh \b1\wX, \b1\wX, \l1\wX
    trn2   \trns2\().2D, \trnt0\().2D, \trnt2\().2D

    mls      \t0\wX, \b0\wX, \mod\nX[0]
    trn1   \trns1\().2D, \trnt1\().2D, \trnt3\().2D
    mls      \t1\wX, \b1\wX, \mod\nX[0]
    trn2   \trns3\().2D, \trnt1\().2D, \trnt3\().2D

.endm

.macro wrap_dX_butterfly_vec_bot a0, a1, b0, b1, t0, t1, mod, l0, h0, l1, h1, wX, nX

    sub     \b0\wX, \a0\wX, \t0\wX
    sub     \b1\wX, \a1\wX, \t1\wX

    add     \a0\wX, \a0\wX, \t0\wX
    add     \a1\wX, \a1\wX, \t1\wX

.endm

.macro wrap_dX_butterfly_vec_bot_oo_barrett_trn_4x4 a0, a1, b0, b1, t0, t1, a4, a5, a6, a7, t4, t5, t6, t7, a8, a9, a10, a11, t8, t9, t10, t11, barrett_const, shrv, Q, wX, nX

    sqdmulh  \t4\wX,  \a4\wX, \barrett_const\nX[1]
    sub      \b0\wX,  \a0\wX, \t0\wX
    sqdmulh  \t5\wX,  \a5\wX, \barrett_const\nX[1]
    sub      \b1\wX,  \a1\wX, \t1\wX
    sqdmulh  \t6\wX,  \a6\wX, \barrett_const\nX[1]
    add      \a0\wX,  \a0\wX, \t0\wX
    sqdmulh  \t7\wX,  \a7\wX, \barrett_const\nX[1]
    add      \a1\wX,  \a1\wX, \t1\wX

    sqdmulh  \t8\wX,  \a8\wX, \barrett_const\nX[1]
    srshr    \t4\wX,  \t4\wX, \shrv
    sqdmulh  \t9\wX,  \a9\wX, \barrett_const\nX[1]
    srshr    \t5\wX,  \t5\wX, \shrv
    sqdmulh \t10\wX, \a10\wX, \barrett_const\nX[1]
    srshr    \t6\wX,  \t6\wX, \shrv
    sqdmulh \t11\wX, \a11\wX, \barrett_const\nX[1]
    srshr    \t7\wX,  \t7\wX, \shrv

    mls      \a4\wX,  \t4\wX, \Q\nX[0]
    srshr    \t8\wX,  \t8\wX, \shrv
    mls      \a5\wX,  \t5\wX, \Q\nX[0]
    srshr    \t9\wX,  \t9\wX, \shrv
    mls      \a6\wX,  \t6\wX, \Q\nX[0]
    srshr   \t10\wX, \t10\wX, \shrv
    mls      \a7\wX,  \t7\wX, \Q\nX[0]
    srshr   \t11\wX, \t11\wX, \shrv

    mls      \a8\wX,  \t8\wX, \Q\nX[0]
    trn1     \t4\().4S, \a4\().4S, \a5\().4S
    mls      \a9\wX,  \t9\wX, \Q\nX[0]
    trn2     \t5\().4S, \a4\().4S, \a5\().4S
    mls     \a10\wX, \t10\wX, \Q\nX[0]
    trn1     \t6\().4S, \a6\().4S, \a7\().4S
    mls     \a11\wX, \t11\wX, \Q\nX[0]

    trn2     \t7\().4S, \a6\().4S, \a7\().4S

    trn1     \a4\().2D, \t4\().2D, \t6\().2D
    trn2     \a6\().2D, \t4\().2D, \t6\().2D
    trn1     \a5\().2D, \t5\().2D, \t7\().2D
    trn2     \a7\().2D, \t5\().2D, \t7\().2D

.endm

.macro wrap_dX_butterfly_vec_mix a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, l0, h0, l1, h1, l2, h2, l3, h3, wX, nX

    sub      \b0\wX, \a0\wX, \t0\wX
    mul      \t2\wX, \b2\wX, \h2\wX
    sub      \b1\wX, \a1\wX, \t1\wX
    mul      \t3\wX, \b3\wX, \h3\wX

    add      \a0\wX, \a0\wX, \t0\wX
    sqrdmulh \b2\wX, \b2\wX, \l2\wX
    add      \a1\wX, \a1\wX, \t1\wX
    sqrdmulh \b3\wX, \b3\wX, \l3\wX

    mls      \t2\wX, \b2\wX, \mod\nX[0]
    mls      \t3\wX, \b3\wX, \mod\nX[0]

.endm

.macro wrap_dX_butterfly_vec_mixl a0, a1, b0, b1, t0, t1, b2, b3, t2, t3, mod, l2, h2, l3, h3, wX, nX, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3

    ldr         \c0, [\srcc_ptr, \memc0]
    sub      \b0\wX, \a0\wX, \t0\wX
    mul      \t2\wX, \b2\wX, \h2\wX
    ldr         \c1, [\srcc_ptr, \memc1]
    sub      \b1\wX, \a1\wX, \t1\wX
    mul      \t3\wX, \b3\wX, \h3\wX

    ldr         \c2, [\srcc_ptr, \memc2]
    add      \a0\wX, \a0\wX, \t0\wX
    sqrdmulh \b2\wX, \b2\wX, \l2\wX
    ldr         \c3, [\srcc_ptr, \memc3]
    add      \a1\wX, \a1\wX, \t1\wX
    sqrdmulh \b3\wX, \b3\wX, \l3\wX

    mls      \t2\wX, \b2\wX, \mod\nX[0]
    mls      \t3\wX, \b3\wX, \mod\nX[0]

.endm

.macro wrap_dX_butterfly_vec_mix_rev a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, l0, h0, l1, h1, l2, h2, l3, h3, wX, nX

    mul      \t0\wX, \b0\wX, \h0\wX
    sub      \b2\wX, \a2\wX, \t2\wX
    mul      \t1\wX, \b1\wX, \h1\wX
    sub      \b3\wX, \a3\wX, \t3\wX

    sqrdmulh \b0\wX, \b0\wX, \l0\wX
    add      \a2\wX, \a2\wX, \t2\wX
    sqrdmulh \b1\wX, \b1\wX, \l1\wX
    add      \a3\wX, \a3\wX, \t3\wX

    mls      \t0\wX, \b0\wX, \mod\nX[0]
    mls      \t1\wX, \b1\wX, \mod\nX[0]

.endm

.macro wrap_dX_butterfly_vec_mix_rev_l4 b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, l0, h0, l1, h1, wX, nX, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3

    ldr         \c0, [\srcc_ptr, \memc0]
    mul      \t0\wX, \b0\wX, \h0\wX
    sub      \b2\wX, \a2\wX, \t2\wX
    ldr         \c1, [\srcc_ptr, \memc1]
    mul      \t1\wX, \b1\wX, \h1\wX
    sub      \b3\wX, \a3\wX, \t3\wX

    ldr         \c2, [\srcc_ptr, \memc2]
    sqrdmulh \b0\wX, \b0\wX, \l0\wX
    add      \a2\wX, \a2\wX, \t2\wX
    ldr         \c3, [\srcc_ptr, \memc3]
    sqrdmulh \b1\wX, \b1\wX, \l1\wX
    add      \a3\wX, \a3\wX, \t3\wX

    mls      \t0\wX, \b0\wX, \mod\nX[0]
    mls      \t1\wX, \b1\wX, \mod\nX[0]

.endm

.macro wrap_dX_butterfly_vec_mix_rev_l3 b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, l0, h0, l1, h1, wX, nX, srcc_ptr, c1, c2, c3, memc1, memc2, memc3

    mul      \t0\wX, \b0\wX, \h0\wX
    sub      \b2\wX, \a2\wX, \t2\wX
    ldr         \c1, [\srcc_ptr, \memc1]
    mul      \t1\wX, \b1\wX, \h1\wX
    sub      \b3\wX, \a3\wX, \t3\wX

    ldr         \c2, [\srcc_ptr, \memc2]
    sqrdmulh \b0\wX, \b0\wX, \l0\wX
    add      \a2\wX, \a2\wX, \t2\wX
    ldr         \c3, [\srcc_ptr, \memc3]
    sqrdmulh \b1\wX, \b1\wX, \l1\wX
    add      \a3\wX, \a3\wX, \t3\wX

    mls      \t0\wX, \b0\wX, \mod\nX[0]
    mls      \t1\wX, \b1\wX, \mod\nX[0]

.endm

// vector-scalar Barrett reduction

.macro wrap_qX_barrett a0, a1, a2, a3, t0, t1, t2, t3, barrett_const, shrv, Q, wX, nX

    sqdmulh \t0\wX, \a0\wX, \barrett_const\nX[1]
    sqdmulh \t1\wX, \a1\wX, \barrett_const\nX[1]

    sqdmulh \t2\wX, \a2\wX, \barrett_const\nX[1]
    srshr   \t0\wX, \t0\wX, \shrv
    sqdmulh \t3\wX, \a3\wX, \barrett_const\nX[1]
    srshr   \t1\wX, \t1\wX, \shrv

    srshr   \t2\wX, \t2\wX, \shrv
    mls     \a0\wX, \t0\wX, \Q\nX[0]
    srshr   \t3\wX, \t3\wX, \shrv
    mls     \a1\wX, \t1\wX, \Q\nX[0]

    mls     \a2\wX, \t2\wX, \Q\nX[0]
    mls     \a3\wX, \t3\wX, \Q\nX[0]

.endm

.macro wrap_oX_barrett a0, a1, a2, a3, t0, t1, t2, t3, a4, a5, a6, a7, t4, t5, t6, t7, barrett_const, shrv, Q, wX, nX

    sqdmulh \t0\wX, \a0\wX, \barrett_const\nX[1]
    sqdmulh \t1\wX, \a1\wX, \barrett_const\nX[1]
    sqdmulh \t2\wX, \a2\wX, \barrett_const\nX[1]
    sqdmulh \t3\wX, \a3\wX, \barrett_const\nX[1]

    srshr   \t0\wX, \t0\wX, \shrv
    sqdmulh \t4\wX, \a4\wX, \barrett_const\nX[1]
    srshr   \t1\wX, \t1\wX, \shrv
    sqdmulh \t5\wX, \a5\wX, \barrett_const\nX[1]
    srshr   \t2\wX, \t2\wX, \shrv
    sqdmulh \t6\wX, \a6\wX, \barrett_const\nX[1]
    srshr   \t3\wX, \t3\wX, \shrv
    sqdmulh \t7\wX, \a7\wX, \barrett_const\nX[1]

    mls     \a0\wX, \t0\wX, \Q\nX[0]
    srshr   \t4\wX, \t4\wX, \shrv
    mls     \a1\wX, \t1\wX, \Q\nX[0]
    srshr   \t5\wX, \t5\wX, \shrv
    mls     \a2\wX, \t2\wX, \Q\nX[0]
    srshr   \t6\wX, \t6\wX, \shrv
    mls     \a3\wX, \t3\wX, \Q\nX[0]
    srshr   \t7\wX, \t7\wX, \shrv

    mls     \a4\wX, \t4\wX, \Q\nX[0]
    mls     \a5\wX, \t5\wX, \Q\nX[0]
    mls     \a6\wX, \t6\wX, \Q\nX[0]
    mls     \a7\wX, \t7\wX, \Q\nX[0]

.endm

// vector-vector Barrett reduction

.macro wrap_qo_barrett_vec a0, a1, a2, a3, t0, t1, t2, t3, barrett_const, shrv, Q, wX, nX

    sqdmulh \t0\wX, \a0\wX, \barrett_const\wX
    sqdmulh \t1\wX, \a1\wX, \barrett_const\wX

    sqdmulh \t2\wX, \a2\wX, \barrett_const\wX
    srshr   \t0\wX, \t0\wX, \shrv
    sqdmulh \t3\wX, \a3\wX, \barrett_const\wX
    srshr   \t1\wX, \t1\wX, \shrv

    srshr   \t2\wX, \t2\wX, \shrv
    mls     \a0\wX, \t0\wX, \Q\wX
    srshr   \t3\wX, \t3\wX, \shrv
    mls     \a1\wX, \t1\wX, \Q\wX

    mls     \a2\wX, \t2\wX, \Q\wX
    mls     \a3\wX, \t3\wX, \Q\wX

.endm

.macro wrap_oo_barrett_vec a0, a1, a2, a3, t0, t1, t2, t3, a4, a5, a6, a7, t4, t5, t6, t7, barrett_const, shrv, Q, wX, nX

    sqdmulh \t0\wX, \a0\wX, \barrett_const\wX
    sqdmulh \t1\wX, \a1\wX, \barrett_const\wX
    sqdmulh \t2\wX, \a2\wX, \barrett_const\wX
    sqdmulh \t3\wX, \a3\wX, \barrett_const\wX

    srshr   \t0\wX, \t0\wX, \shrv
    sqdmulh \t4\wX, \a4\wX, \barrett_const\wX
    srshr   \t1\wX, \t1\wX, \shrv
    sqdmulh \t5\wX, \a5\wX, \barrett_const\wX
    srshr   \t2\wX, \t2\wX, \shrv
    sqdmulh \t6\wX, \a6\wX, \barrett_const\wX
    srshr   \t3\wX, \t3\wX, \shrv
    sqdmulh \t7\wX, \a7\wX, \barrett_const\wX

    mls     \a0\wX, \t0\wX, \Q\wX
    srshr   \t4\wX, \t4\wX, \shrv
    mls     \a1\wX, \t1\wX, \Q\wX
    srshr   \t5\wX, \t5\wX, \shrv
    mls     \a2\wX, \t2\wX, \Q\wX
    srshr   \t6\wX, \t6\wX, \shrv
    mls     \a3\wX, \t3\wX, \Q\wX
    srshr   \t7\wX, \t7\wX, \shrv

    mls     \a4\wX, \t4\wX, \Q\wX
    mls     \a5\wX, \t5\wX, \Q\wX
    mls     \a6\wX, \t6\wX, \Q\wX
    mls     \a7\wX, \t7\wX, \Q\wX

.endm

// Montgomery multiplication

.macro wrap_qX_montgomery_mul b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, wX, nX

    mul      \b0\wX, \t0\wX, \z0\nX[\h0]
    mul      \b1\wX, \t1\wX, \z1\nX[\h1]
    mul      \b2\wX, \t2\wX, \z2\nX[\h2]
    mul      \b3\wX, \t3\wX, \z3\nX[\h3]

    sqrdmulh \t0\wX, \t0\wX, \z0\nX[\l0]
    sqrdmulh \t1\wX, \t1\wX, \z1\nX[\l1]
    sqrdmulh \t2\wX, \t2\wX, \z2\nX[\l2]
    sqrdmulh \t3\wX, \t3\wX, \z3\nX[\l3]

    mls      \b0\wX, \t0\wX, \mod\nX[0]
    mls      \b1\wX, \t1\wX, \mod\nX[0]
    mls      \b2\wX, \t2\wX, \mod\nX[0]
    mls      \b3\wX, \t3\wX, \mod\nX[0]

.endm

.macro wrap_qX_montgomery_mul_in b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, wX, nX

    sqrdmulh \t0\wX, \b0\wX, \z0\nX[\l0]
    sqrdmulh \t1\wX, \b1\wX, \z1\nX[\l1]
    sqrdmulh \t2\wX, \b2\wX, \z2\nX[\l2]
    sqrdmulh \t3\wX, \b3\wX, \z3\nX[\l3]

    mul      \b0\wX, \b0\wX, \z0\nX[\h0]
    mul      \b1\wX, \b1\wX, \z1\nX[\h1]
    mul      \b2\wX, \b2\wX, \z2\nX[\h2]
    mul      \b3\wX, \b3\wX, \z3\nX[\h3]

    mls      \b0\wX, \t0\wX, \mod\nX[0]
    mls      \b1\wX, \t1\wX, \mod\nX[0]
    mls      \b2\wX, \t2\wX, \mod\nX[0]
    mls      \b3\wX, \t3\wX, \mod\nX[0]

.endm

.macro wrap_qX_montgomery_mul_ins b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, wX, nX, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3

    str         \c0, [\srcc_ptr, \memc0]
    sqrdmulh \t0\wX, \b0\wX, \z0\nX[\h0]
    str         \c1, [\srcc_ptr, \memc1]
    sqrdmulh \t1\wX, \b1\wX, \z1\nX[\h1]
    str         \c2, [\srcc_ptr, \memc2]
    sqrdmulh \t2\wX, \b2\wX, \z2\nX[\h2]
    str         \c3, [\srcc_ptr, \memc3]
    sqrdmulh \t3\wX, \b3\wX, \z3\nX[\h3]

    mul      \b0\wX, \b0\wX, \z0\nX[\l0]
    mul      \b1\wX, \b1\wX, \z1\nX[\l1]
    mul      \b2\wX, \b2\wX, \z2\nX[\l2]
    mul      \b3\wX, \b3\wX, \z3\nX[\l3]

    mls      \b0\wX, \t0\wX, \mod\nX[0]
    mls      \b1\wX, \t1\wX, \mod\nX[0]
    mls      \b2\wX, \t2\wX, \mod\nX[0]
    mls      \b3\wX, \t3\wX, \mod\nX[0]

.endm

.macro wrap_qX_montgomery_mul_inl b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, wX, nX, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3

    ldr         \c0, [\srcc_ptr, \memc0]
    sqrdmulh \t0\wX, \b0\wX, \z0\nX[\h0]
    ldr         \c1, [\srcc_ptr, \memc1]
    sqrdmulh \t1\wX, \b1\wX, \z1\nX[\h1]
    ldr         \c2, [\srcc_ptr, \memc2]
    sqrdmulh \t2\wX, \b2\wX, \z2\nX[\h2]
    ldr         \c3, [\srcc_ptr, \memc3]
    sqrdmulh \t3\wX, \b3\wX, \z3\nX[\h3]

    mul      \b0\wX, \b0\wX, \z0\nX[\l0]
    mul      \b1\wX, \b1\wX, \z1\nX[\l1]
    mul      \b2\wX, \b2\wX, \z2\nX[\l2]
    mul      \b3\wX, \b3\wX, \z3\nX[\l3]

    mls      \b0\wX, \t0\wX, \mod\nX[0]
    mls      \b1\wX, \t1\wX, \mod\nX[0]
    mls      \b2\wX, \t2\wX, \mod\nX[0]
    mls      \b3\wX, \t3\wX, \mod\nX[0]

.endm

.macro wrap_qX_montgomery_mul_insl b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, wX, nX, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3

    str         \c0, [\srcc_ptr, \memc0]
    ldr         \d0, [\srcd_ptr, \memd0]
    sqrdmulh \t0\wX, \b0\wX, \z0\nX[\h0]
    str         \c1, [\srcc_ptr, \memc1]
    ldr         \d1, [\srcd_ptr, \memd1]
    sqrdmulh \t1\wX, \b1\wX, \z1\nX[\h1]
    str         \c2, [\srcc_ptr, \memc2]
    ldr         \d2, [\srcd_ptr, \memd2]
    sqrdmulh \t2\wX, \b2\wX, \z2\nX[\h2]
    str         \c3, [\srcc_ptr, \memc3]
    ldr         \d3, [\srcd_ptr, \memd3]
    sqrdmulh \t3\wX, \b3\wX, \z3\nX[\h3]

    mul      \b0\wX, \b0\wX, \z0\nX[\l0]
    mul      \b1\wX, \b1\wX, \z1\nX[\l1]
    mul      \b2\wX, \b2\wX, \z2\nX[\l2]
    mul      \b3\wX, \b3\wX, \z3\nX[\l3]

    mls      \b0\wX, \t0\wX, \mod\nX[0]
    mls      \b1\wX, \t1\wX, \mod\nX[0]
    mls      \b2\wX, \t2\wX, \mod\nX[0]
    mls      \b3\wX, \t3\wX, \mod\nX[0]

.endm



// Montgomery reduction with long

.macro wrap_qX_montgomery c0, c1, c2, c3, l0, l1, l2, l3, h0, h1, h2, h3, t0, t1, t2, t3, Qprime, Q, lX, wX, dwX

    uzp1 \t0\wX, \l0\wX, \h0\wX
    uzp1 \t1\wX, \l1\wX, \h1\wX
    uzp1 \t2\wX, \l2\wX, \h2\wX
    uzp1 \t3\wX, \l3\wX, \h3\wX

    mul \t0\wX, \t0\wX, \Qprime\wX
    mul \t1\wX, \t1\wX, \Qprime\wX
    mul \t2\wX, \t2\wX, \Qprime\wX
    mul \t3\wX, \t3\wX, \Qprime\wX

    smlal  \l0\dwX, \t0\lX, \Q\lX
    smlal2 \h0\dwX, \t0\wX, \Q\wX
    smlal  \l1\dwX, \t1\lX, \Q\lX
    smlal2 \h1\dwX, \t1\wX, \Q\wX
    smlal  \l2\dwX, \t2\lX, \Q\lX
    smlal2 \h2\dwX, \t2\wX, \Q\wX
    smlal  \l3\dwX, \t3\lX, \Q\lX
    smlal2 \h3\dwX, \t3\wX, \Q\wX

    uzp2 \c0\wX, \l0\wX, \h0\wX
    uzp2 \c1\wX, \l1\wX, \h1\wX
    uzp2 \c2\wX, \l2\wX, \h2\wX
    uzp2 \c3\wX, \l3\wX, \h3\wX

.endm

// add_sub, sub_add

.macro wrap_qX_add_sub s0, s1, s2, s3, t0, t1, t2, t3, a0, a1, a2, a3, b0, b1, b2, b3, wX

    add \s0\wX, \a0\wX, \b0\wX
    sub \t0\wX, \a0\wX, \b0\wX
    add \s1\wX, \a1\wX, \b1\wX
    sub \t1\wX, \a1\wX, \b1\wX
    add \s2\wX, \a2\wX, \b2\wX
    sub \t2\wX, \a2\wX, \b2\wX
    add \s3\wX, \a3\wX, \b3\wX
    sub \t3\wX, \a3\wX, \b3\wX

.endm

.macro wrap_qX_sub_add s0, s1, s2, s3, t0, t1, t2, t3, a0, a1, a2, a3, b0, b1, b2, b3, wX

    sub \t0\wX, \a0\wX, \b0\wX
    add \s0\wX, \a0\wX, \b0\wX
    sub \t1\wX, \a1\wX, \b1\wX
    add \s1\wX, \a1\wX, \b1\wX
    sub \t2\wX, \a2\wX, \b2\wX
    add \s2\wX, \a2\wX, \b2\wX
    sub \t3\wX, \a3\wX, \b3\wX
    add \s3\wX, \a3\wX, \b3\wX

.endm


#endif




