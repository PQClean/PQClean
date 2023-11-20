
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

#include "macros_common.inc"

.macro trn_4x4 a0, a1, a2, a3, t0, t1, t2, t3

    trn1 \t0\().4S, \a0\().4S, \a1\().4S
    trn2 \t1\().4S, \a0\().4S, \a1\().4S
    trn1 \t2\().4S, \a2\().4S, \a3\().4S
    trn2 \t3\().4S, \a2\().4S, \a3\().4S

    trn1 \a0\().2D, \t0\().2D, \t2\().2D
    trn2 \a2\().2D, \t0\().2D, \t2\().2D
    trn1 \a1\().2D, \t1\().2D, \t3\().2D
    trn2 \a3\().2D, \t1\().2D, \t3\().2D

.endm


.macro trn_4x4_l3 a0, a1, a2, a3, t0, t1, t2, t3, srcc_ptr, c1, c2, c3, memc1, memc2, memc3

    trn1 \t0\().4S, \a0\().4S, \a1\().4S
    trn2 \t1\().4S, \a0\().4S, \a1\().4S
    ldr        \c1, [\srcc_ptr, \memc1]
    trn1 \t2\().4S, \a2\().4S, \a3\().4S
    trn2 \t3\().4S, \a2\().4S, \a3\().4S

    ldr        \c2, [\srcc_ptr, \memc2]
    trn1 \a0\().2D, \t0\().2D, \t2\().2D
    trn2 \a2\().2D, \t0\().2D, \t2\().2D
    ldr        \c3, [\srcc_ptr, \memc3]
    trn1 \a1\().2D, \t1\().2D, \t3\().2D
    trn2 \a3\().2D, \t1\().2D, \t3\().2D

.endm

.macro trn_4x4_s4 a0, a1, a2, a3, t0, t1, t2, t3, src_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3

    str        \c0, [\src_ptr, \memc0]
    trn1 \t0\().4S, \a0\().4S, \a1\().4S
    trn2 \t1\().4S, \a0\().4S, \a1\().4S
    str        \c1, [\src_ptr, \memc1]
    trn1 \t2\().4S, \a2\().4S, \a3\().4S
    trn2 \t3\().4S, \a2\().4S, \a3\().4S

    str        \c2, [\src_ptr, \memc2]
    trn1 \a0\().2D, \t0\().2D, \t2\().2D
    trn2 \a2\().2D, \t0\().2D, \t2\().2D
    str        \c3, [\src_ptr, \memc3]
    trn1 \a1\().2D, \t1\().2D, \t3\().2D
    trn2 \a3\().2D, \t1\().2D, \t3\().2D

.endm

.macro trn_4x4_l4 a0, a1, a2, a3, t0, t1, t2, t3, src_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3

    ldr        \c0, [\src_ptr, \memc0]
    trn1 \t0\().4S, \a0\().4S, \a1\().4S
    trn2 \t1\().4S, \a0\().4S, \a1\().4S
    ldr        \c1, [\src_ptr, \memc1]
    trn1 \t2\().4S, \a2\().4S, \a3\().4S
    trn2 \t3\().4S, \a2\().4S, \a3\().4S

    ldr        \c2, [\src_ptr, \memc2]
    trn1 \a0\().2D, \t0\().2D, \t2\().2D
    trn2 \a2\().2D, \t0\().2D, \t2\().2D
    ldr        \c3, [\src_ptr, \memc3]
    trn1 \a1\().2D, \t1\().2D, \t3\().2D
    trn2 \a3\().2D, \t1\().2D, \t3\().2D

.endm

.macro trn_4x4_2l4 a0, a1, a2, a3, t0, t1, t2, t3, src0_ptr, src1_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3

    ldr        \c0, [\src0_ptr, \memc0]
    trn1 \t0\().4S, \a0\().4S, \a1\().4S
    trn2 \t1\().4S, \a0\().4S, \a1\().4S
    ldr        \c1, [\src1_ptr, \memc1]
    trn1 \t2\().4S, \a2\().4S, \a3\().4S
    trn2 \t3\().4S, \a2\().4S, \a3\().4S

    ldr        \c2, [\src0_ptr, \memc2]
    trn1 \a0\().2D, \t0\().2D, \t2\().2D
    trn2 \a2\().2D, \t0\().2D, \t2\().2D
    ldr        \c3, [\src1_ptr, \memc3]
    trn1 \a1\().2D, \t1\().2D, \t3\().2D
    trn2 \a3\().2D, \t1\().2D, \t3\().2D

