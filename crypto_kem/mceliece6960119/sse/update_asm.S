
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

# qhasm: int64 s0

# qhasm: int64 s1

# qhasm: int64 s2

# qhasm: enter update_asm
.p2align 5
.global _PQCLEAN_MCELIECE6960119_SSE_update_asm
.global PQCLEAN_MCELIECE6960119_SSE_update_asm
_PQCLEAN_MCELIECE6960119_SSE_update_asm:
PQCLEAN_MCELIECE6960119_SSE_update_asm:
mov %rsp,%r11
and $31,%r11
add $0,%r11
sub %r11,%rsp

# qhasm: s2 = input_1
# asm 1: mov  <input_1=int64#2,>s2=int64#2
# asm 2: mov  <input_1=%rsi,>s2=%rsi
mov  %rsi,%rsi

# qhasm: s0 = mem64[ input_0 + 0 ]
# asm 1: movq   0(<input_0=int64#1),>s0=int64#3
# asm 2: movq   0(<input_0=%rdi),>s0=%rdx
movq   0(%rdi),%rdx

# qhasm: s1 = mem64[ input_0 + 8 ]
# asm 1: movq   8(<input_0=int64#1),>s1=int64#4
# asm 2: movq   8(<input_0=%rdi),>s1=%rcx
movq   8(%rdi),%rcx

# qhasm: s0 = (s1 s0) >> 1
# asm 1: shrd $1,<s1=int64#4,<s0=int64#3
# asm 2: shrd $1,<s1=%rcx,<s0=%rdx
shrd $1,%rcx,%rdx

# qhasm: s1 = (s2 s1) >> 1
# asm 1: shrd $1,<s2=int64#2,<s1=int64#4
# asm 2: shrd $1,<s2=%rsi,<s1=%rcx
shrd $1,%rsi,%rcx

# qhasm: (uint64) s2 >>= 1
# asm 1: shr  $1,<s2=int64#2
# asm 2: shr  $1,<s2=%rsi
shr  $1,%rsi

# qhasm: mem64[ input_0 + 0 ] = s0
# asm 1: movq   <s0=int64#3,0(<input_0=int64#1)
# asm 2: movq   <s0=%rdx,0(<input_0=%rdi)
movq   %rdx,0(%rdi)

# qhasm: mem64[ input_0 + 8 ] = s1
# asm 1: movq   <s1=int64#4,8(<input_0=int64#1)
# asm 2: movq   <s1=%rcx,8(<input_0=%rdi)
movq   %rcx,8(%rdi)

# qhasm: s0 = mem64[ input_0 + 16 ]
# asm 1: movq   16(<input_0=int64#1),>s0=int64#3
# asm 2: movq   16(<input_0=%rdi),>s0=%rdx
movq   16(%rdi),%rdx

# qhasm: s1 = mem64[ input_0 + 24 ]
# asm 1: movq   24(<input_0=int64#1),>s1=int64#4
# asm 2: movq   24(<input_0=%rdi),>s1=%rcx
movq   24(%rdi),%rcx

# qhasm: s0 = (s1 s0) >> 1
# asm 1: shrd $1,<s1=int64#4,<s0=int64#3
# asm 2: shrd $1,<s1=%rcx,<s0=%rdx
shrd $1,%rcx,%rdx

# qhasm: s1 = (s2 s1) >> 1
# asm 1: shrd $1,<s2=int64#2,<s1=int64#4
# asm 2: shrd $1,<s2=%rsi,<s1=%rcx
shrd $1,%rsi,%rcx

# qhasm: (uint64) s2 >>= 1
# asm 1: shr  $1,<s2=int64#2
# asm 2: shr  $1,<s2=%rsi
shr  $1,%rsi

# qhasm: mem64[ input_0 + 16 ] = s0
# asm 1: movq   <s0=int64#3,16(<input_0=int64#1)
# asm 2: movq   <s0=%rdx,16(<input_0=%rdi)
movq   %rdx,16(%rdi)

# qhasm: mem64[ input_0 + 24 ] = s1
# asm 1: movq   <s1=int64#4,24(<input_0=int64#1)
# asm 2: movq   <s1=%rcx,24(<input_0=%rdi)
movq   %rcx,24(%rdi)

# qhasm: s0 = mem64[ input_0 + 32 ]
# asm 1: movq   32(<input_0=int64#1),>s0=int64#3
# asm 2: movq   32(<input_0=%rdi),>s0=%rdx
movq   32(%rdi),%rdx

