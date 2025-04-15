; === Stage 2 Bootloader === 
; AuroraOS, 2025. (Wicks) — Modified to load FS into high memory

[org 0x8000]
bits 16

start_stage2:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; ==== Load KERNEL (sector 4-23) to 0x100000 ====
    mov ah, 0x02        ; BIOS Read Sectors
    mov al, 20          ; Read 20 sectors (adjust to kernel size)
    mov ch, 0
    mov cl, 4
    mov dh, 0
    mov dl, 0x80
    mov bx, 0x0000
    mov es, 0x1000      ; Segment 0x1000:0000 -> 0x100000
    int 0x13
    jc fail

    ; ==== Load FILESYSTEM (sector 24-47) to 0x200000 ====
    mov ah, 0x02
    mov al, 24          ; Read 24 sectors = 512 * 24 = 12288 bytes = 12KB FS blob
    mov ch, 0
    mov cl, 24
    mov dh, 0
    mov dl, 0x80
    mov bx, 0x0000
    mov es, 0x2000      ; Segment 0x2000:0000 -> 0x200000
    int 0x13
    jc fail

    ; ==== Setup GDT ====
    lgdt [gdt_descriptor]

    ; ==== Enter Protected Mode ====
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:protected_mode

; ==== GDT ====
gdt_start:
    dq 0
gdt_code:
    dw 0xFFFF
    dw 0
    db 0
    db 10011010b
    db 11001111b
    db 0
gdt_data:
    dw 0xFFFF
    dw 0
    db 0
    db 10010010b
    db 11001111b
    db 0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; ==== Protected Mode ====
[bits 32]
protected_mode:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov esp, 0x90000

    ; Enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; Enable long mode
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; Paging setup
    mov eax, page_directory
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    jmp 0x08:long_mode_entry

; ==== Long Mode ====
[bits 64]
long_mode_entry:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    mov rsp, 0xA0000

    ; Filesystem is now available at 0x200000
    ; Kernel entry point
    jmp 0x100000

fail:
    hlt
    jmp $

align 4096
page_directory:
    ; Your identity-mapped paging setup goes here (can help write this too!)
