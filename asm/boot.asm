[org 0x7c00]       ; Tell the BIOS that this is the bootloader.
bits 16            ; Use 16-bit real mode.

start:
    cli            ; Disable interrupts.
    hlt            ; Halt the CPU.

halt:
    jmp halt       ; Infinite loop to halt the system.

times 510 - ($ - $$) db 0
dw 0xaa55          ; Boot signature