.endm

.macro trn_4x4_2s4 a0, a1, a2, a3, t0, t1, t2, t3, src0_ptr, src1_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3

    str        \c0, [\src0_ptr, \memc0]
    trn1 \t0\().4S, \a0\().4S, \a1\().4S
    trn2 \t1\().4S, \a0\().4S, \a1\().4S
    str        \c1, [\src1_ptr, \memc1]
    trn1 \t2\().4S, \a2\().4S, \a3\().4S
    trn2 \t3\().4S, \a2\().4S, \a3\().4S

    str        \c2, [\src0_ptr, \memc2]
    trn1 \a0\().2D, \t0\().2D, \t2\().2D
    trn2 \a2\().2D, \t0\().2D, \t2\().2D
    str        \c3, [\src1_ptr, \memc3]
    trn1 \a1\().2D, \t1\().2D, \t3\().2D
    trn2 \a3\().2D, \t1\().2D, \t3\().2D

.endm

// ==== 16-bit start ====

.macro qo_barrett a0, a1, a2, a3, t0, t1, t2, t3, barrett_const, shrv, Q
    wrap_qX_barrett \a0, \a1, \a2, \a3, \t0, \t1, \t2, \t3, \barrett_const, \shrv, \Q, .8H, .H
.endm

.macro oo_barrett a0, a1, a2, a3, t0, t1, t2, t3, a4, a5, a6, a7, t4, t5, t6, t7, barrett_const, shrv, Q
    wrap_oX_barrett \a0, \a1, \a2, \a3, \t0, \t1, \t2, \t3, \a4, \a5, \a6, \a7, \t4, \t5, \t6, \t7, \barrett_const, \shrv, \Q, .8H, .H
.endm


.macro qo_barrett_vec a0, a1, a2, a3, t0, t1, t2, t3, barrett_const, shrv, Q
    wrap_qo_barrett_vec \a0, \a1, \a2, \a3, \t0, \t1, \t2, \t3, \barrett_const, \shrv, \Q, .8H, .H
.endm

.macro oo_barrett_vec a0, a1, a2, a3, t0, t1, t2, t3, a4, a5, a6, a7, t4, t5, t6, t7, barrett_const, shrv, Q
    wrap_oo_barrett_vec \a0, \a1, \a2, \a3, \t0, \t1, \t2, \t3, \a4, \a5, \a6, \a7, \t4, \t5, \t6, \t7, \barrett_const, \shrv, \Q, .8H, .H
.endm


.macro qo_butterfly_top a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3
    wrap_qX_butterfly_top \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .8H, .H
.endm

.macro qo_butterfly_topl b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, src_ptr, c0, c1, c2, c3, mem0, mem1, mem2, mem3
    wrap_qX_butterfly_topl \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .8H, .H, \src_ptr, \c0, \c1, \c2, \c3, \mem0, \mem1, \mem2, \mem3
.endm

.macro qo_butterfly_tops b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, src_ptr, c0, c1, c2, c3, mem0, mem1, mem2, mem3
    wrap_qX_butterfly_tops \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .8H, .H, \src_ptr, \c0, \c1, \c2, \c3, \mem0, \mem1, \mem2, \mem3
.endm

.macro qo_butterfly_topsl b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3
    wrap_qX_butterfly_topsl \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .8H, .H, \srcc_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3, \srcd_ptr, \d0, \d1, \d2, \d3, \memd0, \memd1, \memd2, \memd3
.endm



.macro qo_butterfly_bot a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3
    wrap_qX_butterfly_bot \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .8H, .H
.endm

.macro qo_butterfly_botll a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3
    wrap_qX_butterfly_botll \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, .8H, .H, \srcc_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3, \srcd_ptr, \d0, \d1, \d2, \d3, \memd0, \memd1, \memd2, \memd3
