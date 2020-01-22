
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

# qhasm: reg128 p

# qhasm: reg128 e

# qhasm: reg128 s

# qhasm: int64 buf_ptr

# qhasm: stack128 buf

# qhasm: enter syndrome_asm
.p2align 5
.global _PQCLEAN_MCELIECE8192128F_SSE_syndrome_asm
.global PQCLEAN_MCELIECE8192128F_SSE_syndrome_asm
_PQCLEAN_MCELIECE8192128F_SSE_syndrome_asm:
PQCLEAN_MCELIECE8192128F_SSE_syndrome_asm:
mov %rsp,%r11
and $31,%r11
add $32,%r11
sub %r11,%rsp

# qhasm: input_1 += 1357008
# asm 1: add  $1357008,<input_1=int64#2
# asm 2: add  $1357008,<input_1=%rsi
add  $1357008,%rsi

# qhasm: buf_ptr = &buf
# asm 1: leaq <buf=stack128#1,>buf_ptr=int64#4
# asm 2: leaq <buf=0(%rsp),>buf_ptr=%rcx
leaq 0(%rsp),%rcx

# qhasm: row = 1664
# asm 1: mov  $1664,>row=int64#5
# asm 2: mov  $1664,>row=%r8
mov  $1664,%r8

# qhasm: loop:
._loop:

# qhasm: row -= 1
# asm 1: sub  $1,<row=int64#5
# asm 2: sub  $1,<row=%r8
sub  $1,%r8

# qhasm: s = mem128[ input_1 + 0 ]
# asm 1: movdqu   0(<input_1=int64#2),>s=reg128#1
# asm 2: movdqu   0(<input_1=%rsi),>s=%xmm0
movdqu   0(%rsi),%xmm0

# qhasm: e = mem128[ input_2 + 208 ]
# asm 1: movdqu   208(<input_2=int64#3),>e=reg128#2
# asm 2: movdqu   208(<input_2=%rdx),>e=%xmm1
movdqu   208(%rdx),%xmm1

