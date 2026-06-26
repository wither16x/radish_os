[bits 64]

section .text
global enter_userspace

;; RSI = stack pointer
;; RDI = return address
enter_userspace:
        xor rax, rax

        mov ax, 0x23
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
        push 0x1b
        push rdi

        mov rbp, rsi

        iretq