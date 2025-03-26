; Bootloader built for AuroraOS! Wicks 2024.
; Rewritten for 64 bit 2025.
; 

[org 0x7c00]
bits 16

start:
    ; Set up segments (minimal for transition).
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

    ; Disable interrupts.
    cli

    ; Set up GDT for 64-bit mode.
    lgdt [gdt_descriptor]

    ; Enable protected mode (CR0).
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Long jump to 64-bit code segment.
    jmp 0x08:long_mode_start

[bits 64]
long_mode_start:
    ; Set up segment registers for 64-bit mode (data segments).
    mov ax, 0x10 ; Data selector
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    ; Set up stack (important in 64-bit).
    mov rsp, 0x80000 ; Example stack address (you might need to adjust)

    ; Jump to kernel entry (assuming kernel is loaded at 0x100000 in 64-bit).
    jmp 0x100000

; GDT (Global Descriptor Table) for 64-bit
gdt_start:
    dq 0                    ; Null descriptor (8 bytes)
gdt_code:
    dw 0xffff              ; Limit (bits 0-15)
    dw 0x0000              ; Base (bits 0-15)
    db 0x00                ; Base (bits 16-23)
    db 0b10011010          ; Access byte (Present=1, Privilege=00, Descriptor Type=1 (code/data), Executable=1, DC=0, RW=1 (not used for code), Accessed=0)
    db 0b00100000          ; Flags (Granularity=1 (4KB pages), Size=0 (16-bit/32-bit, set to 0 for 64-bit compatibility), Long Mode=1, Reserved=0)
    db 0x00                ; Base (bits 24-31)
    dd 0x00000000          ; Base (bits 32-63)
gdt_data:
    dw 0xffff              ; Limit (bits 0-15)
    dw 0x0000              ; Base (bits 0-15)
    db 0x00                ; Base (bits 16-23)
    db 0b10010010          ; Access byte (Present=1, Privilege=00, Descriptor Type=1 (code/data), Executable=0, DC=0, RW=1 (read/write), Accessed=0)
    db 0b00000000          ; Flags (Granularity=1, Size=0, Long Mode=0, Reserved=0)
    db 0x00                ; Base (bits 24-31)
    dd 0x00000000          ; Base (bits 32-63)
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; Size of GDT
    dq gdt_start             ; Address of GDT (64-bit address)

times 510-($-$$) db 0
dw 0xaa55 ; Bootloader built for AuroraOS! Wicks 2024.
