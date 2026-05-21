OUTPUT_FORMAT(elf64-x86-64)
ENTRY(kernel_main)

PHDRS
{
        limine_requests PT_LOAD;
        text            PT_LOAD;
        rodata          PT_LOAD;
        data            PT_LOAD;
}

SECTIONS
{
        . = 0xffffffff80000000;

        _lds_kernel_start = .;

        .limine_requests : {
                KEEP(*(.limine_requests_start))
                KEEP(*(.limine_requests))
                KEEP(*(.limine_requests_end))
        } :limine_requests

        . = ALIGN(CONSTANT(MAXPAGESIZE));

        .text : {
                *(.text .text.*)
        } :text

        . = ALIGN(CONSTANT(MAXPAGESIZE));

        .rodata : {
                *(.rodata .rodata.*)
        } :rodata

        .note.gnu.build-id : {
                *(.note.gnu.build-id)
        } :rodata

        .init_array : {
                __init_array = .;
                KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
                KEEP(*(.init_array .ctors))
                __init_array_end = .;
        } :rodata

        . = ALIGN(CONSTANT(MAXPAGESIZE));

        .data : {
                *(.data .data.*)
        } :data

        .bss : {
                *(.bss .bss.*)
                *(COMMON)
        } :data

        /DISCARD/ : {
                *(.eh_frame*)
                *(.note .note.*)
        }

        _lds_kernel_end = .;
}