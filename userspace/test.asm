[bits 64]

[org 0x400000]

; just hang forever (interrupts are still received)
hang:
        hlt
        jmp hang