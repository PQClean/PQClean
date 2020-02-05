
# qhasm: int64 input_0

# qhasm: int64 input_1

# qhasm: int64 input_2

# qhasm: int64 input_3

# qhasm: int64 input_4

# qhasm: int64 input_5

# qhasm: stack64 input_6

# qhasm: stack64 input_7

# qhasm: int64 caller_r11

# qhasm: int64 caller_r12

# qhasm: int64 caller_r13

# qhasm: int64 caller_r14

# qhasm: int64 caller_r15

# qhasm: int64 caller_rbx

# qhasm: int64 caller_rbp

# qhasm: int64 b64

# qhasm: int64 synd

# qhasm: int64 addr

# qhasm: int64 c

# qhasm: int64 c_all

# qhasm: int64 row

# qhasm: int64 p

# qhasm: int64 e

# qhasm: int64 s

# qhasm: reg128 pp

# qhasm: reg128 ee

# qhasm: reg128 ss

# qhasm: int64 buf_ptr

# qhasm: stack128 buf

# qhasm: enter syndrome_asm
.p2align 5
.global _PQCLEAN_MCELIECE460896F_SSE_syndrome_asm
.global PQCLEAN_MCELIECE460896F_SSE_syndrome_asm
_PQCLEAN_MCELIECE460896F_SSE_syndrome_asm:
PQCLEAN_MCELIECE460896F_SSE_syndrome_asm:
mov %rsp,%r11
and $31,%r11
add $32,%r11
sub %r11,%rsp

# qhasm: input_1 += 523740
# asm 1: add  $523740,<input_1=int64#2
# asm 2: add  $523740,<input_1=%rsi
add  $523740,%rsi

# qhasm: buf_ptr = &buf
# asm 1: leaq <buf=stack128#1,>buf_ptr=int64#4
# asm 2: leaq <buf=0(%rsp),>buf_ptr=%rcx
leaq 0(%rsp),%rcx

# qhasm: row = 1248
# asm 1: mov  $1248,>row=int64#5
# asm 2: mov  $1248,>row=%r8
mov  $1248,%r8

# qhasm: loop:
._loop:

# qhasm: row -= 1
# asm 1: sub  $1,<row=int64#5
# asm 2: sub  $1,<row=%r8
sub  $1,%r8

# qhasm: ss = mem128[ input_1 + 0 ]
# asm 1: movdqu   0(<input_1=int64#2),>ss=reg128#1
# asm 2: movdqu   0(<input_1=%rsi),>ss=%xmm0
movdqu   0(%rsi),%xmm0

# qhasm: ee = mem128[ input_2 + 156 ]
# asm 1: movdqu   156(<input_2=int64#3),>ee=reg128#2
# asm 2: movdqu   156(<input_2=%rdx),>ee=%xmm1
movdqu   156(%rdx),%xmm1