# qhasm: s1 = mem64[ input_0 + 40 ]
# asm 1: movq   40(<input_0=int64#1),>s1=int64#4
# asm 2: movq   40(<input_0=%rdi),>s1=%rcx
movq   40(%rdi),%rcx

# qhasm: s0 = (s1 s0) >> 1
# asm 1: shrd $1,<s1=int64#4,<s0=int64#3
# asm 2: shrd $1,<s1=%rcx,<s0=%rdx
shrd $1,%rcx,%rdx

# qhasm: s1 = (s2 s1) >> 1
# asm 1: shrd $1,<s2=int64#2,<s1=int64#4
# asm 2: shrd $1,<s2=%rsi,<s1=%rcx
shrd $1,%rsi,%rcx

# qhasm: (uint64) s2 >>= 1
# asm 1: shr  $1,<s2=int64#2
# asm 2: shr  $1,<s2=%rsi
shr  $1,%rsi

# qhasm: mem64[ input_0 + 32 ] = s0
# asm 1: movq   <s0=int64#3,32(<input_0=int64#1)
# asm 2: movq   <s0=%rdx,32(<input_0=%rdi)
movq   %rdx,32(%rdi)

# qhasm: mem64[ input_0 + 40 ] = s1
# asm 1: movq   <s1=int64#4,40(<input_0=int64#1)
# asm 2: movq   <s1=%rcx,40(<input_0=%rdi)
movq   %rcx,40(%rdi)

# qhasm: s0 = mem64[ input_0 + 48 ]
# asm 1: movq   48(<input_0=int64#1),>s0=int64#3
# asm 2: movq   48(<input_0=%rdi),>s0=%rdx
movq   48(%rdi),%rdx

# qhasm: s1 = mem64[ input_0 + 56 ]
# asm 1: movq   56(<input_0=int64#1),>s1=int64#4
# asm 2: movq   56(<input_0=%rdi),>s1=%rcx
movq   56(%rdi),%rcx

# qhasm: s0 = (s1 s0) >> 1
# asm 1: shrd $1,<s1=int64#4,<s0=int64#3
# asm 2: shrd $1,<s1=%rcx,<s0=%rdx
shrd $1,%rcx,%rdx

# qhasm: s1 = (s2 s1) >> 1
# asm 1: shrd $1,<s2=int64#2,<s1=int64#4
# asm 2: shrd $1,<s2=%rsi,<s1=%rcx
shrd $1,%rsi,%rcx

# qhasm: (uint64) s2 >>= 1
# asm 1: shr  $1,<s2=int64#2
# asm 2: shr  $1,<s2=%rsi
shr  $1,%rsi

# qhasm: mem64[ input_0 + 48 ] = s0
# asm 1: movq   <s0=int64#3,48(<input_0=int64#1)
# asm 2: movq   <s0=%rdx,48(<input_0=%rdi)
movq   %rdx,48(%rdi)

# qhasm: mem64[ input_0 + 56 ] = s1
# asm 1: movq   <s1=int64#4,56(<input_0=int64#1)
# asm 2: movq   <s1=%rcx,56(<input_0=%rdi)
movq   %rcx,56(%rdi)

# qhasm: s0 = mem64[ input_0 + 64 ]
# asm 1: movq   64(<input_0=int64#1),>s0=int64#3
# asm 2: movq   64(<input_0=%rdi),>s0=%rdx
movq   64(%rdi),%rdx

# qhasm: s1 = mem64[ input_0 + 72 ]
# asm 1: movq   72(<input_0=int64#1),>s1=int64#4
# asm 2: movq   72(<input_0=%rdi),>s1=%rcx
movq   72(%rdi),%rcx

# qhasm: s0 = (s1 s0) >> 1
# asm 1: shrd $1,<s1=int64#4,<s0=int64#3
# asm 2: shrd $1,<s1=%rcx,<s0=%rdx
shrd $1,%rcx,%rdx

# qhasm: s1 = (s2 s1) >> 1
# asm 1: shrd $1,<s2=int64#2,<s1=int64#4
# asm 2: shrd $1,<s2=%rsi,<s1=%rcx
shrd $1,%rsi,%rcx

# qhasm: (uint64) s2 >>= 1
# asm 1: shr  $1,<s2=int64#2
# asm 2: shr  $1,<s2=%rsi
shr  $1,%rsi

# qhasm: mem64[ input_0 + 64 ] = s0
# asm 1: movq   <s0=int64#3,64(<input_0=int64#1)
# asm 2: movq   <s0=%rdx,64(<input_0=%rdi)
movq   %rdx,64(%rdi)

# qhasm: mem64[ input_0 + 72 ] = s1
# asm 1: movq   <s1=int64#4,72(<input_0=int64#1)
# asm 2: movq   <s1=%rcx,72(<input_0=%rdi)
movq   %rcx,72(%rdi)