# qhasm: s &= e
# asm 1: pand <e=reg128#2,<s=reg128#1
# asm 2: pand <e=%xmm1,<s=%xmm0
pand %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 16 ]
# asm 1: movdqu   16(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   16(<input_1=%rsi),>p=%xmm1
movdqu   16(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 224 ]
# asm 1: movdqu   224(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   224(<input_2=%rdx),>e=%xmm2
movdqu   224(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 32 ]
# asm 1: movdqu   32(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   32(<input_1=%rsi),>p=%xmm1
movdqu   32(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 240 ]
# asm 1: movdqu   240(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   240(<input_2=%rdx),>e=%xmm2
movdqu   240(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 48 ]
# asm 1: movdqu   48(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   48(<input_1=%rsi),>p=%xmm1
movdqu   48(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 256 ]
# asm 1: movdqu   256(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   256(<input_2=%rdx),>e=%xmm2
movdqu   256(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 64 ]
# asm 1: movdqu   64(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   64(<input_1=%rsi),>p=%xmm1
movdqu   64(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 272 ]
# asm 1: movdqu   272(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   272(<input_2=%rdx),>e=%xmm2
movdqu   272(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 80 ]
# asm 1: movdqu   80(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   80(<input_1=%rsi),>p=%xmm1
movdqu   80(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 288 ]
# asm 1: movdqu   288(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   288(<input_2=%rdx),>e=%xmm2
movdqu   288(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 96 ]
# asm 1: movdqu   96(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   96(<input_1=%rsi),>p=%xmm1
movdqu   96(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 304 ]
# asm 1: movdqu   304(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   304(<input_2=%rdx),>e=%xmm2
movdqu   304(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 112 ]
# asm 1: movdqu   112(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   112(<input_1=%rsi),>p=%xmm1
movdqu   112(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 320 ]
# asm 1: movdqu   320(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   320(<input_2=%rdx),>e=%xmm2
movdqu   320(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 128 ]
# asm 1: movdqu   128(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   128(<input_1=%rsi),>p=%xmm1
movdqu   128(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 336 ]
# asm 1: movdqu   336(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   336(<input_2=%rdx),>e=%xmm2
movdqu   336(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 144 ]
# asm 1: movdqu   144(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   144(<input_1=%rsi),>p=%xmm1
movdqu   144(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 352 ]
# asm 1: movdqu   352(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   352(<input_2=%rdx),>e=%xmm2
movdqu   352(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 160 ]
# asm 1: movdqu   160(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   160(<input_1=%rsi),>p=%xmm1
movdqu   160(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 368 ]
# asm 1: movdqu   368(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   368(<input_2=%rdx),>e=%xmm2
movdqu   368(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 176 ]
# asm 1: movdqu   176(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   176(<input_1=%rsi),>p=%xmm1
movdqu   176(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 384 ]
# asm 1: movdqu   384(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   384(<input_2=%rdx),>e=%xmm2
movdqu   384(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 192 ]
# asm 1: movdqu   192(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   192(<input_1=%rsi),>p=%xmm1
movdqu   192(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 400 ]
# asm 1: movdqu   400(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   400(<input_2=%rdx),>e=%xmm2
movdqu   400(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 208 ]
# asm 1: movdqu   208(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   208(<input_1=%rsi),>p=%xmm1
movdqu   208(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 416 ]
# asm 1: movdqu   416(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   416(<input_2=%rdx),>e=%xmm2
movdqu   416(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 224 ]
# asm 1: movdqu   224(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   224(<input_1=%rsi),>p=%xmm1
movdqu   224(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 432 ]
# asm 1: movdqu   432(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   432(<input_2=%rdx),>e=%xmm2
movdqu   432(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 240 ]
# asm 1: movdqu   240(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   240(<input_1=%rsi),>p=%xmm1
movdqu   240(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 448 ]
# asm 1: movdqu   448(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   448(<input_2=%rdx),>e=%xmm2
movdqu   448(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 256 ]
# asm 1: movdqu   256(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   256(<input_1=%rsi),>p=%xmm1
movdqu   256(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 464 ]
# asm 1: movdqu   464(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   464(<input_2=%rdx),>e=%xmm2
movdqu   464(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 272 ]
# asm 1: movdqu   272(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   272(<input_1=%rsi),>p=%xmm1
movdqu   272(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 480 ]
# asm 1: movdqu   480(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   480(<input_2=%rdx),>e=%xmm2
movdqu   480(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 288 ]
# asm 1: movdqu   288(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   288(<input_1=%rsi),>p=%xmm1
movdqu   288(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 496 ]
# asm 1: movdqu   496(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   496(<input_2=%rdx),>e=%xmm2
movdqu   496(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 304 ]
# asm 1: movdqu   304(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   304(<input_1=%rsi),>p=%xmm1
movdqu   304(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 512 ]
# asm 1: movdqu   512(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   512(<input_2=%rdx),>e=%xmm2
movdqu   512(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 320 ]
# asm 1: movdqu   320(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   320(<input_1=%rsi),>p=%xmm1
movdqu   320(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 528 ]
# asm 1: movdqu   528(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   528(<input_2=%rdx),>e=%xmm2
movdqu   528(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 336 ]
# asm 1: movdqu   336(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   336(<input_1=%rsi),>p=%xmm1
movdqu   336(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 544 ]
# asm 1: movdqu   544(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   544(<input_2=%rdx),>e=%xmm2
movdqu   544(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 352 ]
# asm 1: movdqu   352(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   352(<input_1=%rsi),>p=%xmm1
movdqu   352(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 560 ]
# asm 1: movdqu   560(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   560(<input_2=%rdx),>e=%xmm2
movdqu   560(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 368 ]
# asm 1: movdqu   368(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   368(<input_1=%rsi),>p=%xmm1
movdqu   368(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 576 ]
# asm 1: movdqu   576(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   576(<input_2=%rdx),>e=%xmm2
movdqu   576(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 384 ]
# asm 1: movdqu   384(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   384(<input_1=%rsi),>p=%xmm1
movdqu   384(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 592 ]
# asm 1: movdqu   592(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   592(<input_2=%rdx),>e=%xmm2
movdqu   592(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 400 ]
# asm 1: movdqu   400(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   400(<input_1=%rsi),>p=%xmm1
movdqu   400(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 608 ]
# asm 1: movdqu   608(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   608(<input_2=%rdx),>e=%xmm2
movdqu   608(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 416 ]
# asm 1: movdqu   416(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   416(<input_1=%rsi),>p=%xmm1
movdqu   416(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 624 ]
# asm 1: movdqu   624(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   624(<input_2=%rdx),>e=%xmm2
movdqu   624(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 432 ]
# asm 1: movdqu   432(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   432(<input_1=%rsi),>p=%xmm1
movdqu   432(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 640 ]
# asm 1: movdqu   640(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   640(<input_2=%rdx),>e=%xmm2
movdqu   640(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 448 ]
# asm 1: movdqu   448(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   448(<input_1=%rsi),>p=%xmm1
movdqu   448(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 656 ]
# asm 1: movdqu   656(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   656(<input_2=%rdx),>e=%xmm2
movdqu   656(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 464 ]
# asm 1: movdqu   464(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   464(<input_1=%rsi),>p=%xmm1
movdqu   464(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 672 ]
# asm 1: movdqu   672(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   672(<input_2=%rdx),>e=%xmm2
movdqu   672(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 480 ]
# asm 1: movdqu   480(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   480(<input_1=%rsi),>p=%xmm1
movdqu   480(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 688 ]
# asm 1: movdqu   688(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   688(<input_2=%rdx),>e=%xmm2
movdqu   688(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 496 ]
# asm 1: movdqu   496(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   496(<input_1=%rsi),>p=%xmm1
movdqu   496(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 704 ]
# asm 1: movdqu   704(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   704(<input_2=%rdx),>e=%xmm2
movdqu   704(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 512 ]
# asm 1: movdqu   512(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   512(<input_1=%rsi),>p=%xmm1
movdqu   512(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 720 ]
# asm 1: movdqu   720(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   720(<input_2=%rdx),>e=%xmm2
movdqu   720(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 528 ]
# asm 1: movdqu   528(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   528(<input_1=%rsi),>p=%xmm1
movdqu   528(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 736 ]
# asm 1: movdqu   736(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   736(<input_2=%rdx),>e=%xmm2
movdqu   736(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 544 ]
# asm 1: movdqu   544(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   544(<input_1=%rsi),>p=%xmm1
movdqu   544(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 752 ]
# asm 1: movdqu   752(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   752(<input_2=%rdx),>e=%xmm2
movdqu   752(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 560 ]
# asm 1: movdqu   560(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   560(<input_1=%rsi),>p=%xmm1
movdqu   560(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 768 ]
# asm 1: movdqu   768(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   768(<input_2=%rdx),>e=%xmm2
movdqu   768(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 576 ]
# asm 1: movdqu   576(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   576(<input_1=%rsi),>p=%xmm1
movdqu   576(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 784 ]
# asm 1: movdqu   784(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   784(<input_2=%rdx),>e=%xmm2
movdqu   784(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 592 ]
# asm 1: movdqu   592(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   592(<input_1=%rsi),>p=%xmm1
movdqu   592(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 800 ]
# asm 1: movdqu   800(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   800(<input_2=%rdx),>e=%xmm2
movdqu   800(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 608 ]
# asm 1: movdqu   608(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   608(<input_1=%rsi),>p=%xmm1
movdqu   608(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 816 ]
# asm 1: movdqu   816(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   816(<input_2=%rdx),>e=%xmm2
movdqu   816(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 624 ]
# asm 1: movdqu   624(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   624(<input_1=%rsi),>p=%xmm1
movdqu   624(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 832 ]
# asm 1: movdqu   832(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   832(<input_2=%rdx),>e=%xmm2
movdqu   832(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 640 ]
# asm 1: movdqu   640(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   640(<input_1=%rsi),>p=%xmm1
movdqu   640(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 848 ]
# asm 1: movdqu   848(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   848(<input_2=%rdx),>e=%xmm2
movdqu   848(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 656 ]
# asm 1: movdqu   656(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   656(<input_1=%rsi),>p=%xmm1
movdqu   656(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 864 ]
# asm 1: movdqu   864(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   864(<input_2=%rdx),>e=%xmm2
movdqu   864(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 672 ]
# asm 1: movdqu   672(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   672(<input_1=%rsi),>p=%xmm1
movdqu   672(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 880 ]
# asm 1: movdqu   880(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   880(<input_2=%rdx),>e=%xmm2
movdqu   880(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 688 ]
# asm 1: movdqu   688(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   688(<input_1=%rsi),>p=%xmm1
movdqu   688(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 896 ]
# asm 1: movdqu   896(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   896(<input_2=%rdx),>e=%xmm2
movdqu   896(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 704 ]
# asm 1: movdqu   704(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   704(<input_1=%rsi),>p=%xmm1
movdqu   704(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 912 ]
# asm 1: movdqu   912(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   912(<input_2=%rdx),>e=%xmm2
movdqu   912(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 720 ]
# asm 1: movdqu   720(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   720(<input_1=%rsi),>p=%xmm1
movdqu   720(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 928 ]
# asm 1: movdqu   928(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   928(<input_2=%rdx),>e=%xmm2
movdqu   928(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 736 ]
# asm 1: movdqu   736(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   736(<input_1=%rsi),>p=%xmm1
movdqu   736(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 944 ]
# asm 1: movdqu   944(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   944(<input_2=%rdx),>e=%xmm2
movdqu   944(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 752 ]
# asm 1: movdqu   752(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   752(<input_1=%rsi),>p=%xmm1
movdqu   752(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 960 ]
# asm 1: movdqu   960(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   960(<input_2=%rdx),>e=%xmm2
movdqu   960(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 768 ]
# asm 1: movdqu   768(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   768(<input_1=%rsi),>p=%xmm1
movdqu   768(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 976 ]
# asm 1: movdqu   976(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   976(<input_2=%rdx),>e=%xmm2
movdqu   976(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 784 ]
# asm 1: movdqu   784(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   784(<input_1=%rsi),>p=%xmm1
movdqu   784(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 992 ]
# asm 1: movdqu   992(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   992(<input_2=%rdx),>e=%xmm2
movdqu   992(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: p = mem128[ input_1 + 800 ]
# asm 1: movdqu   800(<input_1=int64#2),>p=reg128#2
# asm 2: movdqu   800(<input_1=%rsi),>p=%xmm1
movdqu   800(%rsi),%xmm1

# qhasm: e = mem128[ input_2 + 1008 ]
# asm 1: movdqu   1008(<input_2=int64#3),>e=reg128#3
# asm 2: movdqu   1008(<input_2=%rdx),>e=%xmm2
movdqu   1008(%rdx),%xmm2

# qhasm: p &= e
# asm 1: pand <e=reg128#3,<p=reg128#2
# asm 2: pand <e=%xmm2,<p=%xmm1
pand %xmm2,%xmm1

# qhasm: s ^= p
# asm 1: pxor <p=reg128#2,<s=reg128#1
# asm 2: pxor <p=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: buf = s
# asm 1: movdqa <s=reg128#1,>buf=stack128#1
# asm 2: movdqa <s=%xmm0,>buf=0(%rsp)
movdqa %xmm0,0(%rsp)

# qhasm: b64 = mem64[ buf_ptr + 0 ]
# asm 1: movq   0(<buf_ptr=int64#4),>b64=int64#6
# asm 2: movq   0(<buf_ptr=%rcx),>b64=%r9
movq   0(%rcx),%r9

# qhasm: c_all = count(b64)
# asm 1: popcnt <b64=int64#6, >c_all=int64#6
# asm 2: popcnt <b64=%r9, >c_all=%r9
popcnt %r9, %r9

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

# qhasm: input_1 -= 816
# asm 1: sub  $816,<input_1=int64#2
# asm 2: sub  $816,<input_1=%rsi
sub  $816,%rsi

# qhasm: =? row-0
# asm 1: cmp  $0,<row=int64#5
# asm 2: cmp  $0,<row=%r8
cmp  $0,%r8
# comment:fp stack unchanged by jump

# qhasm: goto loop if !=
jne ._loop

# qhasm: s = mem128[ input_0 + 0 ]
# asm 1: movdqu   0(<input_0=int64#1),>s=reg128#1
# asm 2: movdqu   0(<input_0=%rdi),>s=%xmm0
movdqu   0(%rdi),%xmm0

# qhasm: e = mem128[ input_2 + 0 ]
# asm 1: movdqu   0(<input_2=int64#3),>e=reg128#2
# asm 2: movdqu   0(<input_2=%rdx),>e=%xmm1
movdqu   0(%rdx),%xmm1

# qhasm: s ^= e
# asm 1: pxor <e=reg128#2,<s=reg128#1
# asm 2: pxor <e=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: mem128[ input_0 + 0 ] = s
# asm 1: movdqu   <s=reg128#1,0(<input_0=int64#1)
# asm 2: movdqu   <s=%xmm0,0(<input_0=%rdi)
movdqu   %xmm0,0(%rdi)

# qhasm: s = mem128[ input_0 + 16 ]
# asm 1: movdqu   16(<input_0=int64#1),>s=reg128#1
# asm 2: movdqu   16(<input_0=%rdi),>s=%xmm0
movdqu   16(%rdi),%xmm0

# qhasm: e = mem128[ input_2 + 16 ]
# asm 1: movdqu   16(<input_2=int64#3),>e=reg128#2
# asm 2: movdqu   16(<input_2=%rdx),>e=%xmm1
movdqu   16(%rdx),%xmm1

# qhasm: s ^= e
# asm 1: pxor <e=reg128#2,<s=reg128#1
# asm 2: pxor <e=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: mem128[ input_0 + 16 ] = s
# asm 1: movdqu   <s=reg128#1,16(<input_0=int64#1)
# asm 2: movdqu   <s=%xmm0,16(<input_0=%rdi)
movdqu   %xmm0,16(%rdi)

# qhasm: s = mem128[ input_0 + 32 ]
# asm 1: movdqu   32(<input_0=int64#1),>s=reg128#1
# asm 2: movdqu   32(<input_0=%rdi),>s=%xmm0
movdqu   32(%rdi),%xmm0

# qhasm: e = mem128[ input_2 + 32 ]
# asm 1: movdqu   32(<input_2=int64#3),>e=reg128#2
# asm 2: movdqu   32(<input_2=%rdx),>e=%xmm1
movdqu   32(%rdx),%xmm1

# qhasm: s ^= e
# asm 1: pxor <e=reg128#2,<s=reg128#1
# asm 2: pxor <e=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: mem128[ input_0 + 32 ] = s
# asm 1: movdqu   <s=reg128#1,32(<input_0=int64#1)
# asm 2: movdqu   <s=%xmm0,32(<input_0=%rdi)
movdqu   %xmm0,32(%rdi)

# qhasm: s = mem128[ input_0 + 48 ]
# asm 1: movdqu   48(<input_0=int64#1),>s=reg128#1
# asm 2: movdqu   48(<input_0=%rdi),>s=%xmm0
movdqu   48(%rdi),%xmm0

# qhasm: e = mem128[ input_2 + 48 ]
# asm 1: movdqu   48(<input_2=int64#3),>e=reg128#2
# asm 2: movdqu   48(<input_2=%rdx),>e=%xmm1
movdqu   48(%rdx),%xmm1

# qhasm: s ^= e
# asm 1: pxor <e=reg128#2,<s=reg128#1
# asm 2: pxor <e=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: mem128[ input_0 + 48 ] = s
# asm 1: movdqu   <s=reg128#1,48(<input_0=int64#1)
# asm 2: movdqu   <s=%xmm0,48(<input_0=%rdi)
movdqu   %xmm0,48(%rdi)

# qhasm: s = mem128[ input_0 + 64 ]
# asm 1: movdqu   64(<input_0=int64#1),>s=reg128#1
# asm 2: movdqu   64(<input_0=%rdi),>s=%xmm0
movdqu   64(%rdi),%xmm0

# qhasm: e = mem128[ input_2 + 64 ]
# asm 1: movdqu   64(<input_2=int64#3),>e=reg128#2
# asm 2: movdqu   64(<input_2=%rdx),>e=%xmm1
movdqu   64(%rdx),%xmm1

# qhasm: s ^= e
# asm 1: pxor <e=reg128#2,<s=reg128#1
# asm 2: pxor <e=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: mem128[ input_0 + 64 ] = s
# asm 1: movdqu   <s=reg128#1,64(<input_0=int64#1)
# asm 2: movdqu   <s=%xmm0,64(<input_0=%rdi)
movdqu   %xmm0,64(%rdi)

# qhasm: s = mem128[ input_0 + 80 ]
# asm 1: movdqu   80(<input_0=int64#1),>s=reg128#1
# asm 2: movdqu   80(<input_0=%rdi),>s=%xmm0
movdqu   80(%rdi),%xmm0

# qhasm: e = mem128[ input_2 + 80 ]
# asm 1: movdqu   80(<input_2=int64#3),>e=reg128#2
# asm 2: movdqu   80(<input_2=%rdx),>e=%xmm1
movdqu   80(%rdx),%xmm1

# qhasm: s ^= e
# asm 1: pxor <e=reg128#2,<s=reg128#1
# asm 2: pxor <e=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: mem128[ input_0 + 80 ] = s
# asm 1: movdqu   <s=reg128#1,80(<input_0=int64#1)
# asm 2: movdqu   <s=%xmm0,80(<input_0=%rdi)
movdqu   %xmm0,80(%rdi)

# qhasm: s = mem128[ input_0 + 96 ]
# asm 1: movdqu   96(<input_0=int64#1),>s=reg128#1
# asm 2: movdqu   96(<input_0=%rdi),>s=%xmm0
movdqu   96(%rdi),%xmm0

# qhasm: e = mem128[ input_2 + 96 ]
# asm 1: movdqu   96(<input_2=int64#3),>e=reg128#2
# asm 2: movdqu   96(<input_2=%rdx),>e=%xmm1
movdqu   96(%rdx),%xmm1

# qhasm: s ^= e
# asm 1: pxor <e=reg128#2,<s=reg128#1
# asm 2: pxor <e=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: mem128[ input_0 + 96 ] = s
# asm 1: movdqu   <s=reg128#1,96(<input_0=int64#1)
# asm 2: movdqu   <s=%xmm0,96(<input_0=%rdi)
movdqu   %xmm0,96(%rdi)

# qhasm: s = mem128[ input_0 + 112 ]
# asm 1: movdqu   112(<input_0=int64#1),>s=reg128#1
# asm 2: movdqu   112(<input_0=%rdi),>s=%xmm0
movdqu   112(%rdi),%xmm0

# qhasm: e = mem128[ input_2 + 112 ]
# asm 1: movdqu   112(<input_2=int64#3),>e=reg128#2
# asm 2: movdqu   112(<input_2=%rdx),>e=%xmm1
movdqu   112(%rdx),%xmm1

# qhasm: s ^= e
# asm 1: pxor <e=reg128#2,<s=reg128#1
# asm 2: pxor <e=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: mem128[ input_0 + 112 ] = s
# asm 1: movdqu   <s=reg128#1,112(<input_0=int64#1)
# asm 2: movdqu   <s=%xmm0,112(<input_0=%rdi)
movdqu   %xmm0,112(%rdi)

# qhasm: s = mem128[ input_0 + 128 ]
# asm 1: movdqu   128(<input_0=int64#1),>s=reg128#1
# asm 2: movdqu   128(<input_0=%rdi),>s=%xmm0
movdqu   128(%rdi),%xmm0

# qhasm: e = mem128[ input_2 + 128 ]
# asm 1: movdqu   128(<input_2=int64#3),>e=reg128#2
# asm 2: movdqu   128(<input_2=%rdx),>e=%xmm1
movdqu   128(%rdx),%xmm1

# qhasm: s ^= e
# asm 1: pxor <e=reg128#2,<s=reg128#1
# asm 2: pxor <e=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: mem128[ input_0 + 128 ] = s
# asm 1: movdqu   <s=reg128#1,128(<input_0=int64#1)
# asm 2: movdqu   <s=%xmm0,128(<input_0=%rdi)
movdqu   %xmm0,128(%rdi)

# qhasm: s = mem128[ input_0 + 144 ]
# asm 1: movdqu   144(<input_0=int64#1),>s=reg128#1
# asm 2: movdqu   144(<input_0=%rdi),>s=%xmm0
movdqu   144(%rdi),%xmm0

# qhasm: e = mem128[ input_2 + 144 ]
# asm 1: movdqu   144(<input_2=int64#3),>e=reg128#2
# asm 2: movdqu   144(<input_2=%rdx),>e=%xmm1
movdqu   144(%rdx),%xmm1

# qhasm: s ^= e
# asm 1: pxor <e=reg128#2,<s=reg128#1
# asm 2: pxor <e=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: mem128[ input_0 + 144 ] = s
# asm 1: movdqu   <s=reg128#1,144(<input_0=int64#1)
# asm 2: movdqu   <s=%xmm0,144(<input_0=%rdi)
movdqu   %xmm0,144(%rdi)

# qhasm: s = mem128[ input_0 + 160 ]
# asm 1: movdqu   160(<input_0=int64#1),>s=reg128#1
# asm 2: movdqu   160(<input_0=%rdi),>s=%xmm0
movdqu   160(%rdi),%xmm0

# qhasm: e = mem128[ input_2 + 160 ]
# asm 1: movdqu   160(<input_2=int64#3),>e=reg128#2
# asm 2: movdqu   160(<input_2=%rdx),>e=%xmm1
movdqu   160(%rdx),%xmm1

# qhasm: s ^= e
# asm 1: pxor <e=reg128#2,<s=reg128#1
# asm 2: pxor <e=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: mem128[ input_0 + 160 ] = s
# asm 1: movdqu   <s=reg128#1,160(<input_0=int64#1)
# asm 2: movdqu   <s=%xmm0,160(<input_0=%rdi)
movdqu   %xmm0,160(%rdi)

# qhasm: s = mem128[ input_0 + 176 ]
# asm 1: movdqu   176(<input_0=int64#1),>s=reg128#1
# asm 2: movdqu   176(<input_0=%rdi),>s=%xmm0
movdqu   176(%rdi),%xmm0

# qhasm: e = mem128[ input_2 + 176 ]
# asm 1: movdqu   176(<input_2=int64#3),>e=reg128#2
# asm 2: movdqu   176(<input_2=%rdx),>e=%xmm1
movdqu   176(%rdx),%xmm1

# qhasm: s ^= e
# asm 1: pxor <e=reg128#2,<s=reg128#1
# asm 2: pxor <e=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: mem128[ input_0 + 176 ] = s
# asm 1: movdqu   <s=reg128#1,176(<input_0=int64#1)
# asm 2: movdqu   <s=%xmm0,176(<input_0=%rdi)
movdqu   %xmm0,176(%rdi)

# qhasm: s = mem128[ input_0 + 192 ]
# asm 1: movdqu   192(<input_0=int64#1),>s=reg128#1
# asm 2: movdqu   192(<input_0=%rdi),>s=%xmm0
movdqu   192(%rdi),%xmm0

# qhasm: e = mem128[ input_2 + 192 ]
# asm 1: movdqu   192(<input_2=int64#3),>e=reg128#2
# asm 2: movdqu   192(<input_2=%rdx),>e=%xmm1
movdqu   192(%rdx),%xmm1

# qhasm: s ^= e
# asm 1: pxor <e=reg128#2,<s=reg128#1
# asm 2: pxor <e=%xmm1,<s=%xmm0
pxor %xmm1,%xmm0

# qhasm: mem128[ input_0 + 192 ] = s
# asm 1: movdqu   <s=reg128#1,192(<input_0=int64#1)
# asm 2: movdqu   <s=%xmm0,192(<input_0=%rdi)
movdqu   %xmm0,192(%rdi)

# qhasm: return
add %r11,%rsp
ret
