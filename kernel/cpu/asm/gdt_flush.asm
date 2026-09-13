%define CODE_SEGMENT            0x08
%define DATA_SEGMENT            0x10

[bits 64]

section .text
global __gdt_flush

__gdt_flush:
        lgdt [rdi]

        push CODE_SEGMENT
        lea rax, [rel reload_cs]
        push rax
        
        retfq

;; Reload the code segment.
reload_cs:
        mov ax, DATA_SEGMENT
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax

        ret