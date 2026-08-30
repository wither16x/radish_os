SECTIONS
{
        . = 0x400000;

        .text : {
                *(.text*)
        }

        . = ALIGN(0x1000);
        .rodata : {
                *(.rodata*)
        }

        . = ALIGN(0x1000);
        .data : {
                *(.data*)
        }

        . = ALIGN(0x1000);
        .init_array : {
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(.init_array))
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)))
        PROVIDE_HIDDEN(__init_array_end = .);
        }

        . = ALIGN(0x1000);
        .bss : {
                *(.bss*)
        }

        . = ALIGN(0x1000);
        .eh_frame : {
                PROVIDE_HIDDEN(__eh_frame_start = .);
                KEEP(*(.eh_frame))
                PROVIDE_HIDDEN(__eh_frame_end = .);
        }

        . = ALIGN(0x1000);
        .eh_frame_hdr : {
                PROVIDE_HIDDEN(__eh_frame_hdr_start = .);
                KEEP(*(.eh_frame_hdr))
                PROVIDE_HIDDEN(__eh_frame_hdr_end = .);
        }
}