# qhasm: s0 = mem64[ input_0 + 80 ]
# asm 1: movq   80(<input_0=int64#1),>s0=int64#3
# asm 2: movq   80(<input_0=%rdi),>s0=%rdx
movq   80(%rdi),%rdx

# qhasm: s1 = mem64[ input_0 + 88 ]
# asm 1: movq   88(<input_0=int64#1),>s1=int64#4
# asm 2: movq   88(<input_0=%rdi),>s1=%rcx
movq   88(%rdi),%rcx

# qhasm: s0 = (s1 s0) >> 1
# asm 1: shrd $1,<s1=int64#4,<s0=int64#3
# asm 2: shrd $1,<s1=%rcx,<s0=%rdx
shrd $1,%rcx,%rdx

# qhasm: s1 = (s2 s1) >> 1
# asm 1: shrd $1,<s2=int64#2,<s1=int64#4
# asm 2: shrd $1,<s2=%rsi,<s1=%rcx
shrd $1,%rsi,%rcx

# qhasm: (uint64) s2 >>= 1
# asm 1: shr  $1,<s2=int64#2
# asm 2: shr  $1,<s2=%rsi
shr  $1,%rsi

# qhasm: mem64[ input_0 + 80 ] = s0
# asm 1: movq   <s0=int64#3,80(<input_0=int64#1)
# asm 2: movq   <s0=%rdx,80(<input_0=%rdi)
movq   %rdx,80(%rdi)

# qhasm: mem64[ input_0 + 88 ] = s1
# asm 1: movq   <s1=int64#4,88(<input_0=int64#1)
# asm 2: movq   <s1=%rcx,88(<input_0=%rdi)
movq   %rcx,88(%rdi)

# qhasm: s0 = mem64[ input_0 + 96 ]
# asm 1: movq   96(<input_0=int64#1),>s0=int64#3
# asm 2: movq   96(<input_0=%rdi),>s0=%rdx
movq   96(%rdi),%rdx

# qhasm: s1 = mem64[ input_0 + 104 ]
# asm 1: movq   104(<input_0=int64#1),>s1=int64#4
# asm 2: movq   104(<input_0=%rdi),>s1=%rcx
movq   104(%rdi),%rcx

# qhasm: s0 = (s1 s0) >> 1
# asm 1: shrd $1,<s1=int64#4,<s0=int64#3
# asm 2: shrd $1,<s1=%rcx,<s0=%rdx
shrd $1,%rcx,%rdx

# qhasm: s1 = (s2 s1) >> 1
# asm 1: shrd $1,<s2=int64#2,<s1=int64#4
# asm 2: shrd $1,<s2=%rsi,<s1=%rcx
shrd $1,%rsi,%rcx

# qhasm: (uint64) s2 >>= 1
# asm 1: shr  $1,<s2=int64#2
# asm 2: shr  $1,<s2=%rsi
shr  $1,%rsi

# qhasm: mem64[ input_0 + 96 ] = s0
# asm 1: movq   <s0=int64#3,96(<input_0=int64#1)
# asm 2: movq   <s0=%rdx,96(<input_0=%rdi)
movq   %rdx,96(%rdi)

# qhasm: mem64[ input_0 + 104 ] = s1
# asm 1: movq   <s1=int64#4,104(<input_0=int64#1)
# asm 2: movq   <s1=%rcx,104(<input_0=%rdi)
movq   %rcx,104(%rdi)

# qhasm: s0 = mem64[ input_0 + 112 ]
# asm 1: movq   112(<input_0=int64#1),>s0=int64#3
# asm 2: movq   112(<input_0=%rdi),>s0=%rdx
movq   112(%rdi),%rdx

# qhasm: s1 = mem64[ input_0 + 120 ]
# asm 1: movq   120(<input_0=int64#1),>s1=int64#4
# asm 2: movq   120(<input_0=%rdi),>s1=%rcx
movq   120(%rdi),%rcx

# qhasm: s0 = (s1 s0) >> 1
# asm 1: shrd $1,<s1=int64#4,<s0=int64#3
# asm 2: shrd $1,<s1=%rcx,<s0=%rdx
shrd $1,%rcx,%rdx

# qhasm: s1 = (s2 s1) >> 1
# asm 1: shrd $1,<s2=int64#2,<s1=int64#4
# asm 2: shrd $1,<s2=%rsi,<s1=%rcx
shrd $1,%rsi,%rcx

