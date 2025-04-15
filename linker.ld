ENTRY(_start)

SECTIONS
{
    . = 0x100000;

    .text ALIGN(4K) :
    {
        _text_start = .;
        *(.text)
        _text_end = .;
    }

    .rodata ALIGN(4K) :
    {
        *(.rodata)
    }

    .data ALIGN(4K) :
    {
        *(.data)
    }

    .bss ALIGN(4K) :
    {
        *(COMMON)
        *(.bss)
    }
    
    /DISCARD/ : { *(.eh_frame) *(.note.GNU-stack) }
}
