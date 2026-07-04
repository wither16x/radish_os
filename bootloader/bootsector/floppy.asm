[bits 16]
[org 0x7c00]

start:
; initialize segments
        xor ax, ax
        mov ds, ax
        mov es, ax
        mov ax, 0x8000
        mov ss, ax
        mov sp, 0xf000

        mov [driveno], dl ; assuming DL contains the drive number
                          ; (which is the case for most BIOSes)

; load stage 2 from disk
        push es
        mov ax, 0x100
        mov es, ax
        xor bx, bx
        mov ah, 0x02
        mov al, 1
        xor ch, ch
        mov cl, 0x02
        xor dh, dh
        mov dl, [driveno]
        int 0x13
        jc hang
        pop es

        cli

        lgdt [gdt_ptr]

        mov eax, cr0
        or eax, 1
        mov cr0, eax

        jmp code_seg:init_protected_mode

;; Loop on HLT to save power
hang:
        hlt
        jmp hang

%include "gdt.asm"

driveno:        db      0

[bits 32]

init_protected_mode:
        mov ax, data_seg
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax
        
        mov esp, 0x90000

        jmp code_seg:0x1000

;; Loop on HLT to save power
hang32:
        hlt
        jmp hang32

; -------------------------------------------------------------
times 510 - ($ - $$) db 0
dw 0xaa55