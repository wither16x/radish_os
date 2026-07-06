%macro irq 1
global __irq_stub%1
__irq_stub%1:
        push %1         ; push the IRQ number
        jmp irq_common
%endmacro

[bits 64]

section .text
extern irq_handler

irq 0                   ; timer
irq 1                   ; keyboard

;; Every IRQ stub should jump to this address.
irq_common:
        cli
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

        cld
        mov rdi, rsp
        call irq_handler

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
        
        add rsp, 8
        sti

        iretq