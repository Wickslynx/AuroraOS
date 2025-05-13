OUTPUT_FORMAT("binary")
ENTRY(_start)
SECTIONS
{
    . = 0x7C00;
    .text : {
        *(.text.boot)  /* Allow specifying boot-specific sections */
        *(.text)
    }
    .rodata : {
        *(.rodata)
    }
    .data : {
        *(.data)
    }
    .bss : {
        *(.bss)
    }
    
    /* Check bootloader size */
    . = ALIGN(16);
    
    /* Ensure we don't exceed 510 bytes (512 - 2 bytes for boot signature) */
    .sig : AT(0x7C00 + 510) {
        SHORT(0xAA55)
    }
}