.endm

.macro qo_butterfly_botss a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3
    wrap_qX_butterfly_botss \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, .8H, .H, \srcc_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3, \srcd_ptr, \d0, \d1, \d2, \d3, \memd0, \memd1, \memd2, \memd3
.endm

.macro qo_butterfly_botsl a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3
    wrap_qX_butterfly_botsl \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, .8H, .H, \srcc_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3, \srcd_ptr, \d0, \d1, \d2, \d3, \memd0, \memd1, \memd2, \memd3
.endm

.macro qo_butterfly_botsls a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3, srce_ptr, e0, e1, e2, e3, meme0, meme1, meme2, meme3
    wrap_qX_butterfly_botsls \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, .8H, .H, \srcc_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3, \srcd_ptr, \d0, \d1, \d2, \d3, \memd0, \memd1, \memd2, \memd3, \srce_ptr, \e0, \e1, \e2, \e3, \meme0, \meme1, \meme2, \meme3
.endm

.macro qo_butterfly_botsl_mul a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3, a4, a5, a6, a7, t4, t5, t6, t7, mod, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7
    wrap_qX_butterfly_botsl_mul \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, .8H, .H, \srcc_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3, \srcd_ptr, \d0, \d1, \d2, \d3, \memd0, \memd1, \memd2, \memd3, \a4, \a5, \a6, \a7, \t4, \t5, \t6, \t7, \mod, \z4, \l4, \h4, \z5, \l5, \h5, \z6, \l6, \h6, \z7, \l7, \h7
.endm



.macro qo_butterfly_mix a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, a4, a5, a6, a7, b4, b5, b6, b7, t4, t5, t6, t7, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7
    wrap_qX_butterfly_mix \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \a4, \a5, \a6, \a7, \b4, \b5, \b6, \b7, \t4, \t5, \t6, \t7, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, \z4, \l4, \h4, \z5, \l5, \h5, \z6, \l6, \h6, \z7, \l7, \h7, .8H, .H
.endm

.macro qo_butterfly_mixl a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, b4, b5, b6, b7, t4, t5, t6, t7, mod, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3
    wrap_qX_butterfly_mixl \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \b4, \b5, \b6, \b7, \t4, \t5, \t6, \t7, \mod, \z4, \l4, \h4, \z5, \l5, \h5, \z6, \l6, \h6, \z7, \l7, \h7, .8H, .H, \srcc_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3
.endm

.macro qo_butterfly_mixll a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, b4, b5, b6, b7, t4, t5, t6, t7, mod, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3
    wrap_qX_butterfly_mixll \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \b4, \b5, \b6, \b7, \t4, \t5, \t6, \t7, \mod, \z4, \l4, \h4, \z5, \l5, \h5, \z6, \l6, \h6, \z7, \l7, \h7, .8H, .H, \srcc_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3, \srcd_ptr, \d0, \d1, \d2, \d3, \memd0, \memd1, \memd2, \memd3
.endm

.macro qo_butterfly_mixss a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, b4, b5, b6, b7, t4, t5, t6, t7, mod, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3, srce_ptr, e0, e1, e2, e3, meme0, meme1, meme2, meme3
    wrap_qX_butterfly_mixss \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \b4, \b5, \b6, \b7, \t4, \t5, \t6, \t7, \mod, \z4, \l4, \h4, \z5, \l5, \h5, \z6, \l6, \h6, \z7, \l7, \h7, .8H, .H, \srcc_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3, \srcd_ptr, \d0, \d1, \d2, \d3, \memd0, \memd1, \memd2, \memd3
.endm

.macro qo_butterfly_mixsls a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, b4, b5, b6, b7, t4, t5, t6, t7, mod, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3, srce_ptr, e0, e1, e2, e3, meme0, meme1, meme2, meme3
    wrap_qX_butterfly_mixsls \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \b4, \b5, \b6, \b7, \t4, \t5, \t6, \t7, \mod, \z4, \l4, \h4, \z5, \l5, \h5, \z6, \l6, \h6, \z7, \l7, \h7, .8H, .H, \srcc_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3, \srcd_ptr, \d0, \d1, \d2, \d3, \memd0, \memd1, \memd2, \memd3, \srce_ptr, \e0, \e1, \e2, \e3, \meme0, \meme1, \meme2, \meme3
