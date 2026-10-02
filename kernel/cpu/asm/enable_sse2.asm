[bits 64]

%include "registers.inc"

section .text
global __enable_sse2

__enable_sse2:
        mov rax, cr0
        and ax, ~CR0_EM
        or ax, CR0_MP
        mov cr0, rax

        mov rax, cr4
        or ax, CR4_OSFXSR << CR4_OSXMMEXCPT
        mov cr4, rax

        ret