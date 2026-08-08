%define USER_CS                 0x1b
%define USER_SS                 0x23

[bits 64]

section .text
global enter_userspace

;; RSI = stack pointer
;; RDI = return address
enter_userspace:
        cli

        xor rax, rax

        mov ax, USER_SS
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
        push USER_CS
        push rdi

        mov rbp, rsi

        iretq