; Bootloader built for AuroraOS! Wicks 2024.

[org 0x7c00]
bits 16

start:
    ; Set up segments
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

    ; Switch to protected mode
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    
    ; Far jump to 32-bit code
    jmp 0x08:protected_mode

[bits 32]
protected_mode:
    ; Set up segment registers for protected mode
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Jump to kernel entry
    jmp 0x1000    ; Assuming kernel is loaded at 0x1000

; GDT (Global Descriptor Table)
gdt_start:
    dd 0, 0       ; Null descriptor
gdt_code:
    dw 0xffff     ; Limit
    dw 0x0000     ; Base
    db 0x00       ; Base
    db 10011010b  ; Flags
    db 11001111b  ; Flags + Limit
    db 0x00       ; Base
gdt_data:
    dw 0xffff
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 510-($-$$) db 0
dw 0xaa55
