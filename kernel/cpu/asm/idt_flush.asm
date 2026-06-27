[bits 64]

section .text
global idt_flush

idt_flush:
        lidt [rdi]
        ret