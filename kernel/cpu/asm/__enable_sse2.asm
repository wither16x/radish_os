[bits 64]

section .text
global __enable_sse2

__enable_sse2:
        mov rax, cr0
        and ax, 0xfffb
        or ax, 0x2
        mov cr0, rax

        mov rax, cr4
        or ax, 3 << 9
        mov cr4, rax

        ret