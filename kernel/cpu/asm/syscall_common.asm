%define KERNEL_DS               0x10

[bits 64]

section .text
extern syscall_handler
global syscall_common

;; This subroutine is responsible for saving the registers, calling the
;; syscall handler and restoring the registers.
;;
;; SYSCALL ABI
;; ==============================================
;; RAX = syscall number
;; RBX, RCX, RDX, RDI, RSI, R8, R9, R10, R11, R12, R13, R14, R15 = arguments
syscall_common:
        push r15
        push r14
        push r13
        push r12
        push r11
        push r10
        push r9
        push r8
        push rbp
        push rdi
        push rsi
        push rdx
        push rcx
        push rbx
        push rax
        mov rax, cr3
        push rax
        mov rax, cr2
        push rax

        mov bx, ds
        push rbx

        mov bx, KERNEL_DS
        mov ds, bx
        mov es, bx
        mov fs, bx
        mov gs, bx

        mov rdi, rsp
        call syscall_handler

        pop rbx
        mov ds, bx
        mov es, bx
        mov fs, bx
        mov gs, bx

        pop rax
        pop rax
        pop rax
        pop rbx
        pop rcx
        pop rdx
        pop rsi
        pop rdi
        pop rbp
        pop r8
        pop r9
        pop r10
        pop r11
        pop r12
        pop r13
        pop r14
        pop r15

        iretq