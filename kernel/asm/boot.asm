[org 0x7C00]
bits 16

start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    cli

    ; Set VGA 320x200x256 mode
    mov ax, 0x0013
    int 0x10

    ; Set up GDT
    lgdt [gdt_descriptor]

    ; Enter protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump to flush pipeline
    jmp 0x08:protected_mode

[bits 32]
protected_mode:
    ; Set data segments
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; Load page table
    mov eax, page_table_pml4
    mov cr3, eax

    ; Enable long mode via MSR
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; Enable paging and long mode
    mov eax, cr0
    or eax, (1 << 31) | 1
    mov cr0, eax

    ; Far jump to long mode
    jmp 0x08:long_mode_start

[bits 64]
long_mode_start:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    mov rsp, 0x80000
    jmp 0x100000

; GDT
gdt_start:
    dq 0
gdt_code:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x9A
    db 0x20
    db 0x00
    dd 0x00000000
gdt_data:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x92
    db 0x00
    db 0x00
    dd 0x00000000
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dq gdt_start

; Page tables
align 4096
page_table_pml4:
    dq page_table_pdpt + 0x03
align 4096
page_table_pdpt:
    dq page_table_pd + 0x03
align 4096
page_table_pd:
    dq 0x0000000000000083

times 510-($-$$) db 0
dw 0xAA55
