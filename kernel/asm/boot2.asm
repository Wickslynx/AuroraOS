; === Stage 2 Bootloader === 
; AuroraOS, 2025. (Wicks) — Loads Kernel + FS, enters Long Mode

[org 0x8000]
[bits 16]

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
    mov cl, 4           ; Sector 4
    mov dh, 0
    mov dl, 0x80
    mov bx, 0x0000
    mov ax, 0x1000
    mov es, ax          ; Segment 0x1000:0000 = 0x100000
    int 0x13
    jc fail

    ; ==== Load FILESYSTEM (sector 24-47) to 0x200000 ====
    mov ah, 0x02
    mov al, 24          ; Read 24 sectors = 12 KB
    mov ch, 0
    mov cl, 24          ; Sector 24
    mov dh, 0
    mov dl, 0x80
    mov bx, 0x0000
    mov ax, 0x2000
    mov es, ax          ; Segment 0x2000:0000 = 0x200000
    int 0x13
    jc fail

    ; ==== Setup GDT ====
    lgdt [gdt_descriptor]

    ; ==== Enter Protected Mode ====
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:protected_mode

fail:
    cli
    hlt
    jmp $

; ==== GDT ====
gdt_start:
    dq 0                       ; Null descriptor
gdt_code:
    dw 0xFFFF                  ; Limit
    dw 0x0000                  ; Base low
    db 0x00                    ; Base mid
    db 10011010b               ; Access
    db 11001111b               ; Granularity
    db 0x00                    ; Base high
gdt_data:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; ==== Protected Mode Setup ====
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

    ; Enable Long Mode via MSR
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; Setup paging (basic identity map)
    mov eax, page_directory
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ; Far jump to long mode
    jmp 0x08:long_mode_entry

; ==== Long Mode Setup ====
[bits 64]
long_mode_entry:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    mov rsp, 0xA0000

    ; Kernel loaded at 0x100000, FS at 0x200000
    jmp 0x100000              ; Jump to kernel

; ==== Page Tables (simple identity map for 1GB) ====
align 4096
page_directory:
    dq page_table + 0x3

align 4096
page_table:
    ; Map first 1GB of memory
%assign i 0
%rep 512
    dq (i * 0x1000) + 0x3
%assign i i + 1
%endrep