.endm

.macro qo_butterfly_mix_rev a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, a4, a5, a6, a7, b4, b5, b6, b7, t4, t5, t6, t7, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7
    wrap_qX_butterfly_mix_rev \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \a4, \a5, \a6, \a7, \b4, \b5, \b6, \b7, \t4, \t5, \t6, \t7, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, \z4, \l4, \h4, \z5, \l5, \h5, \z6, \l6, \h6, \z7, \l7, \h7, .8H, .H
.endm


.macro do_butterfly_vec_top a0, a1, b0, b1, t0, t1, mod, l0, h0, l1, h1
    wrap_dX_butterfly_vec_top \a0, \a1, \b0, \b1, \t0, \t1, \mod, \l0, \h0, \l1, \h1, .8H, .H
.endm

.macro do_butterfly_vec_top_2ltrn_4x4 b0, b1, t0, t1, mod, l0, h0, l1, h1, src0_ptr, src1_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, trns0, trns1, trns2, trns3, trnt0, trnt1, trnt2, trnt3
    wrap_dX_butterfly_vec_top_2ltrn_4x4 \b0, \b1, \t0, \t1, \mod, \l0, \h0, \l1, \h1, .8H, .H, \src0_ptr, \src1_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3, \trns0, \trns1, \trns2, \trns3, \trnt0, \trnt1, \trnt2, \trnt3
.endm

.macro do_butterfly_vec_bot a0, a1, b0, b1, t0, t1, mod, l0, h0, l1, h1
    wrap_dX_butterfly_vec_bot \a0, \a1, \b0, \b1, \t0, \t1, \mod, \l0, \h0, \l1, \h1, .8H, .H
.endm

.macro do_butterfly_vec_bot_oo_barrett_trn_4x4 a0, a1, b0, b1, t0, t1, a4, a5, a6, a7, t4, t5, t6, t7, a8, a9, a10, a11, t8, t9, t10, t11, barrett_const, shrv, Q, wX, nX
    wrap_dX_butterfly_vec_bot_oo_barrett_trn_4x4 \a0, \a1, \b0, \b1, \t0, \t1, \a4, \a5, \a6, \a7, \t4, \t5, \t6, \t7, \a8, \a9, \a10, \a11, \t8, \t9, \t10, \t11, \barrett_const, \shrv, \Q, .8H, .H
.endm

.macro do_butterfly_vec_mix a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, l0, h0, l1, h1, l2, h2, l3, h3
    wrap_dX_butterfly_vec_mix \a0, \a1, \b0, \b1, \t0, \t1, \a2, \a3, \b2, \b3, \t2, \t3, \mod, \l0, \h0, \l1, \h1, \l2, \h2, \l3, \h3, .8H, .H
.endm

.macro do_butterfly_vec_mixl a0, a1, b0, b1, t0, t1, b2, b3, t2, t3, mod, l2, h2, l3, h3, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3
    wrap_dX_butterfly_vec_mixl \a0, \a1, \b0, \b1, \t0, \t1, \b2, \b3, \t2, \t3, \mod, \l2, \h2, \l3, \h3, .8H, .H, \srcc_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3
.endm

.macro do_butterfly_vec_mix_rev a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, l0, h0, l1, h1, l2, h2, l3, h3
    wrap_dX_butterfly_vec_mix_rev \a0, \a1, \b0, \b1, \t0, \t1, \a2, \a3, \b2, \b3, \t2, \t3, \mod, \l0, \h0, \l1, \h1, \l2, \h2, \l3, \h3, .8H, .H
.endm

.macro do_butterfly_vec_mix_rev_l4 b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, l0, h0, l1, h1, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3
    wrap_dX_butterfly_vec_mix_rev_l4 \b0, \b1, \t0, \t1, \a2, \a3, \b2, \b3, \t2, \t3, \mod, \l0, \h0, \l1, \h1, .8H, .H, \srcc_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3
.endm