# qhasm: (uint64) s2 >>= 1
# asm 1: shr  $1,<s2=int64#2
# asm 2: shr  $1,<s2=%rsi
shr  $1,%rsi

# qhasm: mem64[ input_0 + 112 ] = s0
# asm 1: movq   <s0=int64#3,112(<input_0=int64#1)
# asm 2: movq   <s0=%rdx,112(<input_0=%rdi)
movq   %rdx,112(%rdi)

# qhasm: mem64[ input_0 + 120 ] = s1
# asm 1: movq   <s1=int64#4,120(<input_0=int64#1)
# asm 2: movq   <s1=%rcx,120(<input_0=%rdi)
movq   %rcx,120(%rdi)

# qhasm: s0 = mem64[ input_0 + 128 ]
# asm 1: movq   128(<input_0=int64#1),>s0=int64#3
# asm 2: movq   128(<input_0=%rdi),>s0=%rdx
movq   128(%rdi),%rdx

# qhasm: s1 = mem64[ input_0 + 136 ]
# asm 1: movq   136(<input_0=int64#1),>s1=int64#4
# asm 2: movq   136(<input_0=%rdi),>s1=%rcx
movq   136(%rdi),%rcx

# qhasm: s0 = (s1 s0) >> 1
# asm 1: shrd $1,<s1=int64#4,<s0=int64#3
# asm 2: shrd $1,<s1=%rcx,<s0=%rdx
shrd $1,%rcx,%rdx

# qhasm: s1 = (s2 s1) >> 1
# asm 1: shrd $1,<s2=int64#2,<s1=int64#4
# asm 2: shrd $1,<s2=%rsi,<s1=%rcx
shrd $1,%rsi,%rcx

# qhasm: (uint64) s2 >>= 1
# asm 1: shr  $1,<s2=int64#2
# asm 2: shr  $1,<s2=%rsi
shr  $1,%rsi

# qhasm: mem64[ input_0 + 128 ] = s0
# asm 1: movq   <s0=int64#3,128(<input_0=int64#1)
# asm 2: movq   <s0=%rdx,128(<input_0=%rdi)
movq   %rdx,128(%rdi)

# qhasm: mem64[ input_0 + 136 ] = s1
# asm 1: movq   <s1=int64#4,136(<input_0=int64#1)
# asm 2: movq   <s1=%rcx,136(<input_0=%rdi)
movq   %rcx,136(%rdi)

# qhasm: s0 = mem64[ input_0 + 144 ]
# asm 1: movq   144(<input_0=int64#1),>s0=int64#3
# asm 2: movq   144(<input_0=%rdi),>s0=%rdx
movq   144(%rdi),%rdx

# qhasm: s1 = mem64[ input_0 + 152 ]
# asm 1: movq   152(<input_0=int64#1),>s1=int64#4
# asm 2: movq   152(<input_0=%rdi),>s1=%rcx
movq   152(%rdi),%rcx

# qhasm: s0 = (s1 s0) >> 1
# asm 1: shrd $1,<s1=int64#4,<s0=int64#3
# asm 2: shrd $1,<s1=%rcx,<s0=%rdx
shrd $1,%rcx,%rdx

# qhasm: s1 = (s2 s1) >> 1
# asm 1: shrd $1,<s2=int64#2,<s1=int64#4
# asm 2: shrd $1,<s2=%rsi,<s1=%rcx
shrd $1,%rsi,%rcx

# qhasm: (uint64) s2 >>= 1
# asm 1: shr  $1,<s2=int64#2
# asm 2: shr  $1,<s2=%rsi
shr  $1,%rsi

# qhasm: mem64[ input_0 + 144 ] = s0
# asm 1: movq   <s0=int64#3,144(<input_0=int64#1)
# asm 2: movq   <s0=%rdx,144(<input_0=%rdi)
movq   %rdx,144(%rdi)

# qhasm: mem64[ input_0 + 152 ] = s1
# asm 1: movq   <s1=int64#4,152(<input_0=int64#1)
# asm 2: movq   <s1=%rcx,152(<input_0=%rdi)
movq   %rcx,152(%rdi)

# qhasm: s0 = mem64[ input_0 + 160 ]
# asm 1: movq   160(<input_0=int64#1),>s0=int64#3
# asm 2: movq   160(<input_0=%rdi),>s0=%rdx
movq   160(%rdi),%rdx

# qhasm: s1 = mem64[ input_0 + 168 ]
# asm 1: movq   168(<input_0=int64#1),>s1=int64#4
# asm 2: movq   168(<input_0=%rdi),>s1=%rcx
movq   168(%rdi),%rcx