# qhasm: ss &= ee
# asm 1: pand <ee=reg128#2,<ss=reg128#1
# asm 2: pand <ee=%xmm1,<ss=%xmm0
pand %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 16 ]
# asm 1: movdqu   16(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   16(<input_1=%rsi),>pp=%xmm1
movdqu   16(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 172 ]
# asm 1: movdqu   172(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   172(<input_2=%rdx),>ee=%xmm2
movdqu   172(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 32 ]
# asm 1: movdqu   32(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   32(<input_1=%rsi),>pp=%xmm1
movdqu   32(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 188 ]
# asm 1: movdqu   188(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   188(<input_2=%rdx),>ee=%xmm2
movdqu   188(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 48 ]
# asm 1: movdqu   48(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   48(<input_1=%rsi),>pp=%xmm1
movdqu   48(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 204 ]
# asm 1: movdqu   204(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   204(<input_2=%rdx),>ee=%xmm2
movdqu   204(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 64 ]
# asm 1: movdqu   64(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   64(<input_1=%rsi),>pp=%xmm1
movdqu   64(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 220 ]
# asm 1: movdqu   220(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   220(<input_2=%rdx),>ee=%xmm2
movdqu   220(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 80 ]
# asm 1: movdqu   80(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   80(<input_1=%rsi),>pp=%xmm1
movdqu   80(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 236 ]
# asm 1: movdqu   236(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   236(<input_2=%rdx),>ee=%xmm2
movdqu   236(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 96 ]
# asm 1: movdqu   96(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   96(<input_1=%rsi),>pp=%xmm1
movdqu   96(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 252 ]
# asm 1: movdqu   252(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   252(<input_2=%rdx),>ee=%xmm2
movdqu   252(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 112 ]
# asm 1: movdqu   112(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   112(<input_1=%rsi),>pp=%xmm1
movdqu   112(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 268 ]
# asm 1: movdqu   268(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   268(<input_2=%rdx),>ee=%xmm2
movdqu   268(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 128 ]
# asm 1: movdqu   128(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   128(<input_1=%rsi),>pp=%xmm1
movdqu   128(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 284 ]
# asm 1: movdqu   284(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   284(<input_2=%rdx),>ee=%xmm2
movdqu   284(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 144 ]
# asm 1: movdqu   144(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   144(<input_1=%rsi),>pp=%xmm1
movdqu   144(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 300 ]
# asm 1: movdqu   300(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   300(<input_2=%rdx),>ee=%xmm2
movdqu   300(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 160 ]
# asm 1: movdqu   160(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   160(<input_1=%rsi),>pp=%xmm1
movdqu   160(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 316 ]
# asm 1: movdqu   316(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   316(<input_2=%rdx),>ee=%xmm2
movdqu   316(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 176 ]
# asm 1: movdqu   176(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   176(<input_1=%rsi),>pp=%xmm1
movdqu   176(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 332 ]
# asm 1: movdqu   332(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   332(<input_2=%rdx),>ee=%xmm2
movdqu   332(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 192 ]
# asm 1: movdqu   192(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   192(<input_1=%rsi),>pp=%xmm1
movdqu   192(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 348 ]
# asm 1: movdqu   348(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   348(<input_2=%rdx),>ee=%xmm2
movdqu   348(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 208 ]
# asm 1: movdqu   208(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   208(<input_1=%rsi),>pp=%xmm1
movdqu   208(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 364 ]
# asm 1: movdqu   364(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   364(<input_2=%rdx),>ee=%xmm2
movdqu   364(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 224 ]
# asm 1: movdqu   224(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   224(<input_1=%rsi),>pp=%xmm1
movdqu   224(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 380 ]
# asm 1: movdqu   380(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   380(<input_2=%rdx),>ee=%xmm2
movdqu   380(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 240 ]
# asm 1: movdqu   240(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   240(<input_1=%rsi),>pp=%xmm1
movdqu   240(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 396 ]
# asm 1: movdqu   396(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   396(<input_2=%rdx),>ee=%xmm2
movdqu   396(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 256 ]
# asm 1: movdqu   256(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   256(<input_1=%rsi),>pp=%xmm1
movdqu   256(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 412 ]
# asm 1: movdqu   412(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   412(<input_2=%rdx),>ee=%xmm2
movdqu   412(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 272 ]
# asm 1: movdqu   272(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   272(<input_1=%rsi),>pp=%xmm1
movdqu   272(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 428 ]
# asm 1: movdqu   428(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   428(<input_2=%rdx),>ee=%xmm2
movdqu   428(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 288 ]
# asm 1: movdqu   288(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   288(<input_1=%rsi),>pp=%xmm1
movdqu   288(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 444 ]
# asm 1: movdqu   444(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   444(<input_2=%rdx),>ee=%xmm2
movdqu   444(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 304 ]
# asm 1: movdqu   304(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   304(<input_1=%rsi),>pp=%xmm1
movdqu   304(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 460 ]
# asm 1: movdqu   460(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   460(<input_2=%rdx),>ee=%xmm2
movdqu   460(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 320 ]
# asm 1: movdqu   320(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   320(<input_1=%rsi),>pp=%xmm1
movdqu   320(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 476 ]
# asm 1: movdqu   476(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   476(<input_2=%rdx),>ee=%xmm2
movdqu   476(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 336 ]
# asm 1: movdqu   336(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   336(<input_1=%rsi),>pp=%xmm1
movdqu   336(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 492 ]
# asm 1: movdqu   492(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   492(<input_2=%rdx),>ee=%xmm2
movdqu   492(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 352 ]
# asm 1: movdqu   352(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   352(<input_1=%rsi),>pp=%xmm1
movdqu   352(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 508 ]
# asm 1: movdqu   508(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   508(<input_2=%rdx),>ee=%xmm2
movdqu   508(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 368 ]
# asm 1: movdqu   368(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   368(<input_1=%rsi),>pp=%xmm1
movdqu   368(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 524 ]
# asm 1: movdqu   524(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   524(<input_2=%rdx),>ee=%xmm2
movdqu   524(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 384 ]
# asm 1: movdqu   384(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   384(<input_1=%rsi),>pp=%xmm1
movdqu   384(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 540 ]
# asm 1: movdqu   540(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   540(<input_2=%rdx),>ee=%xmm2
movdqu   540(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: pp = mem128[ input_1 + 400 ]
# asm 1: movdqu   400(<input_1=int64#2),>pp=reg128#2
# asm 2: movdqu   400(<input_1=%rsi),>pp=%xmm1
movdqu   400(%rsi),%xmm1

# qhasm: ee = mem128[ input_2 + 556 ]
# asm 1: movdqu   556(<input_2=int64#3),>ee=reg128#3
# asm 2: movdqu   556(<input_2=%rdx),>ee=%xmm2
movdqu   556(%rdx),%xmm2

# qhasm: pp &= ee
# asm 1: pand <ee=reg128#3,<pp=reg128#2
# asm 2: pand <ee=%xmm2,<pp=%xmm1
pand %xmm2,%xmm1

# qhasm: ss ^= pp
# asm 1: pxor <pp=reg128#2,<ss=reg128#1
# asm 2: pxor <pp=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: buf = ss
# asm 1: movdqa <ss=reg128#1,>buf=stack128#1
# asm 2: movdqa <ss=%xmm0,>buf=0(%rsp)
movdqa %xmm0,0(%rsp)

# qhasm: s = *(uint32 *)(input_1 + 416)
# asm 1: movl   416(<input_1=int64#2),>s=int64#6d
# asm 2: movl   416(<input_1=%rsi),>s=%r9d
movl   416(%rsi),%r9d

# qhasm: e = *(uint32 *)(input_2 + 572)
# asm 1: movl   572(<input_2=int64#3),>e=int64#7d
# asm 2: movl   572(<input_2=%rdx),>e=%eax
movl   572(%rdx),%eax

# qhasm: s &= e
# asm 1: and  <e=int64#7,<s=int64#6
# asm 2: and  <e=%rax,<s=%r9
and  %rax,%r9

# qhasm: c_all = count(s)
# asm 1: popcnt <s=int64#6, >c_all=int64#6
# asm 2: popcnt <s=%r9, >c_all=%r9
popcnt %r9, %r9

# qhasm: b64 = mem64[ buf_ptr + 0 ]
# asm 1: movq   0(<buf_ptr=int64#4),>b64=int64#7
# asm 2: movq   0(<buf_ptr=%rcx),>b64=%rax
movq   0(%rcx),%rax

# qhasm: c = count(b64)
# asm 1: popcnt <b64=int64#7, >c=int64#7
# asm 2: popcnt <b64=%rax, >c=%rax
popcnt %rax, %rax

# qhasm: c_all ^= c
# asm 1: xor  <c=int64#7,<c_all=int64#6
# asm 2: xor  <c=%rax,<c_all=%r9
xor  %rax,%r9

# qhasm: b64 = mem64[ buf_ptr + 8 ]
# asm 1: movq   8(<buf_ptr=int64#4),>b64=int64#7
# asm 2: movq   8(<buf_ptr=%rcx),>b64=%rax
movq   8(%rcx),%rax

# qhasm: c = count(b64)
# asm 1: popcnt <b64=int64#7, >c=int64#7
# asm 2: popcnt <b64=%rax, >c=%rax
popcnt %rax, %rax

# qhasm: c_all ^= c
# asm 1: xor  <c=int64#7,<c_all=int64#6
# asm 2: xor  <c=%rax,<c_all=%r9
xor  %rax,%r9

# qhasm: addr = row
# asm 1: mov  <row=int64#5,>addr=int64#7
# asm 2: mov  <row=%r8,>addr=%rax
mov  %r8,%rax

# qhasm: (uint64) addr >>= 3
# asm 1: shr  $3,<addr=int64#7
# asm 2: shr  $3,<addr=%rax
shr  $3,%rax

# qhasm: addr += input_0
# asm 1: add  <input_0=int64#1,<addr=int64#7
# asm 2: add  <input_0=%rdi,<addr=%rax
add  %rdi,%rax

# qhasm: synd = *(uint8  *) (addr + 0)
# asm 1: movzbq 0(<addr=int64#7),>synd=int64#8
# asm 2: movzbq 0(<addr=%rax),>synd=%r10
movzbq 0(%rax),%r10

# qhasm: synd <<= 1
# asm 1: shl  $1,<synd=int64#8
# asm 2: shl  $1,<synd=%r10
shl  $1,%r10

# qhasm: (uint32) c_all &= 1
# asm 1: and  $1,<c_all=int64#6d
# asm 2: and  $1,<c_all=%r9d
and  $1,%r9d

# qhasm: synd |= c_all
# asm 1: or   <c_all=int64#6,<synd=int64#8
# asm 2: or   <c_all=%r9,<synd=%r10
or   %r9,%r10

# qhasm: *(uint8  *) (addr + 0) = synd
# asm 1: movb   <synd=int64#8b,0(<addr=int64#7)
# asm 2: movb   <synd=%r10b,0(<addr=%rax)
movb   %r10b,0(%rax)

# qhasm: input_1 -= 420
# asm 1: sub  $420,<input_1=int64#2
# asm 2: sub  $420,<input_1=%rsi
sub  $420,%rsi

# qhasm: =? row-0
# asm 1: cmp  $0,<row=int64#5
# asm 2: cmp  $0,<row=%r8
cmp  $0,%r8
# comment:fp stack unchanged by jump

# qhasm: goto loop if !=
jne ._loop

# qhasm: ss = mem128[ input_0 + 0 ]
# asm 1: movdqu   0(<input_0=int64#1),>ss=reg128#1
# asm 2: movdqu   0(<input_0=%rdi),>ss=%xmm0
movdqu   0(%rdi),%xmm0

# qhasm: ee = mem128[ input_2 + 0 ]
# asm 1: movdqu   0(<input_2=int64#3),>ee=reg128#2
# asm 2: movdqu   0(<input_2=%rdx),>ee=%xmm1
movdqu   0(%rdx),%xmm1

# qhasm: ss ^= ee
# asm 1: pxor <ee=reg128#2,<ss=reg128#1
# asm 2: pxor <ee=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: mem128[ input_0 + 0 ] = ss
# asm 1: movdqu   <ss=reg128#1,0(<input_0=int64#1)
# asm 2: movdqu   <ss=%xmm0,0(<input_0=%rdi)
movdqu   %xmm0,0(%rdi)

# qhasm: ss = mem128[ input_0 + 16 ]
# asm 1: movdqu   16(<input_0=int64#1),>ss=reg128#1
# asm 2: movdqu   16(<input_0=%rdi),>ss=%xmm0
movdqu   16(%rdi),%xmm0

# qhasm: ee = mem128[ input_2 + 16 ]
# asm 1: movdqu   16(<input_2=int64#3),>ee=reg128#2
# asm 2: movdqu   16(<input_2=%rdx),>ee=%xmm1
movdqu   16(%rdx),%xmm1

# qhasm: ss ^= ee
# asm 1: pxor <ee=reg128#2,<ss=reg128#1
# asm 2: pxor <ee=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: mem128[ input_0 + 16 ] = ss
# asm 1: movdqu   <ss=reg128#1,16(<input_0=int64#1)
# asm 2: movdqu   <ss=%xmm0,16(<input_0=%rdi)
movdqu   %xmm0,16(%rdi)

# qhasm: ss = mem128[ input_0 + 32 ]
# asm 1: movdqu   32(<input_0=int64#1),>ss=reg128#1
# asm 2: movdqu   32(<input_0=%rdi),>ss=%xmm0
movdqu   32(%rdi),%xmm0

# qhasm: ee = mem128[ input_2 + 32 ]
# asm 1: movdqu   32(<input_2=int64#3),>ee=reg128#2
# asm 2: movdqu   32(<input_2=%rdx),>ee=%xmm1
movdqu   32(%rdx),%xmm1

# qhasm: ss ^= ee
# asm 1: pxor <ee=reg128#2,<ss=reg128#1
# asm 2: pxor <ee=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: mem128[ input_0 + 32 ] = ss
# asm 1: movdqu   <ss=reg128#1,32(<input_0=int64#1)
# asm 2: movdqu   <ss=%xmm0,32(<input_0=%rdi)
movdqu   %xmm0,32(%rdi)

# qhasm: ss = mem128[ input_0 + 48 ]
# asm 1: movdqu   48(<input_0=int64#1),>ss=reg128#1
# asm 2: movdqu   48(<input_0=%rdi),>ss=%xmm0
movdqu   48(%rdi),%xmm0

# qhasm: ee = mem128[ input_2 + 48 ]
# asm 1: movdqu   48(<input_2=int64#3),>ee=reg128#2
# asm 2: movdqu   48(<input_2=%rdx),>ee=%xmm1
movdqu   48(%rdx),%xmm1

# qhasm: ss ^= ee
# asm 1: pxor <ee=reg128#2,<ss=reg128#1
# asm 2: pxor <ee=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: mem128[ input_0 + 48 ] = ss
# asm 1: movdqu   <ss=reg128#1,48(<input_0=int64#1)
# asm 2: movdqu   <ss=%xmm0,48(<input_0=%rdi)
movdqu   %xmm0,48(%rdi)

# qhasm: ss = mem128[ input_0 + 64 ]
# asm 1: movdqu   64(<input_0=int64#1),>ss=reg128#1
# asm 2: movdqu   64(<input_0=%rdi),>ss=%xmm0
movdqu   64(%rdi),%xmm0

# qhasm: ee = mem128[ input_2 + 64 ]
# asm 1: movdqu   64(<input_2=int64#3),>ee=reg128#2
# asm 2: movdqu   64(<input_2=%rdx),>ee=%xmm1
movdqu   64(%rdx),%xmm1

# qhasm: ss ^= ee
# asm 1: pxor <ee=reg128#2,<ss=reg128#1
# asm 2: pxor <ee=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: mem128[ input_0 + 64 ] = ss
# asm 1: movdqu   <ss=reg128#1,64(<input_0=int64#1)
# asm 2: movdqu   <ss=%xmm0,64(<input_0=%rdi)
movdqu   %xmm0,64(%rdi)

# qhasm: ss = mem128[ input_0 + 80 ]
# asm 1: movdqu   80(<input_0=int64#1),>ss=reg128#1
# asm 2: movdqu   80(<input_0=%rdi),>ss=%xmm0
movdqu   80(%rdi),%xmm0

# qhasm: ee = mem128[ input_2 + 80 ]
# asm 1: movdqu   80(<input_2=int64#3),>ee=reg128#2
# asm 2: movdqu   80(<input_2=%rdx),>ee=%xmm1
movdqu   80(%rdx),%xmm1

# qhasm: ss ^= ee
# asm 1: pxor <ee=reg128#2,<ss=reg128#1
# asm 2: pxor <ee=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: mem128[ input_0 + 80 ] = ss
# asm 1: movdqu   <ss=reg128#1,80(<input_0=int64#1)
# asm 2: movdqu   <ss=%xmm0,80(<input_0=%rdi)
movdqu   %xmm0,80(%rdi)

# qhasm: ss = mem128[ input_0 + 96 ]
# asm 1: movdqu   96(<input_0=int64#1),>ss=reg128#1
# asm 2: movdqu   96(<input_0=%rdi),>ss=%xmm0
movdqu   96(%rdi),%xmm0

# qhasm: ee = mem128[ input_2 + 96 ]
# asm 1: movdqu   96(<input_2=int64#3),>ee=reg128#2
# asm 2: movdqu   96(<input_2=%rdx),>ee=%xmm1
movdqu   96(%rdx),%xmm1

# qhasm: ss ^= ee
# asm 1: pxor <ee=reg128#2,<ss=reg128#1
# asm 2: pxor <ee=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: mem128[ input_0 + 96 ] = ss
# asm 1: movdqu   <ss=reg128#1,96(<input_0=int64#1)
# asm 2: movdqu   <ss=%xmm0,96(<input_0=%rdi)
movdqu   %xmm0,96(%rdi)

# qhasm: ss = mem128[ input_0 + 112 ]
# asm 1: movdqu   112(<input_0=int64#1),>ss=reg128#1
# asm 2: movdqu   112(<input_0=%rdi),>ss=%xmm0
movdqu   112(%rdi),%xmm0

# qhasm: ee = mem128[ input_2 + 112 ]
# asm 1: movdqu   112(<input_2=int64#3),>ee=reg128#2
# asm 2: movdqu   112(<input_2=%rdx),>ee=%xmm1
movdqu   112(%rdx),%xmm1

# qhasm: ss ^= ee
# asm 1: pxor <ee=reg128#2,<ss=reg128#1
# asm 2: pxor <ee=%xmm1,<ss=%xmm0
pxor %xmm1,%xmm0

# qhasm: mem128[ input_0 + 112 ] = ss
# asm 1: movdqu   <ss=reg128#1,112(<input_0=int64#1)
# asm 2: movdqu   <ss=%xmm0,112(<input_0=%rdi)
movdqu   %xmm0,112(%rdi)

# qhasm: s = mem64[ input_0 + 128 ]
# asm 1: movq   128(<input_0=int64#1),>s=int64#2
# asm 2: movq   128(<input_0=%rdi),>s=%rsi
movq   128(%rdi),%rsi

# qhasm: e = mem64[ input_2 + 128 ]
# asm 1: movq   128(<input_2=int64#3),>e=int64#4
# asm 2: movq   128(<input_2=%rdx),>e=%rcx
movq   128(%rdx),%rcx

# qhasm: s ^= e
# asm 1: xor  <e=int64#4,<s=int64#2
# asm 2: xor  <e=%rcx,<s=%rsi
xor  %rcx,%rsi

# qhasm: mem64[ input_0 + 128 ] = s
# asm 1: movq   <s=int64#2,128(<input_0=int64#1)
# asm 2: movq   <s=%rsi,128(<input_0=%rdi)
movq   %rsi,128(%rdi)

# qhasm: s = mem64[ input_0 + 136 ]
# asm 1: movq   136(<input_0=int64#1),>s=int64#2
# asm 2: movq   136(<input_0=%rdi),>s=%rsi
movq   136(%rdi),%rsi

# qhasm: e = mem64[ input_2 + 136 ]
# asm 1: movq   136(<input_2=int64#3),>e=int64#4
# asm 2: movq   136(<input_2=%rdx),>e=%rcx
movq   136(%rdx),%rcx

# qhasm: s ^= e
# asm 1: xor  <e=int64#4,<s=int64#2
# asm 2: xor  <e=%rcx,<s=%rsi
xor  %rcx,%rsi

# qhasm: mem64[ input_0 + 136 ] = s
# asm 1: movq   <s=int64#2,136(<input_0=int64#1)
# asm 2: movq   <s=%rsi,136(<input_0=%rdi)
movq   %rsi,136(%rdi)

# qhasm: s = mem64[ input_0 + 144 ]
# asm 1: movq   144(<input_0=int64#1),>s=int64#2
# asm 2: movq   144(<input_0=%rdi),>s=%rsi
movq   144(%rdi),%rsi

# qhasm: e = mem64[ input_2 + 144 ]
# asm 1: movq   144(<input_2=int64#3),>e=int64#4
# asm 2: movq   144(<input_2=%rdx),>e=%rcx
movq   144(%rdx),%rcx

# qhasm: s ^= e
# asm 1: xor  <e=int64#4,<s=int64#2
# asm 2: xor  <e=%rcx,<s=%rsi
xor  %rcx,%rsi

# qhasm: mem64[ input_0 + 144 ] = s
# asm 1: movq   <s=int64#2,144(<input_0=int64#1)
# asm 2: movq   <s=%rsi,144(<input_0=%rdi)
movq   %rsi,144(%rdi)

# qhasm: s = *(uint32 *)( input_0 + 152 )
# asm 1: movl   152(<input_0=int64#1),>s=int64#2d
# asm 2: movl   152(<input_0=%rdi),>s=%esi
movl   152(%rdi),%esi

# qhasm: e = *(uint32 *)( input_2 + 152 )
# asm 1: movl   152(<input_2=int64#3),>e=int64#3d
# asm 2: movl   152(<input_2=%rdx),>e=%edx
movl   152(%rdx),%edx

# qhasm: s ^= e
# asm 1: xor  <e=int64#3,<s=int64#2
# asm 2: xor  <e=%rdx,<s=%rsi
xor  %rdx,%rsi

# qhasm: *(uint32 *)( input_0 + 152 ) = s
# asm 1: movl   <s=int64#2d,152(<input_0=int64#1)
# asm 2: movl   <s=%esi,152(<input_0=%rdi)
movl   %esi,152(%rdi)

# qhasm: return
add %r11,%rsp
ret