.macro do_butterfly_vec_mix_rev_l3 b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, l0, h0, l1, h1, srcc_ptr, c1, c2, c3, memc1, memc2, memc3
    wrap_dX_butterfly_vec_mix_rev_l3 \b0, \b1, \t0, \t1, \a2, \a3, \b2, \b3, \t2, \t3, \mod, \l0, \h0, \l1, \h1, .8H, .H, \srcc_ptr, \c1, \c2, \c3, \memc1, \memc2, \memc3
.endm

.macro qo_montgomery_mul_in b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3
    wrap_qX_montgomery_mul_in \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .8H, .H
.endm

.macro qo_montgomery_mul_inl b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3
    wrap_qX_montgomery_mul_inl \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .8H, .H, \srcc_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3
.endm

.macro qo_montgomery_mul_ins b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3
    wrap_qX_montgomery_mul_ins \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .8H, .H, \srcc_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3
.endm

.macro qo_montgomery_mul_insl b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3
    wrap_qX_montgomery_mul_insl \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .8H, .H, \srcc_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3, \srcd_ptr, \d0, \d1, \d2, \d3, \memd0, \memd1, \memd2, \memd3
.endm

// ==== 16-bit end ====

// ==== 32-bit start ====

.macro dq_butterfly_vec_bot a0, a1, b0, b1, t0, t1, mod, l0, h0, l1, h1
    wrap_dX_butterfly_vec_bot \a0, \a1, \b0, \b1, \t0, \t1, \mod, \l0, \h0, \l1, \h1, .4S, .S
.endm

.macro dq_butterfly_vec_top a0, a1, b0, b1, t0, t1, mod, l0, h0, l1, h1
    wrap_dX_butterfly_vec_top \a0, \a1, \b0, \b1, \t0, \t1, \mod, \l0, \h0, \l1, \h1, .4S, .S
.endm

.macro dq_butterfly_vec_top_trn_4x4 a0, a1, b0, b1, t0, t1, mod, l0, h0, l1, h1, trns0, trns1, trns2, trns3, trnt0, trnt1, trnt2, trnt3
    wrap_dX_butterfly_vec_top_trn_4x4 \a0, \a1, \b0, \b1, \t0, \t1, \mod, \l0, \h0, \l1, \h1, .4S, .S, \trns0, \trns1, \trns2, \trns3, \trnt0, \trnt1, \trnt2, \trnt3
.endm

.macro dq_butterfly_vec_top_ltrn_4x4 b0, b1, t0, t1, mod, l0, h0, l1, h1, src0_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, trns0, trns1, trns2, trns3, trnt0, trnt1, trnt2, trnt3
    wrap_dX_butterfly_vec_top_ltrn_4x4 \b0, \b1, \t0, \t1, \mod, \l0, \h0, \l1, \h1, .4S, .S, \src0_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3, \trns0, \trns1, \trns2, \trns3, \trnt0, \trnt1, \trnt2, \trnt3
.endm

.macro dq_butterfly_vec_mix a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, l0, h0, l1, h1, l2, h2, l3, h3
    wrap_dX_butterfly_vec_mix \a0, \a1, \b0, \b1, \t0, \t1, \a2, \a3, \b2, \b3, \t2, \t3, \mod, \l0, \h0, \l1, \h1, \l2, \h2, \l3, \h3, .4S, .S
.endm

.macro dq_butterfly_vec_mix_rev a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, l0, h0, l1, h1, l2, h2, l3, h3
    wrap_dX_butterfly_vec_mix_rev \a0, \a1, \b0, \b1, \t0, \t1, \a2, \a3, \b2, \b3, \t2, \t3, \mod, \l0, \h0, \l1, \h1, \l2, \h2, \l3, \h3, .4S, .S
.endm


.macro dq_butterfly_top a0, a1, b0, b1, t0, t1, mod, z0, l0, h0, z1, l1, h1
    wrap_dX_butterfly_top \a0, \a1, \b0, \b1, \t0, \t1, \mod, \z0, \l0, \h0, \z1, \l1, \h1, .4S, .S
.endm

