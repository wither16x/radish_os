[bits 64]

section .text
global __idt_flush

__idt_flush:
        lidt [rdi]
        ret