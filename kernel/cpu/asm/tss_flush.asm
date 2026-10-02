[bits 64]

%include "segments.inc"

section .text
global __tss_flush

__tss_flush:
        mov ax, TSS_SELECTOR
        ltr ax

        ret