.macro dq_butterfly_topl4 a0, a1, b0, b1, t0, t1, mod, z0, l0, h0, z1, l1, h1, src_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3
    wrap_dX_butterfly_topl4 \a0, \a1, \b0, \b1, \t0, \t1, \mod, \z0, \l0, \h0, \z1, \l1, \h1, .4S, .S, \src_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3
.endm


.macro dq_butterfly_top2l4s4 a0, a1, b0, b1, t0, t1, mod, z0, l0, h0, z1, l1, h1, srcc_ptr, c0, c1, memc0, memc1, srcd_ptr, d0, d1, memd0, memd1, srce_ptr, e0, e1, e2, e3, meme0, meme1, meme2, meme3
    wrap_dX_butterfly_top2l4s4 \a0, \a1, \b0, \b1, \t0, \t1, \mod, \z0, \l0, \h0, \z1, \l1, \h1, .4S, .S, \srcc_ptr, \c0, \c1, \memc0, \memc1, \srcd_ptr, \d0, \d1, \memd0, \memd1, \srce_ptr, \e0, \e1, \e2, \e3, \meme0, \meme1, \meme2, \meme3
.endm




.macro dq_butterfly_bot a0, a1, b0, b1, t0, t1, mod, z0, l0, h0, z1, l1, h1
    wrap_dX_butterfly_bot \a0, \a1, \b0, \b1, \t0, \t1, \mod, \z0, \l0, \h0, \z1, \l1, \h1, .4S, .S
.endm

.macro dq_butterfly_mix a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3
    wrap_dX_butterfly_mix \a0, \a1, \b0, \b1, \t0, \t1, \a2, \a3, \b2, \b3, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .4S, .S
.endm

.macro dq_butterfly_mixl6 a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, srcc_ptr, c0, c1, c2, c3, c4, c5, memc0, memc1, memc2, memc3, memc4, memc5
    wrap_dX_butterfly_mixl6 \a0, \a1, \b0, \b1, \t0, \t1, \a2, \a3, \b2, \b3, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .4S, .S, \srcc_ptr, \c0, \c1, \c2, \c3, \c4, \c5, \memc0, \memc1, \memc2, \memc3, \memc4, \memc5
.endm

.macro dq_butterfly_mix_rev a0, a1, b0, b1, t0, t1, a2, a3, b2, b3, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3
    wrap_dX_butterfly_mix_rev \a0, \a1, \b0, \b1, \t0, \t1, \a2, \a3, \b2, \b3, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .4S, .S
.endm


.macro qq_montgomery_mul b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3
    wrap_qX_montgomery_mul \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .4S, .S
.endm


.macro qq_butterfly_top a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3
    wrap_qX_butterfly_top \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .4S, .S
.endm

.macro qq_butterfly_topl b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, src_ptr, c0, c1, c2, c3, mem0, mem1, mem2, mem3
    wrap_qX_butterfly_topl \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .4S, .S, \src_ptr, \c0, \c1, \c2, \c3, \mem0, \mem1, \mem2, \mem3
.endm



.macro qq_butterfly_bot a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3
    wrap_qX_butterfly_bot \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, .4S, .S
.endm

.macro qq_butterfly_botll a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3
    wrap_qX_butterfly_botll \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, .4S, .S, \srcc_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3, \srcd_ptr, \d0, \d1, \d2, \d3, \memd0, \memd1, \memd2, \memd3
.endm

.macro qq_butterfly_botss a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3
    wrap_qX_butterfly_botss \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, .4S, .S, \srcc_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3, \srcd_ptr, \d0, \d1, \d2, \d3, \memd0, \memd1, \memd2, \memd3
.endm

.macro qq_butterfly_botsls a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3, srce_ptr, e0, e1, e2, e3, meme0, meme1, meme2, meme3
    wrap_qX_butterfly_botsls \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, .4S, .S, \srcc_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3, \srcd_ptr, \d0, \d1, \d2, \d3, \memd0, \memd1, \memd2, \memd3, \srce_ptr, \e0, \e1, \e2, \e3, \meme0, \meme1, \meme2, \meme3
.endm



