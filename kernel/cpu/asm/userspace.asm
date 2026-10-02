%define USER_CS                 0x1b

[bits 64]

%include "segments.inc"

section .text
global enter_userspace

;; RSI = stack pointer
;; RDI = return address
enter_userspace:
        cli

        xor rax, rax

        mov ax, USER_STACK_SEGMENT
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax

        push rax
        push rsi
        pushf
        pop rax
        or rax, 0x200
        push rax
        push USER_CODE_SEGMENT
        push rdi

        mov rbp, rsi

        iretq