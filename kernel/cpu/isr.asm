%macro isr_error_stub 1
global __isr_stub%1
__isr_stub%1:
        jmp isr_common
%endmacro

%macro isr_no_error_stub 1
global __isr_stub%1
__isr_stub%1:
        jmp isr_common
%endmacro

[bits 64]

section .text
extern exception_handler

isr_no_error_stub       3               ; breakpoint

isr_common:
        call exception_handler
        iretq