.macro qq_butterfly_mix a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, a4, a5, a6, a7, b4, b5, b6, b7, t4, t5, t6, t7, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7
    wrap_qX_butterfly_mix \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \a4, \a5, \a6, \a7, \b4, \b5, \b6, \b7, \t4, \t5, \t6, \t7, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, \z4, \l4, \h4, \z5, \l5, \h5, \z6, \l6, \h6, \z7, \l7, \h7, .4S, .S
.endm

.macro qq_butterfly_mixll a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, b4, b5, b6, b7, t4, t5, t6, t7, mod, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3
    wrap_qX_butterfly_mixll \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \b4, \b5, \b6, \b7, \t4, \t5, \t6, \t7, \mod, \z4, \l4, \h4, \z5, \l5, \h5, \z6, \l6, \h6, \z7, \l7, \h7, .4S, .S, \srcc_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3, \srcd_ptr, \d0, \d1, \d2, \d3, \memd0, \memd1, \memd2, \memd3
.endm

.macro qq_butterfly_mixss a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, b4, b5, b6, b7, t4, t5, t6, t7, mod, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3, srce_ptr, e0, e1, e2, e3, meme0, meme1, meme2, meme3
    wrap_qX_butterfly_mixss \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \b4, \b5, \b6, \b7, \t4, \t5, \t6, \t7, \mod, \z4, \l4, \h4, \z5, \l5, \h5, \z6, \l6, \h6, \z7, \l7, \h7, .4S, .S, \srcc_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3, \srcd_ptr, \d0, \d1, \d2, \d3, \memd0, \memd1, \memd2, \memd3
.endm

.macro qq_butterfly_mixssl a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, b4, b5, b6, b7, t4, t5, t6, t7, mod, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7, srcc_ptr, c0, c1, c2, c3, memc0, memc1, memc2, memc3, srcd_ptr, d0, d1, d2, d3, memd0, memd1, memd2, memd3, srce_ptr, e0, e1, e2, e3, meme0, meme1, meme2, meme3
    wrap_qX_butterfly_mixssl \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \b4, \b5, \b6, \b7, \t4, \t5, \t6, \t7, \mod, \z4, \l4, \h4, \z5, \l5, \h5, \z6, \l6, \h6, \z7, \l7, \h7, .4S, .S, \srcc_ptr, \c0, \c1, \c2, \c3, \memc0, \memc1, \memc2, \memc3, \srcd_ptr, \d0, \d1, \d2, \d3, \memd0, \memd1, \memd2, \memd3, \srce_ptr, \e0, \e1, \e2, \e3, \meme0, \meme1, \meme2, \meme3
.endm

.macro qq_butterfly_mix_rev a0, a1, a2, a3, b0, b1, b2, b3, t0, t1, t2, t3, a4, a5, a6, a7, b4, b5, b6, b7, t4, t5, t6, t7, mod, z0, l0, h0, z1, l1, h1, z2, l2, h2, z3, l3, h3, z4, l4, h4, z5, l5, h5, z6, l6, h6, z7, l7, h7
    wrap_qX_butterfly_mix_rev \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, \t0, \t1, \t2, \t3, \a4, \a5, \a6, \a7, \b4, \b5, \b6, \b7, \t4, \t5, \t6, \t7, \mod, \z0, \l0, \h0, \z1, \l1, \h1, \z2, \l2, \h2, \z3, \l3, \h3, \z4, \l4, \h4, \z5, \l5, \h5, \z6, \l6, \h6, \z7, \l7, \h7, .4S, .S
.endm


.macro qq_montgomery c0, c1, c2, c3, l0, l1, l2, l3, h0, h1, h2, h3, t0, t1, t2, t3, Qprime, Q
    wrap_qX_montgomery \c0, \c1, \c2, \c3, \l0, \l1, \l2, \l3, \h0, \h1, \h2, \h3, \t0, \t1, \t2, \t3, \Qprime, \Q, .2S, .4S, .2D
.endm

.macro qq_sub_add s0, s1, s2, s3, t0, t1, t2, t3, a0, a1, a2, a3, b0, b1, b2, b3
    wrap_qX_sub_add \s0, \s1, \s2, \s3, \t0, \t1, \t2, \t3, \a0, \a1, \a2, \a3, \b0, \b1, \b2, \b3, .4S
.endm

// === 32-bit end ====
