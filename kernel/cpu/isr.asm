%macro isr_error_stub 1
global __isr_stub%1
__isr_stub%1:
        push %1
        ; the CPU pushes an error code
        jmp isr_common
%endmacro

%macro isr_no_error_stub 1
global __isr_stub%1
__isr_stub%1:
        push %1
        push 0 ; fake error code
        jmp isr_common
%endmacro

[bits 64]

section .text
extern exception_handler

isr_no_error_stub       3               ; breakpoint
isr_error_stub          14              ; page fault

; stack before jump:
; - error code
; - isr number
isr_common:
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
        mov rdi, rsp
        call exception_handler
        pop rax
        pop rax
        pop rax
        pop rbx
        pop rcx
        pop rdx
        pop rsi
        pop rdi
        pop r15
        pop r8
        pop r9
        pop r10
        pop r11
        pop r12
        pop r13
        pop r14
        pop r15
        add rsp, 16
        iretq