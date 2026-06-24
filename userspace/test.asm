[bits 64]

section .text
global _start

_start:
        mov rax, [constant]
        mov rbx, [random_qword]
        
; just hang forever (interrupts are still received)
hang:
        hlt
        jmp hang

section .rodata
constant:               dd 42

section .data
random_qword:           dq 18