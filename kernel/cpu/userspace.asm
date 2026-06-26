[bits 64]

section .text
global enter_userspace

;; RSI = stack pointer
;; RDI = return address
enter_userspace:
        xor rax, rax

        mov ax, 0x20
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax

        push rax
        push rsi
        pushf
        push 0x18
        push rdi

        mov rbp, rsi

        iretq