[bits 64]

section .text
global __tss_flush

__tss_flush:
        mov ax, 0x28
        ltr ax

        ret