# qhasm: s0 = (s1 s0) >> 1
# asm 1: shrd $1,<s1=int64#4,<s0=int64#3
# asm 2: shrd $1,<s1=%rcx,<s0=%rdx
shrd $1,%rcx,%rdx

# qhasm: s1 = (s2 s1) >> 1
# asm 1: shrd $1,<s2=int64#2,<s1=int64#4
# asm 2: shrd $1,<s2=%rsi,<s1=%rcx
shrd $1,%rsi,%rcx

# qhasm: (uint64) s2 >>= 1
# asm 1: shr  $1,<s2=int64#2
# asm 2: shr  $1,<s2=%rsi
shr  $1,%rsi

# qhasm: mem64[ input_0 + 160 ] = s0
# asm 1: movq   <s0=int64#3,160(<input_0=int64#1)
# asm 2: movq   <s0=%rdx,160(<input_0=%rdi)
movq   %rdx,160(%rdi)

# qhasm: mem64[ input_0 + 168 ] = s1
# asm 1: movq   <s1=int64#4,168(<input_0=int64#1)
# asm 2: movq   <s1=%rcx,168(<input_0=%rdi)
movq   %rcx,168(%rdi)

# qhasm: s0 = mem64[ input_0 + 176 ]
# asm 1: movq   176(<input_0=int64#1),>s0=int64#3
# asm 2: movq   176(<input_0=%rdi),>s0=%rdx
movq   176(%rdi),%rdx

# qhasm: s1 = mem64[ input_0 + 184 ]
# asm 1: movq   184(<input_0=int64#1),>s1=int64#4
# asm 2: movq   184(<input_0=%rdi),>s1=%rcx
movq   184(%rdi),%rcx

# qhasm: s0 = (s1 s0) >> 1
# asm 1: shrd $1,<s1=int64#4,<s0=int64#3
# asm 2: shrd $1,<s1=%rcx,<s0=%rdx
shrd $1,%rcx,%rdx

# qhasm: s1 = (s2 s1) >> 1
# asm 1: shrd $1,<s2=int64#2,<s1=int64#4
# asm 2: shrd $1,<s2=%rsi,<s1=%rcx
shrd $1,%rsi,%rcx

# qhasm: (uint64) s2 >>= 1
# asm 1: shr  $1,<s2=int64#2
# asm 2: shr  $1,<s2=%rsi
shr  $1,%rsi

# qhasm: mem64[ input_0 + 176 ] = s0
# asm 1: movq   <s0=int64#3,176(<input_0=int64#1)
# asm 2: movq   <s0=%rdx,176(<input_0=%rdi)
movq   %rdx,176(%rdi)

# qhasm: mem64[ input_0 + 184 ] = s1
# asm 1: movq   <s1=int64#4,184(<input_0=int64#1)
# asm 2: movq   <s1=%rcx,184(<input_0=%rdi)
movq   %rcx,184(%rdi)

# qhasm: s0 = mem64[ input_0 + 192 ]
# asm 1: movq   192(<input_0=int64#1),>s0=int64#3
# asm 2: movq   192(<input_0=%rdi),>s0=%rdx
movq   192(%rdi),%rdx

# qhasm: s1 = mem64[ input_0 + 200 ]
# asm 1: movq   200(<input_0=int64#1),>s1=int64#4
# asm 2: movq   200(<input_0=%rdi),>s1=%rcx
movq   200(%rdi),%rcx

# qhasm: s0 = (s1 s0) >> 1
# asm 1: shrd $1,<s1=int64#4,<s0=int64#3
# asm 2: shrd $1,<s1=%rcx,<s0=%rdx
shrd $1,%rcx,%rdx

# qhasm: s1 = (s2 s1) >> 1
# asm 1: shrd $1,<s2=int64#2,<s1=int64#4
# asm 2: shrd $1,<s2=%rsi,<s1=%rcx
shrd $1,%rsi,%rcx

# qhasm: (uint64) s2 >>= 1
# asm 1: shr  $1,<s2=int64#2
# asm 2: shr  $1,<s2=%rsi
shr  $1,%rsi

# qhasm: mem64[ input_0 + 192 ] = s0
# asm 1: movq   <s0=int64#3,192(<input_0=int64#1)
# asm 2: movq   <s0=%rdx,192(<input_0=%rdi)
movq   %rdx,192(%rdi)

# qhasm: mem64[ input_0 + 200 ] = s1
# asm 1: movq   <s1=int64#4,200(<input_0=int64#1)
# asm 2: movq   <s1=%rcx,200(<input_0=%rdi)
movq   %rcx,200(%rdi)

# qhasm: return
add %r11,%rsp
ret
