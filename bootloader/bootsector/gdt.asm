[bits 16]

;; This GDT is used to switch to protected mode

gdt:
; null segment
        dq 0
gdt_code:
; code segment
        dw 0xffff
        dw 0
        db 0
        db 0b10011010
        db 0b11001111
        db 0
gdt_data:
; data segment
        dw 0xffff
        dw 0
        db 0
        db 0b10010010
        db 0b11001111
        db 0
gdt_end:

gdt_ptr:
        dw gdt_end - gdt - 1
        dd gdt

code_seg:       equ gdt_code - gdt
data_seg:       equ gdt_data - gdt