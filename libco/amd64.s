
.text
.globl co_swap
.type co_swap, @function
co_swap:
   movq %rsp, (%rsi) /* Save stack pointer, and pop the old one back */
   movq (%rdi), %rsp
   popq %rax

   movq %rbp, 0x8(%rsi) /* Save our non-volatile registers to stack */
   movq %rbx, 0x10(%rsi)
   movq %r12, 0x18(%rsi)
   movq %r13, 0x20(%rsi)
   movq %r14, 0x28(%rsi)
   movq %r15, 0x30(%rsi)

   movq 0x8(%rdi),  %rbp /* Pop back our saved registers */
   movq 0x10(%rdi), %rbx
   movq 0x18(%rdi), %r12
   movq 0x20(%rdi), %r13
   movq 0x28(%rdi), %r14
   movq 0x30(%rdi), %r15
   jmpq *%rax /* Jump back to saved PC */

