[bits 64]

section .text
global _start
extern main, tomato_init, tomato_end

; Note that the System V ABI says that argc, argv and envp are on the stack and not in
; registers like they would be when calling a normal function.
_start:
; Here, save argc/argv/envp in rbx, r12, r13 instead of rdi, rsi, rdx because tomato_init
; might garbage rdi, rsi, rdx. 
        mov rbx, [rsp]                          ; argc
        lea r12, [rsp + 8]                      ; argv
        lea r13, [rsp + 8 + rbx * 8 + 8]        ; envp

        call tomato_init

; call main
        mov rdi, rbx
        mov rsi, r12
        mov rdx, r13
        call main

        call tomato_end
        
; should never be reached
hang:
        jmp hang