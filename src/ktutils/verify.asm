[CPU intelnop]
[bits 64]

.data

.align 32
.global verify64
verify64:

  ; load 64 bytes from rdi and rsi
  movdqu xmm0, [rdi]+0
  movdqu xmm1, [rdi]+16
  movdqu xmm2, [rdi]+32
  movdqu xmm3, [rdi]+48
  movdqu xmm4, [rsi]+0
  movdqu xmm5, [rsi]+16
  movdqu xmm6, [rsi]+32
  movdqu xmm7, [rsi]+48

  ; xor the two sources
  pxor   xmm0, xmm4
  pxor   xmm1, xmm5
  pxor   xmm2, xmm6
  pxor   xmm3, xmm7

  ; fold the results to xmm0
  por    xmm0, xmm1
  por    xmm2, xmm3
  por    xmm0, xmm2
  ; the result is now in xmm0

  ; clear xmm1-xmm4
  pxor   xmm1, xmm1
  pxor   xmm2, xmm2
  pxor   xmm3, xmm3
  pxor   xmm4, xmm4
  pxor   xmm5, xmm5

  ; extract the two qwords
  pextrq rax, xmm0, 0
  pextrq rcx, xmm0, 1
  ; or them together
  or     rax, rcx
  ; clean up rcx and xmm0
  xor    rcx, rcx
  pxor   xmm0, xmm0
  ; rotate by 32, to guarantee that eax is non-zero
  ; if rax is non-zero
  rolq   rax, rax, 32

  ret
