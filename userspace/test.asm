[bits 64]

section .text
_start:
; just hang forever (interrupts are still received)
hang:
        hlt
        jmp hang

section .rodata
constant:               dd 42
string:                 db "Hello, I am a string!", 13, 10, 0

section .data
random_qword:           dq 18

section .bss
buffer16:               resw 1